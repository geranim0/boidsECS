#include <gtest/gtest.h>
#include <boidsECS/flocon_generator.hpp>

TEST(flocon_generator_gstructure, constructor)
{
    using size_t = std::size_t;
    const size_t sz = 100;
    flocon_generator::gstructure<sz, sz> gs;
    
    for (size_t i = 0; i < gs.array_size; i++)
    {
        EXPECT_EQ(0, gs.image[i]);
    }
}

TEST(flocon_generator_gstructure, insert)
{
    using size_t = std::size_t;
    const uint64_t sz = 100;
    flocon_generator::gstructure<sz, sz> gs;
    
    position2i pos{0,0};

    gs.insert(pos);

    uint64_t mask = (uint64_t)1 << 63;

    EXPECT_NE(0, gs.image[0] & mask);
}