#include "win_main.h"

global v2 GlobalWindowDim;

#if BUILD_INTERNAL
#define KmapDllPath "kmap.dll"
#define WindowsLoadDll() \
{ \
    Result.Handle = LoadLibraryA(KmapDllPath); \
    if(Result.Handle == INVALID_HANDLE_VALUE) \
    { \
        sleep(1); \
        continue; \
    } \
}
#define WindowsLoadFunc(Name) \
{ \
    Result.Name = (void *)GetProcAddress(Result.Handle, #Name); \
    if(!Result.Name) \
    { \
        eprintf("GetProcAddress(" #Name ") failed\n"); \
        FreeLibrary(Result.Handle); \
        sleep(1); \
        continue; \
    } \
}
#else
#define WindowsLoadDll()
#define WindowsLoadFunc(Name) Result.Name = Name;
#endif

DECLARE_LOAD_DLL(WindowsLoadDll, WindowsLoadFunc);

internal void
UnloadKmapDll(kmap_dll *Dll)
{
	FreeLibrary(Dll->Handle);
	*Dll = (kmap_dll){0};
}

internal void *
AllocateMemory(void *DesiredBaseAddress, size_t Size)
{
    void *Result =
        VirtualAlloc(DesiredBaseAddress,
                     Size,
                     MEM_COMMIT|MEM_RESERVE, // large pages here
                     PAGE_READWRITE);
	return Result;
}

internal
LOAD_FILE(LoadFile)
{
    load_file_result Result = {0};
	HANDLE FileHandle =
		CreateFile(Path,
				   GENERIC_READ,
				   FILE_SHARE_READ|FILE_SHARE_WRITE,
				   0,
				   OPEN_EXISTING,
				   FILE_ATTRIBUTE_NORMAL,
				   0);

	if(FileHandle == INVALID_HANDLE_VALUE)
	{
        if(FailIfNotFound)
        {
            eprintf("Path not found: %s\n", Path);
            ErrorOut(CreateFile());
        }
        else
        {
            return Result;
        }
	}

	HANDLE FileMapping = 
        CreateFileMapping(FileHandle,
                          0,
                          PAGE_READONLY,
                          0,0,
                          Path);
	if(FileMapping == 0)
	{
		ErrorOut(CreateFileMapping());
	}
	Result.Data = MapViewOfFile(FileMapping,
								FILE_MAP_READ,
								0,
								0,
								0);
	if(Result.Data == 0)
	{
		DWORD ErrorCode = GetLastError();
		ErrorOut(MapViewOfFile());
	}

	MEMORY_BASIC_INFORMATION Unused;
	Result.Size = VirtualQuery(Result.Data, &Unused, sizeof(Unused));
	if(Result.Size == 0)
	{
		ErrorOut(VirtualQuery());
	}

	CloseHandle(FileMapping);
	CloseHandle(FileHandle);
    return Result;
}

internal
UNLOAD_FILE(UnloadFile)
{
	UnmapViewOfFile(MmapResult.Data);
}

internal
WRITE_FILE(MyWriteFile)
{
	HANDLE FileHandle =
		CreateFile(Path,
				   GENERIC_WRITE,
				   FILE_SHARE_READ|FILE_SHARE_WRITE,
				   0,
				   CREATE_ALWAYS,
				   FILE_ATTRIBUTE_NORMAL,
				   0);
    if(FileHandle == INVALID_HANDLE_VALUE)
    {
        eprintf("Couldn't open file %s\n", Path);
        ErrorOut(CreateFile());
    }

    DWORD BytesWritten;
    BOOL WriteSuccess = WriteFile(FileHandle, (LPCVOID)FileData, (DWORD)FileSize, &BytesWritten, 0);
    if(!WriteSuccess)
    {
        ErrorOut(WriteFile());
    }
    CloseHandle(FileHandle);
}

SLEEP(MySleep)
{
    sleep(Seconds);
}

internal void
UpdateWindowDim(HWND Window)
{
    RECT WindowRect;
    GetClientRect(Window, &WindowRect);
    GlobalWindowDim.x = WindowRect.right - WindowRect.left;
    GlobalWindowDim.y = WindowRect.bottom - WindowRect.top;
}

//int
//main(int Argc, char **Argv)
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    {
        char *ExecutableFilename = _pgmptr;
        u32 LastBackslashPos = 0;
        u32 ExecutableFilenameLength = 0;
        for(;ExecutableFilename[ExecutableFilenameLength];
            ++ExecutableFilenameLength)
        {
            if(ExecutableFilename[ExecutableFilenameLength] == '\\')
            {
                LastBackslashPos = ExecutableFilenameLength;
            }
        }
        Assert(LastBackslashPos);

        char *ExecutableDirectory = (char *)alloca(ExecutableFilenameLength);
        Memcpy(ExecutableDirectory, ExecutableFilename, LastBackslashPos);

        SetCurrentDirectory(ExecutableDirectory);

    }
    int Argc = 0;
    char **Argv = (char **)CommandLineToArgvW(GetCommandLineW(), &Argc);

    settings Settings = ParseArgs(Argc, Argv);

    WNDCLASSA WindowClass = 
    {
        .style = CS_HREDRAW|CS_VREDRAW,
        .lpfnWndProc = DefWindowProc,
        .hInstance = Instance,
        .lpszClassName = "KanjiMapWindowClass",
    };

    if(!RegisterClassA(&WindowClass))
    {
        ErrorOut(RegisterClassA());
    }

    // create fake context and load functionality
    HWND DummyWindow =
        CreateWindowEx(WS_EX_LAYERED, // WS_EX_TOPMOST
                       WindowClass.lpszClassName,
                       "Kanji Map",
                       WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       0, 0,
                       Instance,
                       0);
    if(!DummyWindow)
    {
        ErrorOut(CreateWindowEx());
    }

    HDC DummyDC = GetDC(DummyWindow);

    PIXELFORMATDESCRIPTOR DesiredFormatDescriptor =
    {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .iLayerType = PFD_MAIN_PLANE
    };

    int FalsePixelFormat = ChoosePixelFormat(DummyDC, &DesiredFormatDescriptor);
    PIXELFORMATDESCRIPTOR ActualFormatDescriptor;
    DescribePixelFormat(DummyDC, FalsePixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &ActualFormatDescriptor);
    SetPixelFormat(DummyDC, FalsePixelFormat, &ActualFormatDescriptor);

    HGLRC DummyGLRC = wglCreateContext(DummyDC);
    wglMakeCurrent(DummyDC, DummyGLRC);

    LoadOpenGlExtensions();

    wglMakeCurrent(DummyDC, 0);
    wglDeleteContext(DummyGLRC);
    ReleaseDC(DummyWindow, DummyDC);
    DestroyWindow(DummyWindow);


    ///
    HWND Window =
        CreateWindowEx(WS_EX_LAYERED, // WS_EX_TOPMOST
                       WindowClass.lpszClassName,
                       "Kanji Map",
                       WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       0, 0,
                       Instance,
                       0);
    if(!Window)
    {
        ErrorOut(CreateWindowEx());
    }

    SetCursor(LoadCursor(0, IDC_ARROW));

    HDC WindowDC = GetDC(Window);
    SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);

    // create the actual context
    int PixelFormatAttributes[] = 
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_SAMPLES_ARB, Settings.MsaaSamples,
        0
    };
    int PixelFormat;
    UINT PixelFormatCount;
    wglChoosePixelFormatARB(WindowDC, PixelFormatAttributes, 0, 1, &PixelFormat, &PixelFormatCount);
    if(!PixelFormatCount)
    {
        Die("OpenGL: couldn't get the desired pixel format\n");
    }
    PIXELFORMATDESCRIPTOR PixelFormatDescriptor;
    DescribePixelFormat(WindowDC, PixelFormat, sizeof(PixelFormatDescriptor), &PixelFormatDescriptor);
    if(!SetPixelFormat(WindowDC, PixelFormat, &PixelFormatDescriptor))
    {
        Die("OpenGL: failed to set pixel format\n");
    }

    int OpenglAttributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, OPENGL_MAJOR_RELEASE,
        WGL_CONTEXT_MINOR_VERSION_ARB, OPENGL_MINOR_RELEASE,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };

    HGLRC OpenglContext = wglCreateContextAttribsARB(WindowDC, 0, OpenglAttributes);
    if(!OpenglContext)
    {
        Die("OpenGL: couldn't create context\n");
    }
    wglMakeCurrent(WindowDC, OpenglContext);

    wglSwapIntervalEXT(1);

    UpdateWindowDim(Window);
    
    kmap_dll Dll = LoadKmapDll();

