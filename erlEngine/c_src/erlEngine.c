#include <erl_nif.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_image.h>

#define WINDOW_WIDTH (960)
#define WINDOWN_HEIGHT (540)

#define SCROLL_SPEED (300)

ERL_NIF_TERM atom_nok;
ERL_NIF_TERM atom_ok;

ErlNifResourceType *MULT_YIELDING_ARG = NULL;

int load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info){
    atom_nok = enif_make_atom(env, "nok\0");
    atom_ok = enif_make_atom(env, "ok\0");
    return 0;
}

int upgrade(ErlNifEnv* caller_env, void** priv_data, void** old_priv_data, ERL_NIF_TERM load_info){
    return 0;
}

ERL_NIF_TERM test(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) !=0){
        printf("error : %s\n", SDL_GetError());
        return atom_nok;
    }
    printf("SDL initiated, no error\n");

    SDL_Window *win = SDL_CreateWindow("Hello, world !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540,0);
    if(!win){
        printf("error : %s, quit sdl", SDL_GetError());
        SDL_Quit();
        return atom_nok;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
    if(!rend){
        printf("error : %s, close window and quit sdl", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return atom_nok;
    }

    SDL_Surface *surface = IMG_Load("/home/blaise/Documents/heart.png");
    if(!surface){
        printf("error : %s, close window and quit sdl", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return atom_nok;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend,surface);
    SDL_FreeSurface(surface);
    if(!tex){
        printf("error : %s, close window and quit sdl", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return atom_nok;
    }

    SDL_Rect dest;
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = (WINDOW_WIDTH - dest.w) /2;

    float y_pos = WINDOWN_HEIGHT;
    while(dest.y >= -dest.h){
        SDL_RenderClear(rend);
        dest.y = (int) y_pos;
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        y_pos -= (float) SCROLL_SPEED /60;

        SDL_Delay(1000/60);
    }

    //SDL_RenderClear(rend);

    //SDL_Delay(1000);

    //SDL_RenderCopy(rend, tex, NULL,NULL);
    //SDL_RenderPresent(rend);

    //SDL_Delay(5000);

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return atom_ok;
}

ErlNifFunc nif_funcs[] = {
    {"test_fun", 0, test}
};

ERL_NIF_INIT(erlEngine, nif_funcs, load, NULL, upgrade, NULL)