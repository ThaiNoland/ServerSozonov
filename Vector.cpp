#include "Vector.h"
#include <cmath>
#include <limits>

Vector::Vector(const std::vector<float>& data) : values(data) {}

float Vector::calculateSum() const {
    float sum = 0.0f;
    for (float val : values) {
        sum += val;
        if (std::isinf(sum)) {
            if (sum > 0) {
                return std::numeric_limits<float>::infinity();
            } else {
                return -std::numeric_limits<float>::infinity();
            }
        }
    }
    return sum;
}