#if BUILD_INTERNAL
    void *MemoryStartAddress = (void *)Terabytes(3);
#else
    void *MemoryStartAddress = 0;
#endif

	size_t MemorySize = Megabytes(64);
    void *MemoryBase = AllocateMemory(MemoryStartAddress, MemorySize);

    platform_memory Memory =
    {
        .Size = MemorySize,
        .Base = MemoryBase,
        .Platform =
        {
            .LoadFile = LoadFile,
            .UnloadFile = UnloadFile,
            .WriteFile = MyWriteFile,
            .GlGetProcAddress = (void *)wglGetProcAddress,
            .Sleep = MySleep
        },
        .FontPath = Settings.FontPath,
        .CachePath = "./kmap_cache",
        .ShouldUseLightTheme = Settings.ShouldUseLightTheme,
    };

#if BUILD_INTERNAL
    // something to read:
    // https://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw_19.html
    char ShadersPath[] = "../shaders";
    HANDLE NotifyHandles[2] =
    {
        // shaders
        FindFirstChangeNotificationA(ShadersPath,
                                     FALSE,
                                     FILE_NOTIFY_CHANGE_SIZE),
        // dll
        FindFirstChangeNotificationA(".",
                                     FALSE,
                                     FILE_NOTIFY_CHANGE_SIZE),
    };

    b32 ShouldReloadShaders = false;
    b32 ShouldReloadDll = false;
