# Journal de bord du projet 2026

# Systèmes multi agents et apprentissage de politique stochastique

## Contexte

Ce journal de bord suit l'avancement du projet, les décisions techniques, les expérimentations et les difficultés rencontrées au fil du temps.

## Objectifs

- Comprendre les principes des systèmes multi-agents.
- Étudier l'apprentissage de politique stochastique.
- Mettre en place une base expérimentale pour tester des stratégies d'apprentissage.
- Documenter les résultats, les limites et les pistes d'amélioration.

## Suivi des travaux

### Lundi 15 Juin
 
Dans un premier temps, le groupe n'étant pas famillier avec la technologie SDL2 , nous nous sommes mis à faire des recherches sur les bases primaires de son fonctionnement. Nous avons aussi mis en place un repo sur le gitlab d'isima, qui nous permettra de travailler sur des fichiers communs. 

#### Ben Ahmed Issam
 J'ai ensuite commencer a m'informer sur les fonctions nécessaire à la conception du programme X_fenetré. Les fonctions de bases comme SDL_Init , 

#### Gahui Ban 
La programmation X-fenêtre a été commencée. Premièrement, j’ai utilisé la fonction classique telle que “SDL_CreateWindow” pour la création d’une fenêtre, et utilisé l’opération boucle “for” pour afficher plusieurs fenêtres sur l’écran. Une fois l’affichage de plusieurs fenêtres sur l’écran marche bien, j’ai changé de “for” en “while” pour que la fenêtre s’arrête de s’afficher lorsqu'elle atteint le bord de l’écran. Puis, à chaque boucle, j’ajoute une valeur pour les coordonnées de fenêtre à s’afficher pour qu’il se déplace à la boucle suivante. Pour chaque diagonale à afficher sur l’écran, 2 fonctions différentes étaient nécessaires pour faciliter la modification de la position de fenêtre. Le problème principal que j’ai rencontré est que la libération des fenêtres, car au début, le programme ne libère qu’une seule fenêtre. Donc j’ai créé une table de fenêtres avec la quantité de fenêtres limitée pour ne pas faire une boucle d’affichage infinie et la libère en utilisant “for”. À la fin, j’ai obtenu bien le résultat et le programme libère aussi bien toutes les fenêtres.
Pour le serpent, j’ai commencé d’abord de modéliser le serpent : une structure serpent avec sa direction, segment et son angle de tournement. Ensuite, la fonction de modéliser un serpent sous forme d’une série de rectangles d’après les variables “SDLRECT” et “SDL_RENDERFILLRECT”. Pour la couleur de serpent, grâce à la fonction rand, la couleur est choisie au hasard, puis cette couleur est peinte au rectangle contenant le serpent. Pour le mouvement de serpent, c’est le premier rectangle (dont la tête) qui choisit sa position, puis les restes les surviennent, en suivant son précédent. Ici, j’ai utilisé le mouvement grâce à la fonction cos et sin. Le problème principal que j’ai rencontré est la façon de modéliser un mouvement fluide de serpent. À la première, j’ai utilisé une téléportation immédiate d’une case mais ça rendait le mouvement quand même rigide. Pour résoudre cela, j’ai créé une fonction où il laisse l’éloignement minuscule entre 2 rectangles, avec cette distance à calculer à chaque position de ces 2 rectangles, puis s’ils s’éloignent trop, on tire un des 2 en changeant sa position : 
À l’aide de ce programme, le mouvement devient plus fluide. À la fin, le résultat final montre un mouvement assez fluide de serpent.   

