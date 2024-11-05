#pragma once

#include <vector>
#include <string>
#include "basictypes.hpp"  // Assuming this file contains the f_vec structure and other typedefs
#include"bondcoat.hpp"
#include "domain.hpp"

struct Configuration {
    struct Time{
        Time() = default;
        Time(const Time&) = default;
        double dt;

    } time;

    struct Front{
        Front() = default;
        Front(const Front&) = default;

        double vb;
    } front;

    struct Space {
        _long_int dimX, dimY, dimZ;

        // Copy constructor
        Space() = default;
        Space(const Space& other) = default;

        // Assignment operator
        Space& operator=(const Space& other) = default;
    } space;

    struct Layer {
        Layer() = default;

        // Copy constructor
        Layer(const Layer& other) = default;

        // Assignment operator
        Layer& operator=(const Layer& other) = default;

        double tilt_stddev;
        double alpha_g;
        double alpha_t;
        double dk;
        double diff;

        double cosAlphaG[27];

        f_vec prefered_orientation;
        _int grainsNumber;
        _long_int layer_height;
    };

    std::vector<Layer> layers;

    Neighbourhood neighbourhood;

    struct Parallel {
        _int cpu_threads;
        bool gpu;

        // Copy constructor
        Parallel() = default;
        Parallel(const Parallel& other) = default;

        // Assignment operator
        Parallel& operator=(const Parallel& other) = default;
    } parallel;

    struct Output {
        std::string filename;
        std::string format;

        // Copy constructor
        Output() = default;
        Output(const Output& other) = default;

        // Assignment operator
        Output& operator=(const Output& other) = default;
    } output;

    struct Bond {
        double parameters[BondCoat::FunctionParametersNumber];    
    } bond;

    Configuration(const Configuration& other)
        : space(other.space), layers(other.layers),
          neighbourhood(other.neighbourhood),
          parallel(other.parallel), output(other.output) 
          {
            time = other.time;
            front = other.front;
            std::memcpy(bond.parameters, other.bond.parameters, sizeof(double) * BondCoat::FunctionParametersNumber);
          }

    // Assignment operator for Configuration
    Configuration& operator=(const Configuration& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        space = other.space;
        layers = other.layers;
        neighbourhood = other.neighbourhood;
        parallel = other.parallel;
        output = other.output;
        time = other.time;
        front = other.front;
        std::memcpy(bond.parameters, other.bond.parameters, sizeof(double) * BondCoat::FunctionParametersNumber);

        return *this;
    }

    // Default constructor
    Configuration() = default;

    // Destructor
    ~Configuration() = default;

};
