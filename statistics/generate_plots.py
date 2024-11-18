import sys
import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import argparse

class Microstructure:
    VOID = 0xFFFF
    BOND = 0xFFFE

    def __init__(self):
        self.dimX = 0
        self.dimY = 0
        self.dimZ = 0
        self.buffer = None
        self.grainNumber = 0

    def size(self):
        return self.dimX * self.dimY * self.dimZ

    def __call__(self, x, y, z):
        return self.buffer[x, y, z]

class Variable:
    def __init__(self, mean=0.0, stddev=0.0, count=0, porosity=0.0):
        self.mean = mean
        self.stddev = stddev
        self.count = count
        self.porosity = porosity

def processFile(filename):
    with open(filename, 'r') as file:
        size_line = file.readline()
        size = int(size_line.strip())
        microstructure = Microstructure()

        x_vals = []
        y_vals = []
        z_vals = []
        ids = []

        for line in file:
            if not line.strip():
                continue
            x_str, y_str, z_str, id_str = line.strip().split()
            x = int(x_str)
            y = int(y_str)
            z = int(z_str)
            id = int(id_str)

            x_vals.append(x)
            y_vals.append(y)
            z_vals.append(z)
            ids.append(id)

        microstructure.dimX = max(x_vals) + 1
        microstructure.dimY = max(y_vals) + 1
        microstructure.dimZ = max(z_vals) + 1
        valid_ids = [id for id in ids if id != Microstructure.VOID and id != Microstructure.BOND]
        microstructure.grainNumber = max(valid_ids, default=0) + 2

        microstructure.buffer = np.zeros((microstructure.dimX, microstructure.dimY, microstructure.dimZ), dtype=np.uint16)

        for x, y, z, id in zip(x_vals, y_vals, z_vals, ids):
            microstructure.buffer[x, y, z] = id

        return microstructure

def measurePorosity(microstructure):
    total_size = microstructure.buffer.size
    num_void = np.sum(microstructure.buffer == microstructure.VOID)
    return num_void / total_size

def grainDiameters(microstructure):
    result = []
    for y in range(microstructure.dimY):
        slices = {}
        void_count = 0
        total_cells = microstructure.dimX * microstructure.dimZ

        for z in range(microstructure.dimZ):
            for x in range(microstructure.dimX):
                grainID = microstructure(x, y, z)
                if grainID == Microstructure.VOID:
                    void_count += 1
                if grainID != Microstructure.VOID and grainID != Microstructure.BOND:
                    slices[grainID] = slices.get(grainID, 0.0) + 1.0

        # Compute diameters instead of areas
        porosity = void_count / total_cells
        diameters = [2 * math.sqrt(area/ math.pi) for area in slices.values()]
        if diameters:
            mean = sum(diameters) / len(diameters)
            variance = sum((diameter - mean) ** 2 for diameter in diameters)
            stddev = math.sqrt(variance / (len(diameters) - 1)) if len(diameters) > 1 else 0.0
        else:
            mean = stddev = 0.0

        #porosity = void_count / total_cells
        variable = Variable(mean, stddev, len(slices), porosity)
        result.append(variable)
    return result

def create_plots(porosity, grainData):
    # Prepare data for plotting
    data = {
        'Y': list(range(len(grainData))),
        'Mean': [var.mean for var in grainData],
        'Stddev': [var.stddev for var in grainData],
        'Grain count': [var.count for var in grainData],
        'Porosity': [var.porosity for var in grainData]
    }
    df = pd.DataFrame(data)

    # Filter the DataFrame to only include rows where 'Y' is a multiple of 10
    filtered_df = df[df['Y'] % 5 == 0]

    # Plotting the data
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

    # Set window title
    fig.canvas.manager.set_window_title("Microstructure Data Analysis")

    # First subplot (ax1): Mean Diameter and Grain Count
    color1 = 'tab:blue'
    ax1.set_xlabel('Y')
    ax1.set_ylabel('Średnia średnica ziaren', color=color1)
    ax1.errorbar(filtered_df['Y'], filtered_df['Mean'], yerr=filtered_df['Stddev'], fmt='o-', color=color1,
                 ecolor='green', elinewidth=2, capsize=4, label='Średnia średnica ziaren')
    ax1.tick_params(axis='y', labelcolor=color1)
    ax1.grid(True)

    # Create a second y-axis for grain count
    ax1_twin = ax1.twinx()

    color2 = 'tab:red'
    ax1_twin.set_ylabel('Liczba ziaren', color=color2)
    ax1_twin.plot(filtered_df['Y'], filtered_df['Grain count'], color=color2, linestyle='--', label='Liczba ziaren')
    ax1_twin.tick_params(axis='y', labelcolor=color2)

    # Combine legends from both axes
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax1_twin.get_legend_handles_labels()
    ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper right')

    ax1.set_title('Średnia średnica ziarna i odchylenie standardowe w zależności od Y')

    # Second subplot (ax2): Porosity per Y
    ax2.plot(filtered_df['Y'], filtered_df['Porosity'], color='purple', marker='o', label='Porowatość')
    ax2.set_xlabel('Y')
    ax2.set_ylabel('Porowatość')
    total_porosity = porosity * 100
    #ax2.set_title(f'Porowatość na poziomie Y (Całkowita porowatość: {total_porosity:.2f}%)')
    ax2.set_title(f'Porowatość na poziomie Y')
    ax2.grid(True)
    ax2.legend(loc='upper right')

    plt.tight_layout()
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Process microstructure data and generate plots.")
    parser.add_argument('input_file', type=str, help='Path to the input file containing microstructure data')
    args = parser.parse_args()

    microstructure = processFile(args.input_file)
    porosity = measurePorosity(microstructure)
    grainData = grainDiameters(microstructure)
    create_plots(porosity, grainData)

if __name__ == "__main__":
    main()
