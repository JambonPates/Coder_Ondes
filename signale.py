import math
import matplotlib.pyplot as plt

# Paramètres du signal original
A = 1
B = 1
f = 1
g = 1
phi = 0
nb_points = 1000  # résolution du signal continu

# Étape 1 : Génération du signal mathématique
def generate_signal(t_values):
    return [A * math.cos(2 * math.pi * f * t) + B * math.sin(2 * math.pi * g * t + phi) for t in t_values]

# Étape 2 : Lire les données depuis un fichier
def load_samples_from_file(filename):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]
    
    # Récupérer les données sous forme de liste de float
    y_values = [float(val) for val in lines]
    t_values = [i for i in range(len(y_values))]
    
    return t_values, y_values

# Étape 3 : Reconstruction par interpolation linéaire (fluide)
def reconstruct_signal(t_sampled, y_sampled, t_continuous):
    y_reconstructed = []
    for t in t_continuous:
        for i in range(1, len(t_sampled)):
            if t_sampled[i - 1] <= t <= t_sampled[i]:
                t1, t2 = t_sampled[i - 1], t_sampled[i]
                y1, y2 = y_sampled[i - 1], y_sampled[i]
                # Interpolation linéaire
                y = y1 + ((y2 - y1) / (t2 - t1)) * (t - t1)
                y_reconstructed.append(y)
                break
        else:
            # Si t est en dehors de l'intervalle, garder la dernière valeur
            y_reconstructed.append(y_sampled[-1])
    return y_reconstructed

# ======= MAIN =======
# Lire les échantillons depuis données1.txt (échantillonnage)
t_sampled1, y_sampled1 = load_samples_from_file("données1.txt")

# Lire les échantillons depuis données2.txt (quantification)
t_sampled2, y_sampled2 = load_samples_from_file("données2.txt")

# Lire les échantillons depuis données3.txt (encodage binaire)
t_sampled3, y_sampled3 = load_samples_from_file("données3.txt")

# Fusionner les données des trois fichiers
# Combinaison des échantillons et valeurs quantifiées (pour les 3 fichiers)
t_sampled = t_sampled1 + t_sampled2 + t_sampled3
y_sampled = y_sampled1 + y_sampled2 + y_sampled3

# Générer le signal original
duree_totale = max(t_sampled)  # Durée maximale des échantillons
t_continuous = [i * (duree_totale / (nb_points - 1)) for i in range(nb_points)]
y_continuous = generate_signal(t_continuous)

# Reconstruire le signal avec toutes les données
y_reconstructed = reconstruct_signal(t_sampled, y_sampled, t_continuous)

# Affichage
plt.figure(figsize=(12, 6))

# Signal original
plt.plot(t_continuous, y_continuous, label="Signal original", linewidth=1.2)

# Échantillons combinés
plt.stem(t_sampled, y_sampled, linefmt='orange', markerfmt='o', basefmt=" ", label="Échantillons combinés")

# Reconstruction du signal combiné
plt.plot(t_continuous, y_reconstructed, label="Reconstruction combinée", linestyle='--', color='red')

# Titres et labels
plt.xlabel("Temps (s)")
plt.ylabel("Amplitude")
plt.title("Signal original et reconstruction combinée")
plt.legend()
plt.grid(True)
plt.ylim(-2, 2)
plt.tight_layout()

# Afficher le graphique
plt.show()
