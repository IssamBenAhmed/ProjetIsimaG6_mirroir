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

   #### BEN AHMED Issam
   J'ai ensuite commencer a m'informer sur les fonctions nécessaire à la conception du programme X_fenetré. Les fonctions de bases comme SDL_Init , SDL_CreateWindow ...
   Les débuts furent difficile car je n'avais pas l'habitude de manipuler de telles structures, mais ce fut enrichissant.
   Découvrir git était une épreuve à traverser en parrallèle du code, chaque memebre du groupe utilise cet outil à sa manière. 

   #### BAN Gahui
   J'ai commencé à programmer X-fenêtre. Pour cela, j'ai utilisé la fonction de création de window en stockant les fenêtres dans un tableau de 100 cases, et j'ai utilisé l'opération de boucle "while" pour afficher la fenêtre sur l'écran. Le serpent m'aide à apprendre à tracer une figure sur l'écran et à gérer le déplacement, ce qui est une bonne première étape pour le début du projet.


   #### LAAMARCHI Nabil
   J’ai commencé par programmer la partie X-fenêtre. Pour cela, j’ai utilisé la fonction SDL_CreateWindow afin de créer plusieurs fenêtres SDL, puis je les ai stockées dans un tableau pour pouvoir les manipuler facilement avec une boucle. Ensuite, j’ai utilisé une boucle principale while pour garder le programme ouvert et lire les événements du clavier. Grâce aux flèches, on peut déplacer toutes les fenêtres, avec la touche a on peut les agrandir, avec r on peut les réduire, et avec espace on peut les replacer sous forme de X. Le deuxième programme, qui dessine un homme avec des formes géométriques, m’a aidé à apprendre comment tracer des figures sur l’écran avec SDL, comme les cercles, les rectangles et les lignes. Il m’a aussi permis de comprendre le déplacement d’un objet dans une fenêtre.

### Mardi 16 Juin

Étapes préliminaires complétées : l'intégralité du groupe a terminé le bac à serpents, ainsi que les sprites, à quelques optimisations près.
L'affichage des sprites par déplacement de la zone d'affichage à la fois sur la source (selection de la vignette), et sur la destination ( déplacement sur la fenetre), nous aurons donner de bonne idées pour les derniers détail de notre jeu final.
Une réflexion sur la structure et les étapes de conception du jeu de la vie a été engagée, et certaines étapes seront préparées à l'avance.

#### BEN AHMED Issam
 L'animation des mouvements du serpent fut une grande source de problème pour ma part, il fut difficile d'imaginer la logique derrière un mouvement aussi harmonieux sans qu'il paraisse saccadé.
 La solution fut l'usage de valeurs aléatoire pour l'angle pris par la tête au début du mouvement,
 La tête est ensuite soumise à un mouvement sinusoidal par rapport au temps, récupéré par la fonction SDL_GetTicks
 Le passage des coordonées polaires aux coordonées cartésienne ayant été vu pendant l'année , il fut plaisant de réinvestir de telles conaissances.
 
 Pour ce qui est du mouvement du reste du corps on utilise une variable appelée SPACING : 
  pour tout segment du serpent excepté la tête, si le segment juste devant s'éloigne d'une distance euclidienne supérieure à SPACING, il est téléporté directement à une distance strictement égale à celle de SPACING, et ceux à chaque frame, ce qui donne un bel effet continu et fluide.
 Des pseudos-collisions on été crées : lorsque la tete du serpent atteint le bord de la fenetre une décison est prise pour inverser son angle et le faire partir de l'autre côté.

Ce fut aussi l'occasion de m'entrainer sur les changements de couleurs.

Durant l'affichage des sprite j'ai aussi appris à utiliser les texture,ce ne fut pas sans difficultée
L'utilisation d'une sprite sheet , avec un déplacement à la fois sur la feuille de sprite et sur la fenetre pour simuler un déplacement, a été nouveau mais intuitif pour ma part.


