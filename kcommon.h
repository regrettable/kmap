#ifndef KCOMMON_H
#define KCOMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define StaticAssert(Expr) _Static_assert((Expr), #Expr)

#define true 1
#define false 0

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int32_t b32;

typedef float f32;
typedef double f64;

// NOTE: our character type instead of platform-dependent wchar_t
typedef int32_t c32;

StaticAssert(sizeof(f32) == 4);
StaticAssert(sizeof(f64) == 8);

#define internal static
#define global static

#define Kilobytes(N) (1024LL*(N))
#define Megabytes(N) (1024LL*Kilobytes(N))
#define Gigabytes(N) (1024LL*Megabytes(N))
#define Terabytes(N) (1024LL*Gigabytes(N))

#define Maximum(a, b) ((a) > (b) ? (a) : (b))
#define Minimum(a, b) ((a) < (b) ? (a) : (b))

#ifdef __linux__
#define OS_LINUX 1
#define OS_WINDOWS 0
#elif defined(_WIN32)
#define OS_LINUX 0
#define OS_WINDOWS 1
#include <windows.h>
#else
#error "Unknown operating system"
#endif

#include <stdlib.h>
#define Exit(ReturnCode) exit(ReturnCode)

#define Foreach(Index, Count) for(u32 Index = 0; Index < (Count); ++Index)
#define ArrayCount(Arr) (sizeof(Arr)/sizeof(Arr[0]))

#if OS_WINDOWS
#include <dbghelp.h>
#define PrintBacktrace() \
{ \
    void *Stack[256]; \
    HANDLE Process = GetCurrentProcess(); \
    SymInitialize(Process, NULL, TRUE); \
    unsigned short FrameCount = CaptureStackBackTrace(0, 256, Stack, NULL); \
    SYMBOL_INFO *Symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256, 1); \
    Symbol->MaxNameLen = 255; \
    Symbol->SizeOfStruct = sizeof(SYMBOL_INFO); \
    eprintf("Stack backtrace (line %d, file %s, function %s):\n", __LINE__, __FILE__, __FUNCTION__); \
    for(unsigned short FrameIndex = 0; FrameIndex < FrameCount; FrameIndex++) \
    { \
        SymFromAddr(Process, (DWORD64)(Stack[FrameIndex]), 0, Symbol); \
        eprintf("[%d]: %s [0x%llx]\n", FrameCount - FrameIndex - 1, Symbol->Name, Symbol->Address); \
    } \
    free(Symbol); \
}
#elif OS_LINUX
#include <execinfo.h>
#define PrintBacktrace() \
{ \
    void *Backtraces[256]; \
    int BacktraceCount = backtrace(Backtraces, (int)ArrayCount(Backtraces)); \
    char **BacktraceStrings = backtrace_symbols((void * const *)Backtraces, BacktraceCount); /* this can return null. why??? */ \
    if(BacktraceStrings) \
    { \
        eprintf("Stack backtrace (line %d, file %s, function %s):\n", __LINE__, __FILE__, __FUNCTION__); \
        for(int BacktraceIndex = 0; BacktraceIndex < BacktraceCount; ++BacktraceIndex) \
        { \
            eprintf("[%d]: %s\n", BacktraceIndex, BacktraceStrings[BacktraceIndex]); \
        } \
        free(BacktraceStrings); \
    } \
}
#else
#define PrintBacktrace()
#endif

#if BUILD_INTERNAL
#define BreakHere do { __asm__ __volatile__ ("int $3"); __attribute__((unused)) int a = 0; __attribute__((unused)) int b = 0; } while(0);
#define Assert(Expr) do { if(!(Expr)) { PrintBacktrace(); BreakHere; }} while(0);
#else
#define BreakHere
#define Assert(Expr) do { if(!(Expr)) { PrintBacktrace(); Exit(1); }} while(0);
#endif

#define InvalidCodePath do {Assert(!true)} while(0);
#define InvalidDefaultCase default: { InvalidCodePath; }


#define eprintf(Format...) fprintf(stderr, Format)
#define Die(...) { PrintBacktrace(); eprintf(__VA_ARGS__); Exit(1); }

