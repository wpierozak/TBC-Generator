#include<stdexcept>
#include<climits>
#include"domain.hpp"

const cell Domain::VOID = cell(__UINT16_MAX__, std::numeric_limits<double>::max());
const cell Domain::BOND = cell(__UINT16_MAX__ - 1, std::numeric_limits<double>::max());

Domain::Domain(_long_int X, _long_int Y, _long_int Z, Neighbourhood n):
    dimX(X), dimY(Y), dimZ(Z), neighbourhood(n)
{
    buffer = std::shared_ptr<cell[]>(new cell[static_cast<_long_int>(dimX)*static_cast<_long_int>(dimZ)*static_cast<_long_int>(dimY)]);
    for(_long_int n = 0; n < static_cast<_long_int>(dimX)*static_cast<_long_int>(dimZ)*static_cast<_long_int>(dimY); n++)
    buffer[n] = Domain::VOID;   
}

cell Domain::state(_long_int x, _long_int y, _long_int z) const
{
    if(dimZ > 1)
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY) && (z >= 0 && z < dimZ);
        if(inside) return buffer[_long_int(y)*(dimX * dimZ) + _long_int(z)*dimX + _long_int(x)];
    }
    else
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY);
        if(inside) return buffer[_long_int(y)*(dimX * dimZ) + _long_int(x)];
    }
    
    
   
    if(x >= dimX) x = x - dimX;
    else if(x < 0) x = dimX + x;

    if(y >= dimY) y = y - dimY;
    else if(y < 0) y = dimY + y;

    if(z >= dimZ) z = z - dimZ;
    else if(z < 0) z = dimZ + z;
        
    return buffer[_long_int(y)*(dimX * dimZ) + _long_int(z)*dimX + _long_int(x)];
}