layout(location = 0) out vec4 oColor;

uniform vec4 Color1;
uniform vec4 Color2;
uniform float Time;

float rand(vec2 p) { return fract(sin(dot(p, vec2(12.9898, 4.1414)))*43758.5453);}

float noise(vec2 p)
{
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.-2.*u);
    float res = mix(mix(rand(ip), rand(ip+vec2(1.,0.)), u.x),
                    mix(rand(ip+vec2(0.,1.)), rand(ip+vec2(1.)), u.x),
                    u.y);
    return res*res;
}

float fbm(vec2 p, vec2 Off)
{
    const mat2 mtx = mat2(.8, .6, -.6, .8);
    float f = 0.;
    f +=     .5*noise(p + Off); p = mtx*p*2.01;
    f +=    .25*noise(p); p = mtx*p*2.02;
    f +=   .125*noise(p); p = mtx*p*2.03;
    f +=  .0625*noise(p); p = mtx*p*2.04;
    f += .03125*noise(p + sin(Off));
    return f/.96875;
}

void
main()
{
    vec2 Pt = .01*(gl_FragCoord.xy/CameraZoom + CameraPos);
    float Noise = fbm(.1*Pt, vec2(-Time*.02))*fbm(Pt + fbm(Pt, vec2(Time*.51, Time*.2)), vec2(Time*.49, Time*0.1));
    oColor = mix(Color1, Color2, Noise);
}