#if OS_WINDOWS
#define ErrorOut(Cause) \
{ \
	DWORD Code = GetLastError(); \
	char *ErrorMessage = 0; \
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | \
				   FORMAT_MESSAGE_FROM_SYSTEM | \
   				   FORMAT_MESSAGE_IGNORE_INSERTS,  \
   				   0, Code, \
				   MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), \
				   (LPSTR)&ErrorMessage, 0, 0); \
	Die(#Cause " failed on line %d of %s with: 0x%lx %s", __LINE__, __FILE__, Code, ErrorMessage); \
}
    
#elif OS_LINUX
#include <errno.h>
#include <string.h>
#define ErrorOut(Cause) { Die(#Cause " failed on line %d of %s with %s\n", __LINE__, __FILE__, strerror(errno)); }
#else
#error "implement this"
#endif

internal b32
StringsEqual(char *Str1, char *Str2)
{
    b32 Result = true;
    for(u32 CharIndex = 0;
        Str1[CharIndex] != 0;
        ++CharIndex)
    {
        if(Str1[CharIndex] != Str2[CharIndex])
        {
            Result = false;
            break;
        }
        if(Str1[CharIndex] == '\0')
        {
            break;
        }
    }
    return Result;
}

internal size_t
CopyStringLen(char *Src, char *Dest, size_t Len)
{
    size_t BytesCopied = 0;
    for(;
        BytesCopied < Len;
        ++BytesCopied)
    {
        char C = *(Src + BytesCopied);
        Dest[BytesCopied] = C;
        if(!C)
        {
            break;
        }
    }
    return BytesCopied;
}

#define Swap(X, Y) {typeof(X) SwapTemp = (X); (X) = (Y); (Y) = SwapTemp; }

typedef struct
{
    void *Base;
    size_t TotalSize;
    void *Top;
    size_t ArrayElementSize;
    u32 TempCount;
    u32 ArrayCount;
} memory_arena;

internal memory_arena
InitializeArena(void *Base, size_t TotalSize)
{
    Assert(Base);

    memory_arena Result =
    {
        .Base = Base,
        .TotalSize = TotalSize,
        .Top = Base,
        .TempCount = 0,
        .ArrayElementSize = 0
    };
    return Result;
}

internal size_t
GetArenaSize(memory_arena *Arena)
{
    size_t Result = (size_t)(Arena->Top - Arena->Base);
    return Result;
}

#define PushStaticArray(Arena, Type, Num) (Type *)PushSize(Arena, sizeof(Type)*Num)
#define PushStruct(Arena, Type) (Type *)PushSize(Arena, sizeof(Type))

internal void *
PushSize(memory_arena *Arena, size_t Size)
{
    Assert(Size > 0);
    Assert(((Arena)->ArrayElementSize == 0) || (Arena->ArrayElementSize == Size));
    void *PreviousTop = Arena->Top; 
    void *NewTop = (u8 *)PreviousTop + Size;

    Assert((u8 *)NewTop < ((u8 *)Arena->Base + Arena->TotalSize));
    Assert(NewTop > PreviousTop);
    Arena->Top = NewTop;
    return PreviousTop;
}

// NOTE: the array abstraction below is meant for the times
// where we want to push an unknown amount of array elements in a row
// and is meant to prevent pushing something else by accident

#define PushElement(Arena, Element) \
{ \
    typeof(Element) *Ptr = PushSize((Arena), (Arena)->ArrayElementSize); \
    *Ptr = (Element); \
    ++(Arena)->ArrayCount; \
}

internal void *
StartDynamicArray_(memory_arena *Arena, size_t Size)
{
    Assert(Arena->ArrayElementSize == 0);
    Assert(Arena->ArrayCount == 0);
    Arena->ArrayElementSize = Size;
    return Arena->Top;
}

#define StartDynamicArray(Arena, Type) (Type *)StartDynamicArray_((Arena), sizeof(Type))

internal u32
EndDynamicArray(memory_arena *Arena)
{
    Assert(Arena->ArrayElementSize);
    Arena->ArrayElementSize = 0;
    u32 NumElements = Arena->ArrayCount;
    Arena->ArrayCount = 0;
    return NumElements;
}

internal void
PopSize(memory_arena *Arena, size_t Size)
{
    Assert(Arena->TempCount == 0); // do not use with temps
    Assert(Size > 0);
    void *PreviousTop = Arena->Top;
    void *Top = (u8 *)PreviousTop - Size;
    Assert(Top >= Arena->Base);
    Arena->Top = Top;
}

