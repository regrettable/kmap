layout(location = 0) in vec2 iPos;

layout(location = 0) out vec4 oColor;
layout(location = 1) out float oCenterDist;

uniform vec2 FirstPositionWorld;
uniform float LineWidthInPixels;

uniform vec4 Color1;
uniform vec4 Color2;

vec2 Masks[4] =
{
    vec2(1., 1.),  // P1 + Perp (bottom left)
    vec2(1., -1.), // P1 - Perp (bottom right)
    vec2(0., 1.),  // P2 + Perp (top left)
    vec2(0., -1.)  // P2 - Perp (top right)
};

void
main()
{
    vec2 FirstPosition = WorldSpaceToScreenSpace(FirstPositionWorld);
    vec2 SecondPosition = WorldSpaceToScreenSpace(iPos);

    vec2 Line = normalize(FirstPosition - SecondPosition);
    vec2 LineWidth = LineWidthInPixels / CameraDim;
    // NOTE: since we do antialiasing ourselves,
    // make the line twice as big to account for it.
    vec2 Perp = vec2(-Line.y, Line.x) * 2. * LineWidth;
    vec2 Mask = Masks[gl_VertexID];
    vec2 Position = mix(FirstPosition, SecondPosition, Mask.x) + (Perp * Mask.y);
    gl_Position = vec4(Position, 0., 1.);
    oCenterDist = Mask.y;
    oColor = mix(Color1, Color2, Mask.x);
}
