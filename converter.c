#include "converter.h"

typedef struct
{
    char *Data;
    size_t Pos;
    size_t Size;
} parser;

typedef struct
{
    string JmdictName;
    word_flag KmdbFlag;
} flag_mapping;

#define FlagMapping(Flag, Name) { .JmdictName = { .Size = ArrayCount(Name) - 1, .Data = Name}, .KmdbFlag = Flag }
global flag_mapping dial_Flags[] =
{
    FlagMapping(WF_bra, "bra"),
    FlagMapping(WF_hob, "hob"),
    FlagMapping(WF_ksb, "ksb"),
    FlagMapping(WF_ktb, "ktb"),
    FlagMapping(WF_kyb, "kyb"),
    FlagMapping(WF_kyu, "kyu"),
    FlagMapping(WF_nab, "nab"),
    FlagMapping(WF_osb, "osb"),
    FlagMapping(WF_rkb, "rkb"),
    FlagMapping(WF_thb, "thb"),
    FlagMapping(WF_tsb, "tsb"),
    FlagMapping(WF_tsug, "tsug"),
};

global flag_mapping field_Flags[] =
{
    FlagMapping(WF_agric, "agric"),
    FlagMapping(WF_anat, "anat"),
    FlagMapping(WF_archeol, "archeol"),
    FlagMapping(WF_archit, "archit"),
    FlagMapping(WF_art, "art"),
    FlagMapping(WF_astron, "astron"),
    FlagMapping(WF_audvid, "audvid"),
    FlagMapping(WF_aviat, "aviat"),
    FlagMapping(WF_baseb, "baseb"),
    FlagMapping(WF_biochem, "biochem"),
    FlagMapping(WF_biol, "biol"),
    FlagMapping(WF_bot, "bot"),
    FlagMapping(WF_Buddh, "Buddh"),
    FlagMapping(WF_bus, "bus"),
    FlagMapping(WF_chem, "chem"),
    FlagMapping(WF_Christn, "Christn"),
    FlagMapping(WF_cloth, "cloth"),
    FlagMapping(WF_comp, "comp"),
    FlagMapping(WF_cryst, "cryst"),
    FlagMapping(WF_ecol, "ecol"),
    FlagMapping(WF_econ, "econ"),
    FlagMapping(WF_elec, "elec"),
    FlagMapping(WF_electr, "electr"),
    FlagMapping(WF_embryo, "embryo"),
    FlagMapping(WF_engr, "engr"),
    FlagMapping(WF_ent, "ent"),
    FlagMapping(WF_finc, "finc"),
    FlagMapping(WF_fish, "fish"),
    FlagMapping(WF_food, "food"),
    FlagMapping(WF_gardn, "gardn"),
    FlagMapping(WF_genet, "genet"),
    FlagMapping(WF_geogr, "geogr"),
    FlagMapping(WF_geol, "geol"),
    FlagMapping(WF_geom, "geom"),
    FlagMapping(WF_go, "go"),
    FlagMapping(WF_golf, "golf"),
    FlagMapping(WF_gramm, "gramm"),
    FlagMapping(WF_grmyth, "grmyth"),
    FlagMapping(WF_hanaf, "hanaf"),
    FlagMapping(WF_horse, "horse"),
    FlagMapping(WF_law, "law"),
    FlagMapping(WF_ling, "ling"),
    FlagMapping(WF_logic, "logic"),
    FlagMapping(WF_MA, "MA"),
    FlagMapping(WF_mahj, "mahj"),
    FlagMapping(WF_math, "math"),
    FlagMapping(WF_mech, "mech"),
    FlagMapping(WF_med, "med"),
    FlagMapping(WF_met, "met"),
    FlagMapping(WF_mil, "mil"),
    FlagMapping(WF_music, "music"),
    FlagMapping(WF_ornith, "ornith"),
    FlagMapping(WF_paleo, "paleo"),
    FlagMapping(WF_pathol, "pathol"),
    FlagMapping(WF_pharm, "pharm"),
    FlagMapping(WF_phil, "phil"),
    FlagMapping(WF_photo, "photo"),
    FlagMapping(WF_physics, "physics"),
    FlagMapping(WF_physiol, "physiol"),
    FlagMapping(WF_print, "print"),
    FlagMapping(WF_psy, "psy"),
    FlagMapping(WF_psych, "psych"),
    FlagMapping(WF_rail, "rail"),
    FlagMapping(WF_Shinto, "Shinto"),
    FlagMapping(WF_shogi, "shogi"),
    FlagMapping(WF_sports, "sports"),
    FlagMapping(WF_stat, "stat"),
    FlagMapping(WF_sumo, "sumo"),
    FlagMapping(WF_telec, "telec"),
    FlagMapping(WF_tradem, "tradem"),
    FlagMapping(WF_vidg, "vidg"),
    FlagMapping(WF_zool, "zool"),
};

global flag_mapping ke_inf_Flags[] =
{
    FlagMapping(WF_ateji, "ateji"),
    FlagMapping(WF_ke_ik, "ik"),
    FlagMapping(WF_ke_iK, "iK"),
    FlagMapping(WF_io, "io"),
    FlagMapping(WF_oK, "oK"),
    FlagMapping(WF_rK, "rK"),
};

global flag_mapping misc_Flags[] =
{
    FlagMapping(WF_abbr, "abbr"),
    FlagMapping(WF_arch, "arch"),
    FlagMapping(WF_char, "char"),
    FlagMapping(WF_chn, "chn"),
    FlagMapping(WF_col, "col"),
    FlagMapping(WF_company, "company"),
    FlagMapping(WF_creat, "creat"),
    FlagMapping(WF_dated, "dated"),
    FlagMapping(WF_dei, "dei"),
    FlagMapping(WF_derog, "derog"),
    FlagMapping(WF_doc, "doc"),
    FlagMapping(WF_ev, "ev"),
    FlagMapping(WF_fam, "fam"),
    FlagMapping(WF_fem, "fem"),
    FlagMapping(WF_fict, "fict"),
    FlagMapping(WF_form, "form"),
    FlagMapping(WF_given, "given"),
    FlagMapping(WF_group, "group"),
    FlagMapping(WF_hist, "hist"),
    FlagMapping(WF_hon, "hon"),
    FlagMapping(WF_hum, "hum"),
    FlagMapping(WF_id, "id"),
    FlagMapping(WF_joc, "joc"),
    FlagMapping(WF_leg, "leg"),
    FlagMapping(WF_m_sl, "m-sl"),
    FlagMapping(WF_male, "male"),
    FlagMapping(WF_myth, "myth"),
    FlagMapping(WF_net_sl, "net-sl"),
    FlagMapping(WF_obj, "obj"),
    FlagMapping(WF_obs, "obs"),
    FlagMapping(WF_obsc, "obsc"),
    FlagMapping(WF_on_mim, "on-mim"),
    FlagMapping(WF_organization, "organization"),
    FlagMapping(WF_oth, "oth"),
    FlagMapping(WF_person, "person"),
    FlagMapping(WF_place, "place"),
    FlagMapping(WF_poet, "poet"),
    FlagMapping(WF_pol, "pol"),
    FlagMapping(WF_product, "product"),
    FlagMapping(WF_proverb, "proverb"),
    FlagMapping(WF_quote, "quote"),
    FlagMapping(WF_rare, "rare"),
    FlagMapping(WF_relig, "relig"),
    FlagMapping(WF_sens, "sens"),
    FlagMapping(WF_serv, "serv"),
    FlagMapping(WF_sl, "sl"),
    FlagMapping(WF_station, "station"),
    FlagMapping(WF_surname, "surname"),
    FlagMapping(WF_uk, "uk"),
    FlagMapping(WF_unclass, "unclass"),
    FlagMapping(WF_vulg, "vulg"),
    FlagMapping(WF_work, "work"),
    FlagMapping(WF_X, "X"),
    FlagMapping(WF_yoji, "yoji"),
};