internal void
ResetArena(memory_arena *Arena)
{
    Assert(Arena->TempCount == 0);
    Arena->Top = Arena->Base;
}

internal void *
ArenaTop(memory_arena *Arena)
{
    return Arena->Top;
}

internal u8
ConvertSingleUtf8(char *Ptr, c32 *Converted)
{
    static u8 ByteCountTable[] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
        2, 2, 2, 2,
        3, 3,
        4, 0
    };

    static u8 ShiftTable[] =
    {
        0, 0, 6, 12, 18
    };

    static u8 AndTable[] =
    {
        0, 127, 31, 15, 7
    };

    u32 C = (u8)*Ptr++;
    u8 ByteCount = ByteCountTable[C >> 3];

    Assert(ByteCount);

    u8 ShiftAmount = ShiftTable[ByteCount];
    u8 AndAmount = AndTable[ByteCount];
    C &= AndAmount;
    C <<= ShiftAmount;

    switch(ByteCount)
    {
        case 4:
        {
            C |= ((*Ptr++) & 63) << 12;
        } // fallthrough
        case 3:
        {
            C |= ((*Ptr++) & 63) << 6;
        } // fallthrough
        case 2:
        {
            C |= (*Ptr++) & 63;
        } break;
        default: {};
    }
    *Converted = C;
    return ByteCount;
}

internal memory_arena *
PushArena(memory_arena *OldArena, size_t NewSize)
{
    // reserve place for the struct itself
    memory_arena *Result = PushStruct(OldArena, memory_arena);
    void *NewBase = PushSize(OldArena, NewSize);
    *Result = InitializeArena(NewBase, NewSize);
    return Result;
}

internal memory_arena
Subarena(memory_arena *OldArena, size_t NewSize)
{
    void *NewBase = PushSize(OldArena, NewSize);
    memory_arena Result = InitializeArena(NewBase, NewSize);
    return Result;
}

typedef struct
{
    memory_arena *Arena;
    void *Top;
} temp_memory;

internal temp_memory
BeginTempMemory(memory_arena *Arena)
{
    temp_memory Result;
    Result.Arena = Arena;
    Result.Top = Arena->Top;
    ++Arena->TempCount;
    return Result;
}

internal void
EndTempMemory(temp_memory Temp)
{
    memory_arena *Arena = Temp.Arena;
    Assert(Arena->TempCount > 0);
    Arena->TempCount--;
    Assert(Arena->Top >= Temp.Top);
    Arena->Top = Temp.Top;
}

internal void
Memset(void *Dest, u8 Value, size_t DestSize)
{
    Assert(DestSize);

    size_t TotalInts = DestSize >> 2;
    u32 WideValue = Value << 24 | Value << 16 | Value << 8 | Value;
    for(u32 U32Index = 0;
        U32Index < TotalInts;
        ++U32Index)
    {
        ((u32 *)Dest)[U32Index] = WideValue;
    }
    for(u32 ByteIndex = 0;
        ByteIndex < (DestSize & 3);
        ++ByteIndex)
    {
        ((u8 *)Dest)[ByteIndex] = Value;
    }
}

#define ZeroArray(Arr) Memset((Arr), 0, sizeof(Arr))

inline void Memcpy(void *Dst, void *Src, size_t Size);

void
Memcpy(void *Dst, void *Src, size_t Size)
{
    for(size_t Count = 0;
        Count < Size;
        ++Count)
    {
        ((char *)Dst)[Count] = ((char *)Src)[Count];
    }
}

#include "kmath.h"

typedef enum
{
	MB_Left = 1,
	MB_Right = 1 << 1,
    MB_Middle = 1 << 2,
} mouse_button;

typedef enum
{
    Key_ZoomIn = 1,
    Key_ZoomOut = 1 << 1,
    Key_ZoomReset = 1 << 2,
    Key_ChangeTheme = 1 << 3,
    Key_PreviousSimilarWord = 1 << 4,
    Key_NextSimilarWord = 1 << 5,
    Key_ToggleKanjiOverlay = 1 << 6,
    Key_Paste = 1 << 7,
    Key_CursorLeft = 1 << 8,
    Key_CursorRight = 1 << 9,
    Key_Backspace = 1 << 10,
    Key_Delete = 1 << 11,
    Key_Search = 1 << 12,
    Key_NextWordInSearch = 1 << 13,
    Key_PreviousWordInSearch = 1 << 14,
    Key_ToggleBookmarkView = 1 << 15,
} key_code;

