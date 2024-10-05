#include<iostream>
#include<fstream>
#include<tuple>
#include<set>
#include<algorithm>
#include<ctime>

#include"postproc.hpp"
#include"BMP/libbmp.h"
#include"BMP/EasyBMP.h"

#define BUFFER_SIZE 1000

std::tuple<uint8_t, uint8_t, uint8_t> generateRandomColor() {
    return std::make_tuple(static_cast<uint8_t>(std::rand() % 256),
                           static_cast<uint8_t>(std::rand() % 256),
                           static_cast<uint8_t>(std::rand() % 256));
}

uint8_t* defineColors(uint16_t grainNum) {
    std::srand(static_cast<unsigned>(std::time(0))); // Seed the random generator
    uint8_t* colorsArray = new uint8_t[3 * grainNum];
    std::set<std::tuple<uint8_t, uint8_t, uint8_t>> uniqueColors;
    for (uint16_t i = 0; i < grainNum; ++i) {
        std::tuple<uint8_t, uint8_t, uint8_t> color;
        do {
            color = generateRandomColor();
        } while (uniqueColors.find(color) != uniqueColors.end());

        uniqueColors.insert(color);
        colorsArray[i * 3] = std::get<0>(color);
        colorsArray[i * 3 + 1] = std::get<1>(color);
        colorsArray[i * 3 + 2] = std::get<2>(color);
    }

    return colorsArray;
}

void saveMicrostructureFile(Domain& domain, Configuration& config)
{
    std::string filename = config.output.filename;
    _long_int size = _long_int(domain.dimX)*_long_int(domain.dimY)*_long_int(domain.dimZ);
    
    std::ofstream file;
    file.open(filename);

    char buffer[BUFFER_SIZE];
    file.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);

    if(file.is_open() == false)
        throw std::runtime_error("Output file cannot be created/loade\n");

    file << size << std::endl << std::endl;

    size_t step = _long_int(domain.dimX) * _long_int(domain.dimY) * _long_int(domain.dimZ) / 10;
    _long_int counter = 0; 
    _long_int line_counter=0;

    for(_long_int y = 0; y < domain.dimY; y++)
    for(_long_int z = 0; z < domain.dimZ; z++)
    for(_long_int x = 0; x < domain.dimX; x++)
        {
            file << x << ' ' << y << ' ' << z << ' ' << domain(x, y, z) << std::endl;
        }

    file.close();
}