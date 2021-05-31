#version 140

out vec4 colorOut;

uniform uint w;
uniform uint h;

uniform uint num_ints_w;
uniform uint[35000] pixel_data;
 
void main()
{
    uint x = uint(round(gl_FragCoord.x * w));
    uint y = uint(round(gl_FragCoord.y * h));

    uint col = x / uint(32);
    uint mask = uint(1) << (uint(31) - (x % uint(32)));

    float activ = (pixel_data[y * num_ints_w + col] & mask) != uint(0) ? 1.0 : 0.0;

    colorOut = vec4(activ * 1.0, activ*1.0, activ*1.0, 1.0);
}