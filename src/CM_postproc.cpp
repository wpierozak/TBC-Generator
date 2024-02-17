#include<iostream>
#include"CM_postproc.hpp"
#include"BMP/libbmp.h"

#define LOGS

void fillColorBuffer(Domain& caDomain, const int threadNum)
{
    cm_state * domain = caDomain.getAbuffer();
    cm_size grainNum = caDomain.getNucleusNum();
    cm_size cellsNum = caDomain.getCellsNum();
    cm_colorampl* colorsArray = defineColors(grainNum);
    cm_colorampl* colorBuffer = new cm_colorampl[cellsNum*3];

    //#pragma omp parallel for default(none) shared(colorBuffer, domain, colorsArray, cellsNum) num_threads(threadNum)
    for(cm_size idx = 0; idx < cellsNum; idx++)
    {
        colorBuffer[idx*3] = colorsArray[domain[idx]*3];
        colorBuffer[idx*3 + 1] = colorsArray[domain[idx]*3] + 1;
        colorBuffer[idx*3 + 2] = colorsArray[domain[idx]*3 + 2];
    }

    caDomain.setColorBuffer(colorBuffer);
    delete[] colorsArray;
}

cm_colorampl* defineColors(cm_size grainNum)
{
    int spec = (256)/grainNum;
    cm_colorampl* colorsArray = new cm_colorampl[3*grainNum];
    for(int i = 0; i < grainNum; i++)
    {
        colorsArray[i*3] = spec * ( (i%2 == 0) ? i: grainNum - i);
        colorsArray[i*3+1] = spec * ( (i%2 == 0) ? i: grainNum - i);
        colorsArray[i*3+2] = spec * ( (i%2 == 0) ? i: grainNum - i);
    }
    return colorsArray;
}

void createBitmap(Domain& caDomain)
{
    BmpImg bmp(caDomain.getDimX(), caDomain.getDimY());
    cm_colorampl* colorBuffer = caDomain.getColorBuffer();
    cm_size dimX = caDomain.getDimX();
    cm_size dimY = caDomain.getDimY();
    for(int y = 0; y < dimY; y++)
    for(int x = 0; x < dimX; x++)
    {
        bmp.set_pixel(x, y, colorBuffer[(y*dimX + x)*3], colorBuffer[(y*dimX + x)*3 + 1], colorBuffer[(y*dimX + x)*3 + 2] );
    }
    bmp.write(caDomain.getOutputFile());
}