### Laamarchi Nabil
La programmation X-fenêtre a été réalisée avec la bibliothèque SDL2. Premièrement, j’ai commencé par initialiser SDL avec `SDL_Init`, puis j’ai récupéré la taille de l’écran grâce à `SDL_GetCurrentDisplayMode`, car les positions des fenêtres dépendent directement de la largeur et de la hauteur de l’écran. Ensuite, j’ai créé plusieurs fenêtres en utilisant `SDL_CreateWindow`. Pour éviter d’écrire chaque fenêtre une par une, j’ai utilisé un tableau de fenêtres `SDL_Window *fenetres[NB_FENETRES]`, ce qui permet de stocker toutes les fenêtres au même endroit et de les manipuler facilement avec des boucles `for`.Après la création des fenêtres, j’ai calculé les distances entre elles avec les variables `pas_x` et `pas_y`. Ces deux variables permettent de placer les fenêtres régulièrement sur l’écran. Pour former la lettre X, j’ai utilisé deux boucles : la première place les fenêtres sur la diagonale qui va du haut gauche vers le bas droit, et la deuxième place les fenêtres sur la diagonale inverse, du haut droit vers le bas gauche. Pour éviter d’avoir deux fenêtres superposées au centre du X, j’ai sauté la fenêtre du milieu dans la deuxième diagonale avec une condition `if`.Ensuite, j’ai ajouté une boucle principale `while` pour garder le programme ouvert et attendre les actions de l’utilisateur. Les événements sont récupérés avec `SDL_PollEvent`. Si l’utilisateur appuie sur les flèches du clavier, toutes les fenêtres se déplacent ensemble vers le haut, le bas, la gauche ou la droite grâce à `SDL_GetWindowPosition` et `SDL_SetWindowPosition`. J’ai aussi ajouté la touche `a` pour agrandir toutes les fenêtres avec `SDL_SetWindowSize`, et la touche `r` pour les réduire. Pour éviter que les fenêtres deviennent trop petites, j’ai utilisé des limites minimales avec `LARGEUR_MIN` et `HAUTEUR_MIN`.J’ai également ajouté la touche espace pour replacer toutes les fenêtres correctement en forme de X. Cette partie est importante, car si l’utilisateur a déplacé, agrandi ou réduit les fenêtres, le programme peut recalculer les bonnes positions et reformer le X proprement. La touche `q` ou `Échap` permet de quitter le programme. J’ai aussi utilisé `SDL_Delay(10)` pour ralentir un peu la boucle principale afin d’éviter que le processeur travaille inutilement.Le problème principal rencontré était la gestion de plusieurs fenêtres en même temps, surtout au moment de les libérer. Au début, il était facile de détruire une seule fenêtre, mais comme le programme utilise 29 fenêtres, il fallait toutes les détruire proprement. Pour résoudre ce problème, j’ai utilisé une boucle `for` à la fin du programme avec `SDL_DestroyWindow`, ce qui permet de libérer chaque fenêtre une par une. À la fin, le programme affiche correctement les fenêtres sous forme de X, permet de les déplacer, de les agrandir, de les réduire, de les replacer, puis libère correctement toutes les fenêtres avant de fermer SDL.


### Mardi 16 Juin

Étapes préliminaires complétées : l'intégralité du groupe a terminé le bac à serpents, ainsi que les sprites, à quelques optimisations près.
Une réflexion sur la structure et les étapes de conception du jeu de la vie a été engagée, et certaines étapes seront préparées à l'avance.

gahui ban :  le programme “serpent ” a été finalisé et l’animation commence. Cela m’entraine d’afficher, superposer, déplacer avec le mouvement de Sprite ou non. 

 
### Mercredi 17 Juin

Le groupe a compris le principe d’un **SMA** : plusieurs agents qui agissent dans un même environnement selon des règles simples.Nous avons aussi compris le **Jeu de la Vie**, nous l’avons codé, puis nous avons commencé à travailler sur le deuxième jeu : **le jeu de Tron**.

LAAMARCHI Nabil:J’ai compris le principe d’un **SMA** et le fonctionnement du **Jeu de la Vie**, puis j’ai participé au codage du jeu.J’ai pris la tâche de **dessiner l’interface graphique du Jeu de la Vie**, et j’ai aussi commencé à travailler sur la partie **interface graphique du jeu de Tron**.

### Jeudi 18 juin

Nous avons terminé le **jeu de Tron** avec et nous avons finalisé les dernières parties du projet.
Nous avons aussi rédigé la **présentation de soutenance** afin d’expliquer notre travail, les jeux réalisés et les tâches de chacun.

LAAMARCHI Nabil:J’ai codé **l’interface graphique du jeu de Tron** avec SDL2.J’ai aussi ajouté le contrôle du joueur avec le clavier, ce qui permet de déplacer la moto dans l’arène pendant le jeu.

