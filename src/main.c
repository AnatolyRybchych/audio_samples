#include <SDL2/SDL.h>

int main();
void panic_if_sdl_err(int err);
void panic(const char *msg);

int main(){
    panic_if_sdl_err(SDL_Init(SDL_INIT_AUDIO));

    printf("qwe\n");

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
