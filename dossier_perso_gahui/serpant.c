#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20
#define SEGMENT 20
#define MAX 50

typedef struct {
    int x[MAX];
    int y[MAX];
    int len;
    int dirX;
    int dirY;
} Snake;


void initSnake(Snake *s)
{
    s->len = 10;

    for (int i = 0; i < s->len; i++) {
        s->x[i] = 200 - i * SEGMENT;
        s->y[i] = 200;
    }

    s->dirX = SEGMENT;
    s->dirY = 0;
}

int plus_ou_moins(){
    srand(time(NULL));
    return rand()%2 ; // 1==plus , 0 == moins
}




void moveSnake(Snake *s)
{
    for (int i = s->len - 1; i > 0; i--) {
        s->x[i] = s->x[i - 1];
        s->y[i] = s->y[i - 1];
    }


    s->dirX = rand()%20;
    s->dirY = rand()%30;

    if(plus_ou_moins ==0){
        s->x[0] -= s->dirX;

    }
    else{
        s->x[0] += s->dirX;

    }

    if(plus_ou_moins ==0){
        s->y[0] -= s->dirY;

    }
    else{
        s->y[0] += s->dirY;

    }
}




void randomColor(SDL_Renderer *r)
{
    Uint8 red = rand() % 256;
    Uint8 green = rand() % 256;
    Uint8 blue = rand() % 256;

    SDL_SetRenderDrawColor(r, red, green, blue, 100);
}




void drawSnake(SDL_Renderer *r, Snake *s)
{
    for (int i = 0; i < s->len; i++) {

        randomColor(r);

        SDL_Rect rect = {
            s->x[i],
            s->y[i],
            20,
            SEGMENT
        };

        SDL_RenderFillRect(r, &rect);
    }
}


int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    srand(time(NULL));

    SDL_Window *window = SDL_CreateWindow(
        "Snake rectangles",
        100, 100,
        1000, 1000,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Snake snake;
    initSnake(&snake);

    int running = 1;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
        }

        moveSnake(&snake);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawSnake(renderer, &snake);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}