typedef struct
{
	v2 Coordinates;
    f32 Scroll;
	u32 Buttons;
} mouse;

#define MaxCodepointsInWord 256
typedef struct
{
    u32 Count;
    c32 Data[MaxCodepointsInWord];
} codepoints;

inline codepoints ConvertToCodepoints(char *String);
codepoints ConvertToCodepoints(char *String)
{
    codepoints Result = {0};
    while(*String)
    {
        String += ConvertSingleUtf8(String, Result.Data + Result.Count++);
    }
    Assert(Result.Count < ArrayCount(Result.Data));
    return Result;
}

internal size_t
EncodeCodepointsToUtf8(codepoints Codepoints, char *Result, size_t MaxSize)
{
    size_t Size = 0;
    Foreach(CodepointIndex, Codepoints.Count)
    {
        c32 Codepoint = Codepoints.Data[CodepointIndex];
        size_t CodepointSize = 0;
        if(Codepoint < 0x7f)
        {
            CodepointSize = 1;
            Result[0] = Codepoint;
        }
        else if(Codepoint < 0x7ff)
        {
            CodepointSize = 2;
            Result[0] = (Codepoint >> 6) | 0xc0;
            Result[1] = (Codepoint & 0x3f) | 0x80;
        }
        else if(Codepoint < 0xffff)
        {
            CodepointSize = 3;
            Result[0] = (Codepoint >> 12) | 0xe0;
            Result[1] = ((Codepoint >> 6) & 0x3f) | 0x80;
            Result[2] = (Codepoint & 0x3f) | 0x80;
        }
        else
        {
            CodepointSize = 4;
            Result[0] = (Codepoint >> 18) | 0xf0;
            Result[1] = ((Codepoint >> 12) & 0x3f) | 0x80;
            Result[2] = ((Codepoint >> 6) & 0x3f) | 0x80;
            Result[3] = (Codepoint & 0x3f) | 0x80;
        }
        Result += CodepointSize;
        Size += CodepointSize;
        Assert(Size < MaxSize);
    }
    Assert((Size + 1) < MaxSize);
    Result[Size + 1] = 0;
    Size++;
    return Size;
}


typedef struct
{
    mouse Mouse;
    u32 PressedKeys;
    codepoints CodepointBuffer;
    char *Clipboard;
    f64 Time;
} input;

#if OS_WINDOWS
inline void sleep(int Sec);

void sleep(int Sec)
{
	Sleep(Sec*1000);
}
#endif

#include <time.h>
internal f64
DiffTimespec(struct timespec Time1, struct timespec Time2)
{
    f64 IntDiff = difftime(Time1.tv_sec, Time2.tv_sec);
    f64 FractDiff = ((f64)(Time1.tv_nsec - Time2.tv_nsec) / 1000000000.);
    f64 Result = IntDiff + FractDiff;
    return Result;
}

typedef enum
{
    CT_None = 0,
    CT_Text = 1,
    CT_Highlight = 2,
    CT_Background = 3,
    CT_Line1 = 4,
    CT_Line2 = 5,
    //
    CT_Count
} color_type;

// NOTE: don't forget padding
typedef struct
{
    v4 Colors[CT_Count];
    f32 LineWidth;
    f32 DictionaryEntryPadding;
    f32 UiFontPixelHeight;
    f32 UiBorderWidth;
} theme;

#if DEBUG_INFO
typedef struct
{
    long long unsigned int Cycles;
    f64 Seconds;
    char *Name;
    u32 CallCount;
} timing;

global u32 GlobalTimingLevel;
global timing GlobalTimings[1024],
       PreviousFrameTimings[1024];

#include <x86intrin.h> // _rdtsc

