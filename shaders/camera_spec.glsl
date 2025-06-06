uniform vec2 CameraPos;
uniform vec2 CameraDim;
uniform float CameraZoom;
uniform float CameraDefaultZoom;

vec2
PixelSpaceToScreenSpace(vec2 Pos)
{
    vec2 Result = ((Pos / CameraDim) * 2.) - 1.;
    return Result;
}

vec2
WorldSpaceToPixelSpace(vec2 Pos)
{
    return (CameraZoom * (Pos - CameraPos)) + (CameraDim*.5);
}

vec2
WorldSpaceToScreenSpace(vec2 Pos)
{
    vec2 CameraCorrected = (Pos - CameraPos) * CameraZoom;
    vec2 ScreenSpace = CameraCorrected / (CameraDim*.5);
    return ScreenSpace;
}

vec2
ScreenToWorldSpace(vec2 Pos)
{
    vec2 CameraCorrected = Pos * (CameraDim*.5);
    vec2 WorldSpace = (CameraCorrected / CameraZoom) + CameraPos;
    return WorldSpace;
}
