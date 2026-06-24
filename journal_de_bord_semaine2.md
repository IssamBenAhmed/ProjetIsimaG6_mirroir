# Journal de bord du projet 2026


# Aprentissage automatique : REINFORCE sur le jeu de TRONZZ

  ## Contexte
  Ce journal de bord retrace l’avancement de notre projet TronZZ, qui consiste à développer un jeu de motos lumineuses en C/SDL2, en passant d’une intelligence artificielle réactive à une version améliorée utilisant l’apprentissage par renforcement avec REINFORCE. 
![Schéma REINFORCE](image/image.webp)
  ## Objectifs

  
* Comprendre le principe de l’apprentissage par renforcement.
* Étudier la méthode REINFORCE et son fonctionnement.
* Appliquer la méthode REINFORCE à notre jeu TronZZ afin d’améliorer le comportement des agents.
* Comprendre le principe de la parallélisation dans un programme.
* Créer deux versions du projet : une version sans parallélisation et une version avec parallélisation.
* Mesurer les temps d’exécution des différentes parties du programme.
* Comparer les performances entre la version sans parallélisation et la version avec parallélisation.
* Analyser les résultats obtenus, les limites rencontrées et les pistes d’amélioration possibles.

  
 

  ## Suivi des travaux 

  ### Lundi 22 juin
  Tous le groupe a compris le principe du REINFORCE et on a commencé de l'implémenter. 
  #### Nabil 
  J'ai commencé par comprendre le principe général de l’apprentissage automatique, en particulier l’idée qu’un agent   peut améliorer son comportement à partir de ses expériences.J'ai ensuite réfléchi à plusieurs façons d’appliquer cette idée à notre jeu TronZZ, notamment en utilisant les actions, les récompenses et les trajectoires des agents. Cette réflexion a permis de préparer les premières idées pour intégrer l’apprentissage par renforcement dans le projet.


![Schéma REINFORCE](image/lundi.png.jpeg)
  #### Gahui 

  #### Issam

  ### Mardi 23 juin
  On a fini l'écriture du code du REINFORCE, on a corrigé des erreurs de compilation et on a lancé le trainage dans le soir.

  #### Nabil
  J'ai intégré le principe de l’effet néon dans l’interface graphique du jeu afin de donner un style plus proche de l’univers Tron. J'ai également ajouté un écran d’entrée permettant de démarrer la partie avec la touche **Espace** ou **Entrée**. Enfin, J'ai mis en place un écran de fin qui affiche le résultat de la partie avec un message **WIN** ou **LOSE**.J'ai également amélioré l’affichage des cellules pleines en ajoutant deux couches de lueur autour de chaque cellule ainsi qu’un point blanc au centre.

  