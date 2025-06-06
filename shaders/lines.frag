layout(location = 0) in vec4 iColor;
layout(location = 1) in float iCenterDist;

layout(location = 0) out vec4 oColor;

uniform float Time;

void
main()
{
#if 1

    float Alpha = (1. - abs(iCenterDist));
    oColor = vec4(iColor.rgb, Alpha);
#else
    oColor = vec4(iColor.rgb, 1.);
#endif
}
