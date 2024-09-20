#pragma once

#include<list>
#include<unordered_map>
#include"config.hpp"
#include"grain.hpp"

class Generator
{
    public:
    struct Subspace
    {
        _long_int x0,x1;
        _long_int y0,y1;
        _long_int z0,z1;
    };

    Generator(Domain& domain, Subspace subspace);
    void update_grains(std::unordered_map<uint16_t, Grain>& grains);
    void set_g0(_int g0) { m_g0 = g0; }
    void set_prefered_orientation(f_vec p){ m_prefered_orientation = p; }
    void set_alpha_g(double alpha){m_alpha_g = alpha;}
    void set_alpha_t(double alpha){m_alpha_t = alpha;}

    void run(Domain& input, Domain& output, double ct);
    Subspace& subspace() {return m_subspace;}

    private: 

    double FastArcTan(double x) 
    {
        return M_PI_4*x - x*(fabs(x) - 1)*(0.2447 + 0.0663*fabs(x));
    }

    double atan_jw(double x) {
        return 8.430893743524 * x / (3.2105332277903100 + sqrt(27.2515970979709 + 29.3591908371266 * x * x));
    }

    double dt(double&& d, f_vec&& virtual_pos, const Grain& grain)
    {
        //return d/(cos(atan((virtual_pos - grain.center).cross(grain.orientation).norm())/2.0) * (grain.orientation * m_prefered_orientation));
        
        // double g_p_dot_2 = grain.orientation * m_prefered_orientation;
        // g_p_dot_2 *= g_p_dot_2;
        // double atan_2 = atan_jw((virtual_pos - grain.center).cross(grain.orientation).norm()) *0.5;
        // atan_2 *= atan_2;
        // return d/fabs( (1.0 - 0.5 * atan_2*(atan_2 - atan_2 * 12.0)) * ( 8.0*g_p_dot_2 * (g_p_dot_2 - 1.0) + 1.0 ) );
        
        // return (d*d)/fabs( 0.5 * ( 8.0*pow(virtual_pos*grain.orientation,2) * (pow(virtual_pos*grain.orientation,2) - 1.0) + 1.0 ) *
        //         ( 8.0*pow(grain.orientation * m_prefered_orientation,2) * (pow(grain.orientation * m_prefered_orientation,2) - 1.0) + 1.0 )
        //         );


        double theta = acos(grain.orientation * m_prefered_orientation);
        double arctan = atan_jw((virtual_pos - grain.center).cross(grain.orientation).norm());

        return d/fabs(cos(arctan*m_alpha_g)*cos(m_alpha_t*theta));
    }

    f_vec virtual_pos(const f_vec& pos, f_vec&& relative_pos, const f_vec& space)
    {
        f_vec virtual_pos = pos;

        if(pos.x + relative_pos.x < 0)
        {
            virtual_pos.x = space.x + relative_pos.x;
        }
        else if(pos.x + relative_pos.x >= space.x)
        {
            virtual_pos.x = -relative_pos.x;
        }

        if(pos.y + relative_pos.y < 0)
        {
            virtual_pos.y = space.y + relative_pos.y;
        }
        else if(pos.y + relative_pos.y >= space.y)
        {
            virtual_pos.y = -relative_pos.y;
        }

        if(pos.z + relative_pos.z < 0)
        {
            virtual_pos.z = space.z + relative_pos.z;
        }
        else if(pos.z + relative_pos.z >= space.z)
        {
            virtual_pos.z = -relative_pos.z;
        }

        return virtual_pos;
    }

    Domain m_domain;
    Subspace m_subspace;

    f_vec m_prefered_orientation;
    _int m_g0;
    double m_velocityShape;
    double m_alpha_g;
    double m_alpha_t;

    std::unordered_map<uint16_t, Grain> m_grains;
};