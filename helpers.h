#include<stdlib.h>

/*
    Helper functions used in the program of MANDELBROT ZOOM


*/

long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max){

    // Function map to change the value x from scale [in_min,in_max] to scale [out_min,out_max]
    return (x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
}

long double random(long double in_min, long double in_max){

    // Returns a random value in the range [in_min,in_max]
    int random_val = rand()%100;
    return map(random_val,0,100,in_min,in_max);
}