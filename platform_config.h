#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

typedef enum
{
    KeyPurpose_Assign,
    KeyPurpose_Call
} key_purpose;

typedef void key_func();

internal void
ExitProgram()
{
    Exit(0);
}

typedef union
{
    int VirtualKey;
    key_func *Func;
    key_code Keycode;
} key_parameter;

#if OS_WINDOWS
typedef struct
{
    b32 NeedsShift;
    UINT Keysym;
    key_purpose Purpose;
    key_parameter Parameter;
} windows_key;

// put your windows keys below
global windows_key KeyDescriptions[] =
{
    { 0, '-', KeyPurpose_Assign, {.Keycode = Key_ZoomOut} },
    { 0, '+',  KeyPurpose_Assign, {.Keycode = Key_ZoomIn} },
    { 0, '=', KeyPurpose_Assign, {.Keycode = Key_ZoomReset} },
    { 0, VK_F1, KeyPurpose_Assign, {.Keycode = Key_ChangeTheme} },
    { 0, VK_F2, KeyPurpose_Assign, {.Keycode = Key_ToggleKanjiOverlay} },
    { 0, VK_F3, KeyPurpose_Assign, {.Keycode = Key_Paste} },
    { 0, VK_F4, KeyPurpose_Assign, {.Keycode = Key_PreviousWordInSearch} },
    { 0, VK_F5, KeyPurpose_Assign, {.Keycode = Key_NextWordInSearch} },
    { 0, VK_F6, KeyPurpose_Assign, {.Keycode = Key_ToggleBookmarkView} },
    { 0, VK_RETURN, KeyPurpose_Assign, {.Keycode = Key_Search} },
    { 0, VK_BACK, KeyPurpose_Assign, {.Keycode = Key_Backspace} },
    { 0, VK_DELETE, KeyPurpose_Assign, {.Keycode = Key_Delete} },
    { 0, VK_LEFT, KeyPurpose_Assign, {.Keycode = Key_CursorLeft} },
    { 0, VK_RIGHT, KeyPurpose_Assign, {.Keycode = Key_CursorRight} },
    { 0, VK_TAB, KeyPurpose_Assign, {.Keycode = Key_NextSimilarWord} },
    { 1, VK_TAB, KeyPurpose_Assign, {.Keycode = Key_PreviousSimilarWord} },
    { 0, VK_ESCAPE, KeyPurpose_Call, {.Func = ExitProgram} }
};
#elif OS_LINUX
typedef struct
{
    b32 NeedsShift;
    KeySym Keysym;
    key_purpose Purpose;
    key_parameter Parameter;
} linux_key;

// put your linux keys below
global linux_key KeyDescriptions[] =
{
    { 0, XK_minus, KeyPurpose_Assign, {.Keycode = Key_ZoomOut} },
    { 0, XK_plus,  KeyPurpose_Assign, {.Keycode = Key_ZoomIn} },
    { 0, XK_equal, KeyPurpose_Assign, {.Keycode = Key_ZoomReset} },
    { 0, XK_F1, KeyPurpose_Assign, {.Keycode = Key_ChangeTheme} },
    { 0, XK_F2, KeyPurpose_Assign, {.Keycode = Key_ToggleKanjiOverlay} },
    { 0, XK_F3, KeyPurpose_Assign, {.Keycode = Key_Paste} },
    { 0, XK_F4, KeyPurpose_Assign, {.Keycode = Key_PreviousWordInSearch} },
    { 0, XK_F5, KeyPurpose_Assign, {.Keycode = Key_NextWordInSearch} },
    { 0, XK_F6, KeyPurpose_Assign, {.Keycode = Key_ToggleBookmarkView}},
    { 0, XK_Return, KeyPurpose_Assign, {.Keycode = Key_Search} },
    { 0, XK_BackSpace, KeyPurpose_Assign, {.Keycode = Key_Backspace} },
    { 0, XK_Delete, KeyPurpose_Assign, {.Keycode = Key_Delete} },
    { 0, XK_Left, KeyPurpose_Assign, {.Keycode = Key_CursorLeft} },
    { 0, XK_Right, KeyPurpose_Assign, {.Keycode = Key_CursorRight} },
    { 0, XK_Tab, KeyPurpose_Assign, {.Keycode = Key_NextSimilarWord} },
    { 1, XK_Tab, KeyPurpose_Assign, {.Keycode = Key_PreviousSimilarWord} },
    { 0, XK_Escape, KeyPurpose_Call, {.Func = ExitProgram} }
};
#else
#warning "fill this out"
#endif


#endif // PLATFORM_CONFIG_H
