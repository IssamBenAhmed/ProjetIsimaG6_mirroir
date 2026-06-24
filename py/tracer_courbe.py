import pandas as pd
import matplotlib.pyplot as plt

# 1. Chargement des donnees (adapte le nom du fichier selon tes besoins)
colonnes = ['episode', 'temps_sec', 'survie_moyenne', 'survie_max', 'taux_victoire', 'g0_moyen', 'pct_forward']
fichier_entree = 'log_15_perceptions.csv'

try:
    df = pd.read_csv(fichier_entree, names=colonnes, header=None)
    df['episode'] = pd.to_numeric(df['episode'], errors='coerce')
    df = df.dropna()
except FileNotFoundError:
    print(f"Erreur : le fichier {fichier_entree} est introuvable.")
    exit()

# 2. Calcul de la moyenne mobile exponentielle (m)
# alpha = 0.05 est le standard recommande par ton prof
df['m'] = df['g0_moyen'].ewm(alpha=0.05, adjust=False).mean()

# 3. Export du nouveau tableau avec la colonne 'm'
fichier_sortie = 'logs_finaux_avec_m.csv'
df.to_csv(fichier_sortie, index=False)
print(f"Tableau sauvegarde avec succes sous : {fichier_sortie}")

# 4. Tracage de la courbe de convergence
plt.figure(figsize=(12, 6))

# La courbe brute en arriere-plan (transparente) pour montrer le bruit
plt.plot(df['episode'], df['g0_moyen'], label='G0 Brut', alpha=0.2, color='gray')

# La courbe lissee (m) en evidence
plt.plot(df['episode'], df['m'], label='Score Lissé (m, alpha=0.05)', color='red', linewidth=2)

plt.title('Convergence de l\'IA Smatron (210k épisodes)')
plt.xlabel('Épisodes')
plt.ylabel('Score de Récompense')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.5)

# Sauvegarde l'image pour le powerpoint
plt.savefig('courbe_convergence_finale.png', dpi=300, bbox_inches='tight')
print("Graphique sauvegarde sous : courbe_convergence_finale.png")

plt.show()