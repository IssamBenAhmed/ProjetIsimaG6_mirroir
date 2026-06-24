import numpy as np

# float32 correspond exactement au type 'float' de ton code c
theta = np.fromfile('data/cerveau.bin', dtype=np.float32)

# on redimensionne le tableau plat en matrice 15x3 comme dans agent.c
theta = theta.reshape((15, 3))

# formatage pour eviter la notation scientifique illisible et limiter a 4 decimales
np.set_printoptions(suppress=True, precision=4)

print("matrice theta (15 variables x 3 actions) :")
print(theta)