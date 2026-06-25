# Documentation Doxygen du Projet

Nous avons séparé la documentation technique en 3 configurations distinctes pour plus de clarté :

1.  **Projets Personnels** (X fenêtré, Nid de serpent, Sprites)
    *   Configuration : `Doxyfile_projets_perso`
    *   Sortie HTML : `docs/doxygen/projets_perso/html/index.html`
2.  **Jeu de la Vie** (SMA Conway)
    *   Configuration : `Doxyfile_jeu_de_la_vie`
    *   Sortie HTML : `docs/doxygen/jeu_de_la_vie/html/index.html`
3.  **TronZZ** (Moteur multi-agents & Apprentissage par renforcement REINFORCE)
    *   Configuration : `Doxyfile_tronzz`
    *   Sortie HTML : `docs/doxygen/tronzz/html/index.html`

---

## Générer la documentation :

Pour re-générer les documentations à tout moment, exécutez la commande suivante à la racine du projet :

```bash
doxygen Doxyfile_projets_perso && doxygen Doxyfile_jeu_de_la_vie && doxygen Doxyfile_tronzz
```

## Consulter la documentation :

Ouvrez simplement l'un des fichiers `index.html` ci-dessus dans votre navigateur Web (ex: Firefox ou Chrome).