#### BAN Gahui

 Le programme « Serpent » a été finalisé et le travail sur l’animation des sprites a commencé. Cette phase m’a permis d’apprendre à afficher, superposer et déplacer des textures avec SDL2_image. La première étape consistait à superposer des images et à les modifier dynamiquement. La principale difficulté a été de comprendre certaines fonctions de SDL2_image et de corriger plusieurs erreurs de syntaxe. Cette étape était indispensable pour maîtriser l’affichage des textures et préparer les animations plus avancées. Ensuite, j’ai créé une fonction auxiliaire nommée « play_with_texture_3_alter » afin de réaliser le déplacement d’une image. Cette fonction permet notamment de produire un mouvement circulaire et de gérer l’apparition progressive de l’image grâce à la fonction « SDL_SetTextureAlphaMod ». La modélisation du mouvement et le réglage de sa vitesse ont constitué les principales difficultés, résolues à l’aide des fonctions trigonométriques cos et sin. Enfin, j’ai commencé l’animation par changement de sprite à chaque image affichée. Grâce aux fonctions « SDL_GetWindowSize » et « SDL_QueryTexture », j’ai pu sélectionner différentes parties d’une texture source et obtenir une animation fonctionnelle.
 

 
### Mercredi 17 Juin

 Le groupe a compris le principe d’un **SMA** : plusieurs agents isolés et aveugles qui agissent dans un même environnement selon des règles simples.Nous avons aussi compris le **Jeu de la Vie**, nous l’avons codé, puis nous avons commencé à travailler sur le deuxième jeu : **le jeu de Tron**.

 #### BEN AHMED Issam
 Après le mini cours en amphithéatre, le groupe avait une idée plus ou moins claire de ce qu'était un SMA et de son fonctionnement.
 À force de question auprès des professeurs et des camarades, nous étions parvenu à nous imaginer une division équitable du travail et à la compréhension globale du rôle du SMA dans le jeu de la vie : c'est lui qui joue le role d'automate , n'ayant aucune appréhension du monde mise à part ce que le maitre du jeu lui fournis.
 Son seul rôle est d'appliquer ses propres règles et de renvoyer une décision à prendre, et ceux pour chaque cellule de la grille.
 J'ai pu coder la partie agent sans soucis, avant d'apporter mon soutien a mes camarades dans leur partie respective.

 #### LAAMARCHI Nabil
  J’ai compris le principe d’un **SMA** et le fonctionnement du **Jeu de la Vie**, puis j’ai participé au codage du jeu.J’ai pris la tâche de **dessiner l’interface graphique du Jeu de la Vie**, et j’ai aussi commencé à travailler sur la partie **interface graphique du jeu de Tron**.

### Jeudi 18 juin

 Nous avons terminé le **jeu de Tron** avec et nous avons finalisé les dernières parties du projet.
 Nous avons aussi rédigé la **présentation de soutenance** afin d’expliquer notre travail, les jeux réalisés et les tâches de chacun.

#### LAAMARCHI Nabil
 J’ai codé **l’interface graphique du jeu de Tron** avec SDL2.J’ai aussi ajouté le contrôle du joueur avec le clavier, ce qui permet de déplacer la moto dans l’arène pendant le jeu.

#### BAN Gahui
 Le projet de création d’un jeu basé sur les SMA continue. J’ai distribué la première version de ma fonction permettant de calculer la perception devant, à gauche et à droite de la moto, puis d’afficher l’action choisie par l’agent dans le jeu. 
Comme nous utilisons une matrice pour représenter l’espace dans lequel évolue l’agent, j’ai réutilisé une partie de la fonction de calcul de la perception pour mettre à jour le déplacement de la moto, notamment le calcul du vecteur de déplacement en fonction de sa direction. Cependant, contrairement à ce que je pensais au départ, la direction est définie relativement à la vue de l’utilisateur et non à celle de la moto. J’ai donc dû corriger certaines parties de mon code afin que le déplacement de la moto ne dépende plus de son orientation. Par exemple, la direction « haut » correspond toujours au haut de l’écran du point de vue de l’utilisateur, et non à l’avant de la moto. 
Après ces modifications, le comportement du jeu semble correct. Ce qui m’a particulièrement surpris, c’est que j’ai pu simplifier mon code et le rendre plus clair et plus efficace en ajoutant quelques variables ainsi que des fonctions auxiliaires, notamment pour la gestion des vecteurs. De plus, les discussions avec les membres de mon groupe m’ont permis d’améliorer ma compréhension du fonctionnement et des concepts du jeu.

