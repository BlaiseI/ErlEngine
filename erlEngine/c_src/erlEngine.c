#include <erl_nif.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_image.h>

#define WINDOW_WIDTH (960)
#define WINDOW_HEIGHT (540)

#define SCROLL_SPEED (300)
#define SPEED (300)

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

    SDL_Window *win = SDL_CreateWindow("Hello, world !", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,0);
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

    SDL_Surface *surface = IMG_Load("/home/blaise/Documents/heart2.png");
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
    
    float x_pos = (WINDOW_WIDTH - dest.w) /2;
    float y_pos = (WINDOW_HEIGHT- dest.h) /2;
    float x_vel = 0;
    float y_vel = 0;

    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    int close_requested = 0;
    while(!close_requested){
        SDL_Event event;
        while(SDL_PollEvent(&event)){

            switch (event.type){
            case SDL_QUIT:
                close_requested = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode){
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                default:
                    break;
                }
                break;
            }
        }

        int mouse_x, mouse_y;
        int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

        x_vel = y_vel = 0;
        if(up && !down) y_vel=-SPEED;
        if(!up && down) y_vel= SPEED;
        if(left && !right) x_vel=-SPEED;
        if(right && !left) x_vel= SPEED;

        if (x_pos <= 0)
            x_pos = 0;
        if (y_pos <= 0)
            y_pos = 0;
        if (x_pos >= (WINDOW_WIDTH - dest.w))
            x_pos = WINDOW_WIDTH - dest.w;
        if (y_pos >= (WINDOW_HEIGHT - dest.h))
            y_pos = WINDOW_HEIGHT - dest.h;


        // Update the sprite position
        y_pos += y_vel / 60;
        x_pos += x_vel / 60;

        // Set the positions in the spriteRect
        dest.x = (int)x_pos;
        dest.y = (int)y_pos;

        // Clear the window
        SDL_RenderClear(rend);

        // Draw the image to the window
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        // Wait
        SDL_Delay(1000 / 60);
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