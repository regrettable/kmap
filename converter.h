#ifndef CONVERTER_H
#define CONVERTER_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/inotify.h>

#include <string.h>
#include <errno.h>
#include <locale.h>

#include <stdio.h>

#include <stdlib.h>

#include "kcommon.h"
#include "kmdb.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

typedef struct word_index_list word_index_list;

struct word_index_list
{
    u32 Index;
    word_index_list *Next;
};

typedef struct
{
    size_t Size;
    char *Data;
} string;
#define ConstString(Cstring) (string){.Size = sizeof(Cstring) - 1, .Data = (Cstring)}

#define EntryMaxStrings 256

typedef struct
{
    size_t ReadingOffset;
    size_t WritingOffset;
    size_t MeaningOffset;
    size_t FlagOffset;
    string Writing;
    string Readings[EntryMaxStrings];
    string Meanings[EntryMaxStrings];
    u8 Flags[EntryMaxStrings];
    u8 ReadingCount;
    u8 MeaningCount;
    u8 FlagCount;
} word_entry;

#define MaxRadicalCount 32
#define MaxSimilarKanji 128
#define MaxFrequentlyUsedWords 8

typedef struct
{
    u8 OnReadingCount;
    u8 KunReadingCount;
    u8 MeaningCount;
    u8 RadicalCount;
    u8 SimilarKanjiCount;
    u8 FrequentlyUsedWordCount;
    string OnReadings[EntryMaxStrings];
    string KunReadings[EntryMaxStrings];
    string Meanings[EntryMaxStrings];
    u32 FrequentlyUsedWordIndices[MaxFrequentlyUsedWords];
    c32 Radicals[MaxRadicalCount];
    c32 SimilarKanji[MaxSimilarKanji];
} kanji_entry;

typedef struct kanji_info2 kanji_info2;

struct kanji_info2
{
    c32 Codepoint;
    kanji_entry Entry;

    u32 WordAppearanceCount;
    word_index_list FirstWordIndex;
    kanji_info2 *Next;
};

#endif // CONVERTER_H
