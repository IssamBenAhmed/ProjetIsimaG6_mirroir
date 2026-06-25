import os
import pandas as pd
import matplotlib.pyplot as plt

# Définition des chemins des fichiers logs
data_dir = "/home/issam/Bureau/Project_LinFolder/projetisimag6/TronZZ/SMA_REINFORCE/SMA_REINFORCE_SANS PARALLELISATION/data"
files = {
    "3 Perceptions (Rayons)": "logs_3_perception.csv",
    "10 Perceptions (Rayons + Zones + Radar)": "log_10_perception.csv",
    "15 Perceptions (Complet)": "log_15_perceptions.csv"
}

# Style du graphique (Premium Dark Mode)
plt.style.use('dark_background')
fig, ax = plt.subplots(figsize=(12, 7))

# Couleurs néon assorties
colors = {
    "3 Perceptions (Rayons)": "#ff007f",      # Rose néon
    "10 Perceptions (Rayons + Zones + Radar)": "#00f0ff", # Cyan néon
    "15 Perceptions (Complet)": "#39ff14"     # Vert néon
}

# Coefficient de lissage donné par le professeur (EMA alpha)
smoothing_alpha = 0.05

for label, filename in files.items():
    filepath = os.path.join(data_dir, filename)
    if not os.path.exists(filepath):
        print(f"Fichier absent: {filepath}")
        continue
    
    # Lecture du CSV sans en-tête
    df = pd.read_csv(filepath, header=None, names=[
        'episode', 'temps_sec', 'survie_moyenne', 'survie_max', 'taux_victoire', 'g0_moyen', 'pct_forward'
    ])
    
    # 1. Calcul du retour non-atténué (sans gamma) d'après la structure de récompense du C :
    # - Safe step : +2.0
    # - Mort : -100.0 (perte) / Victoire : +100.0
    # somme_R ≈ 2 * survie_moyenne - 100.0 * (1 - win_rate/100) + 100.0 * (win_rate/100)
    # somme_R ≈ 2 * survie_moyenne - 100.0 + 2 * win_rate
    df['undiscounted_return'] = 2.0 * df['survie_moyenne'] - 100.0 + 2.0 * df['taux_victoire']
    
    # 2. Application de la formule du prof (Moyenne dynamique exponentielle - EMA)
    # m_t = x_t * alpha + m_{t-1} * (1 - alpha)
    # On utilise ewm de pandas avec adjust=False pour correspondre exactement à la relation de récurrence
    df['m'] = df['undiscounted_return'].ewm(alpha=smoothing_alpha, adjust=False).mean()
    
    # Plot de la courbe brute en très transparent (pour montrer la variance)
    ax.plot(df['episode'], df['undiscounted_return'], color=colors[label], alpha=0.15, linestyle='-')
    
    # Plot de la moyenne dynamique m du prof en ligne pleine et épaisse
    ax.plot(df['episode'], df['m'], color=colors[label], linewidth=2.5, label=f"{label} (Moyenne dynamique $m$)")

# Habillage esthétique
ax.set_title("Évolution de l'apprentissage (Moyenne dynamique $m$ des Récompenses)", fontsize=16, fontweight='bold', pad=20, color='#ffffff')
ax.set_xlabel("Épisodes d'entraînement", fontsize=12, labelpad=10, color='#cccccc')
ax.set_ylabel("Moyenne dynamique $m$ (Reward Sum)", fontsize=12, labelpad=10, color='#cccccc')

# Grille discrète
ax.grid(True, linestyle='--', alpha=0.2, color='#555555')

# Légende premium
legend = ax.legend(loc='lower right', fontsize=11, framealpha=0.8, facecolor='#111111', edgecolor='#444444')
for text in legend.get_texts():
    text.set_color('#ffffff')

# Limites et mise en page
ax.set_xlim(0, 210000)
plt.tight_layout()

# Sauvegarde de l'image
output_path = os.path.join(data_dir, "learning_curves_m.png")
plt.savefig(output_path, dpi=300)
print(f"Graphique sauvegardé avec succès dans: {output_path}")
