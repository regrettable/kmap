layout(location = 0) in flat vec4 iColor;
layout(location = 1) in vec2 iVertexCoord;
layout(location = 2) in flat vec2 iHalfDim;

layout(location = 0) out vec4 oColor;

uniform vec4 BackgroundColor;
uniform float HighlightedBorderWidth;

void
main()
{
    vec2 BorderDim = vec2(HighlightedBorderWidth);
    vec2 Coefficients = smoothstep(iHalfDim - BorderDim,
                                   iHalfDim,
                                   abs(iVertexCoord));
    float Coefficient = clamp(max(Coefficients.x, Coefficients.y), 0., 1.);
    oColor = mix(BackgroundColor, iColor, Coefficient);
}
