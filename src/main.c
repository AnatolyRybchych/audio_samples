#include <SDL2/SDL.h>

#include "snippet.h"

int main();
void panic_if_sdl_err(int err);
void panic(const char *msg);
void play_snippet(const Snippet *snippet);

double interpolate_linear(double progress){
    return progress;
}

double interpolate_sqr(double progress){
    return progress * progress;
}

double interpolate_sqrt(double progress){
    return sqrt(progress);
}

double impose_simple_mix(double first, double second){
    double result = first + second;
    if(result > 1.0){
        result = 1.0;
    }
    else if(result < -1.0){
        result = -1.0;
    }
    
    return result;
}

double impose_simple_mix_low(double first, double second){
    return first * 0.5 + second * 0.5;
}

int main(){
    panic_if_sdl_err(SDL_Init(SDL_INIT_AUDIO));

    Snippet s, s1, s2;
    snippet_empty(&s);
    snippet_empty(&s1);
    snippet_empty(&s2);

    snippet_fill_freq(&s, 1.0, PITCH);
    snippet_fill_freq(&s1, 1.0, 500);
    snippet_fill_freq(&s2, 1.0, 600);

    snippet_impose(&s, &s1, impose_simple_mix);
    snippet_impose(&s, &s2, impose_simple_mix_low);

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
