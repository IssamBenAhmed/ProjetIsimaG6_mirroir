#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>



#define SIZE 20
#define SEGMENT 20
#define MAX 50
#define LIMIT 0.8
#define PI 3.14
#define ECRAN_HAUTEUR 1000
#define ECRAN_LARGEUR 1000


typedef struct { //type de serpant
    float x[MAX];
    float y[MAX];
    int len;
    float dirX;
    float dirY;
    float angle;
} Snake;


void initSnake(Snake *s) //initialisation
{
    s->len = 40;

    for (int i = 0; i < s->len; i++) {
        s->x[i] = 200 - i * SEGMENT;
        s->y[i] = 200;
    }

    s->dirX = SEGMENT;
    s->dirY = 0;
}

void moveSnake(Snake *s)
{
    // 1. tête (mouvement fluide contrôlé)
    s->angle += (rand() %100 - 50) * 0.005f;

    float speed = 9.0f;

    s->dirX = cos(s->angle) * speed;
    s->dirY = sin(s->angle) * speed;

    s->x[0] += s->dirX;
    s->y[0] += s->dirY;

    //revendiquer si le serpent essaie d'échapper l'écran

    if (s->x[0] <0){ 
        s->x[0] = 0;
        s->angle = PI - s-> angle ;

    }
    if (s->y[0] <0){ 
        s->y[0] = 0;
        s->angle = - s-> angle ;

    }
    if (s->x[0] > ECRAN_LARGEUR){
        s->x[0] = ECRAN_LARGEUR;
        s->angle = PI- s->angle;
    }
    if (s->y[0] > ECRAN_HAUTEUR){ 
        s->y[0] = ECRAN_HAUTEUR;
        s->angle = - s-> angle ;
    
    }
    // 2. corps (follow fluide)
    for(int i = 1; i < s->len; i++) {
            // calcul du vecteur de direction entre le segment precedent et l'actuel
            float dx = s->x[i-1] - s->x[i];
            float dy = s->y[i-1] - s->y[i];
            
            // calcul de la distance absolue avec le theoreme de pythagore
            float dist = sqrt(dx*dx + dy*dy);
            
            // si l'articulation s'eloigne au-dela de l'espacement autorise, on la tire
            if (dist > 10.0f) {
                // (dx / dist) et (dy / dist) creent un vecteur normalise (de taille 1)
                // on multiplie ce vecteur par l'espacement voulu pour placer le segment a la bonne distance
                // on soustrait ce resultat a la position du segment precedent
                s->x[i] = s->x[i-1] - (dx / dist) * 10.0f;
                s->y[i] = s->y[i-1] - (dy / dist) * 10.0f;
            }

    }
}

void randomColor(SDL_Renderer *r) //radominize color each secondes
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

    SDL_Window *window = SDL_CreateWindow( //creation et vérification de écran
        "Snake rectangles",
        100, 100,
        1000, 1000,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Snake snake;
    initSnake(&snake);

    int running = 1;//true
    SDL_Event e;

    while (running) //event loop(mouvement de serpant)
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

    SDL_DestroyRenderer(renderer); // fin de jeu
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}