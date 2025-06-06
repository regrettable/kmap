layout(location = 0) in vec2 iTexCoord;
layout(location = 1) in vec3 iColor;

layout(location = 0) out vec4 oColor;

uniform sampler2D TextureAtlas;

void
main()
{
    float TexColor = texture(TextureAtlas, iTexCoord).r;
    oColor = vec4(iColor, TexColor);
}
