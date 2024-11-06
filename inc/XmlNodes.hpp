#pragma once

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "basictypes.hpp"

namespace XmlNodes
{

    struct Time
    {
        static constexpr const char* n_name = "Time";
        static constexpr const char* n_dt = "dt";

        static Time parse(rapidxml::xml_node<>* node);

        float dt;
    };

    struct Front
    {
        static constexpr const char* n_name = "Front";
        static constexpr const char* n_vb = "vb";

        static Front parse(rapidxml::xml_node<>* node);

        float vb;
    };
    

    struct Space
    {
        static constexpr const char* n_name = "Space";
        static constexpr const char* n_dimX = "dimX";
        static constexpr const char* n_dimY = "dimY";
        static constexpr const char* n_dimZ = "dimZ";
        
        static Space parse(rapidxml::xml_node<>* node);

        _long_int dimX, dimY, dimZ;
    };

    struct Output
    {
        static constexpr const char* n_name = "Output";
        static constexpr const char* n_filename = "filename";
        static constexpr const char* n_format = "format";

        static Output parse(rapidxml::xml_node<>* node);

        std::string filename;
        std::string format;
    };

    struct Neighbourhood
    {
        static constexpr const char* n_name = "Neighbourhood";
        static constexpr const char* n_x0 = "x0";
        static constexpr const char* n_x1 = "x1";
        static constexpr const char* n_y0 = "y0";
        static constexpr const char* n_y1 = "y1";
        static constexpr const char* n_z0 = "z0";
        static constexpr const char* n_z1 = "z1";

        static Neighbourhood parse(rapidxml::xml_node<>* node);

        _long_int x0, x1;
        _long_int y0, y1;
        _long_int z0, z1;
    };

    struct Bond
    {
        static constexpr const char* n_name = "Bond";
        static constexpr const char* n_A = "A";
        static constexpr const char* n_B = "B";
        static constexpr const char* n_C = "C";
        static constexpr const char* n_D = "D";
        static constexpr const char* n_E = "E";
        static constexpr const char* n_F = "F";
        static constexpr const char* n_G = "G";
        static constexpr const char* n_H = "H";
        static constexpr const char* n_Z = "Z";

        static Bond parse(rapidxml::xml_node<>* node);

        float A, B, C, D, E, F, G, H, Z;
    };

    struct Layer
    {
        static constexpr const char* n_name = "Layer";
        static constexpr const char* n_grains_number = "grains_number";
        static constexpr const char* n_height = "height";
        static constexpr const char* n_tilt_stddev = "tilt_stddev";
        static constexpr const char* n_alpha_g = "alpha_g";
        static constexpr const char* n_alpha_t = "alpha_t";
        static constexpr const char* n_anisotropy = "anisotropy";
        static constexpr const char* n_diffusion = "diffusion";

        struct PreferedOrientation
        {
            static constexpr const char* n_name = "prefered_orientation";
            static constexpr const char* n_x = "x";
            static constexpr const char* n_y = "y";
            static constexpr const char* n_z = "z";

            static PreferedOrientation parse(rapidxml::xml_node<>* node);

            float x, y, z;
        } prefered_orientation;

        static Layer parse(rapidxml::xml_node<>* node);

        std::string name;
        _int grains_number;
        _long_int height;
        float tilt_stddev;
        float alpha_g{0.5};
        float alpha_t{4.0};
        float anisotropy;
        float diffusion;
    };
    
    struct Parallel
    {
        static constexpr const char* n_name = "Parallel";
        static constexpr const char* n_cpu_threads = "cpu_threads";
        static constexpr const char* n_gpu = "gpu";

        static Parallel parse(rapidxml::xml_node<>* node);

        bool gpu;
        _int cpu_threads;
    };
}
