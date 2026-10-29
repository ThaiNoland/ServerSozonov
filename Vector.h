#pragma once
#include <vector>

class Vector {
private:
    std::vector<float> values;

public:
    Vector() = default;
    Vector(const std::vector<float>& data);
    float calculateSum() const;
};

