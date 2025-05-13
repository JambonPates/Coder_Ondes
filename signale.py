import math
import matplotlib.pyplot as plt

# Paramètres du signal original
A = 1
B = 1
f = 1
g = 1
phi = 0
nb_points = 1000  # résolution du signal continu

# Génération du signal mathématique
def generate_signal(t_values):
    return [A * math.cos(2 * math.pi * f * t) + B * math.sin(2 * math.pi * g * t + phi) for t in t_values]

# Lecture d'un fichier simple avec 1 valeur par ligne
def load_samples_from_file(filename):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]
    y_values = [float(val.replace(',', '.')) for val in lines]
    t_values = [i for i in range(len(y_values))]
    return t_values, y_values

# Lecture du fichier texte.txt (avec durée, intervalle, puis valeurs)
def load_from_texte_file(filename):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]
    
    duree = float(lines[0].replace(',', '.'))
    intervalle = float(lines[1].replace(',', '.'))
    y_values = [float(val.replace(',', '.')) for val in lines[2:]]
    t_values = [i * intervalle for i in range(len(y_values))]
    
    return t_values, y_values

# Interpolation linéaire
def reconstruct_signal(t_sampled, y_sampled, t_continuous):
    y_reconstructed = []
    for t in t_continuous:
        for i in range(1, len(t_sampled)):
            t1, t2 = t_sampled[i - 1], t_sampled[i]
            y1, y2 = y_sampled[i - 1], y_sampled[i]

            if t1 <= t <= t2:
                if t2 == t1:
                    # Temps identiques → impossible d'interpoler, on prend y1
                    y_reconstructed.append(y1)
                else:
                    # Interpolation linéaire normale
                    y = y1 + ((y2 - y1) / (t2 - t1)) * (t - t1)
                    y_reconstructed.append(y)
                break
        else:
            # Hors plage : utiliser la dernière valeur connue
            y_reconstructed.append(y_sampled[-1])
    return y_reconstructed

# ======= MAIN =======

# Charger les fichiers
t_sampled1, y_sampled1 = load_samples_from_file("données1.txt")
t_sampled2, y_sampled2 = load_samples_from_file("données2.txt")
t_sampled3, y_sampled3 = load_samples_from_file("données3.txt")
t_sampled_txt, y_sampled_txt = load_from_texte_file("texte.txt")

# Fusionner toutes les données (temps et valeurs)
t_sampled = t_sampled1 + t_sampled2 + t_sampled3 + t_sampled_txt
y_sampled = y_sampled1 + y_sampled2 + y_sampled3 + y_sampled_txt

# Trier les données par ordre croissant de temps
sorted_data = sorted(zip(t_sampled, y_sampled))
t_sampled, y_sampled = zip(*sorted_data)

# Générer le signal original
duree_totale = max(t_sampled)
t_continuous = [i * (duree_totale / (nb_points - 1)) for i in range(nb_points)]
y_continuous = generate_signal(t_continuous)

# Reconstruction
y_reconstructed = reconstruct_signal(t_sampled, y_sampled, t_continuous)

# Affichage
plt.figure(figsize=(12, 6))
plt.plot(t_continuous, y_continuous, label="Signal original", linewidth=1.2)
plt.stem(t_sampled, y_sampled, linefmt='orange', markerfmt='o', basefmt=" ", label="Échantillons combinés")
plt.plot(t_continuous, y_reconstructed, label="Reconstruction combinée", linestyle='--', color='red')

plt.xlabel("Temps (s)")
plt.ylabel("Amplitude")
plt.title("Signal original et reconstruction combinée")
plt.legend()
plt.grid(True)
plt.ylim(-2, 2)
plt.tight_layout()
plt.show()
