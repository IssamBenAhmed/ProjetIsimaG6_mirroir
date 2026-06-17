### GAHUI MJ :
 gérer la perception de chaque cellule , gérer la grille fictive  et l'allumage des cellules selon le résultat reçu par les agents ( Issam ) , gérer le changement d'adresse entre grille fictive et grille affichée.
### NABIL GRILLE : 
 gérer la conception , l'esthétique de la grille, gérer les options utilisateurs ( zooom , allumage artificiel , déplacement sur la grille ).
### ISSAM AGENT :
 gérer les règles du jeu et le transfert de l'information(  l'allumage , le maintient , l'extinction )




# Architecture SMA — Spécifications du Jeu de la Vie (Projet ZZ1)

Ce document définit la répartition des rôles et les spécifications techniques des fonctions pour l'implémentation du Jeu de la Vie, selon une architecture Système Multi-Agents (SMA) réactive et synchrone.

---

## 1. Membre 2 : L'Agent (Le Cerveau Réactif)
L'agent est totalement isolé et aveugle. Son unique rôle est d'appliquer la logique locale de Conway à partir des données fournies par le Maître du Jeu (MJ).

### `agir_agent`

    int agir_agent(int etat_actuel, int nb_voisins_vivants);

* **Objectif :** Appliquer strictement les règles déterministes de survie, naissance et mort de Conway.
* **Arguments :**
  * `etat_actuel` : L'état de la cellule à l'instant *t* (0 pour mort, 1 pour vivant).
  * `nb_voisins_vivants` : La perception locale transmise par le MJ (entier de 0 à 8).
* **Sortie :** L'état souhaité pour l'instant *t+1* (1 si la cellule doit être allumée, 0 si elle doit être éteinte).

---

## 2. Membre 3 : Le Maître du Jeu (L'Orchestrateur)
Le MJ détient la vision globale de l'espace. Il calcule la perception des agents et assure la synchronicité stricte des transitions d'états.

### `compter_voisins`

    int compter_voisins(int grille[WIDTH][HEIGHT], int x, int y);

* **Objectif :** Calculer le voisinage de Moore autour d'une coordonnée cible en appliquant un modulo pour l'effet torique (monde sans bords).
* **Arguments :**
  * `grille` : La matrice simulant le monde à l'instant *t*.
  * `x`, `y` : Les coordonnées de la cellule à analyser.
* **Sortie :** Le nombre de voisins vivants (entier de 0 à 8).

### `maj_synchrone`

    void maj_synchrone(int grille_courante[WIDTH][HEIGHT], int grille_future[WIDTH][HEIGHT]);

* **Objectif :** Parcourir l'intégralité de la grille. Pour chaque case, récupérer le nombre de voisins via `compter_voisins`, interroger l'agent via `agir_agent`, et inscrire le résultat dans la grille de l'instant *t+1*.
* **Arguments :**
  * `grille_courante` : Matrice source active (instant *t*).
  * `grille_future` : Matrice tampon de destination (instant *t+1*).
* **Sortie :** Aucune.

> **Bascule de grille (Pointeurs) :** L'échange d'adresses pour le cycle suivant se fait dans la boucle principale du `main.c` par une permutation d'indices en $O(1)$ (`courant = 1 - courant;`).

---

## 3. Membre 1 : Le Graphisme & Interactions (L'Interface SDL2)
Ce module prend en charge le rendu visuel de la matrice et capture les actions de l'utilisateur sur la fenêtre.

### `dessiner_grille`

    void dessiner_grille(SDL_Renderer *renderer, int grille[WIDTH][HEIGHT], float zoom, int cam_x, int cam_y);

* **Objectif :** Effectuer le rendu graphique du monde en appliquant les transformations de la caméra (décalages spatiaux et facteur de zoom).
* **Arguments :**
  * `renderer` : Le gestionnaire de rendu SDL2.
  * `grille` : La matrice active à afficher.
  * `zoom` : Le niveau de zoom courant.
  * `cam_x`, `cam_y` : Les coordonnées de décalage de la vue.
* **Sortie :** Aucune.

### `gerer_interaction`

    void gerer_interaction(SDL_Event *event, int grille[WIDTH][HEIGHT], float zoom, int cam_x, int cam_y);

* **Objectif :** Intercepter les entrées souris (clics et molette) pour modifier le zoom, déplacer la caméra ou inverser artificiellement l'état d'une cellule en convertissant les pixels de l'écran en indices de la matrice.
* **Arguments :**
  * `event` : Le pointeur vers l'événement SDL capturé.
  * `grille` : La matrice modifiable en temps réel.
  * `zoom`, `cam_x`, `cam_y` : Les variables de configuration de la vue.
* **Sortie :** Aucune.