#include<SDL2/SDL.h>
#include<string>
#include<linux/input.h>
#include"helpers.h"
#include<iostream>

using namespace std;

// HEIGHT and WIDTH of window
int WIDTH = 200;
int HEIGHT = 200;

/*
    Variables used to plot the Mandelbrot set in a range

    The pixels in Width of the window is mapped to [MINX,MAXX]
    The pixels in Width of the window is mapped to [MINY,MAXY]
    
    if z = z*z+c doesn't increases value 2 within MAX_ITR number of iterations
        then it is in the mandelbrot set

*/

/*
    Global variables to plot the image x axis in the range [MINX,MAXX]
    MIN_INCX and MAX_DECX updates the MINX and MAXX values in each image

*/
long double MINX = -2.84;
long double MAXX = 1.0;
long double MAX_DECX = 0.1;
long double MIN_INCX = 0.15;

/*
    Global variables to plot the image y axis in the range [MINY,MAXY]
    MIN_INCY and MAX_DECY updates the MINY and MAXY values in each image

*/
long double MINY = -2.84;
long double MAXY = 1.0;
long double MAX_DECY = 0.1;
long double MIN_INCY = 0.15;

/*
    Helper variables required to update the above variables
    factor decreases by fac_t in each iteration
*/
long double factor = 1;
long double fac_t = 0.9349;
long int MAX_ITR = 200;
long double CHANGE = 0.001;

// bool value to to check if the code is still running
bool session = true;

// bool value to check if image to be saved or not
bool SAVE = true;

// Variables required for the spiral zoom in
bool spiral_zoom = true;
long double ROTATE = 0.000001;
long double CHANGE2 = 10*CHANGE*ROTATE;

// Returns the number of iterations required till mod(z) increases the value 2
int findMandelbrot(long double cr, long double ci){
    int i=0;

    // starting value of z = 0
    long double zr=0.0, zi=0.0;
    while(i<MAX_ITR && (zr*zr+zi*zi)<4.0){

        /*
            Implements the mathematical equation of mandelbrot set z = z*z+c

            zr = zr*zr-zi*zi+cr;
            zi = 2*zi*zr+ci;
        */
        long double temp = zr*zr-zi*zi+cr;
        zi = 2*zr*zi+ci;
        zr = temp;
        i++;
    }
    return i;
}

// Updates the left increment and right decrement value whenever called
void update_by_change_X(long double change){
    MAX_DECX -= change;
    MIN_INCX += change;
}

// Updates the up decrement and down increment value whenever called
void update_by_change_Y(long double change){
    MAX_DECY -= change;
    MIN_INCY += change;
}

void events(SDL_Event event){
    
    // Handels all possible events from keyboard
    while(SDL_PollEvent(&event)){
    
        // Stops the program if SDL window is closed
        if(event.type==SDL_QUIT){
            session = false;
            return;
        }

        // Keyboard events
        if(event.type==SDL_KEYDOWN){

            // Press q to quit the window
            if(event.key.keysym.sym==SDLK_q){
                session = false;
                return;
            }

            // Right arrow pressed to move the zoom in to right
            if(event.key.keysym.sym==SDLK_RIGHT){
                update_by_change_X(CHANGE);
                cout<<"RIGHT\n";
                return;
            }

            // Left arrow pressed to move the zoom in to left
            if(event.key.keysym.sym==SDLK_LEFT){
                update_by_change_X(-1*CHANGE);
                cout<<"LEFT\n";
                return;
            }

            // Up arrow pressed to move the zoom in to up    
            if(event.key.keysym.sym==SDLK_UP){
                update_by_change_Y(-1*CHANGE);
                cout<<"UP\n";
                return;
            }

            // Down arrow pressed to move the zoom in to down
            if(event.key.keysym.sym==SDLK_DOWN){
                update_by_change_Y(1*CHANGE);
                cout<<"DOWN\n";
                return;
            }
        }
    }
}