#endif

    input PreviousInput = {0};
    LARGE_INTEGER TimerFrequency;
    QueryPerformanceFrequency(&TimerFrequency);

    LARGE_INTEGER StartTime;
    QueryPerformanceCounter(&StartTime);

    for(;;)
    {
        input Input = {0};

        // handle input here
        {
            MSG Message;
            while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
            {
                switch(Message.message)
                {
                    case WM_DESTROY:
                    case WM_CLOSE:
                    {
                        Exit(0);
                    } break;
                    case WM_MOUSEWHEEL:
                    {
                        Input.Mouse.Scroll = ((f32)GET_WHEEL_DELTA_WPARAM(Message.wParam)) / 120.f;
                    } break;
                    case WM_SYSKEYUP:
                    case WM_SYSKEYDOWN:
                    case WM_KEYUP:
                    case WM_KEYDOWN:
                    {
                        b32 IsPressed = ((Message.lParam & (1 << 31)) == 0);

                        b32 ShiftIsPressed = !!GetKeyState(VK_SHIFT);
                        if(IsPressed)
                        {
                            b32 HandledKey = false;
                            Foreach(KeyIndex, ArrayCount(KeyDescriptions))
                            {
                                windows_key *Key = KeyDescriptions + KeyIndex;
                                if(Message.wParam == Key->Keysym &&
                                   (Key->NeedsShift == ShiftIsPressed))
                                {
                                    switch(Key->Purpose)
                                    {
                                        case KeyPurpose_Assign:
                                        {
                                            Input.PressedKeys |= Key->Parameter.Keycode;
                                        } break;
                                        case KeyPurpose_Call:
                                        {
                                            Key->Parameter.Func();
                                        } break;
                                    }
                                    HandledKey = true;
                                }
                            }
                            if(!HandledKey)
                            {
                                TranslateMessage(&Message);
                                DispatchMessage(&Message);
                            }
                        }
                    } break;
                    case WM_CHAR:
                    case WM_SYSCHAR:
                    {
                        c32 Codepoint = (c32)Message.wParam;
                        Assert(Input.CodepointBuffer.Count + 1 < ArrayCount(Input.CodepointBuffer.Data));
                        Input.CodepointBuffer.Data[Input.CodepointBuffer.Count++] = Codepoint;
                    } break;
                    case WM_SETCURSOR:
                    {
                        SetCursor(LoadCursor(0, IDC_ARROW));
                    } break;
                }
                DispatchMessage(&Message);
            }

            POINT MousePos;
            GetCursorPos(&MousePos);
            ScreenToClient(Window, &MousePos);
            Input.Mouse.Coordinates.x = MousePos.x;
            Input.Mouse.Coordinates.y = MousePos.y;
#define AssignButton(WindowsButton, OurButton) \
            if(GetKeyState(WindowsButton) & (1 << 15)) \
            { \
                Input.Mouse.Buttons |= OurButton; \
            } \
            else \
            { \
                Input.Mouse.Buttons &= ~OurButton; \
            }
            AssignButton(VK_LBUTTON, MB_Left);
            AssignButton(VK_MBUTTON, MB_Middle);
            AssignButton(VK_RBUTTON, MB_Right);

            // clipboard
            do
            {
                if(!OpenClipboard(Window))
                {
                    eprintf("Windows: couldn't open the clipboard\n");
                    break;
                }
                HANDLE Clipboard;
                if(!(Clipboard = GetClipboardData(CF_UNICODETEXT)))
                {
                    eprintf("Windows: couldn't get clipboard data\n");
                    CloseClipboard();
                    break;
                }
                WCHAR *Utf16Clipboard;
                if(!(Utf16Clipboard = GlobalLock(Clipboard)))
                {
                    eprintf("Windows: failed to lock the clipboard\n");
                    CloseClipboard();
                    break;
                }
                int Utf8ClipboardSize;
                if(!(Utf8ClipboardSize = WideCharToMultiByte(CP_UTF8, 0, Utf16Clipboard, -1, 0, 0, 0, 0)))
                {
                    eprintf("Windows: failed to get clipboard size\n");
                    GlobalUnlock(Clipboard);
                    CloseClipboard();
                    break;
                }

                if(Input.Clipboard)
                {
                    free(Input.Clipboard);
                }
                Input.Clipboard = (char *)calloc(Utf8ClipboardSize, 1);
                if(!WideCharToMultiByte(CP_UTF8, 0, Utf16Clipboard, -1, Input.Clipboard, Utf8ClipboardSize, 0, 0))
                {
                    eprintf("Windows: failed to convert clipboard to utf-8\n");
                    free(Input.Clipboard);
                    Input.Clipboard = 0;
                    GlobalUnlock(Clipboard);
                    CloseClipboard();
                    break;
                }

                GlobalUnlock(Clipboard);
                CloseClipboard();
            } while(0);
        }

        //

#if BUILD_INTERNAL
        DWORD WaitStatus = WaitForMultipleObjects(ArrayCount(NotifyHandles), NotifyHandles, FALSE, 0);
        switch(WaitStatus)
        {
            case WAIT_ABANDONED:
            {
                Die("WAIT_ABANDONED on line %d ???????\n", __LINE__);
            } break;
            case WAIT_OBJECT_0:
            {
                ShouldReloadShaders = true;
                if(FindNextChangeNotification(NotifyHandles[0]) == FALSE)
                {
                    Die("FindNextChangeNotification() failed on line %d\n", __LINE__)
                }
            } break;
            case WAIT_OBJECT_0 + 1:
            {
                ShouldReloadDll = true;
                if(FindNextChangeNotification(NotifyHandles[1]) == FALSE)
                {
                    Die("FindNextChangeNotification() failed on line %d\n", __LINE__)
                }
            } break;
            case WAIT_TIMEOUT:
            {
                // do nothing
            } break;
            case WAIT_FAILED:
            {
                Die("WaitForSingleObject() failed\n");
            } break;
            default:
            { 
                Die("WaitForSingleObject() returned a strange value: %ld\n", WaitStatus);
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

        UpdateWindowDim(Window);
        Input.Mouse.Coordinates.y = GlobalWindowDim.y - Input.Mouse.Coordinates.y;
        LARGE_INTEGER CurrentTime;
        QueryPerformanceCounter(&CurrentTime);
        Input.Time = (f64)(CurrentTime.QuadPart - StartTime.QuadPart) / (f64)TimerFrequency.QuadPart;

        Dll.RenderFrame(&Input, &PreviousInput, GlobalWindowDim, &Memory);

#if BUILD_INTERNAL
        if(ShouldReloadShaders)
        {
            Dll.ReloadShaders(&Memory);
            printf("Shaders were reloaded\n");
            ShouldReloadShaders = false;
        }
#endif
        SwapBuffers(WindowDC);

        //
        PreviousInput = Input;
    }
    return 0;
}
