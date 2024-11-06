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

    BondCoat(const float*parameters)
    {
        std::memcpy(m_parameters.data(), parameters, sizeof(float)*FunctionParametersNumber); 
    }

    void fill(Domain& domain);
    
    void parameters(float* param) 
    { 
        std::memcpy(m_parameters.data(), param, sizeof(float)*FunctionParametersNumber); 
    }
    const float* parameters() const 
    { 
        return m_parameters.data(); 
    }

    float maxThickness() 
    { 
        return *std::max_element(m_parameters.begin(), m_parameters.end());
    }

    private:
    std::array<float, FunctionParametersNumber> m_parameters;

    float shape_function(float x, float z) const
    {
        return m_parameters[0]*sin(m_parameters[1]*x) + m_parameters[2]*cos(m_parameters[3]*x) 
            + m_parameters[4]*sin(m_parameters[5]*z) + m_parameters[6]*cos(m_parameters[7]*z) + m_parameters[8];
    }
};