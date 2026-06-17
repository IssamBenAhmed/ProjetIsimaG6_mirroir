# Architecture SMA — Spécifications de Tron (Light Cycles)

Ce document définit la répartition des rôles et les spécifications techniques des fonctions pour le projet Tron. Le monde est représenté par une matrice globale `int grille[WIDTH][HEIGHT]` où :
* `0` = Case vide
* `1` = Moto / Traînée du Joueur humain
* `2` = Moto / Traînée de l'Agent IA 1
* `3` = Moto / Traînée de l'Agent IA 2
* `4` = Moto / Traînée de l'Agent IA 3

---

## 1. ISSAM : L'Agent (Le Cerveau décisionnel & RL)
Ton rôle se concentre exclusivement sur l'intelligence et le choix des trajectoires. L'agent est isolé : il ne connaît pas sa position absolue sur la carte, il ne reçoit qu'un vecteur de perception locale (distances avant/gauche/droite) et décide de changer ou non de direction.

### `choisir_action`

    int choisir_action(float perception[3]);

* **Objectif :** Analyser l'espace libre disponible et renvoyer un choix de rotation. Cette fonction abritera d'abord des règles réflexes (heuristiques), puis le réseau de neurones REINFORCE.
* **Arguments :**
  * `perception` : Tableau de 3 flottants contenant la distance normalisée (ou le nombre de cases libres) dans 3 directions : `[0]` pour l'Avant, `[1]` pour la Gauche, `[2]` pour la Droite.
* **Sortie :** Un entier représentant l'action : `0` pour Continuer tout droit, `1` pour Tourner à gauche, `2` pour Tourner à droite.

---

## 2. GAHUI : Le Maître du Jeu (L'Orchestrateur du Monde)
Le MJ gère la physique, l'évaluation de l'espace, le déplacement des motos dans la matrice et le cycle de vie des joueurs.

### `calculer_perception`

    void calculer_perception(int grille[WIDTH][HEIGHT], int x, int y, int direction, float perception[3]);

* **Objectif :** Lancer 3 "rayons" (avant, gauche, droite) depuis la position d'une moto pour compter le nombre de cases vides avant un obstacle (mur ou traînée). Remplit le tableau destiné à l'Agent.
* **Arguments :** La grille, les coordonnées actuelles de la moto, sa direction courante (Haut/Bas/Gauche/Droite) et le tableau `perception` à remplir.

### `mettre_a_jour_monde`

    void mettre_a_jour_monde(int grille[WIDTH][HEIGHT], int pos_motos[5][2], int dir_motos[5], bool etats_vie[5]);

* **Objectif :** Faire avancer d'une case toutes les motos vivantes en fonction de leur direction.
  1. Vérifier si la case ciblée est libre (`0`). Si non, marquer la moto comme morte (`etats_vie = false`).
  2. Si la case est libre, y inscrire l'ID de la moto (`1` à `4`) pour marquer le nouveau mur.
  3. Mettre à jour le tableau des positions globales `pos_motos`.

### `nettoyer_trainee`

    void nettoyer_trainee(int grille[WIDTH][HEIGHT], int id_moto);

* **Objectif :** Lorsqu'une moto meurt, parcourir la matrice pour remplacer toutes les cases portant son `id_moto` par du vide (`0`), libérant ainsi l'espace pour les survivants.

---

## 3. NABIL : Le Graphisme & Entrées (L'Interface SDL2)
Le graphiste gère l'affichage complet du jeu, l'attribution des couleurs spécifiques à chaque joueur/IA, et capture les commandes du joueur humain.

### `dessiner_arene`

    void dessiner_arene(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT]);

* **Objectif :** Parcourir la matrice et dessiner l'arène à l'écran. Chaque ID numérique est associé à une couleur esthétique néon :
  * `0` : Noir (fond)
  * `1` : Bleu Cyan (Joueur)
  * `2` : Orange (IA 1)
  * `3` : Vert (IA 2)
  * `4` : Rose/Violet (IA 3)

### `capturer_evenements`

    void capturer_evenements(SDL_Event *event, int *direction_joueur, bool *running);

* **Objectif :** Écouter le clavier en continu. Si le joueur appuie sur une touche directionnelle (Flèches ou ZQSD), modifier la variable `direction_joueur` stockée dans le `main.c` (en interdisant le demi-tour instantané automatique qui provoquerait un suicide). Gérer également la touche Échap pour quitter.