#include<iostream>
#include<fstream>
#include"CM_logs.hpp"
#include"CM_postproc.hpp"
#include"BMP/libbmp.h"
#include"BMP/EasyBMP.h"

#define BUFFER_LINES_NUMBER 1000

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

void createBitmap(Configuration& config)
{
    int threads_num = config.threadsNum;
    cm_colorampl* colorsArray = defineColors(config.grainsNumber);
    std::string filename = config.outputFile;
    std::string dir = config.outputDir;

   #pragma omp parallel for schedule(static) num_threads(threads_num) firstprivate(dimX, dimY, domain, colorsArray, domain, filename, dir)
    for(cm_pos z = 0; z < config.domain->dimZ; z++)
    {
        BMP bmp;
        bmp.SetSize(config.domain->dimX, config.domain->dimY);    
        for(cm_pos y = 0; y < config.domain->dimY; y++)
        for(cm_pos x = 0; x < config.domain->dimX; x++)
        {
            RGBApixel pixel;
            cm_state grain = (*config.domain)(x, y, z);
            if( grain != Domain::VOID)
            {
                pixel.Red =  colorsArray[(grain)*3];
                pixel.Green = colorsArray[(grain)*3 + 1];
                pixel.Blue =  colorsArray[(grain)*3 + 2];
            }
            else
            {
                pixel.Red = pixel.Green = pixel.Blue = 0;
            }
       
            bmp.SetPixel(x,config.domain->dimY - y - 1, pixel);
    }

    std::string output = dir + std::string("/") + filename + std::string("_") + std::to_string(z) + std::string(".bmp");
    bmp.WriteToFile(output.c_str());
    }

    delete[] colorsArray;
}

void saveMicrostructureFile(Configuration* config)
{
    cm_colorampl* colorsArray = defineColors(config->grainsNumber);
    std::string filename = config->outputDir + "/" + config->outputFile;

    cm_int nonVoid = 0;
    cm_int size = cm_int(config->domain->dimX)*cm_int(config->domain->dimY)*cm_int(config->domain->dimZ);
    
    #pragma omp parallel for num_threads(config->threadsNumber) reduction(+= nonVoid)
    for(cm_int idx = 0; idx < size; idx++)
    {
        if((*config->domain)(idx) != Domain::VOID)
        {
            nonVoid++;
        }
    }

    std::ofstream file;
    file.open(filename);

    if(file.is_open() == false)
        throw std::runtime_error("Output file cannot be created/loade\n");

    if(LogManager::Manager().logmode()) LogManager::Manager().text(std::string("Non void fields: ") + std::to_string(nonVoid));
    file << nonVoid << std::endl << std::endl;
    size_t step = cm_int(config->domain->dimX) * cm_int(config->domain->dimY) * cm_int(config->domain->dimZ) / 10;
    cm_int counter = 0; 
    cm_int line_counter=0;
    std::string buffer;
    switch (config->msFileFormat)
    {
    case MsFileFormat::xyz:
    {
        buffer.resize(nonVoid*10);
        for(cm_pos y = 0; y < config->domain->dimY; y++)
        for(cm_pos z = 0; z < config->domain->dimZ; z++)
        for(cm_pos x = 0; x < config->domain->dimX; x++)
        {
            if(LogManager::Manager().logmode())
            {
                counter++;
                if(counter % (size/10) == 0)
                LogManager::Manager().text(std::string("Progress: ") + std::to_string(counter) + std::string("/") + std::to_string(size));
            }
            if((*config->domain)(x, y, z) != Domain::VOID)
            {
                //file << x << ' ' << y << ' ' << z << ' ' << (*config->domain)(x, y, z) << std::endl;
                buffer += std::to_string(x) + ' ' + std::to_string(y) + ' ' + std::to_string(z) + ' ' + std::to_string((*config->domain)(x, y, z));
                buffer += "\n";
            }
         
        }
        file.write(buffer.c_str(), buffer.size()-1);
        break;

    case MsFileFormat::xyzrgb:
        for(cm_pos y = 0; y < config->domain->dimY; y++)
        for(cm_pos z = 0; z < config->domain->dimZ; z++)
        for(cm_pos x = 0; x < config->domain->dimX; x++)
        {
            if((*config->domain)(x, y, z) != Grain::NON_VALID)
            {
                file << x << ' ' << y << ' ' << z << ' ' << colorsArray[(*config->domain)(x, y, z)] << 
                colorsArray[(*config->domain)(x, y, z) + 1] << colorsArray[(*config->domain)(x, y, z) + 2] << (*config->domain)(x, y, z) << std::endl;
            }
        }
    }
        break;
    
    default:
        throw std::runtime_error("Format (yet) not supported!");
        break;
    }

    file.close();
    delete[] colorsArray;
}