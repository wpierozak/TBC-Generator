#include<iostream>
#include"CM_postproc.hpp"
#include"BMP/libbmp.h"
#include"BMP/EasyBMP.h"

cm_colorampl* defineColors(cm_size grainNum)
{
    cm_colorampl* colorsArray = new cm_colorampl[3*grainNum];
    for(int i = 0; i < grainNum; i++)
    {
        colorsArray[i*3] = std::rand();
        colorsArray[i*3+1] = std::rand();
        colorsArray[i*3+2] = std::rand();
    }
    return colorsArray;
}

void createBitmap(GeneratorConfig& caDomain, const int threadsNum)
{
    cm_state * domain = caDomain.getDomain();
    cm_size grainNum = caDomain.getNucleusNum();
    cm_size cellsNum = caDomain.getCellsNum();
    cm_size dimX = caDomain.getDimX();
    cm_size dimY = caDomain.getDimY();
    cm_size dimZ = caDomain.getDimZ();
    cm_colorampl* colorsArray = defineColors(grainNum);
    std::string filename = caDomain.getOutputFile();
    std::string dir = caDomain.getOutpuDir();

   #pragma omp parallel for schedule(static) num_threads(threads_num) firstprivate(dimX, dimY, domain, colorsArray, domain, filename, dir)
    for(cm_pos z = 0; z < dimZ; z++)
    {
        BMP bmp;
        bmp.SetSize(dimX, dimY);    
        for(cm_pos y = 0; y < dimY; y++)
        for(cm_pos x = 0; x < dimX; x++)
        {
            RGBApixel pixel;
            cm_state grain = domain[caDomain.getIdx(x, y, z)];
            if( grain != EMPTY)
            {
                pixel.Red =  colorsArray[(grain-1)*3];
                pixel.Green = colorsArray[(grain-1)*3 + 1];
                pixel.Blue =  colorsArray[(grain-1)*3 + 2];
            }
            else
            {
                pixel.Red = pixel.Green = pixel.Blue = 0;
            }
       
            bmp.SetPixel(x,dimY - y - 1, pixel);
    }

    std::string output = dir + std::string("/") + filename + std::string("_") + std::to_string(z) + std::string(".bmp");
    bmp.WriteToFile(output.c_str());
    }

    delete[] colorsArray;
}