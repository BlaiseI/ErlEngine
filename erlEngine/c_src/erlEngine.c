#include <erl_nif.h>
#include <stdio.h>
#include <SDL2/SDL.h>

int load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info){
    return 0;
}

int upgrade(ErlNifEnv* caller_env, void** priv_data, void** old_priv_data, ERL_NIF_TERM load_info){
    return 0;
}

int main(void){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) !=0){
        printf("error : %s\n", SDL_GetError());
        return 1;
    }
    printf("SDL initiated, no error\n");
    return 0;
}

ErlNifFunc nif_funcs[] = {
    {"initiate", 0, main}
};

ERL_NIF_INIT(numerl, nif_funcs, load, NULL, upgrade, NULL)