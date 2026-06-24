#!/usr/bin/env python3
"""
lire_theta.py — affiche la matrice theta (15 x 3) contenue dans un cerveau.bin.

Usage:
    python3 lire_theta.py chemin/vers/cerveau.bin

"""

import sys
import struct

NB_FEATURES = 15
NB_ACTIONS = 3

NOMS_FEATURES = [
    "phi0  distance_mur_devant",
    "phi1  distance_mur_gauche",
    "phi2  distance_mur_droite",
    "phi3  densite_avant_gauche",
    "phi4  densite_avant_droite",
    "phi5  densite_arriere_gauche",
    "phi6  densite_arriere_droite",
    "phi7  zone_adversaire_1",
    "phi8  zone_adversaire_2",
    "phi9  zone_adversaire_3",
    "phi10 densite_AG * densite_AD",
    "phi11 dist_devant * dist_gauche",
    "phi12 densite_ArG * densite_ArD",
    "phi13 dist_devant * densite_AD",
    "phi14 dist_droite * dist_gauche",
]

NOMS_ACTIONS = ["FORWARD", "LEFT", "RIGHT"]


def charger_theta(chemin):
    with open(chemin, "rb") as f:
        data = f.read()

    nb_floats = len(data) // 4
    attendu = NB_FEATURES * NB_ACTIONS

    if len(data) % 4 != 0:
        print(f"Attention : taille du fichier ({len(data)} octets) non multiple de 4, "
              f"le fichier semble corrompu ou n'est pas un cerveau theta brut.")

    if nb_floats != attendu:
        print(f"Attention : {nb_floats} flottants trouves, {attendu} attendus "
              f"({NB_FEATURES}x{NB_ACTIONS}). Affichage avec ce qui est disponible.\n")

    valeurs = struct.unpack("<" + "f" * nb_floats, data[:nb_floats * 4])

    # reconstruction en matrice 15x3 (ou moins si fichier tronque)
    theta = []
    for i in range(NB_FEATURES):
        ligne = []
        for j in range(NB_ACTIONS):
            idx = i * NB_ACTIONS + j
            ligne.append(valeurs[idx] if idx < nb_floats else None)
        theta.append(ligne)

    return theta


def afficher_theta(theta):
    largeur_nom = max(len(n) for n in NOMS_FEATURES) + 2
    largeur_col = 14

    # en-tete
    entete = " " * largeur_nom + "".join(f"{a:>{largeur_col}}" for a in NOMS_ACTIONS)
    print(entete)
    print("-" * len(entete))

    for i, ligne in enumerate(theta):
        nom = NOMS_FEATURES[i] if i < len(NOMS_FEATURES) else f"phi{i}"
        cellules = ""
        for val in ligne:
            if val is None:
                cellules += f"{'--':>{largeur_col}}"
            else:
                cellules += f"{val:>{largeur_col}.4f}"
        print(f"{nom:<{largeur_nom}}{cellules}")

    print()

    # petites stats utiles pour reperer une divergence (poids satures)
    toutes_valeurs = [v for ligne in theta for v in ligne if v is not None]
    if toutes_valeurs:
        v_min = min(toutes_valeurs)
        v_max = max(toutes_valeurs)
        v_abs_max = max(abs(v_min), abs(v_max))
        nb_zeros_exacts = sum(1 for v in toutes_valeurs if v == 0.0)

        print(f"min = {v_min:.4f}   max = {v_max:.4f}   |max abs| = {v_abs_max:.4f}")
        print(f"poids exactement a 0.0 : {nb_zeros_exacts} / {len(toutes_valeurs)}")

        if v_abs_max > 50:
            print("-> magnitude elevee (>50) : signe possible de saturation du softmax / divergence.")
        if nb_zeros_exacts > 0:
            print("-> des poids a 0.0 exact indiquent des features jamais mises a jour "
                  "(ACTIVE_FEATURES trop bas, ou features non encore activees).")


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 lire_theta.py chemin/vers/cerveau.bin")
        sys.exit(1)

    chemin = sys.argv[1]

    try:
        theta = charger_theta(chemin)
    except FileNotFoundError:
        print(f"Fichier introuvable : {chemin}")
        sys.exit(1)

    print(f"\nMatrice theta lue depuis : {chemin}\n")
    afficher_theta(theta)


if __name__ == "__main__":
    main()