#include<iostream>
#include"CM_postproc.hpp"
#include"BMP/libbmp.h"
#include"BMP/EasyBMP.h"

#define LOGS

void fillColorBuffer(Domain& caDomain, const int threadNum)
{
    cm_state * domain = caDomain.getAbuffer();
    cm_size grainNum = caDomain.getNucleusNum();
    cm_size cellsNum = caDomain.getCellsNum();
    cm_colorampl* colorsArray = defineColors(grainNum);
    cm_colorampl* colorBuffer = new cm_colorampl[cellsNum*3];

    #pragma omp parallel for default(none) shared(colorBuffer, domain, colorsArray, cellsNum) num_threads(threadNum)
    cm_size dimX = caDomain.getDimX();
    cm_size dimY = caDomain.getDimY();
    for(cm_size y = 0; y < dimY; y++)
    for(cm_size x = 0; x < dimX; x++)
    {
        colorBuffer[(y*dimX + x)*3] = (domain[(y*dimX + x)] != EMPTY) ? colorsArray[(domain[(y*dimX + x)]-1)*3]: 0;
        colorBuffer[(y*dimX + x)*3 + 1] = (domain[(y*dimX + x)] != EMPTY) ? colorsArray[(domain[(y*dimX + x)]-1)*3 + 1]: 0;
        colorBuffer[(y*dimX + x)*3 + 2] = (domain[(y*dimX + x)] != EMPTY) ? colorsArray[(domain[(y*dimX + x)]-1)*3 + 2]: 0;
    }

    caDomain.setColorBuffer(colorBuffer);
    delete[] colorsArray;
}

cm_colorampl* defineColors(cm_size grainNum)
{
    cm_colorampl* colorsArray = new cm_colorampl[3*grainNum];
    for(int i = 0; i < grainNum; i++)
    {
        colorsArray[i*3] = std::rand()%256;
        colorsArray[i*3+1] = std::rand()%256;
        colorsArray[i*3+2] = std::rand()%256;
    }
    return colorsArray;
}

void createBitmap(Domain& caDomain, const int threadsNum)
{
    cm_state * domain = caDomain.getAbuffer();
    cm_size grainNum = caDomain.getNucleusNum();
    cm_size cellsNum = caDomain.getCellsNum();
    cm_size dimX = caDomain.getDimX();
    cm_size dimY = caDomain.getDimY();
    cm_colorampl* colorsArray = defineColors(grainNum);

    BMP bmp;

    bmp.SetSize(dimX, dimY);
    cm_colorampl* colorBuffer = caDomain.getColorBuffer();

    #pragma omp parallel for schedule(static) num_threads(threads_num) firstprivate(dimX, dimY, domain, colorsArray, domain, EMPTY)
    for(cm_pos y = 0; y < dimY; y++)
    for(cm_pos x = 0; x < dimX; x++)
    {
        RGBApixel pixel;
        pixel.Red = (domain[(y*dimX + x)] != EMPTY) ? colorsArray[(domain[(y*dimX + x)]-1)*3]: 0;
        pixel.Green = (domain[(y*dimX + x)] != EMPTY) ? colorsArray[(domain[(y*dimX + x)]-1)*3 + 1]: 0;
        pixel.Blue = (domain[(y*dimX + x)] != EMPTY) ? colorsArray[(domain[(y*dimX + x)]-1)*3 + 2]: 0;
        bmp.SetPixel(x,dimY - y - 1, pixel);
    }

    bmp.WriteToFile(caDomain.getOutputFile().c_str());

    delete[] colorsArray;
}