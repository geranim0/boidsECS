#include <gtest/gtest.h>
#include <boidsECS/bresenham.hpp>

TEST(bresenham, works_as_expected)
{
    std::vector<position2i> result;
    bool eq;
    
    result = bresenham::bresenham(position2i{0, 0}, position2i{0, 0});
    eq = result == std::vector<position2i>{{0, 0}, };
    EXPECT_TRUE(eq);

    result = bresenham::bresenham(position2i{0, 0}, position2i{0, 1});
    eq = result == std::vector<position2i>{{0, 0}, {0, 1}, };
    EXPECT_TRUE(eq);

    result = bresenham::bresenham(position2i{0, 0}, position2i{1, 0});
    eq = result == std::vector<position2i>{{0, 0}, {1, 0}, };
    EXPECT_TRUE(eq);

    result = bresenham::bresenham(position2i{0, 0}, position2i{1, 1});
    eq = result == std::vector<position2i>{{0, 0}, {1, 1}, };
    EXPECT_TRUE(eq);

    result = bresenham::bresenham(position2i{-1, -4}, position2i{1, 4});
    eq = result == std::vector<position2i>{{-1, -4}, {0, -3}, {0, -2}, {0, -1}, {0, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, };
    EXPECT_TRUE(eq);

    result = bresenham::bresenham(position2i{-3, -1}, position2i{4, 1});
    eq = result == std::vector<position2i>{{-3, -1}, {-2, 0}, {-1, 0}, {0, 0}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, };
    EXPECT_TRUE(eq);


}