#include <boidsECS/helpers.hpp>

bool operator==(position2i const& lhs, position2i const& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator<(position2i const& lhs, position2i const& rhs)
{
    if (lhs.x < rhs.x)
    {
        return true;
    }
    else if (lhs.x > rhs.x)
    {
        return false;
    }
    else
    {
        return lhs.y < rhs.y;
    }
}