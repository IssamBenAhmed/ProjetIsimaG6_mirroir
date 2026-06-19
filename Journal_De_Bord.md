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

Laamarchi Nabil: J’ai commencé par programmer la partie X-fenêtre. Pour cela, j’ai utilisé la fonction SDL_CreateWindow afin de créer plusieurs fenêtres SDL, puis je les ai stockées dans un tableau pour pouvoir les manipuler facilement avec une boucle. Ensuite, j’ai utilisé une boucle principale while pour garder le programme ouvert et lire les événements du clavier. Grâce aux flèches, on peut déplacer toutes les fenêtres, avec la touche a on peut les agrandir, avec r on peut les réduire, et avec espace on peut les replacer sous forme de X. Le deuxième programme, qui dessine un homme avec des formes géométriques, m’a aidé à apprendre comment tracer des figures sur l’écran avec SDL, comme les cercles, les rectangles et les lignes. Il m’a aussi permis de comprendre le déplacement d’un objet dans une fenêtre.

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

