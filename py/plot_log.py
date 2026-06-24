import pandas as pd
import matplotlib.pyplot as plt

# charger les donnees
data = pd.read_csv('SMA_REINFORCE_SANS PARALLELISATION/data/logs.csv')

# creer un dashboard 2x2
fig, axs = plt.subplots(2, 2, figsize=(14, 10))
fig.suptitle('evolution de l\'ia selon l\'ajout de perceptions (200k episodes)', fontsize=16, fontweight='bold')

# fonction utilitaire pour tracer les lignes de changement de phase
def ajouter_marqueurs(ax):
    ax.axvline(x=10000, color='red', linestyle='--', alpha=0.8, label='passage a 10 perceptions')
    ax.axvline(x=100000, color='darkred', linestyle='-.', alpha=0.8, label='passage a 15 perceptions')

# 1. frames de survie
axs[0, 0].plot(data['episode'], data['survie_moyenne'], label='moyenne', color='blue', linewidth=2)
axs[0, 0].plot(data['episode'], data['survie_max'], label='max', color='lightblue', linestyle='--', alpha=0.7)
ajouter_marqueurs(axs[0, 0])
axs[0, 0].set_title('temps de survie (frames)')
axs[0, 0].set_ylabel('frames')
axs[0, 0].legend()
axs[0, 0].grid(True, linestyle=':', alpha=0.6)

# 2. proportion de mouvements tout droit
axs[0, 1].plot(data['episode'], data['pct_forward'], color='purple', linewidth=2)
axs[0, 1].axhline(y=33.3, color='grey', linestyle='--', label='hasard (33%)')
ajouter_marqueurs(axs[0, 1])
axs[0, 1].set_title('proportion d\'actions "tout droit"')
axs[0, 1].set_ylabel('pourcentage (%)')
axs[0, 1].legend()
axs[0, 1].grid(True, linestyle=':', alpha=0.6)

# 3. taux de victoire
axs[1, 0].plot(data['episode'], data['taux_victoire'], color='orange', linewidth=2)
ajouter_marqueurs(axs[1, 0])
axs[1, 0].set_title('taux de victoire')
axs[1, 0].set_xlabel('episodes')
axs[1, 0].set_ylabel('victoires (%)')
axs[1, 0].legend()
axs[1, 0].grid(True, linestyle=':', alpha=0.6)

# 4. retour g0
axs[1, 1].plot(data['episode'], data['g0_moyen'], color='green', linewidth=2)
ajouter_marqueurs(axs[1, 1])
axs[1, 1].set_title('recompense globale (g0 moyen)')
axs[1, 1].set_xlabel('episodes')
axs[1, 1].set_ylabel('score')
axs[1, 1].legend()
axs[1, 1].grid(True, linestyle=':', alpha=0.6)

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.show()