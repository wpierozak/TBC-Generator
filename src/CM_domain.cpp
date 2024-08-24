#include<stdexcept>
#include<climits>
#include"CM_domain.hpp"

const cell Domain::VOID = cell(-1, std::numeric_limits<double>::max());

Domain::Domain(cm_pos X, cm_pos Y, cm_pos Z, Neighbourhood n):
    dimX(X), dimY(Y), dimZ(Z), neighbourhood(n)
{
    buffer = std::shared_ptr<cell[]>(new cell[static_cast<cm_pos>(dimX)*static_cast<cm_pos>(dimZ)*static_cast<cm_pos>(dimY)]);
    for(cm_pos n = 0; n < static_cast<cm_pos>(dimX)*static_cast<cm_pos>(dimZ)*static_cast<cm_pos>(dimY); n++)
    buffer[n] = Domain::VOID;   
}

cell Domain::state(cm_pos x, cm_pos y, cm_pos z) const
{
    if(dimZ > 1)
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY) && (z >= 0 && z < dimZ);
        if(inside) return buffer[cm_pos(y)*(dimX * dimZ) + cm_pos(z)*dimX + cm_pos(x)];
    }

    else
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY);
        if(inside) return buffer[cm_pos(y)*(dimX * dimZ) + cm_pos(x)];
    }
    
    switch (bc)
    {
    case BC::absorption :
        return Domain::VOID;
    break;

    case BC::bouncy:
        if(x >= dimX) x = dimX - 1 - (x-dimX);
        else if(x < 0) x = -x - 1;
        
        if(y >= dimY) y = dimY - 1 - (y-dimY);
        else if(y < 0) y = -y - 1;

        if(z >= dimZ) z = dimZ - 1 - (z-dimZ);
        else if(z < 0) z = -z - 1;

        return buffer[cm_pos(y)*(dimX * dimZ) + cm_pos(z)*dimX + cm_pos(x)];
    break;

    case BC::periodic:
        if(x >= dimX) x = x - dimX;
        else if(x < 0) x = dimX + x;

        if(y >= dimY) y = y - dimY;
        else if(y < 0) y = dimY + y;

        if(z >= dimZ) z = z - dimZ;
        else if(z < 0) z = dimZ + z;
        
        return buffer[cm_pos(y)*(dimX * dimZ) + cm_pos(z)*dimX + cm_pos(x)];
    break;

    default:
        throw std::runtime_error("Invalid bc");
        break;
    }
}