global flag_mapping pos_Flags[] =
{
    FlagMapping(WF_adj_f, "adj-f"),
    FlagMapping(WF_adj_i, "adj-i"),
    FlagMapping(WF_adj_ix, "adj-ix"),
    FlagMapping(WF_adj_kari, "adj-kari"),
    FlagMapping(WF_adj_ku, "adj-ku"),
    FlagMapping(WF_adj_na, "adj-na"),
    FlagMapping(WF_adj_nari, "adj-nari"),
    FlagMapping(WF_adj_no, "adj-no"),
    FlagMapping(WF_adj_pn, "adj-pn"),
    FlagMapping(WF_adj_shiku, "adj-shiku"),
    FlagMapping(WF_adj_t, "adj-t"),
    FlagMapping(WF_adv, "adv"),
    FlagMapping(WF_adv_to, "adv-to"),
    FlagMapping(WF_aux, "aux"),
    FlagMapping(WF_aux_adj, "aux-adj"),
    FlagMapping(WF_aux_v, "aux-v"),
    FlagMapping(WF_conj, "conj"),
    FlagMapping(WF_cop, "cop"),
    FlagMapping(WF_ctr, "ctr"),
    FlagMapping(WF_exp, "exp"),
    FlagMapping(WF_int, "int"),
    FlagMapping(WF_n, "n"),
    FlagMapping(WF_n_adv, "n-adv"),
    FlagMapping(WF_n_pr, "n-pr"),
    FlagMapping(WF_n_pref, "n-pref"),
    FlagMapping(WF_n_suf, "n-suf"),
    FlagMapping(WF_n_t, "n-t"),
    FlagMapping(WF_num, "num"),
    FlagMapping(WF_pn, "pn"),
    FlagMapping(WF_pref, "pref"),
    FlagMapping(WF_prt, "prt"),
    FlagMapping(WF_suf, "suf"),
    FlagMapping(WF_unc, "unc"),
    FlagMapping(WF_v_unspec, "v-unspec"),
    FlagMapping(WF_v1, "v1"),
    FlagMapping(WF_v1_s, "v1-s"),
    FlagMapping(WF_v2a_s, "v2a-s"),
    FlagMapping(WF_v2b_k, "v2b-k"),
    FlagMapping(WF_v2b_s, "v2b-s"),
    FlagMapping(WF_v2d_k, "v2d-k"),
    FlagMapping(WF_v2d_s, "v2d-s"),
    FlagMapping(WF_v2g_k, "v2g-k"),
    FlagMapping(WF_v2g_s, "v2g-s"),
    FlagMapping(WF_v2h_k, "v2h-k"),
    FlagMapping(WF_v2h_s, "v2h-s"),
    FlagMapping(WF_v2k_k, "v2k-k"),
    FlagMapping(WF_v2k_s, "v2k-s"),
    FlagMapping(WF_v2m_k, "v2m-k"),
    FlagMapping(WF_v2m_s, "v2m-s"),
    FlagMapping(WF_v2n_s, "v2n-s"),
    FlagMapping(WF_v2r_k, "v2r-k"),
    FlagMapping(WF_v2r_s, "v2r-s"),
    FlagMapping(WF_v2s_s, "v2s-s"),
    FlagMapping(WF_v2t_k, "v2t-k"),
    FlagMapping(WF_v2t_s, "v2t-s"),
    FlagMapping(WF_v2w_s, "v2w-s"),
    FlagMapping(WF_v2y_k, "v2y-k"),
    FlagMapping(WF_v2y_s, "v2y-s"),
    FlagMapping(WF_v2z_s, "v2z-s"),
    FlagMapping(WF_v4b, "v4b"),
    FlagMapping(WF_v4g, "v4g"),
    FlagMapping(WF_v4h, "v4h"),
    FlagMapping(WF_v4k, "v4k"),
    FlagMapping(WF_v4m, "v4m"),
    FlagMapping(WF_v4n, "v4n"),
    FlagMapping(WF_v4r, "v4r"),
    FlagMapping(WF_v4s, "v4s"),
    FlagMapping(WF_v4t, "v4t"),
    FlagMapping(WF_v5aru, "v5aru"),
    FlagMapping(WF_v5b, "v5b"),
    FlagMapping(WF_v5g, "v5g"),
    FlagMapping(WF_v5k, "v5k"),
    FlagMapping(WF_v5k_s, "v5k-s"),
    FlagMapping(WF_v5m, "v5m"),
    FlagMapping(WF_v5n, "v5n"),
    FlagMapping(WF_v5r, "v5r"),
    FlagMapping(WF_v5r_i, "v5r-i"),
    FlagMapping(WF_v5s, "v5s"),
    FlagMapping(WF_v5t, "v5t"),
    FlagMapping(WF_v5u, "v5u"),
    FlagMapping(WF_v5u_s, "v5u-s"),
    FlagMapping(WF_v5uru, "v5uru"),
    FlagMapping(WF_vi, "vi"),
    FlagMapping(WF_vk, "vk"),
    FlagMapping(WF_vn, "vn"),
    FlagMapping(WF_vr, "vr"),
    FlagMapping(WF_vs, "vs"),
    FlagMapping(WF_vs_c, "vs-c"),
    FlagMapping(WF_vs_i, "vs-i"),
    FlagMapping(WF_vs_s, "vs-s"),
    FlagMapping(WF_vt, "vt"),
    FlagMapping(WF_vz, "vz"),
};

