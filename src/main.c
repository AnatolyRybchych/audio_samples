#include <SDL2/SDL.h>

#include "snippet.h"

int main();
void panic_if_sdl_err(int err);
void panic(const char *msg);
void play_snippet(const Snippet *snippet);

double linear(double progress){
    return progress;
}

int main(){
    panic_if_sdl_err(SDL_Init(SDL_INIT_AUDIO));

    Snippet s, s1, s2;
    snippet_empty(&s);
    snippet_empty(&s1);
    snippet_empty(&s2);

    snippet_fill_freq(&s1, 0.3, PITCH);
    snippet_attack(&s1, 0.1, linear);
    snippet_release(&s1, 0.1, linear);
    snippet_fill_freq(&s2, 0.5, 600);

    snippet_append(&s, &s1);
    snippet_append(&s, &s1);
    snippet_append(&s, &s1);

    play_snippet(&s);
    snippet_free(&s);
    snippet_free(&s1);
    snippet_free(&s2);

    return 0;
}

void panic_if_sdl_err(int err){
    if(err) {
        panic(SDL_GetError());
    }
}

void panic(const char *msg){
    fprintf(stderr, "error: %s\n", msg);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

typedef struct{
    const Snippet *snippet;
    int samples_played;
    int closed;
} SnippetPalyData;

void snippet_audio_callback(SnippetPalyData *data, int16_t *stream, int samples_to_write){
    if(data->snippet->samples_cnt <= data->samples_played){
        if(data->closed == 0){
            SDL_CloseAudio();
            data->closed = 1;
        }
    }
    else if(samples_to_write > 0){
        *stream = (INT16_MAX - 1) * data->snippet->samples[data->samples_played++];
        snippet_audio_callback(data, stream+1, samples_to_write - 1);
    }
}

void audio_callback(void *user_data, Uint8 *stream, int len){
    SDL_assert(len % 2 == 0);
    snippet_audio_callback((SnippetPalyData*)user_data, (int16_t*)stream, len / 2);
}

void play_snippet(const Snippet *snippet){
    SnippetPalyData play_data;
    SDL_AudioSpec want, have;

    play_data.samples_played = 0;
    play_data.snippet = snippet;
    play_data.closed = 0;

    SDL_zero(want);
    want.freq = SNIPPET_FREQ;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 4096;
    want.callback = audio_callback;
    want.userdata = &play_data;

    panic_if_sdl_err(SDL_OpenAudio(&want, &have));
    if (have.format != want.format) {
        panic("unsupported format: AUDIO_S16");
    }

    SDL_PauseAudio(0);
    SDL_Delay(snippet->samples_cnt / 44100.0 * 1000.0);
    if(play_data.closed == 0){
        SDL_CloseAudio();
    }
}
