layout(location = 0) in vec2 iTextureTopLeft;
layout(location = 1) in vec2 iTextureBottomRight;
layout(location = 2) in vec2 iScreenBottomLeft;
layout(location = 3) in vec2 iScreenTopRight;
layout(location = 4) in vec3 iColor;
layout(location = 5) in float iDepth;

layout(location = 0) out vec2 oTexCoord;
layout(location = 1) out vec3 oColor;

vec2 Masks[4] =
{
    vec2(0., 1.), // bottom right
    vec2(0., 0.), // top right
    vec2(1., 1.), // bottom left
    vec2(1., 0.)  // top left
};

// Pos: mask * P1 + (1 - mask) * P2

void
main()
{
    vec2 ScreenBottomLeft = PixelSpaceToScreenSpace(iScreenBottomLeft);
    vec2 ScreenTopRight = PixelSpaceToScreenSpace(iScreenTopRight);

    vec2 Mask = Masks[gl_VertexID];
    vec2 Position = mix(ScreenTopRight, ScreenBottomLeft, Mask);
    // NOTE: since screen y is the reverse of texture, we pass them the other way as well
    vec2 TextureCoordinate = mix(iTextureBottomRight, iTextureTopLeft, Mask);
    gl_Position = vec4(Position, iDepth, 1.);
    oTexCoord = TextureCoordinate;
    oColor = iColor.rgb;
}
