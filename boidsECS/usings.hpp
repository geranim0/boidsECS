#pragma once

#include <cstdint>

using Id = std::size_t;

//model uint
using muint = uint;

class DeltaTime
{
public:
    explicit DeltaTime(double val)
    : value(val)
    {}

    explicit operator double() const noexcept
    {
        return value;
    }

private:
    double value;
};