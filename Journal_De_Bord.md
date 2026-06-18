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
 J'ai commencé à programmer X-fenêtre. Pour cela, j'ai utilisé la fonction de création de window en stockant les fenêtres dans un tableau de 100 cases, et j'ai utilisé l'opération de boucle "while" pour afficher la fenêtre sur l'écran. Le serpent m'aide à apprendre à tracer une figure sur l'écran et à gérer le déplacement, ce qui est une bonne première étape pour le début du projet.


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