// Updates the Variables used for updating the zoom factor on four sides to create the spiral effect
void spiral_update(int count){

    // This variable returns the coorinate of the zoom direction
    int x = (count/10)%4;

    // Four cases of updates each for a particular coordinate
    if(x==0){
        update_by_change_Y(CHANGE2);
        update_by_change_X(10*CHANGE*ROTATE-CHANGE2);
    }
    else if(x==1){
        update_by_change_Y(-1*CHANGE2);
        update_by_change_X(10*CHANGE*ROTATE-CHANGE2);
    }
    else if(x==2){
        update_by_change_Y(-1*CHANGE2);
        update_by_change_X(-1*(10*CHANGE*ROTATE-CHANGE2));
    }
    else if(x==3){
        update_by_change_Y(CHANGE2);
        update_by_change_X(-1*(10*CHANGE*ROTATE-CHANGE2));
    }

    // Update the helper variables used for spiral update
    CHANGE2 -= ROTATE*CHANGE;

    if(CHANGE2==0)ROTATE *= -1;
    else if(CHANGE2==10*CHANGE)ROTATE *= 1;
}

// Creates the full image for a particular MIN and MAX value
void draw(SDL_Event event, SDL_Renderer *renderer, int count){

    // Colors each pixel in the window (x,y) where x E[0,WIDTH] and y E[0,HEIGHT]
    for(int x=0; x<WIDTH; x++){
        
        // Keyboard and mouse events
        events(event);

        // Executes the spiral zoom feature 
        if(spiral_zoom)spiral_update(count);

        for(int y=0; y<HEIGHT; y++){
            
            /*
                The value c calculated for each pixel
                Each pixel is mapped in the range [MIN,MAX]
            */ 
            long double cr = map(x,0,WIDTH,MINX,MAXX);
            long double ci = map(y,0,HEIGHT,MINY,MAXY);

            int n = findMandelbrot(cr,ci);
            
            // The pixel gets color if it escapes from the mandelbrot set
            // Also make the brightness to 0 if less than 20 to make boundaries more clear
            int bright = map(n,0,MAX_ITR,0,255);
            if(n==MAX_ITR || bright<20){
                bright =0;
            }

            // Coloring of the pixel(x,y)
            int red = map(bright*bright,0,255*255,0,255);
            int green = bright;
            int blue = map(sqrt(bright),0,sqrt(255),0,255);
            SDL_SetRenderDrawColor(renderer,red,green,blue,255);
            SDL_RenderDrawPoint(renderer,x,y);
        }
    }
}

// Updates the global variables for the next picture to be drawn
void update(int count){
    
    MAXX -= MAX_DECX*factor;
    MINX += MIN_INCX*factor;
    MAXY -= MAX_DECY*factor;
    MINY += MIN_INCY*factor;

    factor *= fac_t;
    MAX_ITR += 5;

    /*
    cout<<"Left Zoom : "<<MIN_INCX<<"\n";
    cout<<"Right Zoom : "<<MAX_DECX<<"\n";
    cout<<"Up Zoom : "<<MIN_INCY<<"\n";
    cout<<"Down Zoom : "<<MAX_DECY<<"\n";
    cout<<MAX_DECX<<" "<<MIN_INCX<<" "<<MAX_DECY<<" "<<MIN_INCY<<"\n";
    */
    cout<<"Next Image\n";
    if(count>30){
        MAX_ITR *= 1.02;
    }

    // Starts from the begning if any axes' range becomes zero
    if(MAXX<=MINX || MAXY<=MINY){
        // Initial value of the variables
        MAXX = 1.0;
        MINX = -2.84;
        MAXY = 1.0;
        MINY = -2.84;
        MAX_DECX = 0.1;
        MIN_INCX = 0.15;
        MAX_DECY = 0.1;
        MIN_INCY = 0.15;

        factor = 1;
        MAX_ITR = 200;
        cout<<"==============================================================\n";
        cout<<"                       NEXT ROUND                             \n";
        cout<<"==============================================================\n";
    }
}

// Saves every image im images directory with .bmp extension
void save(SDL_Renderer *renderer, SDL_Window *window, int count){
    if(!SAVE)return;

    SDL_Surface *sshot = SDL_GetWindowSurface(window);
    SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_ABGR8888,sshot->pixels,sshot->pitch);
    string file = "images/"+to_string(count)+".bmp";
    SDL_SaveBMP(sshot,file.c_str());
    SDL_FreeSurface(sshot);   
}


