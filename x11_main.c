#include "x11_main.h"

#define KmapDllPath "kmap.so"

internal void
SetIMPosition(XIC MyInputContext, int X, int Y)
{
    if(MyInputContext)
    {
        XPoint MyPoint = {.x = X, .y = Y};
        XVaNestedList Attributes = XVaCreateNestedList(0, XNSpotLocation, &MyPoint, (void *)0);
        XSetICValues(MyInputContext, XNPreeditAttributes, Attributes, (void *)0);
        XFree(Attributes);
    }
}

typedef struct
{
    u32 PressedKeys;
    u32 PreviouslyPressedKeys;
} game_input;

#if BUILD_INTERNAL
#define LinuxLoadFunc(Name) \
{ \
    Result.Name = dlsym(Result.Handle, #Name); \
    if(Result.Name == 0) \
    { \
        eprintf("dlsym(" #Name "): %s", dlerror()); \
        dlclose(Result.Handle); \
        sleep(1); \
        continue; \
    } \
}
#define LinuxLoadDll() \
{ \
    Result.Handle = dlopen("./" KmapDllPath, RTLD_NOW); \
    if(Result.Handle == 0) \
    { \
        eprintf("dlopen() error: %s\n", dlerror()); \
        sleep(1); \
        continue; \
    } \
}
#else
#define LinuxLoadDll()
#define LinuxLoadFunc(Name) Result.Name = Name;
#endif

DECLARE_LOAD_DLL(LinuxLoadDll, LinuxLoadFunc);

internal void
UnloadKmapDll(kmap_dll *Dll)
{
    dlclose(Dll->Handle);
	*Dll = (kmap_dll){0};
}


internal void *
AllocateMemory(void *DesiredBaseAddress, size_t Size)
{
	void *Result =
        mmap(DesiredBaseAddress, Size,
             PROT_READ|PROT_WRITE,
             MAP_ANONYMOUS|MAP_PRIVATE,
             -1, 0); 
    if(Result == MAP_FAILED)
    {
        ErrorOut(mmap());
    }
	return Result;
}

internal
LOAD_FILE(LoadFile)
{
    load_file_result Result = {0};
    int Handle = open(Path, O_RDONLY);
    if(Handle == -1)
    {
        if(FailIfNotFound)
        {
            eprintf("Path: %s ", Path);
            ErrorOut(open());
        }
        else
        {
#if 0
            char *Error = 0;
            switch(errno)
            {
#define AE(Err) case Err: Error = #Err; break;
                AE(EACCES);
                AE(EBADF);
                AE(EBUSY);
                AE(EDQUOT);
                AE(EEXIST);
                AE(EFAULT);
                AE(EFBIG);
                AE(EINTR);
                AE(EINVAL);
                AE(EISDIR);
                AE(ELOOP);
                AE(EMFILE);
                AE(ENAMETOOLONG);
                AE(ENFILE);
                AE(ENODEV);
                AE(ENOENT);
                AE(ENOMEM);
                AE(ENOSPC);
                AE(ENOTDIR);
                AE(ENXIO);
                AE(EOPNOTSUPP);
                AE(EOVERFLOW);
                AE(EPERM);
                AE(EROFS);
                AE(ETXTBSY);
                AE(EWOULDBLOCK);
            }
            eprintf("open(): (%s) %s\n", Error, strerror(errno));
#else
#endif
            goto CloseHandle;
        }
    }
    struct stat StatBuffer;
    if(fstat(Handle, &StatBuffer) == -1)
    {
        ErrorOut(fstat());
    }
    Result.Size = StatBuffer.st_size;
    Result.Data = mmap(0, Result.Size, PROT_READ, MAP_PRIVATE, Handle, 0);
    if(Result.Data == MAP_FAILED)
    {
        ErrorOut(mmap());
    }
CloseHandle:
    close(Handle);
    return Result;
}

internal
UNLOAD_FILE(UnloadFile)
{
    munmap(MmapResult.Data, MmapResult.Size);
}

internal
WRITE_FILE(WriteFile)
{
    Assert(FileSize);
    int Handle = open(Path, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if(Handle == -1)
    {
        ErrorOut(open());
    }
    int BytesWritten = write(Handle, FileData, FileSize);
    if(BytesWritten == -1)
    {
        ErrorOut(write());
    }
    else if((size_t)BytesWritten != FileSize)
    {
        Die("write(): tried writing %ld bytes, but only written %d\n", FileSize, BytesWritten);
    }
    close(Handle);
}

internal
SLEEP(MySleep)
{
    sleep(Seconds);
}

internal void
UpdateClipboard(Display *Dpy, Window Win, Atom Clipboard, Atom ClipboardProperty, Atom Utf8, input *Input)
{
    if(Input->Clipboard)
    {
        XFree(Input->Clipboard);
    }
    Atom Da, Type;
    int Di;
    unsigned long ClipboardLength, Dul;
    unsigned char *Temp = 0;

    // dummy call to get length of clipboard
    XGetWindowProperty(Dpy, Win, ClipboardProperty, 0, 0, False, AnyPropertyType, &Type, &Di, &Dul, &ClipboardLength, &Temp);
    XFree(Temp);
    Atom Incr = XInternAtom(Dpy, "INCR", False);
    if(Incr == Type)
    {
        eprintf("XInternAtom: data is too large and INCR is not implemented\n");
    }
    XGetWindowProperty(Dpy, Win, ClipboardProperty, 0, ClipboardLength, False, AnyPropertyType, &Da, &Di, &Dul, &Dul, (unsigned char **)&Input->Clipboard);
    XDeleteProperty(Dpy, Win, ClipboardProperty);

    // reinitialize selection
    XConvertSelection(Dpy, Clipboard, Utf8, ClipboardProperty, Win, CurrentTime);
}

int
main(int Argc, char **Argv)
{
    setlocale(LC_CTYPE, "");

    {
        char ExecutablePath[1024];
        size_t ExecutablePathLength = readlink("/proc/self/exe", ExecutablePath, sizeof(ExecutablePath));
        size_t LastSlashPos = 0;
        for(size_t ByteIndex = 0;
            ByteIndex < ExecutablePathLength;
            ++ByteIndex)
        {
            if(ExecutablePath[ByteIndex] == '/')
            {
                LastSlashPos = ByteIndex;
            }
        }

        Assert(LastSlashPos);
        Assert(LastSlashPos + 1 < ExecutablePathLength);

        ExecutablePath[LastSlashPos + 1] = 0;
        chdir(ExecutablePath);
    }

    settings Settings = ParseArgs(Argc, Argv);

	kmap_dll Dll = LoadKmapDll();

    // setup window
    XSetLocaleModifiers("");
    Display *Dpy = XOpenDisplay(0);

    if(!Dpy)
    {
        ErrorOut(XOpenDisplay());
    }

    Window MyRoot = DefaultRootWindow(Dpy);
    GLint MyGlWindowAttributes[] =
    {
        GLX_RGBA, GLX_DEPTH_SIZE, 8, GLX_DOUBLEBUFFER, None
    };
    XVisualInfo *MyVisualInfo = glXChooseVisual(Dpy, 0, MyGlWindowAttributes);
    if(!MyVisualInfo)
    {
        ErrorOut(glXChooseVisual());
    }

    Colormap MyColormap = XCreateColormap(Dpy, MyRoot, MyVisualInfo->visual, AllocNone);
    XSetWindowAttributes MySetWindwoAttributes =
    {
        .colormap = MyColormap,
        .event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | VisibilityChangeMask
    };

    Window MyWindow =
        XCreateWindow(Dpy, MyRoot, 0, 0, 800, 600, 0, MyVisualInfo->depth, InputOutput, MyVisualInfo->visual, CWColormap | CWEventMask, &MySetWindwoAttributes);
    XMapWindow(Dpy, MyWindow);
    XStoreName(Dpy, MyWindow, "Kanji map");

    // setup clipboard
    b32 ClipboardAvailable = true;
    Atom Clipboard = XInternAtom(Dpy, "CLIPBOARD", False);
    Atom Utf8 = XInternAtom(Dpy, "UTF8_STRING", False);
    // NOTE: the atom name is irrelevant
    _Xconst char *ClipboardPropertyName = "clipboard property";
    Atom ClipboardProperty = XInternAtom(Dpy, ClipboardPropertyName, False);
    if((Clipboard != None) && (Utf8 != None))
    {
        XConvertSelection(Dpy, Clipboard, Utf8, ClipboardProperty, MyWindow, CurrentTime);
    }
    else
    {
        ClipboardAvailable = false;
    }

    if(!ClipboardAvailable)
    {
        eprintf("Error: clipboard is not available\n");
    }

    // setup input method
    // https://gist.github.com/Determinant/19bbecb6db35312861f6cf9f54fdd3a5
    XIM MyInputMethod = XOpenIM(Dpy, 0, 0, 0);
    XIC MyInputContext = 0;
    if(MyInputMethod)
    {
        MyInputContext = XCreateIC(MyInputMethod, 
                                   XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                                   XNClientWindow, MyWindow, (void *)0);

        XSetICFocus(MyInputContext);
        SetIMPosition(MyInputContext, 20, 20);
    }

    // setup opengl
    LoadOpenGlExtensions();

    int VisualAttributes[] =
    {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_DEPTH_SIZE, 8,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_SAMPLES, Settings.MsaaSamples,
        None
    };

    int FBConfigCount;
    GLXFBConfig *FBConfigs =
        glXChooseFBConfig(Dpy, DefaultScreen(Dpy),
                          VisualAttributes, &FBConfigCount);

    if(!FBConfigs)
    {
        ErrorOut(glXChooseFBConfig());
    }

    GLXFBConfig ChosenFBConfig = FBConfigs[0];

    int ContextAttributes[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, OPENGL_MAJOR_RELEASE,
        GLX_CONTEXT_MINOR_VERSION_ARB, OPENGL_MINOR_RELEASE,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB|GLX_CONTEXT_DEBUG_BIT_ARB,
        None
    };

    GLXContext MyGLXContext = 
        glXCreateContextAttribsARB(Dpy, ChosenFBConfig, 0, true, ContextAttributes);

    //GLXContext MyGLXContext = glXCreateContext(Dpy, MyVisualInfo, 0, GL_TRUE);
    glXMakeCurrent(Dpy, MyWindow, MyGLXContext);

    glXSwapIntervalEXT(Dpy, MyWindow, 1);

    glEnable(GL_DEBUG_OUTPUT);

    // setup memory
#if BUILD_INTERNAL
    void *MemoryStartAddress = (void *)Terabytes(3);
#else
    void *MemoryStartAddress = 0;
#endif

	size_t MemorySize = Megabytes(256);
	void *MemoryBase = AllocateMemory(MemoryStartAddress, MemorySize);
    char CacheFilePath[512] = {0};
    size_t CachePathLength = CopyString(CacheFilePath, Settings.CachePath);
    char CacheFilename[] = "/kmap_cache";
    CopyString(CacheFilePath + CachePathLength, CacheFilename);

	platform_memory Memory =
	{
		.Size = MemorySize,
		.Base = MemoryBase,
		.Platform = 
		{
			.LoadFile = LoadFile,
			.UnloadFile = UnloadFile,
            .WriteFile = WriteFile,
			.GlGetProcAddress = (void *)glXGetProcAddressARB,
            .Sleep = MySleep
		},
        .FontPath = Settings.FontPath,
        .CachePath = CacheFilePath,
        .ShouldUseLightTheme = Settings.ShouldUseLightTheme,
	};

#if BUILD_INTERNAL
    char ShadersPath[] = "../shaders";
    int InotifyFD = inotify_init1(IN_NONBLOCK);
    if(InotifyFD == -1)
    {
        Die("inotify_init() failed with %s\n", strerror(errno));
    }
    int InotifyWatches[2] =
    {
        inotify_add_watch(InotifyFD, ShadersPath,
                          IN_MODIFY | IN_MOVED_TO),
        inotify_add_watch(InotifyFD, ".",
                          IN_MODIFY | IN_MOVED_TO)
    };

    for(u32 WatchIndex = 0; WatchIndex < ArrayCount(InotifyWatches); ++WatchIndex)
    {
        if(InotifyWatches[WatchIndex] == -1)
        {
            Die("inotify_add_watch() failed with %s\n", strerror(errno));
        }
    }

	b32 ShouldReloadShaders = false;
	b32 ShouldReloadDll = false;
#endif

    input PreviousInput = {0};
    UpdateClipboard(Dpy, MyWindow, Clipboard, ClipboardProperty, Utf8, &PreviousInput);
    u32 MouseButtons = 0;
    int MouseX = 0, MouseY = 0;

    struct timespec InitialTime;
    clock_gettime(CLOCK_REALTIME, &InitialTime);

    for(;;)
    {
        struct timespec ThisFrameTime;
        clock_gettime(CLOCK_REALTIME, &ThisFrameTime);
        input Input = 
        {
            .PressedKeys = 0,//PreviousInput.PressedKeys,
            .Time = DiffTimespec(ThisFrameTime, InitialTime),
            .CodepointBuffer = {0},
            .Clipboard = PreviousInput.Clipboard,
        };

        // NOTE: for some reason, the non-blocking functions (XCheckWindowEvent etc.) don't seem to work here,
        // because every keyboard event gets filtered by the IME
        // Do I not understand something?
        //
        // TODO: to reduce delay, process input on a separate thread into a rolling buffer, and whenever the main program needs input,
        // look up the most recent value in that buffer

        while(XQLength(Dpy))
        {
            XEvent Event;
            XNextEvent(Dpy, &Event);
            if(XFilterEvent(&Event, None))
            {
                continue;
            }

            switch(Event.type)
            {
                case KeyPress:
                {
                    XKeyEvent *KeyEvent = &Event.xkey;
                    KeySym Keysym = XLookupKeysym(KeyEvent, 0);
                    b32 HandledKey = false;
                    //printf("Keysym: %ld, ShiftMask: %d\n", Keysym, !!(KeyEvent->state & ShiftMask));

                    for(u32 KeyIndex = 0; KeyIndex < ArrayCount(KeyDescriptions); ++KeyIndex)
                    {
                        linux_key *Key = KeyDescriptions + KeyIndex;
                        if(Key->Keysym == Keysym)
                        {
                            switch(Key->Purpose)
                            {
                                case KeyPurpose_Assign:
                                {
                                    if(Key->NeedsShift == !!(KeyEvent->state & ShiftMask))
                                    {
                                        Input.PressedKeys |= Key->Parameter.Keycode;
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                } break;
                                case KeyPurpose_Call:
                                {
                                    if(Event.type == KeyPress)
                                    {
                                        Key->Parameter.Func();
                                    }
                                } break;
                                InvalidDefaultCase;
                            }
                            HandledKey = true;
                            break;
                        }
                    }

                    if(!HandledKey && (Event.type == KeyPress))
                    {
                        if(MyInputContext)
                        {
#define InputBufferSize 1024
                            char InputBuffer[InputBufferSize];
                            Status LookupStatus;
                            int InputBufferLength = Xutf8LookupString(MyInputContext, KeyEvent, InputBuffer, InputBufferSize, 0, &LookupStatus);
                            if(LookupStatus == XBufferOverflow)
                            {
                                eprintf("Input string is too long.\n");
                            }
                            else if(InputBufferLength > 0)
                            {
                                // weird issue where control + character doesn't actually return the character, but the corresponding control sequence instead
                                // (who would even want that???)
                                if((InputBufferLength == 1) && !(InputBuffer[0] >= ' ' && InputBuffer[0] <= '~'))
                                {
                                    break;
                                }

                                SetIMPosition(MyInputContext, 20, 20);
                                codepoints Converted = {0};
                                i32 ConvertedBytesSoFar = 0;
                                while(ConvertedBytesSoFar < InputBufferLength)
                                {
                                    ConvertedBytesSoFar += ConvertSingleUtf8(InputBuffer + ConvertedBytesSoFar,
                                                                             Converted.Data + Converted.Count++);
                                }
                                Assert(Input.CodepointBuffer.Count + Converted.Count < ArrayCount(Input.CodepointBuffer.Data));
                                Memcpy(Input.CodepointBuffer.Data + Input.CodepointBuffer.Count, Converted.Data, Converted.Count*sizeof(c32));
                                Input.CodepointBuffer.Count += Converted.Count; 
                            }
                        }
                        else
                        {
                            char InputBuffer[32] = {0};
                            int InputBufferLength = XLookupString(KeyEvent, InputBuffer, 32, 0, 0);
                            codepoints Converted = {0};
                            i32 ConvertedBytesSoFar = 0;
                            while(ConvertedBytesSoFar < InputBufferLength)
                            {
                                ConvertedBytesSoFar += ConvertSingleUtf8(InputBuffer + ConvertedBytesSoFar,
                                                                         Converted.Data + Converted.Count++);
                            }
                            Assert(Input.CodepointBuffer.Count + Converted.Count < ArrayCount(Input.CodepointBuffer.Data));
                            Memcpy(Input.CodepointBuffer.Data + Input.CodepointBuffer.Count, Converted.Data, Converted.Count*sizeof(c32));
                            Input.CodepointBuffer.Count += Converted.Count; 
                        }
                    }
                } break;
                case MotionNotify:
                {
                    MouseX = Event.xmotion.x;
                    MouseY = Event.xmotion.y;
                } break;
                case ButtonPress:
                case ButtonRelease:
                {
                    switch(Event.xbutton.button)
                    {
#define AssignButton(XName, OurName) \
                        case XName: \
                        { \
                            if(Event.type == ButtonPress) \
                            { \
                                MouseButtons |= OurName; \
                            } \
                            else \
                            { \
                                MouseButtons &= ~OurName; \
                            } \
                        } break;
                        AssignButton(Button1, MB_Left);
                        AssignButton(Button2, MB_Middle);
                        AssignButton(Button3, MB_Right);
                        // up is 4, down is 5
                        case Button4:
                        {
                            if(Event.type == ButtonPress)
                            {
                                Input.Mouse.Scroll = 1.f;
                            }
                        } break;
                        case Button5:
                        {
                            if(Event.type == ButtonPress)
                            {
                                Input.Mouse.Scroll = -1.f;
                            }
                        } break;
                    }
                } break;
                default: {}
            }
        }

        // check if shaders need to be reloaded
#if BUILD_INTERNAL
        char Buffer[4096]
            __attribute__ ((aligned(__alignof__(struct inotify_event))));
        for(;;)
        {
            int ReadLen = read(InotifyFD, Buffer, sizeof(Buffer));
            if(ReadLen == -1)
            {
                if(errno == EAGAIN)
                {
                    break;
                }
                else
                {
                    ErrorOut(read());
                }
            }
            struct inotify_event *Event;
            for(char *BufferPos = Buffer;
                BufferPos < Buffer + ReadLen;
                BufferPos += sizeof(struct inotify_event) + Event->len)
            {
                Event = (struct inotify_event *)BufferPos;
                if(!Event->len)
                {
                    Die("No event length on %d ???\n", __LINE__);
                }

                if(Event->wd == InotifyWatches[0])
                {
                    // reload the entire program for now
                    ShouldReloadShaders = true;
                }
                else if(Event->wd == InotifyWatches[1])
                {
                    if(StringsEqual(Event->name, KmapDllPath))
                    {
                        ShouldReloadDll = true;
                    }
                }
            }
        }

        if(ShouldReloadDll)
        {
            Dll.FinishWork(&Memory);
            UnloadKmapDll(&Dll);
            Dll = LoadKmapDll();
            ShouldReloadDll = false;

            printf("Reloaded the dll\n");
        }
#endif

        XWindowAttributes MyXWindowAttributes;
        XGetWindowAttributes(Dpy, MyWindow, &MyXWindowAttributes);
        v2 WindowDim = V2(MyXWindowAttributes.width, MyXWindowAttributes.height);

        Input.Mouse.Coordinates = V2(MouseX, WindowDim.y - MouseY);
        Input.Mouse.Buttons = MouseButtons;
        if(ClipboardAvailable)
        {
            UpdateClipboard(Dpy, MyWindow, Clipboard, ClipboardProperty, Utf8, &Input);
        }

        // draw
        Dll.RenderFrame(&Input, &PreviousInput, WindowDim, &Memory);
        glXSwapBuffers(Dpy, MyWindow);

#if BUILD_INTERNAL
        if(ShouldReloadShaders)
        {
			Dll.ReloadShaders(&Memory);
            printf("Shaders were reloaded\n");
			ShouldReloadShaders = false;
        }
#endif

        XSync(Dpy, False);

        //
        PreviousInput = Input;
    }
    return 0;
}
