// sprite animation fait par LAAMARCHI NABIL

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Programme : Chien avec parallaxe anim� et obstacle plein

    Id�e du programme :
    - On cr�e une fen�tre SDL.
    - On charge trois fonds avec parallaxe1.png, parallaxe2.png et
   parallaxe3.png.
    - parallaxe1.png repr�sente le ciel.
    - parallaxe2.png repr�sente les nuages.
    - parallaxe3.png repr�sente le sol.
    - Les fonds se d�placent lentement sur l'axe x avec des vitesses
   diff�rentes.
    - On charge un chien avec fonddeux.png.
    - Le chien est une image qui contient plusieurs positions du chien.
    - On affiche seulement une partie de l'image du chien � chaque fois.
    - Le chien se d�place avec les fl�ches du clavier.
    - On dessine un obstacle plein vert.
    - Si le chien touche l'obstacle :
        l'obstacle devient rouge,
        le score augmente de 1,
        le chien ne traverse pas l'obstacle.
    - q ou �chap permet de quitter le programme.
*/

#define LARGEUR_FENETRE 1000
#define HAUTEUR_FENETRE 550

#define VITESSE_CHIEN 6

#define VITESSE_PARALLAXE1 1
#define VITESSE_PARALLAXE2 2
#define VITESSE_PARALLAXE3 4

#define CHEVAUCHEMENT_FOND 3

#define HAUTEUR_NUAGES 220
#define POSITION_NUAGES_Y 50

#define HAUTEUR_SOL 130
#define POSITION_SOL_Y 420

#define NB_COLONNES_CHIEN 3
#define NB_LIGNES_CHIEN 3
#define NB_IMAGES_CHIEN 9

