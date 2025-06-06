layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iColor;

layout(location = 0) out vec3 oColor;

void main()
{
    gl_Position = vec4(PixelSpaceToScreenSpace(iPos.xy), 1., 1.);
    gl_PointSize = iPos.z;
    oColor = iColor;
}