global flag_mapping re_inf_Flags[] =
{
    FlagMapping(WF_gikun, "gikun"),
    FlagMapping(WF_ik, "ik"),
    FlagMapping(WF_ok, "ok"),
    FlagMapping(WF_uK, "uK"),
};

internal word_flag
GetFlagValue(size_t FlagMappingCount, flag_mapping *FlagMappings, string FlagString)
{
    word_flag Result = WF_None;
    for(u32 FlagIndex = 0; FlagIndex < FlagMappingCount; ++FlagIndex)
    {
        flag_mapping *Mapping = FlagMappings + FlagIndex;
        string MappingName = Mapping->JmdictName;
        if(MappingName.Size == (FlagString.Size - 2))
        {
            b32 StringsEqual = true;
            for(u32 ByteIndex = 0; ByteIndex < MappingName.Size; ++ByteIndex)
            {
                if(MappingName.Data[ByteIndex] != FlagString.Data[ByteIndex + 1])
                {
                    StringsEqual = false;
                    break;
                }
            }
            if(StringsEqual)
            {
                Result = Mapping->KmdbFlag;
                break;
            }
        }
    }
    Assert(Result != WF_None);
    return Result;
}

internal load_file_result
MemoryMap(char *Path)
{
    load_file_result Result = {0};
#if OS_LINUX
    int Handle = open(Path, O_RDONLY);
    if(Handle == -1)
    {
		eprintf("Path: %s ", Path);
        ErrorOut(open());
    }
    struct stat StatBuffer;
    if(fstat(Handle, &StatBuffer) == -1)
    {
        ErrorOut(fstat());
    }
    Result.Size = StatBuffer.st_size;
    Result.Data = mmap(0, Result.Size, PROT_READ, MAP_PRIVATE, Handle, 0);
    if(!Result.Data)
    {
        ErrorOut(mmap());
    }
    close(Handle);
#elif OS_WINDOWS
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
		WinError("CreateFile()");
		Exit(1);
	}

	HANDLE FileMapping = 
		CreateFileMapping(FileHandle,
						  0,
						  PAGE_READONLY,
						  0,0,
						  Path);
	if(FileMapping == 0)
	{
		WinError("CreateFileMapping()");
		Exit(1);
	}
	Result.Data = MapViewOfFile(FileMapping,
								FILE_MAP_READ,
								0,
								0,
								0);
	if(Result.Data == 0)
	{
		DWORD ErrorCode = GetLastError();
		WinError("MapViewOfFile()");
		Exit(1);
	}

	MEMORY_BASIC_INFORMATION Unused;
	Result.Size = VirtualQuery(Result.Data, &Unused, sizeof(Unused));
	if(Result.Size == 0)
	{
		WinError("VirtualQuery()");
		Exit(1);
	}

	CloseHandle(FileMapping);
	CloseHandle(FileHandle);
#else
	FILE *File = fopen(Path, "r");
	fseek(File, 0, SEEK_END);
	Result.Size = ftell(File);
	fseek(File, 0, SEEK_SET);
	Result.Data = malloc(Result.Size);
	if(fread(Result.Data, 1, Result.Size, File) != Result.Size)
	{
		ErrorOut(fread());
	}
#endif
    return Result;
}

internal void
MemoryUnmap(load_file_result MmapResult)
{
#if OS_LINUX
    munmap(MmapResult.Data, MmapResult.Size);
#elif OS_WINDOWS
	UnmapViewOfFile(MmapResult.Data);
#else
	free(MmapResult.Data);
#endif
}

internal c32
GetChar(parser *Parser)
{
    c32 Result = 0;
    if(Parser->Pos < Parser->Size)
    {
        Parser->Pos += ConvertSingleUtf8(Parser->Data + Parser->Pos, &Result);
    }
    return Result;
}

internal c32
Peek(parser *Parser, size_t PosOffset)
{
    c32 Result = 0;
    if(Parser->Pos + PosOffset < Parser->Size)
    {
        ConvertSingleUtf8(Parser->Data + Parser->Pos + PosOffset, &Result);
    }
    return Result;
}

internal void
SkipTillCharacter(parser *Parser, c32 C)
{
    while(Parser->Pos < Parser->Size)
    {
        c32 Result;
        size_t ResultSize = ConvertSingleUtf8(Parser->Data + Parser->Pos, &Result);
        if(Result == C)
        {
            break;
        }
        Parser->Pos += ResultSize;
    }
    //c32 Result;
    //size_t NewPos = Parser->Pos + ConvertSingleUtf8(Parser->Data + Parser->Pos, &Result);
    //if(Result != C)
    //{
    //    size_t ConvertedSize = 0;
    //    while(Result != C)
    //    {
    //        ConvertedSize = ConvertSingleUtf8(Parser->Data + NewPos, &Result);
    //        NewPos += ConvertedSize;
    //    }
    //    Parser->Pos = NewPos - ConvertedSize;
    //}
}

#define SkipTillAfterCharacter(Parser, C) \
{ \
    SkipTillCharacter((Parser), (C)); \
    Assert(GetChar(Parser) == (C)); \
}


internal b32
StartsWith_(parser *Parser, size_t StringSize, char *String)
{
    b32 Result = true;
    for(u32 StringPos = 0; StringPos < StringSize; ++StringPos)
    {
        if(!(Peek(Parser, StringPos) == String[StringPos]))
        {
            Result = false;
            break;
        }
    }
    return Result;
}

#define StartsWith(Parser, CString) StartsWith_(Parser, ArrayCount(CString) - 1, CString)

internal string
ParseLeafTag(parser *Parser)
{
    SkipTillAfterCharacter(Parser, '>');
    string Result = 
    {
        .Data = Parser->Data + Parser->Pos
    };
    size_t StartPos = Parser->Pos;
    SkipTillCharacter(Parser, '<');
    Result.Size = Parser->Pos - StartPos;
    return Result;
}

internal codepoints
ConvertStringToCodepoints(string String)
{
    codepoints Result = {0};
    for(u32 TextPos = 0;
        TextPos < String.Size;
        )
    {
        u8 *Encoded = (u8 *)String.Data + TextPos;
        TextPos += ConvertSingleUtf8((char *)Encoded,
                                     Result.Data + Result.Count);
        Result.Count++;
    }
    Assert(Result.Count < ArrayCount(Result.Data));
    return Result;
}

typedef struct
{
    char *Data;
    u64 Position;
    u64 Allocated;
    u64 Used;
} mapped_file;

internal void
WriteToFile(size_t DataSize, void *Data, mapped_file *File)
{
    Assert(File->Position + DataSize <= File->Allocated);
    Memcpy(File->Data + File->Position, Data, DataSize);
    File->Position += DataSize;
    File->Used = Maximum(File->Position, File->Used);
}

