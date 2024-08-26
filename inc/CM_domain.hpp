#pragma once
#include<memory>
#include"CM_basictypes.hpp"

enum class BC{absorption, bouncy, periodic};
enum class MsFileFormat{xyz, xyzrgb};

struct Neighbourhood
{
    Neighbourhood()= default;
    Neighbourhood(const Neighbourhood& n):
        dx0(n.dx0), dx1(n.dx1), dy0(n.dy0), dy1(n.dy1), dz0(n.dz0), dz1(n.dz1) {}
    Neighbourhood(_long_int x0, _long_int x1, _long_int y0, _long_int y1, _long_int z0, _long_int z1):
        dx0(x0), dx1(x1), dy0(y0), dy1(y1), dz0(z0), dz1(z1) {}
    _long_int dx0, dx1;
    _long_int dy0, dy1;
    _long_int dz0, dz1;
};  

class Domain
{
    private:
    std::shared_ptr<cell[]> buffer;

    public:
    Domain() = delete;
    Domain(const Domain&) = default;
    Domain(Domain&&);
    Domain& operator=(const Domain&) = delete;
    Domain(_long_int dimX, _long_int dimY, _long_int dimZ, Neighbourhood neighbourhood);

    static const cell VOID;
    const _long_int dimX;
    const _long_int dimY;
    const _long_int dimZ;
    const Neighbourhood neighbourhood;

    BC bc = BC::absorption;

    cell& operator()(_long_int x, _long_int y, _long_int z)
    {
        return buffer[(y*dimZ + z)*dimX + x];
    }

    cell& operator()(_long_int pos)
    {
        return buffer[pos];
    }

    cell state(_long_int x, _long_int y, _long_int z) const; 
};