int main(void) {
  SDL_Window *window = NULL;     // Fen�tre principale.
  SDL_Renderer *renderer = NULL; // Outil pour dessiner dans la fen�tre.

  SDL_Texture *parallaxe1 = NULL; // Texture du ciel.
  SDL_Texture *parallaxe2 = NULL; // Texture des nuages.
  SDL_Texture *parallaxe3 = NULL; // Texture du sol.

  SDL_Texture *chien = NULL;       // Texture du chien.
  SDL_Texture *texte_score = NULL; // Texture du texte du score.

  TTF_Font *font = NULL; // Police utilis�e pour �crire le score.

  SDL_Event event;               // Variable qui re�oit les �v�nements.
  SDL_bool continuer = SDL_TRUE; // Variable qui garde le programme ouvert.

  SDL_Rect p1_fond1; // Premier rectangle du ciel.
  SDL_Rect p1_fond2; // Deuxi�me rectangle du ciel.

  SDL_Rect p2_fond1; // Premier rectangle des nuages.
  SDL_Rect p2_fond2; // Deuxi�me rectangle des nuages.

  SDL_Rect p3_fond1; // Premier rectangle du sol.
  SDL_Rect p3_fond2; // Deuxi�me rectangle du sol.

  SDL_Rect chien_rect;   // Rectangle du chien affich� � l'�cran.
  SDL_Rect source_chien; // Partie de l'image du chien � afficher.

  SDL_Rect obstacle;   // Rectangle de l'obstacle plein.
  SDL_Rect texte_rect; // Rectangle du texte.

  SDL_Color couleur_texte = {255, 255, 255,
                             255}; // Couleur blanche pour le score.

  int position_p1_x = 0; // Position horizontale du ciel.
  int position_p2_x = 0; // Position horizontale des nuages.
  int position_p3_x = 0; // Position horizontale du sol.

  int largeur_repetition = LARGEUR_FENETRE - CHEVAUCHEMENT_FOND;

  int ancien_x;                   // Ancienne position x du chien.
  int ancien_y;                   // Ancienne position y du chien.
  int score = 0;                  // Score du joueur.
  int collision = 0;              // Vaut 1 si le chien touche l'obstacle.
  int collision_deja_comptee = 0; // �vite d'ajouter trop de points.

  int chien_texture_w = 0;    // Largeur totale de l'image du chien.
  int chien_texture_h = 0;    // Hauteur totale de l'image du chien.
  int frame_largeur = 0;      // Largeur d'une seule image du chien.
  int frame_hauteur = 0;      // Hauteur d'une seule image du chien.
  int frame_actuelle = 0;     // Image actuelle du chien.
  int compteur_animation = 0; // Compteur pour ralentir l'animation.
  int chien_bouge = 0;        // Vaut 1 si le chien bouge.

  SDL_RendererFlip flip_chien =
      SDL_FLIP_NONE; // Sert � retourner le chien � gauche.

  char texte[50]; // Texte qui contient le score.

  /*
      D�marrage de SDL.
  */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Erreur SDL_Init : %s\n", SDL_GetError());
    return 1;
  }

  /*
      D�marrage de SDL_image pour charger les images PNG et JPG.
  */
  if ((IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)) ==
      0) {
    printf("Erreur IMG_Init : %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
  }

  /*
      D�marrage de SDL_ttf pour �crire le score.
  */
  if (TTF_Init() < 0) {
    printf("Erreur TTF_Init : %s\n", TTF_GetError());
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  /*
      Cr�ation de la fen�tre.
  */
  window = SDL_CreateWindow("Chien avec parallaxe - obstacle plein",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            LARGEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Erreur creation fenetre : %s\n", SDL_GetError());
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  /*
      Cr�ation du renderer.
  */
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    printf("Erreur creation renderer : %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  /*
      Chargement des images.

      Il faut avoir dans le m�me dossier :
      parallaxe1.png  : ciel
      parallaxe2.png  : nuages
      parallaxe3.png  : sol
      fonddeux.png    : image du chien avec plusieurs mouvements
      font.ttf
  */
  parallaxe1 = IMG_LoadTexture(renderer, "parallaxe1.png");

  if (parallaxe1 == NULL) {
    printf("Erreur chargement parallaxe1.png : %s\n", IMG_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  parallaxe2 = IMG_LoadTexture(renderer, "parallaxe2.png");

  if (parallaxe2 == NULL) {
    printf("Erreur chargement parallaxe2.png : %s\n", IMG_GetError());
    SDL_DestroyTexture(parallaxe1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  parallaxe3 = IMG_LoadTexture(renderer, "parallaxe3.png");

  if (parallaxe3 == NULL) {
    printf("Erreur chargement parallaxe3.png : %s\n", IMG_GetError());
    SDL_DestroyTexture(parallaxe2);
    SDL_DestroyTexture(parallaxe1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  chien = IMG_LoadTexture(renderer, "fonddeux.png");

  if (chien == NULL) {
    printf("Erreur chargement fonddeux.png : %s\n", IMG_GetError());
    SDL_DestroyTexture(parallaxe3);
    SDL_DestroyTexture(parallaxe2);
    SDL_DestroyTexture(parallaxe1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  /*
      On active la transparence pour les images qui ont un fond transparent.
  */
  SDL_SetTextureBlendMode(parallaxe2, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(chien, SDL_BLENDMODE_BLEND);

  /*
      On r�cup�re la taille totale de l'image du chien.

      L'image du chien contient 3 colonnes et 3 lignes.
      Donc il y a 9 petites images du chien.
  */
  SDL_QueryTexture(chien, NULL, NULL, &chien_texture_w, &chien_texture_h);

  frame_largeur = chien_texture_w / NB_COLONNES_CHIEN;
  frame_hauteur = chien_texture_h / NB_LIGNES_CHIEN;

  /*
      Au d�but, on affiche la premi�re partie de l'image du chien.
  */
  source_chien.x = 0;
  source_chien.y = 0;
  source_chien.w = frame_largeur;
  source_chien.h = frame_hauteur;

  /*
      Chargement de la police pour le score.
  */
  font = TTF_OpenFont("font.ttf", 32);

  if (font == NULL) {
    printf("Erreur chargement font.ttf : %s\n", TTF_GetError());
    SDL_DestroyTexture(chien);
    SDL_DestroyTexture(parallaxe3);
    SDL_DestroyTexture(parallaxe2);
    SDL_DestroyTexture(parallaxe1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  /*
      Initialisation des deux fonds de la premi�re couche.

      parallaxe1.png repr�sente le ciel.
      Le ciel prend toute la fen�tre.
  */
  p1_fond1.x = 0;
  p1_fond1.y = 0;
  p1_fond1.w = LARGEUR_FENETRE + CHEVAUCHEMENT_FOND;
  p1_fond1.h = HAUTEUR_FENETRE;

  p1_fond2.x = largeur_repetition;
  p1_fond2.y = 0;
  p1_fond2.w = LARGEUR_FENETRE + CHEVAUCHEMENT_FOND;
  p1_fond2.h = HAUTEUR_FENETRE;

  /*
      Initialisation des deux fonds de la deuxi�me couche.

      parallaxe2.png repr�sente les nuages.
      Les nuages sont affich�s en haut.
      Important : l'image des nuages doit avoir un fond transparent.
  */
  p2_fond1.x = 0;
  p2_fond1.y = POSITION_NUAGES_Y;
  p2_fond1.w = LARGEUR_FENETRE + CHEVAUCHEMENT_FOND;
  p2_fond1.h = HAUTEUR_NUAGES;

  p2_fond2.x = largeur_repetition;
  p2_fond2.y = POSITION_NUAGES_Y;
  p2_fond2.w = LARGEUR_FENETRE + CHEVAUCHEMENT_FOND;
  p2_fond2.h = HAUTEUR_NUAGES;

  /*
      Initialisation des deux fonds de la troisi�me couche.

      parallaxe3.png repr�sente le sol.
      Le sol ne prend pas toute la fen�tre.
      Il est affich� seulement en bas.
  */
  p3_fond1.x = 0;
  p3_fond1.y = POSITION_SOL_Y;
  p3_fond1.w = LARGEUR_FENETRE + CHEVAUCHEMENT_FOND;
  p3_fond1.h = HAUTEUR_SOL;

  p3_fond2.x = largeur_repetition;
  p3_fond2.y = POSITION_SOL_Y;
  p3_fond2.w = LARGEUR_FENETRE + CHEVAUCHEMENT_FOND;
  p3_fond2.h = HAUTEUR_SOL;

  /*
      Initialisation du chien.
  */
  chien_rect.x = 100;
  chien_rect.y = 330;
  chien_rect.w = 140;
  chien_rect.h = 90;

  /*
      Initialisation de l'obstacle plein.
  */
  obstacle.x = 720;
  obstacle.y = 260;
  obstacle.w = 180;
  obstacle.h = 160;

  printf("Commandes :\n");
  printf("Fleches du clavier : deplacer le chien\n");
  printf("q ou Echap : quitter\n");

  /*
      Boucle principale.
  */
  while (continuer) {
    /*
        Lecture des �v�nements.
    */
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        continuer = SDL_FALSE;
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE ||
            event.key.keysym.sym == SDLK_q) {
          continuer = SDL_FALSE;
        }
      }
    }

    /*
        On garde l'ancienne position du chien.

        Si le chien touche l'obstacle,
        on le remet � cette ancienne position.
    */
    ancien_x = chien_rect.x;
    ancien_y = chien_rect.y;

    /*
        Au d�but de chaque tour, on consid�re que le chien ne bouge pas.
        Si une fl�che est appuy�e, on mettra chien_bouge � 1.
    */
    chien_bouge = 0;

    /*
        D�placement fluide avec les touches maintenues.
    */
    const Uint8 *touches = SDL_GetKeyboardState(NULL);

    if (touches[SDL_SCANCODE_UP]) {
      chien_rect.y = chien_rect.y - VITESSE_CHIEN;
      chien_bouge = 1;
    }

    if (touches[SDL_SCANCODE_DOWN]) {
      chien_rect.y = chien_rect.y + VITESSE_CHIEN;
      chien_bouge = 1;
    }

    if (touches[SDL_SCANCODE_LEFT]) {
      chien_rect.x = chien_rect.x - VITESSE_CHIEN;
      chien_bouge = 1;
      flip_chien = SDL_FLIP_HORIZONTAL;
    }

    if (touches[SDL_SCANCODE_RIGHT]) {
      chien_rect.x = chien_rect.x + VITESSE_CHIEN;
      chien_bouge = 1;
      flip_chien = SDL_FLIP_NONE;
    }

    /*
        Animation du chien.

        Si le chien bouge, on change la partie de l'image � afficher.
        Si le chien ne bouge pas, on revient � la premi�re image.
    */
    if (chien_bouge) {
      compteur_animation++;

      if (compteur_animation >= 6) {
        frame_actuelle = (frame_actuelle + 1) % NB_IMAGES_CHIEN;
        compteur_animation = 0;
      }
    } else {
      frame_actuelle = 0;
      compteur_animation = 0;
    }

    /*
        Calcul de la partie de l'image du chien � afficher.

        L'image est d�coup�e en 3 colonnes et 3 lignes.
    */
    source_chien.x = (frame_actuelle % NB_COLONNES_CHIEN) * frame_largeur;
    source_chien.y = (frame_actuelle / NB_COLONNES_CHIEN) * frame_hauteur;
    source_chien.w = frame_largeur;
    source_chien.h = frame_hauteur;

    /*
        Emp�cher le chien de sortir de la fen�tre.
    */
    if (chien_rect.x < 0) {
      chien_rect.x = 0;
    }

    if (chien_rect.y < 0) {
      chien_rect.y = 0;
    }

    if (chien_rect.x + chien_rect.w > LARGEUR_FENETRE) {
      chien_rect.x = LARGEUR_FENETRE - chien_rect.w;
    }

    if (chien_rect.y + chien_rect.h > HAUTEUR_FENETRE) {
      chien_rect.y = HAUTEUR_FENETRE - chien_rect.h;
    }

    /*
        Test de collision entre le chien et l'obstacle.
    */
    collision = SDL_HasIntersection(&chien_rect, &obstacle);

    if (collision) {
      /*
          Le chien ne peut pas traverser l'obstacle.
          Donc on revient � l'ancienne position.
      */
      chien_rect.x = ancien_x;
      chien_rect.y = ancien_y;

      /*
          On ajoute 1 point seulement au d�but du contact.
          Si on garde la touche appuy�e, le score ne monte pas trop vite.
      */
      if (collision_deja_comptee == 0) {
        score++;
        collision_deja_comptee = 1;
        printf("Collision avec l'obstacle ! Score : %d\n", score);
      }
    } else {
      /*
          Quand le chien ne touche plus l'obstacle,
          on pourra recompter une prochaine collision.
      */
      collision_deja_comptee = 0;
    }

    /*
        D�placement horizontal lent des fonds.

        La premi�re couche, le ciel, part vers la gauche lentement.
        La deuxi�me couche, les nuages, part vers la gauche un peu plus vite.
        La troisi�me couche, le sol, part vers la gauche encore plus vite.

        C'est ce mouvement diff�rent qui donne l'effet de parallaxe.
    */
    position_p1_x = position_p1_x - VITESSE_PARALLAXE1;
    position_p2_x = position_p2_x - VITESSE_PARALLAXE2;
    position_p3_x = position_p3_x - VITESSE_PARALLAXE3;

    if (position_p1_x <= -largeur_repetition) {
      position_p1_x = position_p1_x + largeur_repetition;
    }

    if (position_p2_x <= -largeur_repetition) {
      position_p2_x = position_p2_x + largeur_repetition;
    }

    if (position_p3_x <= -largeur_repetition) {
      position_p3_x = position_p3_x + largeur_repetition;
    }

    p1_fond1.x = position_p1_x;
    p1_fond2.x = position_p1_x + largeur_repetition;

    p2_fond1.x = position_p2_x;
    p2_fond2.x = position_p2_x + largeur_repetition;

    p3_fond1.x = position_p3_x;
    p3_fond2.x = position_p3_x + largeur_repetition;

    /*
        Cr�ation du texte du score.
        On d�truit l'ancien texte avant d'en cr�er un nouveau.
    */
    if (texte_score != NULL) {
      SDL_DestroyTexture(texte_score);
      texte_score = NULL;
    }

    sprintf(texte, "Score : %d", score);

    SDL_Surface *surface_texte =
        TTF_RenderText_Blended(font, texte, couleur_texte);

    if (surface_texte != NULL) {
      texte_score = SDL_CreateTextureFromSurface(renderer, surface_texte);

      texte_rect.x = 20;
      texte_rect.y = 20;
      texte_rect.w = surface_texte->w;
      texte_rect.h = surface_texte->h;

      SDL_FreeSurface(surface_texte);
    }

    /*
        Affichage.
    */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /*
        Afficher les deux fonds de la premi�re couche.

        parallaxe1.png = ciel.
    */
    SDL_RenderCopy(renderer, parallaxe1, NULL, &p1_fond1);
    SDL_RenderCopy(renderer, parallaxe1, NULL, &p1_fond2);

    /*
        Afficher les deux fonds de la deuxi�me couche.

        parallaxe2.png = nuages.
    */
    SDL_RenderCopy(renderer, parallaxe2, NULL, &p2_fond1);
    SDL_RenderCopy(renderer, parallaxe2, NULL, &p2_fond2);

    /*
        Afficher les deux fonds de la troisi�me couche.

        parallaxe3.png = sol.
        Le sol est seulement en bas.
    */
    SDL_RenderCopy(renderer, parallaxe3, NULL, &p3_fond1);
    SDL_RenderCopy(renderer, parallaxe3, NULL, &p3_fond2);

    /*
        Afficher le chien.

        On n'affiche pas toute l'image du chien.
        On affiche seulement la partie indiqu�e par source_chien.
    */
    SDL_RenderCopyEx(renderer, chien, &source_chien, &chien_rect, 0, NULL,
                     flip_chien);

    /*
        Dessiner l'obstacle plein.

        Vert : pas de collision.
        Rouge : collision.
    */
    if (collision) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }

    SDL_RenderFillRect(renderer, &obstacle);

    /*
        Afficher le score.
    */
    if (texte_score != NULL) {
      SDL_RenderCopy(renderer, texte_score, NULL, &texte_rect);
    }

    /*
        Montrer le r�sultat dans la fen�tre.
    */
    SDL_RenderPresent(renderer);

    /*
        Petite pause pour un mouvement fluide.
    */
    SDL_Delay(16);
  }

  /*
      Fermeture propre.
  */
  if (texte_score != NULL) {
    SDL_DestroyTexture(texte_score);
  }

  TTF_CloseFont(font);

  SDL_DestroyTexture(chien);

  SDL_DestroyTexture(parallaxe3);
  SDL_DestroyTexture(parallaxe2);
  SDL_DestroyTexture(parallaxe1);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}