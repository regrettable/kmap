#ifndef KMAP_H
#define KMAP_H

#include "kcommon.h"
#include "theme.h"
#include <float.h>
#include "gl_helpers.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "kmdb.h"

#define IsKana(Codepoint) (((Codepoint) >= 0x3041) && ((Codepoint) <= 0x30fe))

typedef struct
{
    v2 Pos;
    v2 Dim;
    v3 Color;
	v2 Offset;
} character_display_data;

typedef struct
{
    u8 ReadingCount;
    u8 MeaningCount;
    u8 FlagCount;
    char *Writing;
    char *Meanings;
    char *Readings;
    u8 *Flags;
} word;

typedef struct
{
    v2 AtlasTopLeft;
    int X0, X1, Y0, Y1, Advance;
} glyph;

typedef struct
{
    glyph Glyph;
    u32 ColorType;
} text_display_data;

typedef struct
{
    b32 IsOccupied;
    b32 IsLocked;
    i32 Codepoint;
    f32 Scale;
    glyph Glyph;
    u32 Recency;
} ta_cell;

typedef struct
{
	v3 Coordinates;
	v3 Color;
} square;

typedef struct
{
	u32 SquareCount;
	square Squares[4096];
	GLuint VAO;
	GLuint VBO;
} debug_queue;

typedef struct
{
    v2 ScreenBottomLeft;
    v2 ScreenTopRight;
    v2 TextureTopLeft;
    v2 TextureBottomRight;
    f32 Depth;
    v4 Color;
} ui_text_display_data;

#define DepthRange 100000.f

typedef struct
{
    rect Rect;
    v4 Color;
    f32 Depth;
} text_background;

typedef struct
{
    u32 MaxCharacterCount;
    u32 MaxBackgroundCount;
    u32 CharacterCount;
    u32 BackgroundCount;
    text_background *Backgrounds;
    ui_text_display_data *Characters;
} text_render_queue;

typedef struct
{
    v2 Pos;
    v2 Dim;
    f32 Zoom;
    f32 DefaultZoom;

    // for smooth interpolation
    v2 Startpoint;
    v2 Endpoint;
    f32 MovementTime;
    f32 MovementStartTime;
    f32 ZoomOrigin;
    f32 ZoomTarget;
    b32 MovesTowardsEndpoint;
} camera;

typedef struct
{
    u32 WordCount;
    u8 OnReadingCount;
    u8 KunReadingCount;
    u8 MeaningCount;
    u8 RadicalCount;
    u8 SimilarKanjiCount;
    u8 FrequentlyUsedWordCount;
    char *OnReadings;
    char *KunReadings;
    char *Meanings;
    c32 *Radicals;
    c32 *SimilarKanji;
    u32 *WordIndices;
    u32 *FrequentlyUsedWordIndices;
} kanji_data;

typedef struct
{
    u32 KanjiCount;
    kanji_info *KanjiInfos;
    char *Data;
} kanji_hashtable;

typedef struct
{
    v2 Pos;
    v3 Color;
} background_display_data;

typedef struct
{
    u32 WordIndex;
    rect BoundingBox;
} word_reference;

#endif // ifndef KMAP_H
