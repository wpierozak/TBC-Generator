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
    Neighbourhood(cm_pos x0, cm_pos x1, cm_pos y0, cm_pos y1, cm_pos z0, cm_pos z1):
        dx0(x0), dx1(x1), dy0(y0), dy1(y1), dz0(z0), dz1(z1) {}
    cm_pos dx0, dx1;
    cm_pos dy0, dy1;
    cm_pos dz0, dz1;
};  

class Domain
{
    private:
    std::shared_ptr<cm_state[]> buffer;

    public:
    Domain() = delete;
    Domain(const Domain&) = default;
    Domain(Domain&&);
    Domain& operator=(const Domain&) = delete;
    Domain(cm_pos dimX, cm_pos dimY, cm_pos dimZ, Neighbourhood neighbourhood);

    static const cm_state VOID{static_cast<cm_state>(-1)};
    const cm_pos dimX;
    const cm_pos dimY;
    const cm_pos dimZ;
    const Neighbourhood neighbourhood;

    BC bc = BC::absorption;

    cm_state& operator()(cm_pos x, cm_pos y, cm_pos z)
    {
        return buffer[(y*dimZ + z)*dimX + x];
    }

    cm_state& operator()(cm_pos pos)
    {
        return buffer[pos];
    }

    cm_state state(cm_pos x, cm_pos y, cm_pos z) const; 
};