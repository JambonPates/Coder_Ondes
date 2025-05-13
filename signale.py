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

# Étape 2 : Lire les données depuis texte.txt
def load_samples_from_file(filename):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]
    
    duree = float(lines[0])
    intervalle = float(lines[1])
    y_sampled = [float(val) for val in lines[2:]]
    t_sampled = [i * intervalle for i in range(len(y_sampled))]
    
    return t_sampled, y_sampled, duree

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
# Lire les échantillons depuis texte.txt
t_sampled, y_sampled, duree_totale = load_samples_from_file("texte.txt")

# Générer le signal original
t_continuous = [i * (duree_totale / (nb_points - 1)) for i in range(nb_points)]
y_continuous = generate_signal(t_continuous)

# Reconstruire le signal de manière fluide
y_reconstructed = reconstruct_signal(t_sampled, y_sampled, t_continuous)

# Affichage
plt.figure(figsize=(12, 6))
plt.plot(t_continuous, y_continuous, label="Signal original", linewidth=1.2)
plt.stem(t_sampled, y_sampled, linefmt='orange', markerfmt='o', basefmt=" ", label="Échantillons")
plt.plot(t_continuous, y_reconstructed, label="Reconstruction fluide", linestyle='--', color='red')

plt.xlabel("Temps (s)")
plt.ylabel("Amplitude")
plt.title("Signal original et reconstruction fluide depuis texte.txt")
plt.legend()
plt.grid(True)
plt.ylim(-2, 2)
plt.tight_layout()
plt.show()
