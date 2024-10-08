#pragma once

#include<cstring>
#include"basictypes.hpp"
#include<array>
#include<algorithm>

class Domain;

class BondCoat
{
    public:
    static constexpr int FunctionParametersNumber = 9;

    BondCoat(const double*parameters)
    {
        std::memcpy(m_parameters.data(), parameters, sizeof(double)*FunctionParametersNumber); 
    }

    void fill(Domain& domain);
    
    void parameters(double* param) 
    { 
        std::memcpy(m_parameters.data(), param, sizeof(double)*FunctionParametersNumber); 
    }
    const double* parameters() const 
    { 
        return m_parameters.data(); 
    }

    double maxThickness() 
    { 
        return *std::max_element(m_parameters.begin(), m_parameters.end());
    }

    private:
    std::array<double, FunctionParametersNumber> m_parameters;

    double shape_function(double x, double z) const
    {
        return m_parameters[0]*sin(m_parameters[1]*x) + m_parameters[2]*cos(m_parameters[3]*x) 
            + m_parameters[4]*sin(m_parameters[5]*z) + m_parameters[6]*cos(m_parameters[7]*z) + m_parameters[8];
    }
};