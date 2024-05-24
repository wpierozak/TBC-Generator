# Configuration Overview

This document describes the configuration settings for a grain growth simulation. The settings are defined in an XML file and control various aspects of the simulation, including dimensions, output options, grain properties, and computational parameters.

## Configuration Parameters

### Dimensions

The dimensions of the simulation domain are specified in micrometers.

- **dimX**: X dimension of the simulation domain (micrometers)
- **dimY**: Y dimension of the simulation domain (micrometers)
- **dimZ**: Z dimension of the simulation domain (micrometers)

### Output Settings

These parameters define the output directory, filename, and file format for the simulation results.

- **output_dir**: Directory where the simulation results will be saved.
- **output_filename**: Filename for the output results.
- **ms_file_format**: Format of the output file (`xyz` or `xyzrgb`).

### Neighbourhood Settings

The neighbourhood settings define the range of offsets to consider in the x, y, and z directions.

- **x0**: Minimum x-offset to consider.
- **x1**: Maximum x-offset to consider.
- **y0**: Minimum y-offset to consider.
- **y1**: Maximum y-offset to consider.
- **z0**: Minimum z-offset to consider.
- **z1**: Maximum z-offset to consider.

### Grain Settings

The grain settings define the properties of individual grains, including their number, radius, tilt, length, top fraction, and angle of widening. Each property has a mean and standard deviation.

- **grains_number**: Total number of grains in the simulation.

#### Grain Properties

- **radius**:  
  - **mean**: Mean radius of the grains (micrometers).
  - **std**: Standard deviation of the radius (micrometers).
  - **min**: Optional minimum value for the radius.
  - **max**: Optional maximum value for the radius.

- **tilt**:  
  - **mean**: Mean tilt angle of the grains (degrees).
  - **std**: Standard deviation of the tilt angle (degrees).
  - **min**: Optional minimum value for the tilt.
  - **max**: Optional maximum value for the tilt.

- **length**:  
  - **mean**: Mean length of the grains (micrometers).
  - **std**: Standard deviation of the length (micrometers).
  - **min**: Optional minimum value for the length.
  - **max**: Optional maximum value for the length.

- **top_frac**:  
  - **mean**: Mean top fraction of the grains.
  - **std**: Standard deviation of the top fraction.
  - **min**: Optional minimum value for the top fraction.
  - **max**: Optional maximum value for the top fraction.

- **widen**:  
  - **mean**: Mean widen angle of the grains (degrees).
  - **std**: Standard deviation of the widen angle (degrees).
  - **min**: Optional minimum value for the widen angle.
  - **max**: Optional maximum value for the widen angle.

- **resolution**: Resolution of the grain simulation (`LOW` or `HIGH`).

### Computational Settings

These parameters define the computational resources and boundary conditions for the simulation.

- **threads_number**: Number of threads to use for the simulation, enabling parallel processing.

- **boundry_conditions**: Type of boundary conditions to apply in the simulation (`absorption`, `bouncy`, or `periodic`).

- **bitmaps**: Indicates whether to generate bitmap images (`true` or `false`).

## Example XML Configuration

```xml
<?xml version="1.0"?>
<Configuration>

    <dimX>...</dimX> <!-- micrometers -->
    <dimY>...</dimY>
    <dimZ>...</dimZ> <!-- micrometers -->

    <output_dir>...</output_dir>
    <output_filename>...</output_filename>
    <ms_file_format>...</ms_file_format>

    <neighbourhood>   
        <x0>...</x0>
        <x1>...</x1>
        <y0>...</y0>
        <y1>...</y1>
        <z0>...</z0>
        <z1>...</z1>
    </neighbourhood>

    <grains_number>...</grains_number>

    <Grain>    
        <radius>    <!-- micrometers -->
            <mean>...</mean>
            <std>...</std>
        </radius>

        <tilt>  <!-- degrees -->
            <mean>...</mean>
            <std>...</std>
        </tilt>

        <length> <!-- micrometers -->
            <mean>...</mean>
            <std>...</std>
        </length>

        <top_frac>
            <mean>...</mean>
            <std>...</std>
        </top_frac>

        <widen> <!-- degrees -->
            <mean>...</mean>
            <std>...</std>
        </widen>

        <resolution>...</resolution>
    </Grain>

    <threads_number>...</threads_number>
    <boundry_conditions>...</boundry_conditions>
    <bitmaps>...</bitmaps>

</Configuration>
