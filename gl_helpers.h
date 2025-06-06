#ifndef GL_HELPERS_H
#define GL_HELPERS_H

// opengl extensions
#if OS_LINUX
#include <GL/glx.h>
#include <GL/glxext.h>
#define GlPlatformSpecificExtensions(X) \
    X(PFNGLXSWAPINTERVALEXTPROC, glXSwapIntervalEXT), \
    X(PFNGLXCREATECONTEXTATTRIBSARBPROC, glXCreateContextAttribsARB)
#elif OS_WINDOWS
#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/wglext.h>
#include <GL/glext.h>
#define GlPlatformSpecificExtensions(X) \
    X(PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB), \
    X(PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT), \
    X(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB)
#else
#warn "Define platform-specific extensions for your platform"
#endif

typedef struct
{
    const GLubyte *GlName;
    void **Address;
} opengl_extension;

#define GlCommonExtensions(X) \
    X(PFNGLCREATESHADERPROC, glCreateShader), \
    X(PFNGLSHADERSOURCEPROC, glShaderSource), \
    X(PFNGLCOMPILESHADERPROC, glCompileShader), \
    X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog), \
    X(PFNGLGETSHADERIVPROC, glGetShaderiv), \
    X(PFNGLDELETESHADERPROC, glDeleteShader), \
    X(PFNGLDELETEPROGRAMPROC, glDeleteProgram), \
    X(PFNGLCREATEPROGRAMPROC, glCreateProgram), \
    X(PFNGLATTACHSHADERPROC, glAttachShader), \
    X(PFNGLLINKPROGRAMPROC, glLinkProgram), \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv), \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog), \
    X(PFNGLUSEPROGRAMPROC, glUseProgram), \
    X(PFNGLBINDBUFFERPROC, glBindBuffer), \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray), \
    X(PFNGLBINDBUFFERBASEPROC, glBindBufferBase), \
    X(PFNGLBUFFERDATAPROC, glBufferData), \
    X(PFNGLBUFFERSUBDATAPROC, glBufferSubData), \
    X(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback), \
    X(PFNGLDEBUGMESSAGECONTROLPROC, glDebugMessageControl), \
    X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation), \
    X(PFNGLUNIFORM1FPROC, glUniform1f), \
    X(PFNGLUNIFORM2FPROC, glUniform2f), \
    X(PFNGLUNIFORM4FPROC, glUniform4f), \
    X(PFNGLUNIFORM1UIPROC, glUniform1ui), \
    X(PFNGLUNIFORM4FVPROC, glUniform4fv), \
    X(PFNGLUNIFORM3FVPROC, glUniform3fv), \
    X(PFNGLGENBUFFERSPROC, glGenBuffers), \
    X(PFNGLMAPBUFFERPROC, glMapBuffer), \
    X(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange), \
    X(PFNGLFLUSHMAPPEDBUFFERRANGEPROC, glFlushMappedBufferRange), \
    X(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays), \
    X(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap), \
    X(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer), \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray), \
    X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer), \
    X(PFNGLTEXSTORAGE2DPROC, glTexStorage2D), \
    X(PFNGLTEXSTORAGE1DPROC, glTexStorage1D), \
    X(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced), \
    X(PFNGLVERTEXATTRIBDIVISORPROC, glVertexAttribDivisor) \

#define GlExtensions(X) \
    GlPlatformSpecificExtensions(X), \
    GlCommonExtensions(X)

#define CreateExtensionVariable(Type, Name) UNUSED__##Type__##Name; internal Type Name

internal char
GlExtensions(CreateExtensionVariable);

#define DeclareExtension(Type, Name) (opengl_extension){ .GlName = (const GLubyte *)#Name, .Address = (void **)&Name }

internal opengl_extension GlobalGlExtensions[] =
{
    GlExtensions(DeclareExtension)
};

typedef void (* gl_function)(void);
typedef gl_function load_gl_extension(const GLubyte *ExtensionName);
#if OS_LINUX
load_gl_extension *LoadGlExtension = glXGetProcAddressARB;
#elif OS_WINDOWS
internal gl_function
WinGlGetProcAddress(const GLubyte *ExtensionName)
{
    gl_function Result = (gl_function)wglGetProcAddress((const char *)ExtensionName);
    return Result;
}
load_gl_extension *LoadGlExtension = WinGlGetProcAddress;
#else
#error "fill this out"
#endif

internal void
LoadOpenGlExtensions()
{
    Foreach(ExtensionIndex, ArrayCount(GlobalGlExtensions))
    {
        opengl_extension *Extension = GlobalGlExtensions + ExtensionIndex;
        void *Address = LoadGlExtension(Extension->GlName);

        if(!Address)
        {
            Die("Couldn't load extension %s()\n", Extension->GlName);
        }
        *Extension->Address = Address;
    }
}

#endif // GL_HELPERS_H
