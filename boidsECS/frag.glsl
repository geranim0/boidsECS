#version 430 core

out vec4 colorOut;

uniform int num_ints_w;

layout(std430, binding = 0) buffer pixel_data
{
    uint values[];
};
 
void main()
{
    uint x = uint(gl_FragCoord.x);
    uint y = uint(gl_FragCoord.y);
    uint col = x / uint(32);

    //needed because we actually access 64 bit uints as 2 32 bit uints
    //and the first we access (we would like it to be the high-bits one)
    //is the low-bits one 
    if (col % 2 == 1)
    {
        col -= 1;
    }
    else
    {
        col += 1;
    }

    uint mask = uint(1) << (uint(31) - (x % uint(32)));

    if ((values[y * num_ints_w + col] & mask) != 0)
    {
        colorOut = vec4(1.0, 1.0, 1.0, 1.0);
    }
    else
    {
        colorOut = vec4(0.0, 0.0, 0.0, 0.0);
    }
}