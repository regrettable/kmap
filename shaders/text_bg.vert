layout(location = 0) in vec2 iTopLeft;
layout(location = 1) in vec2 iBottomRight;
layout(location = 2) in vec4 iColor;
layout(location = 3) in float iDepth;

layout(location = 0) flat out vec4 oColor;
layout(location = 1) out vec2 oVertexCoord;
layout(location = 2) flat out vec2 oHalfDim;

uniform float DictionaryEntryPadding;

vec2 Masks[4] =
{
    vec2(1., 1.), // bottom right
    vec2(0., 1.), // top right
    vec2(1., 0.), // bottom left
    vec2(0., 0.), // top left
};

vec2 DimMasks[4] =
{
    vec2(-1., -1.),
    vec2(1., -1.),
    vec2(-1., 1.),
    vec2(1., 1.)
};

void
main()
{
    vec2 BgDim = (iBottomRight - iTopLeft);

    vec2 HalfDim = BgDim*.5;
    oColor = iColor;
    vec2 Mask = Masks[gl_VertexID];
    gl_Position =
        vec4(mix(PixelSpaceToScreenSpace(iBottomRight + DictionaryEntryPadding),
                 PixelSpaceToScreenSpace(iTopLeft - DictionaryEntryPadding),
                 Mask), iDepth, 1.);
    oVertexCoord = HalfDim*DimMasks[gl_VertexID];
    oHalfDim = HalfDim;
}
