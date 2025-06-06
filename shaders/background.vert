void
main()
{
    vec2 WindowRectVerts[4] = vec2[4](vec2(-1.), vec2(-1.,1.), vec2(1.,-1.), vec2(1.));
    gl_Position = vec4(WindowRectVerts[gl_VertexID], 0., 1.);
}