internal void
ChangeWritingPosition(mapped_file *File, size_t NewPosition)
{
    Assert(NewPosition <= File->Allocated);
    File->Position = NewPosition;
    File->Used = Maximum(File->Position, File->Used);
}

internal void
WriteStringToFile(FILE *File, string String)
{
    if(fwrite(String.Data, 1, String.Size, File) != String.Size)
    {
        ErrorOut(fwrite());
    }
}

internal kanji_entry *
GetKanjiEntry(kanji_info2 *KanjiHashtable, c32 Codepoint)
{
    u16 Hash = GetKanjiHash(Codepoint);
    for(kanji_info2 *Storage = KanjiHashtable + Hash;
        Storage;
        Storage = Storage->Next)
    {
        if(Storage->Codepoint == Codepoint)
        {
            return &Storage->Entry;
        }
    }
    return 0;
}

// NOTE: current file format in order:
// 1. header
// 2. kanji hashtable (contains the amount of words that have a given kanji, as well as offset into the array of their indices)
// 3. word description table (contains mostly the offsets into the raw data array, and the word's position)
// 4. the kanji data (indices one after the other, on and kun meanings and readings)
// 5. the word data (contains the writing, meanings and pronounciations of words, all utf8-encoded and null-terminated)
//
// all strings are null-terminated and encoded as utf-8
int
main(int Argc, char **Argv)
{
    char Zero = 0;

    char *JmdictPath = 0,
         *KanjidicPath = 0,
         *KradfilePath = 0,
         *OutputFilename = 0,
         *FreqlistPath = 0,
         *ShadersDirectoryPath = 0,
         *SimilarKanjiPath = 0;
    b32 ShowHelpAndQuit = false;

    for(int ArgIndex = 0;
        ArgIndex < Argc;
        ++ArgIndex)
    {
        char *Arg = Argv[ArgIndex];
        if(StringsEqual(Arg, "-j"))
        {
            JmdictPath = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-k"))
        {
            KanjidicPath = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-o"))
        {
            OutputFilename = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-s"))
        {
            ShadersDirectoryPath = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-r"))
        {
            KradfilePath = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-f"))
        {
            FreqlistPath = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-y"))
        {
            SimilarKanjiPath = Argv[++ArgIndex];
        }
        else if(StringsEqual(Arg, "-h"))
        {
            ShowHelpAndQuit = true;
            break;
        }
    }

    if(ShowHelpAndQuit || !(JmdictPath && KanjidicPath && OutputFilename && KradfilePath && FreqlistPath))
    {
        Die("USAGE: %s -j INPUT_JMDICT -k INPUT_KANJIDIC -r INPUT_KRADFILE -o OUTPUT_DATABASE -f FREQLIST_PATH [ -s SHADERS_DIRECTORY ] [ -y SIMILAR_KANJI_DATABASE ]\n", Argv[0]);
    }

    // allocate enough memory so that we never need to worry about reallocation
    u64 OutputMemorySize = Megabytes(128);
    mapped_file OutputMemory =
    {
        .Data = calloc(1, OutputMemorySize),
        .Position = 0,
        .Allocated = OutputMemorySize,
        .Used = 0
    };

    load_file_result Jmdict = MemoryMap(JmdictPath);
    load_file_result Kanjidic = MemoryMap(KanjidicPath);
    load_file_result Kradfile = MemoryMap(KradfilePath);
    load_file_result Freqlist = MemoryMap(FreqlistPath);

    printf("Jmdict size: %ld\n", Jmdict.Size);
    printf("Kanjidic size: %ld\n", Kanjidic.Size);
    printf("Kradfile size: %ld\n", Kradfile.Size);

    parser Parser_ =
    {
        .Data = Jmdict.Data,
        .Size = Jmdict.Size
    };
    parser *Parser = &Parser_;

    // NOTE: we treat each writing as a separate word
    u32 WordEntryCount = 0;
    while(Parser->Pos < Parser->Size)
    {
        SkipTillCharacter(Parser, '<');
        if(GetChar(Parser) == 0)
        {
            break;
        }
        if(StartsWith(Parser, "entry"))
        {
            u32 WritingCount = 0;

            while(true)
            {
                SkipTillAfterCharacter(Parser, '<');
                if(StartsWith(Parser, "/entry"))
                {
                    break;
                }
                else if(StartsWith(Parser, "keb"))
                {
                    ++WritingCount;
                }
            }

            WordEntryCount += WritingCount;
        }
    }
    printf("Entry count (with each writing counting as entry): %d\n", WordEntryCount);

    word_entry *Entries = malloc(sizeof(word_entry)*WordEntryCount);
    if(!Entries)
    {
        ErrorOut(malloc());
    }

    Parser->Pos = 0;
    for(u32 EntryIndex = 0;
        Parser->Pos < Parser->Size;)
    {
        SkipTillCharacter(Parser, '<');
        if(GetChar(Parser) == 0)
        {
            break;
        }
        if(StartsWith(Parser, "entry"))
        {
            u32 WritingCount = 0;
            string Writings[EntryMaxStrings] = {0};
            word_entry Entry = {0};

            while(true)
            {
                SkipTillAfterCharacter(Parser, '<');
                if(StartsWith(Parser, "/entry"))
                {
                    break;
                }
                else
                {
                    if(StartsWith(Parser, "reb"))
                    {
                        Entry.Readings[Entry.ReadingCount++] = ParseLeafTag(Parser);
                        Assert(Entry.ReadingCount < ArrayCount(Entry.Readings));
                    }
                    else if(StartsWith(Parser, "keb"))
                    {
                        Writings[WritingCount++] = ParseLeafTag(Parser);
                        Assert(WritingCount < ArrayCount(Writings));
                    }
                    else if(StartsWith(Parser, "gloss"))
                    {
                        Entry.Meanings[Entry.MeaningCount++] = ParseLeafTag(Parser);
                        Assert(Entry.MeaningCount < ArrayCount(Entry.Meanings));
                    }
                    // NOTE: flags (for whatever reason) can repeat, and we don't want that
#define MapFlag(Entity) \
                    else if(StartsWith(Parser, #Entity)) \
                    {\
                        string FlagString = ParseLeafTag(Parser);\
                        u8 Flag = GetFlagValue(ArrayCount(Entity##_Flags), Entity##_Flags, FlagString); \
                        b32 FoundFlag = false; \
                        for(u32 FlagIndex = 0; FlagIndex < Entry.FlagCount; ++FlagIndex) \
                        { \
                            if(Entry.Flags[FlagIndex] == Flag) \
                            { \
                                FoundFlag = true; \
                            } \
                        } \
                        if(!FoundFlag) \
                        { \
                            Entry.Flags[Entry.FlagCount++] = Flag; \
                        } \
                    }
                    MapFlag(dial)
                    MapFlag(field)
                    MapFlag(ke_inf)
                    MapFlag(misc)
                    MapFlag(pos)
                    MapFlag(re_inf)
                }
            }

            for(u32 WritingIndex = 0; WritingIndex < WritingCount; ++WritingIndex)
            {
                Entry.Writing = Writings[WritingIndex];
                Entries[EntryIndex++] = Entry;
            }

            if(EntryIndex == WordEntryCount)
            {
                break;
            }
        }
    }
    printf("parsed words\n");

    // fill out the kanji hashtable for the existing words
    size_t HashtableCellCount = PermanentKanjiStorageSize;
    kanji_info2 *KanjiHashtable2 = calloc(HashtableCellCount, sizeof(kanji_info2));

    u32 WordIndexCount = 0;
    u32 KanjiCount = 0;
    for(u32 EntryIndex = 0; EntryIndex < WordEntryCount; ++EntryIndex)
    {
        codepoints Codepoints = ConvertStringToCodepoints(Entries[EntryIndex].Writing);
        for(u32 CharacterIndex = 0; CharacterIndex < Codepoints.Count; ++CharacterIndex)
        {
            c32 Codepoint = Codepoints.Data[CharacterIndex];
            if(IsKanji(Codepoint))
            {
                u16 Hash = GetKanjiHash(Codepoint);
                kanji_info2 *Storage = KanjiHashtable2 + Hash;
                for(;
                    Storage;
                    Storage = Storage->Next)
                {
                    if(Storage->Codepoint == Codepoint)
                    {
                        // found ourselves
                        break;
                    }
                    else if(!Storage->Next)
                    {
                        // this is the first time we see this character,
                        // insert it
                        Storage->Next = calloc(1, sizeof(kanji_info2));
                        Storage = Storage->Next;
                        Storage->Codepoint = Codepoint;
                        Storage->FirstWordIndex = (word_index_list){0};
                        Storage->Next = 0;
                        Storage->WordAppearanceCount = 0;
                        ++KanjiCount;
                        break;
                    }
                }

                word_index_list *LastWordIndex = &Storage->FirstWordIndex;
                for(;
                    LastWordIndex->Next;
                    LastWordIndex = LastWordIndex->Next);

                LastWordIndex->Next = calloc(1, sizeof(word_index_list));
                LastWordIndex = LastWordIndex->Next;
                LastWordIndex->Index = EntryIndex;
                LastWordIndex->Next = 0;

                Storage->WordAppearanceCount++;
                ++WordIndexCount;
            }
        }
    }
    printf("created kanji hashtable\n");
    //MemoryUnmap(Jmdict);

    // parse the kanjidic and update the hashtable
    Parser->Pos = 0;
    Parser->Size = Kanjidic.Size;
    Parser->Data = Kanjidic.Data;

    for(u32 EntryIndex = 0;
        Parser->Pos < Parser->Size;)
    {
        SkipTillCharacter(Parser, '<');
        if(GetChar(Parser) == 0)
        {
            break;
        }
        if(StartsWith(Parser, "character"))
        {
            c32 Codepoint = 0;
            kanji_entry Entry = {0};

            while(true)
            {
                SkipTillAfterCharacter(Parser, '<');
                if(StartsWith(Parser, "/character"))
                {
                    break;
                }
                else
                {
                    if(StartsWith(Parser, "literal"))
                    {
                        Assert(Codepoint == 0);
                        codepoints Literal = ConvertStringToCodepoints(ParseLeafTag(Parser));
                        Assert(Literal.Count == 1);
                        Codepoint = Literal.Data[0];
                    }
                    else if(StartsWith(Parser, "reading r_type=\"ja_on\""))
                    {
                        Entry.OnReadings[Entry.OnReadingCount++] = ParseLeafTag(Parser);
                        Assert(Entry.OnReadingCount < ArrayCount(Entry.OnReadings));
                    }
                    else if(StartsWith(Parser, "reading r_type=\"ja_kun\""))
                    {
                        Entry.KunReadings[Entry.KunReadingCount++] = ParseLeafTag(Parser);
                        Assert(Entry.KunReadingCount < ArrayCount(Entry.KunReadings));
                    }
                    else if(StartsWith(Parser, "meaning>"))
                    {
                        Entry.Meanings[Entry.MeaningCount++] = ParseLeafTag(Parser);
                        Assert(Entry.MeaningCount < ArrayCount(Entry.Meanings));
                    }
                }
            }
            // update the hashtable entry
            u16 Hash = GetKanjiHash(Codepoint);
            kanji_info2 *Storage = KanjiHashtable2 + Hash;
            for(;
                Storage;
                Storage = Storage->Next)
            {
                if(Storage->Codepoint == Codepoint)
                {
                    Storage->Entry = Entry;
                    break;
                }
            }
        }
    }
    printf("parsed kanjidic and updated kanji hashtable\n");
    //MemoryUnmap(Kanjidic);

    Parser->Pos = 0;
    Parser->Size = Kradfile.Size;
    Parser->Data = Kradfile.Data;
    while(Parser->Pos < Parser->Size)
    {
        c32 Kanji;
        for(;;)
        {
            Kanji = GetChar(Parser);
            if(Kanji == '#')
            {
                SkipTillAfterCharacter(Parser, '\n');
            }
            else
            {
                break;
            }
        }
        c32 Space = GetChar(Parser);
        Assert(Space == ' ');
        c32 Column = GetChar(Parser);
        Assert(Column == ':');
        Space = GetChar(Parser);
        Assert(Space == ' ');

        kanji_entry *Entry = GetKanjiEntry(KanjiHashtable2, Kanji);

        for(;;)
        {
            c32 Radical = GetChar(Parser);
            // NOTE: we still want to parse out kanji even if we
            // didn't find it in our database
            if(Entry)
            {
                Entry->Radicals[Entry->RadicalCount++] = Radical;
                Assert(Entry->RadicalCount < MaxRadicalCount);
            }
            c32 NextCharacter = GetChar(Parser);
            if(NextCharacter != ' ')
            {
                Assert(NextCharacter == '\n');
                break;
            }
        }
    }
    printf("parsed radicals and updated kanji table\n");
    //MemoryUnmap(Kradfile);

    //
    Parser->Pos = 0;
    Parser->Size = Freqlist.Size;
    Parser->Data = Freqlist.Data;

    SkipTillAfterCharacter(Parser, '\n');
    SkipTillAfterCharacter(Parser, '\n');
    SkipTillAfterCharacter(Parser, '\n');
    SkipTillAfterCharacter(Parser, '\n');

    while(Parser->Pos < Parser->Size)
    {

        //u32 WordIndex = 0;
        //while((Digit = GetChar(Parser)) != ' ')
        //{
        //    WordIndex *= 10;
        //    WordIndex += Digit - '0';
        //}
        //WordIndex--;
        SkipTillAfterCharacter(Parser, ' ');

        c32 Digit = 0;
        u32 Whole = 0;
        while((Digit = GetChar(Parser)) != '.')
        {
            Whole *= 10;
            Whole += Digit - '0';
        }
        u32 Decimal = 0;
        while((Digit = GetChar(Parser)) != ' ')
        {
            Decimal *= 10;
            Decimal += Digit - '0';
        }

        f32 Popularity = Whole + (.01f * Decimal);
        string Word = 
        {
            .Data = Parser->Data + Parser->Pos,
            .Size = Parser->Pos
        };
        SkipTillAfterCharacter(Parser, '\n');
        Word.Size = Parser->Pos - Word.Size - 1;

        codepoints Codepoints = ConvertStringToCodepoints(Word);

        // since frequency list is ordered, we don't check anything
        for(u32 CodepointIndex = 0; CodepointIndex < Codepoints.Count; ++CodepointIndex)
        {
            c32 Codepoint = Codepoints.Data[CodepointIndex];
            if(IsKanji(Codepoint))
            {
                u16 Hash = GetKanjiHash(Codepoint);
                kanji_info2 *Storage = KanjiHashtable2 + Hash;
                for(;
                    Storage;
                    Storage = Storage->Next)
                {
                    if(Storage->Codepoint == Codepoint)
                    {
                        if(Storage->Entry.FrequentlyUsedWordCount < MaxFrequentlyUsedWords)
                        {
                            for(word_index_list *List = &Storage->FirstWordIndex;
                                List;
                                List = List->Next)
                            {
                                if(Entries[List->Index].Writing.Size == Word.Size)
                                {
                                    b32 WordsMatch = true;
                                    char *Writing = Entries[List->Index].Writing.Data;
                                    for(u32 CharIndex = 0; CharIndex < Word.Size; ++CharIndex)
                                    {
                                        if(Word.Data[CharIndex] != Writing[CharIndex])
                                        {
                                            WordsMatch = false;
                                        }
                                    }
                                    if(WordsMatch)
                                    {
                                        Storage->Entry.FrequentlyUsedWordIndices[Storage->Entry.FrequentlyUsedWordCount++] = List->Index;
                                        break;
                                    }
                                }
                                //
                            }
                        }
                        //
                        break;
                    }
                }
            }
            else if(Codepoint == '\n')
            {
                break;
            }
        }
    }
    printf("parsed frequency list and updated kanji table\n");
    //MemoryUnmap(Freqlist);
    //

    if(SimilarKanjiPath)
    {
        load_file_result SimilarKanji = MemoryMap(SimilarKanjiPath);
        Parser->Pos = 0;
        Parser->Size = SimilarKanji.Size;
        Parser->Data = SimilarKanji.Data;
        while(Parser->Pos < Parser->Size)
        {
            c32 Kanji = GetChar(Parser);
            Assert(IsKanji(Kanji));
            c32 Space = GetChar(Parser);
            Assert(Space == ' ');
            kanji_entry *KanjiEntry = GetKanjiEntry(KanjiHashtable2, Kanji);
            if(KanjiEntry)
            {
                while(true)
                {
                    c32 SimilarKanji = GetChar(Parser);
                    Assert(KanjiEntry->SimilarKanjiCount + 1 < MaxSimilarKanji);
                    KanjiEntry->SimilarKanji[KanjiEntry->SimilarKanjiCount++] = SimilarKanji;
                    Space = GetChar(Parser);
                    Assert(Space == ' ');
                    c32 Skip;
                    do
                    {
                        Skip = GetChar(Parser);
                    } while(Skip == '.' || (Skip >= '0' && Skip <= '9'));
                    if(Skip == '\n')
                    {
                        break;
                    }
                    Assert(Skip == ' ');
                }
            }
        }
    }
    else
    {
        for(u32 HashtableCellIndex = 0; HashtableCellIndex < HashtableCellCount; ++HashtableCellIndex)
        {
            for(kanji_info2 *KanjiInfo = KanjiHashtable2[HashtableCellIndex].Next;
                KanjiInfo;
                KanjiInfo = KanjiInfo->Next)
            {
                for(kanji_info2 *OtherKanjiInfo = KanjiInfo->Next;
                    OtherKanjiInfo;
                    OtherKanjiInfo = OtherKanjiInfo->Next)
                {
                    u32 SameRadicalCount = 0;
                    for(u32 FirstKanjiRadicalIndex = 0; FirstKanjiRadicalIndex < KanjiInfo->Entry.RadicalCount; ++FirstKanjiRadicalIndex)
                    {
                        for(u32 SecondKanjiRadicalIndex = 0; SecondKanjiRadicalIndex < OtherKanjiInfo->Entry.RadicalCount; ++SecondKanjiRadicalIndex)
                        {
                            if(KanjiInfo->Entry.Radicals[FirstKanjiRadicalIndex] == OtherKanjiInfo->Entry.Radicals[SecondKanjiRadicalIndex])
                            {
                                SameRadicalCount++;
                            }
                        }
                    }
                    if((((f32)(SameRadicalCount) / (f32)(KanjiInfo->Entry.RadicalCount)) >= .75f) && (KanjiInfo->Entry.SimilarKanjiCount < ArrayCount(KanjiInfo->Entry.SimilarKanji)))
                    {
                        KanjiInfo->Entry.SimilarKanji[KanjiInfo->Entry.SimilarKanjiCount++] = OtherKanjiInfo->Codepoint;
                    }
                    if((((f32)(SameRadicalCount) / (f32)(OtherKanjiInfo->Entry.RadicalCount)) >= .75f) && (OtherKanjiInfo->Entry.SimilarKanjiCount < ArrayCount(OtherKanjiInfo->Entry.SimilarKanji)))
                    {
                        OtherKanjiInfo->Entry.SimilarKanji[OtherKanjiInfo->Entry.SimilarKanjiCount++] = KanjiInfo->Codepoint;
                    }
                }

                for(u32 HashtableCellIndex1 = HashtableCellIndex + 1;
                    HashtableCellIndex1 < HashtableCellCount;
                    ++HashtableCellIndex1)
                {
                    for(kanji_info2 *OtherKanjiInfo = KanjiHashtable2[HashtableCellIndex1].Next;
                        OtherKanjiInfo;
                        OtherKanjiInfo = OtherKanjiInfo->Next)
                    {
                        u32 SameRadicalCount = 0;
                        for(u32 FirstKanjiRadicalIndex = 0; FirstKanjiRadicalIndex < KanjiInfo->Entry.RadicalCount; ++FirstKanjiRadicalIndex)
                        {
                            for(u32 SecondKanjiRadicalIndex = 0; SecondKanjiRadicalIndex < OtherKanjiInfo->Entry.RadicalCount; ++SecondKanjiRadicalIndex)
                            {
                                if(KanjiInfo->Entry.Radicals[FirstKanjiRadicalIndex] == OtherKanjiInfo->Entry.Radicals[SecondKanjiRadicalIndex])
                                {
                                    SameRadicalCount++;
                                }
                            }
                        }
                        if((((f32)(SameRadicalCount) / (f32)(KanjiInfo->Entry.RadicalCount)) >= .75f) && (KanjiInfo->Entry.SimilarKanjiCount < ArrayCount(KanjiInfo->Entry.SimilarKanji)))
                        {
                            KanjiInfo->Entry.SimilarKanji[KanjiInfo->Entry.SimilarKanjiCount++] = OtherKanjiInfo->Codepoint;
                        }
                        if((((f32)(SameRadicalCount) / (f32)(OtherKanjiInfo->Entry.RadicalCount)) >= .75f) && (OtherKanjiInfo->Entry.SimilarKanjiCount < ArrayCount(OtherKanjiInfo->Entry.SimilarKanji)))
                        {
                            OtherKanjiInfo->Entry.SimilarKanji[OtherKanjiInfo->Entry.SimilarKanjiCount++] = KanjiInfo->Codepoint;
                        }
                    }
                }
            }
        }
    }

    printf("recorded similar kanji\n");

    kmdb_header Header =
    {
        .Magic = KMDB_MAGIC,
        .Version = KMDB_CURRENT_VERSION,
        .WordCount = WordEntryCount,
        .KanjiCount = KanjiCount,
        .KanjiHashtableOffset = sizeof(kmdb_header),
    };

    Header.WordDescriptionsOffset = Header.KanjiHashtableOffset + sizeof(kanji_info)*KanjiCount;
    Header.KanjiDataOffset = Header.WordDescriptionsOffset + sizeof(kmdb_word_description)*WordEntryCount;

    ChangeWritingPosition(&OutputMemory, Header.KanjiDataOffset);
    //fseek(OutputFile, Header.KanjiDataOffset, SEEK_SET);

    // NOTE: first, we write out the kanji data and set the offsets at the same time,
    // then the word data, then the kanji table, then the word table.

    size_t Hashtable1Size = sizeof(kanji_info)*KanjiCount;
    kanji_info *Hashtable1 = calloc(Hashtable1Size, 1);
    u32 KanjiIndex = 0;

    size_t KanjiDataOffset = 0;
    for(u32 HashtableCellIndex = 0; HashtableCellIndex < HashtableCellCount; ++HashtableCellIndex)
    {
        for(kanji_info2 *KanjiInfo2 = KanjiHashtable2[HashtableCellIndex].Next;
            KanjiInfo2;
            KanjiInfo2 = KanjiInfo2->Next)
        {
            kanji_entry Entry = KanjiInfo2->Entry;
            kanji_info KanjiInfo1 = 
            {
                .Codepoint = KanjiInfo2->Codepoint,
                .WordIndexCount = KanjiInfo2->WordAppearanceCount,
                .OnReadingCount = Entry.OnReadingCount,
                .KunReadingCount = Entry.KunReadingCount,
                .MeaningCount = Entry.MeaningCount,
                .RadicalCount = Entry.RadicalCount,
                .SimilarKanjiCount = Entry.SimilarKanjiCount,
                .FrequentlyUsedWordCount = Entry.FrequentlyUsedWordCount
            };

            // word indices
            KanjiInfo1.WordIndexOffset = KanjiDataOffset;

            for(word_index_list *IndexList = KanjiInfo2->FirstWordIndex.Next;
                IndexList;
                IndexList = IndexList->Next)
            {
                WriteToFile(sizeof(IndexList->Index), &IndexList->Index, &OutputMemory);
            }
            KanjiDataOffset += sizeof(((word_index_list *)0)->Index)*KanjiInfo1.WordIndexCount;

            // on readings
            KanjiInfo1.OnReadingOffset = KanjiDataOffset;
            for(u32 OnReadingIndex = 0; OnReadingIndex < Entry.OnReadingCount; ++OnReadingIndex)
            {
                string Reading = Entry.OnReadings[OnReadingIndex];
                WriteToFile(Reading.Size, Reading.Data, &OutputMemory);
                WriteToFile(1, &Zero, &OutputMemory);
                KanjiDataOffset += Reading.Size + 1;
            }

            // kun readings
            KanjiInfo1.KunReadingOffset = KanjiDataOffset;
            for(u32 KunReadingIndex = 0; KunReadingIndex < Entry.KunReadingCount; ++KunReadingIndex)
            {
                string Reading = Entry.KunReadings[KunReadingIndex];
                WriteToFile(Reading.Size, Reading.Data, &OutputMemory);
                WriteToFile(1, &Zero, &OutputMemory);
                KanjiDataOffset += Reading.Size + 1;
            }

            // meanings
            KanjiInfo1.MeaningOffset = KanjiDataOffset;
            for(u32 MeaningIndex = 0; MeaningIndex < Entry.MeaningCount; ++MeaningIndex)
            {
                string Meaning = Entry.Meanings[MeaningIndex];
                WriteToFile(Meaning.Size, Meaning.Data, &OutputMemory);
                WriteToFile(1, &Zero, &OutputMemory);
                KanjiDataOffset += Meaning.Size + 1;
            }

            // radicals
            KanjiInfo1.RadicalOffset = KanjiDataOffset;
            size_t RadicalsSize = sizeof(Entry.Radicals[0])*Entry.RadicalCount;
            WriteToFile(RadicalsSize, Entry.Radicals, &OutputMemory);
            KanjiDataOffset += RadicalsSize;

            // similar kanji
            KanjiInfo1.SimilarKanjiOffset = KanjiDataOffset;
            size_t SimilarKanjiSize = sizeof(Entry.SimilarKanji[0])*Entry.SimilarKanjiCount;
            WriteToFile(SimilarKanjiSize, Entry.SimilarKanji, &OutputMemory);
            KanjiDataOffset += SimilarKanjiSize;

            // freqently used words
            KanjiInfo1.FrequentlyUsedWordIndexOffset = KanjiDataOffset;
            size_t FrequentlyUsedWordIndicesSize = sizeof(Entry.FrequentlyUsedWordIndices[0])*Entry.FrequentlyUsedWordCount;
            WriteToFile(FrequentlyUsedWordIndicesSize, Entry.FrequentlyUsedWordIndices, &OutputMemory);
            KanjiDataOffset += FrequentlyUsedWordIndicesSize;

            //
            Hashtable1[KanjiIndex++] = KanjiInfo1;
        }
    }
    printf("wrote out kanji\n");

    // write the data before the descriptions
    Header.WordDataOffset = OutputMemory.Position;

    size_t WordDataOffset = 0;

    for(u32 EntryIndex = 0; EntryIndex < WordEntryCount; ++EntryIndex)
    {
        // writing
        word_entry *Entry = Entries + EntryIndex;
        Entry->WritingOffset = WordDataOffset;
        string *Writing = &Entry->Writing;
        WriteToFile(Writing->Size, Writing->Data, &OutputMemory);
        WriteToFile(1, &Zero, &OutputMemory);
        WordDataOffset += Writing->Size + 1;

        // readings
        Entry->ReadingOffset = WordDataOffset;
        for(u32 ReadingIndex = 0; ReadingIndex < Entry->ReadingCount; ++ReadingIndex)
        {
            string *Reading = Entry->Readings + ReadingIndex;
            WriteToFile(Reading->Size, Reading->Data, &OutputMemory);
            WriteToFile(1, &Zero, &OutputMemory);
            WordDataOffset += Reading->Size + 1;
        }

        // meanings
        Entry->MeaningOffset = WordDataOffset;
        for(u32 MeaningIndex = 0; MeaningIndex < Entry->MeaningCount; ++MeaningIndex)
        {
            string *Meaning = Entry->Meanings + MeaningIndex;
            WriteToFile(Meaning->Size, Meaning->Data, &OutputMemory);
            WriteToFile(1, &Zero, &OutputMemory);
            WordDataOffset += Meaning->Size + 1;
        }

        // flags
        Entry->FlagOffset = WordDataOffset;
        WriteToFile(Entry->FlagCount, Entry->Flags, &OutputMemory);
        WordDataOffset += Entry->FlagCount;
    }
    printf("wrote out words\n");

    // write the shaders
    Header.ShaderDataOffset = OutputMemory.Position;
    if(ShadersDirectoryPath)
    {
        char FullPath[512] = {0};
        size_t ShadersDirectoryLength = CopyString(FullPath, ShadersDirectoryPath);
        if(FullPath[ShadersDirectoryLength] != '/')
        {
            FullPath[ShadersDirectoryLength++] = '/';
        }
        char *FilePath = FullPath + ShadersDirectoryLength;
        for(u32 ShaderProgramIndex = 0; ShaderProgramIndex < SP_Count; ++ShaderProgramIndex)
        {
            shader_program_description *Desc = ShaderPrograms + ShaderProgramIndex;
#define WriteShader(SType) \
            for(u32 IncludeIndex = 0; IncludeIndex < MaxShaderIncludes; ++IncludeIndex) \
            { \
                char *IncludePath = Desc->SType##Includes[IncludeIndex]; \
                if(!IncludePath) \
                { \
                    break; \
                } \
                size_t IncludeLength = CopyString(FilePath, IncludePath); \
                FilePath[IncludeLength] = 0; \
                printf("writing %s\n", FullPath); \
                load_file_result Include = MemoryMap(FullPath); \
                WriteToFile(Include.Size, Include.Data, &OutputMemory); \
                MemoryUnmap(Include); \
            } \
            char *SType##Path = FullPath; \
            size_t SType##PathLength = CopyString(FilePath, Desc->SType##Path); \
            FilePath[SType##PathLength] = 0; \
            printf("writing %s\n", FullPath); \
            load_file_result SType##Data = MemoryMap(FullPath); \
            WriteToFile(SType##Data.Size, SType##Data.Data, &OutputMemory); \
            WriteToFile(1, &Zero, &OutputMemory); \
            MemoryUnmap(SType##Data);

            WriteShader(Vertex);
            WriteShader(Fragment);
        }
    }

    // write the kanji description hashtable
    ChangeWritingPosition(&OutputMemory, Header.KanjiHashtableOffset);
    WriteToFile(Hashtable1Size, Hashtable1, &OutputMemory);

    // now write the headers
    ChangeWritingPosition(&OutputMemory, Header.WordDescriptionsOffset);

    size_t WordDescriptionsSize = sizeof(kmdb_word_description)*WordEntryCount;
    kmdb_word_description *WordDescriptions = calloc(1, WordDescriptionsSize);

    for(u32 EntryIndex = 0;
        EntryIndex < WordEntryCount;
        ++EntryIndex)
    {
        word_entry *Entry = Entries + EntryIndex;
        kmdb_word_description WordDescription = 
        {
            .ReadingCount = Entry->ReadingCount,
            .MeaningCount = Entry->MeaningCount,
            .FlagCount = Entry->FlagCount,
            .WritingOffset = Entry->WritingOffset,
            .ReadingOffset = Entry->ReadingOffset,
            .MeaningOffset = Entry->MeaningOffset,
            .FlagOffset = Entry->FlagOffset,
        };

        WordDescriptions[EntryIndex] = WordDescription;
    }

    WriteToFile(WordDescriptionsSize, WordDescriptions, &OutputMemory);

    // finally write out the header
    ChangeWritingPosition(&OutputMemory, 0);
    WriteToFile(sizeof(Header), &Header, &OutputMemory);

    FILE *OutputFile = fopen(OutputFilename, "w");
    if(!OutputFile)
    {
        ErrorOut(fopen());
    }
#if 0
    // save raw bytes to disk
    if(fwrite(OutputMemory.Data, 1, OutputMemory.Used, OutputFile) != OutputMemory.Used)
    {
        ErrorOut(fwrite());
    }
#else
    // save as c header
    WriteStringToFile(OutputFile, ConstString("static char GlobalDatabaseBytes[] = {\n"));
    char OutputByte[5];
    OutputByte[0] = '0';
    OutputByte[1] = 'x';
    OutputByte[4] = ',';
    string OutputString = {.Size = 5, .Data = OutputByte};
    for(u64 Bytei = 0; Bytei < OutputMemory.Used; ++Bytei)
    {
        unsigned char Byte = (unsigned char)OutputMemory.Data[Bytei];
        // convert each byte to 5 characters in the form of 0xAB,
        unsigned char Nibbles[2] = {Byte >> 4, Byte & 0xF};
        for(u32 Nibblei = 0; Nibblei < 2; ++Nibblei)
        {
            unsigned char Repr = Nibbles[Nibblei];
            Assert((Repr <= 0xF));
            if(Repr < 10) { Repr += '0'; }
            else { Repr += 'A' - 10; }
            OutputByte[2+Nibblei] = (char)Repr;
        }
        WriteStringToFile(OutputFile, OutputString);
    }
    WriteStringToFile(OutputFile, ConstString("\n};\n"));
#endif
}
