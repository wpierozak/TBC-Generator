#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "basictypes.hpp"  // Assuming this file contains the f_vec structure and other typedefs
#include "bondcoat.hpp"
#include "domain.hpp"

struct Configuration {
    struct Time {
        float dt;

        void print() const {
            std::cout << "Time:\n  dt: " << dt << "\n";
        }
    } time;

    struct Front {
        float vb;

        void print() const {
            std::cout << "Front:\n  vb: " << vb << "\n";
        }
    } front;

    struct Space {
        _long_int dimX, dimY, dimZ;

        void print() const {
            std::cout << "Space:\n  dimX: " << dimX
                      << "\n  dimY: " << dimY
                      << "\n  dimZ: " << dimZ << "\n";
        }
    } space;

    struct Layer {
        float tilt_stddev;
        float alpha_g;
        float alpha_t;
        float dk;
        float diff;
        float cosAlphaG[27];
        f_vec prefered_orientation;
        _int grainsNumber;
        _long_int layer_height;

        void print() const {
            std::cout << "Layer:\n"
                      << "  tilt_stddev: " << tilt_stddev << "\n"
                      << "  alpha_g: " << alpha_g << "\n"
                      << "  alpha_t: " << alpha_t << "\n"
                      << "  dk: " << dk << "\n"
                      << "  diff: " << diff << "\n"
                      << "  grainsNumber: " << grainsNumber << "\n"
                      << "  layer_height: " << layer_height << "\n"
                      << "  prefered_orientation: (" 
                      << prefered_orientation.x << ", " 
                      << prefered_orientation.y << ", " 
                      << prefered_orientation.z << ")\n";
        }
    };

    std::vector<Layer> layers;

    struct Parallel {
        _int cpu_threads;
        bool gpu;

        void print() const {
            std::cout << "Parallel:\n"
                      << "  cpu_threads: " << cpu_threads << "\n"
                      << "  gpu: " << (gpu ? "true" : "false") << "\n";
        }
    } parallel;

    struct Output {
        std::string filename;
        std::string format;

        void print() const {
            std::cout << "Output:\n"
                      << "  filename: " << filename << "\n"
                      << "  format: " << format << "\n";
        }
    } output;

    struct Bond {
        float parameters[BondCoat::FunctionParametersNumber];

        void print() const {
            std::cout << "Bond Parameters:\n";
            for (size_t i = 0; i < BondCoat::FunctionParametersNumber; ++i) {
                std::cout << "  param[" << i << "]: " << parameters[i] << "\n";
            }
        }
    } bond;

    // Constructor, Assignment, and Destructor as previously defined

    void print() const {
        std::cout << "Configuration:\n";
        time.print();
        front.print();
        space.print();
        std::cout << "Layers:\n";
        for (size_t i = 0; i < layers.size(); ++i) {
            std::cout << "  Layer " << i + 1 << ":\n";
            layers[i].print();
        }
        parallel.print();
        output.print();
        bond.print();
    }
};
