import csv
import numpy as np
import matplotlib.pyplot as plt

# Read in the data from the CSV file
csv_file_path = './cachedFiles/CoM/CoM_2805.csv'
with open(csv_file_path, 'r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    next(csv_reader) # Skip the header row
    x = []
    y = []
    for row in csv_reader:
        x.append(float(row[0]))
        y.append(float(row[1]))

print('Total number of points: ', len(x))
# * calculate density of CoM points
local_density = []
print('Calculating local density...')
for i in range(len(x)):
    # * calculate the distance between point i and all other points
    distances = []
    for j in range(len(x)):
        if i != j:
            distances.append(np.sqrt((x[i] - x[j])**2 + (y[i] - y[j])**2))
    # * calculate the local density
    local_density.append(1 / np.mean(distances))

# Plot the data
figure = plt.figure(figsize=(6, 5), dpi=200)
plt.scatter(x, y, c=local_density ,s=2)
plt.xlabel('x')
plt.ylabel('y')
plt.xlim(0, 105)
plt.ylim(0, 105)
plt.title('Center of Mass')
plt.colorbar()
plt.grid(True)

plt.show()
