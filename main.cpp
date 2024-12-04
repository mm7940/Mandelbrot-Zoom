#include<iostream>
#include"mandelbrot.h"


int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    //SDL_CreateWindowAndRenderer(1440,900,0,&window,&renderer);
    SDL_CreateWindowAndRenderer(WIDTH,HEIGHT,0,&window,&renderer);
    SDL_RenderSetLogicalSize(renderer,WIDTH,HEIGHT);

    int count =0;
    
    while(session){

        update(count);
        SDL_RenderPresent(renderer);

        draw(event,renderer,count);
        //cout<<MAX<<" "<<MIN<<"\n";

        save(renderer,window,count);
        count++;
    }
    return 0;
}