#define StartTiming(BlockName) \
    struct timespec TimeStart_##BlockName; \
    clock_gettime(CLOCK_REALTIME, &TimeStart_##BlockName); \
    long long unsigned int RdtscStart_##BlockName = _rdtsc(); \
    ++GlobalTimingLevel;

#define EndTiming_(BlockName, BlockNameStr) \
    long long unsigned int RdtscEnd_##BlockName = _rdtsc(); \
    struct timespec TimeEnd_##BlockName; \
    clock_gettime(CLOCK_REALTIME, &TimeEnd_##BlockName); \
    timing *Timing_##BlockName = GlobalTimings + __COUNTER__; \
    Timing_##BlockName->Cycles += RdtscEnd_##BlockName - RdtscStart_##BlockName; \
    Timing_##BlockName->Seconds += DiffTimespec(TimeEnd_##BlockName, TimeStart_##BlockName); \
    Timing_##BlockName->Name = BlockNameStr; \
    Timing_##BlockName->CallCount++;

#define EndTiming(BlockName) EndTiming_(BlockName, #BlockName)

#else
#define StartTiming(BlockName)
#define EndTiming(BlockName)
#define EndTiming_(BlockName, BlockNameStr)
#define EndTimingLoop(BlockName, LoopCount)
#endif

#define EndTimingFunc() EndTiming_(Func, __FUNCTION__)
#define StartTimingFunc() StartTiming(Func)

typedef struct
{
    size_t Size;
    void *Data;
} load_file_result;

// requested functions
#define LOAD_FILE(Name) load_file_result Name(char *Path, b32 FailIfNotFound)
typedef LOAD_FILE(load_file);

#define UNLOAD_FILE(Name) void Name(load_file_result MmapResult)
typedef UNLOAD_FILE(unload_file);

#define WRITE_FILE(Name) void Name(char *Path, size_t FileSize, void *FileData)
typedef WRITE_FILE(write_file);

#define SLEEP(Name) void Name(int Seconds)
typedef SLEEP(platform_sleep);

typedef struct
{
	load_file *LoadFile;
	unload_file *UnloadFile;
    write_file *WriteFile;
    platform_sleep *Sleep;
	void *GlGetProcAddress;
} platform_functions;

typedef struct
{
	void *Base;
	size_t Size;
	platform_functions Platform;
    char *FontPath;
    char *CachePath;
    b32 ShouldUseLightTheme;
} platform_memory;

// provided functions
#define RENDER_FRAME(Name) void Name(input *Input, input *PreviousInput, v2 WindowDim, platform_memory *Memory)
typedef RENDER_FRAME(render_frame);

#define RELOAD_SHADERS(Name) void Name(platform_memory *Memory)
typedef RELOAD_SHADERS(reload_shaders);

#define FINISH_WORK(Name) void Name(platform_memory *Memory)
typedef FINISH_WORK(finish_work);

// cross-platform functionality

typedef struct
{
#if OS_WINDOWS
	HANDLE Handle;
#elif OS_LINUX
    void *Handle;
#endif
	render_frame *RenderFrame;
	reload_shaders *ReloadShaders;
	finish_work *FinishWork;
} kmap_dll;

#define DECLARE_LOAD_DLL(LoadDll, LoadFunc) \
internal kmap_dll \
LoadKmapDll() \
{ \
	kmap_dll Result = {0}; \
	for(;;) \
    { \
        LoadDll(); \
        LoadFunc(RenderFrame); \
        LoadFunc(ReloadShaders); \
        LoadFunc(FinishWork); \
        break; \
    } \
	return Result; \
}

internal size_t
CopyString(char *Dest, char *Src)
{
    size_t CopiedLength = 0;
    for(;Src[CopiedLength]; ++CopiedLength)
    {
        Dest[CopiedLength] = Src[CopiedLength];
    }
    return CopiedLength;
}

typedef struct
{
    u8 MsaaSamples;
    char *FontPath;
    char *CachePath;
    b32 ShouldUseLightTheme;
} settings;

internal settings
ParseArgs(int Argc, char **Argv)
{
    settings Settings =
    {
        .MsaaSamples = 0,
        .FontPath = 0,
        .CachePath = 0,
        .ShouldUseLightTheme = false
    };
    for(int ArgIndex = 0;
        ArgIndex < Argc;
        ++ArgIndex)
    {
#define IsArg(Name) StringsEqual(Argv[ArgIndex], (Name))
        if(IsArg("-f"))
        {
            Settings.FontPath = Argv[ArgIndex + 1];
        }
        else if(IsArg("-c"))
        {
            Settings.CachePath = Argv[ArgIndex + 1];
        }
        else if(IsArg("-t"))
        {
            if(ArgIndex != (Argc - 1))
            {
                switch(Argv[ArgIndex + 1][0])
                {
                    case 'l':
                    {
                        Settings.ShouldUseLightTheme = true;
                    } break;
                    case 'd':
                    {
                        Settings.ShouldUseLightTheme = false;
                    } break;
                    default:
                    {
                        eprintf("Warning: an invalid theme argument after -t, using the default theme...\n");
                    };
                }
            }
            else
            {
                eprintf("Warning: argument required after -t, using default theme...\n");
            }
        }
        else if(IsArg("-h") || IsArg("--help"))
        {
            eprintf("USAGE: %s [-f FONTPATH]\n", Argv[0]);
        }
        else if(IsArg("-s") || IsArg("--samples"))
        {
            Settings.MsaaSamples = 0;
            for(char *Char = Argv[ArgIndex + 1];
                *Char;
                ++Char)
            {
                char Digit = *Char;
                if((Digit < '0') || (Digit > '9'))
                {
                    eprintf("Warning: invalid amount of samples (expected a number), falling back to 4...\n");
                    Settings.MsaaSamples = 4;
                    break;
                }
                Settings.MsaaSamples *= 10;
                Settings.MsaaSamples += Digit - '0';
            }
        }
    }

    if(!Settings.FontPath)
    {
#if OS_WINDOWS
        char *Fonts[] =
        {
            //"C:\\windows\\Fonts\\msgothic.ttc",
            //"C:\\windows\\Fonts\\NotoSerifCJK-Regular.ttc",
            //"C:\\windows\\Fonts\\meriyo.ttc",
            //"C:\\windows\\Fonts\\sourcehansans.ttc",
            "/usr/share/fonts/noto-cjk/NotoSerifCJK-Regular.ttc" // for wine
        };
#elif OS_LINUX
        char *Fonts[] =
        {
            "/usr/share/fonts/noto-cjk/NotoSerifCJK-Regular.ttc",
            "/usr/share/fonts/adobe-source-han-serif/SourceHanSerif-Regular.ttc"
        };
#else
#error "set the preferred font paths"
#endif
        u32 FirstExistingFontIndex = -1;
        Foreach(FontIndex, ArrayCount(Fonts))
        {
            FILE *Font = fopen(Fonts[FontIndex], "r");
            if(Font)
            {
                fclose(Font);
                FirstExistingFontIndex = FontIndex;
                break;
            }
        }
        if(FirstExistingFontIndex > ArrayCount(Fonts))
        {
            eprintf("Couldn't find any fonts to render chinese characters. Tried these:\n");
            Foreach(FontIndex, ArrayCount(Fonts))
            {
                eprintf("\t%s\n", Fonts[FontIndex]);
            }
            eprintf("and couldn't open any. Try specifying a font as a -f argument when launching the program.\n");
            Exit(1);
        }
#if BUILD_INTERNAL
        eprintf("Using %s\n", Fonts[FirstExistingFontIndex]);
#endif

        Settings.FontPath = Fonts[FirstExistingFontIndex];
    }

    if(!Settings.CachePath)
    {
#if BUILD_INTERNAL
        Settings.CachePath = ".";
#else
#if OS_LINUX
        char *RelevantVariables[] =
        {
            "KMAP_CACHE_PATH",
            "XDG_CACHE_HOME",
        };
        Foreach(VariableIndex, ArrayCount(RelevantVariables))
        {
            Settings.CachePath = getenv(RelevantVariables[VariableIndex]);
            if(Settings.CachePath)
            {
                break;
            }
        }
        if(!Settings.CachePath)
        {
            char *HomeDir = getenv("HOME");
            if(!HomeDir)
            {
                Die("Couldn't determine the appropriate cache location. Set KMAP_CACHE_PATH environment variable or pass the path with -c argument to kmap\n");
            }
            else
            {
                Settings.CachePath = calloc(1024, 1);
                size_t HomeDirLen = CopyString(Settings.CachePath, HomeDir);
                char CacheDir[] = "/.cache";
                CopyString(Settings.CachePath + HomeDirLen, CacheDir);
            }
        }
#elif OS_WINDOWS
        Settings.CachePath = ".";
#else
#warning "Set the cache path"
#endif
#endif
    }
    return Settings;
}

#endif // ifndef KCOMMON_H
