#include"CM_postproc_GPU.cuh"

__constant__ cm_pos dimX;
__constant__ cm_pos dimY;
__constant__ cm_pos dimZ;

void fillColorBuffer_GPU(GeneratorConfig& caDomain)
{
    cm_state * domain = caDomain.getAbuffer();
    cm_size grainNum = caDomain.getNucleusNum();
    cm_colorampl* colorsArray = defineColors(grainNum);

    cm_size free, total;
    cudaMemGetInfo(&free, &total);

    cudaMemcpyToSymbol(dimX, &dimX, sizeof(cm_pos));
    cudaMemcpyToSymbol(dimY, &dimY, sizeof(cm_pos));
    cudaMemcpyToSymbol(dimZ, &dimZ, sizeof(cm_pos));

    cm_size cellSize = caDomain.getCellSizeInBytes();
    cm_size colorsSize = caDomain.getColorsSizeInBytes();
    cm_size cellsNum = caDomain.getCellsNum();

    int chunkNum = ((cellSize + colorsSize)*cellsNum + free - 1)/free;
    cm_size chunk = (cellsNum + chunkNum - 1)/chunkNum;

    cm_state* domain_D = nullptr;
    cm_state* colorBuffer_D = nullptr;
    cm_colorampl* colorsArray_D = nullptr;
    cudaMalloc(&domain_D, chunk*cellSize);
    cudaMalloc(&colorBuffer_D, chunk*colorsSize);
    cudaMalloc(&colorsArray_D, colorsSize*grainNum);
    cudaMemcpy(colorsArray_D, colorsArray, colorsSize*grainNum, cudaMemcpyHostToDevice);

    cm_size pos = 0;
    cm_colorampl* colorBuffer = (cm_colorampl*) malloc(caDomain.getColorBufferSizeInBytes());

    dim3 blockSize = {32,1,1};
    dim3 gridSize = {(chunk + blockSize.x - 1)/blockSize.x, 1, 1};
    for(cm_size i = 0; i < chunkNum; i++)
    {
        cudaMemcpy(domain_D, domain + i*chunk*cellSize, chunk*cellSize, cudaMemcpyHostToDevice);
        cudaMemcpy(colorBuffer_D, colorBuffer + i*chunk*colorsSize, chunk*colorsSize, cudaMemcpyHostToDevice);
        assignColors<<<blockSize, gridSize>>>(domain_D, colorBuffer_D, colorsArray_D);
        cudaDeviceSynchronize();
        cudaMemcpy(domain + i*chunk*cellSize, domain_D, chunk*cellSize, cudaMemcpyDeviceToHost);
        cudaMemcpy(colorBuffer + i*chunk*colorsSize, colorBuffer_D,  chunk*colorsSize, cudaMemcpyDeviceToHost);
    }

    caDomain.setColorBuffer(colorBuffer);

    cudaFree(domain_D);
    cudaFree(colorBuffer_D);
    cudaFree(colorsArray_D);
    delete[] colorsArray;
}

__global__ void assignColors(cm_state* caDomain, cm_colorampl* rgb, const cm_colorampl* __restrict__ colorArray)
{
    int idx = threadIdx.x;
    rgb[idx*3] = colorArray[caDomain[idx]*3];
    rgb[idx*3 + 1] = colorArray[caDomain[idx]*3 + 1];
    rgb[idx*3 + 2] = colorArray[caDomain[idx]*3 + 2];
}