#ifndef KMDB_H
#define KMDB_H

//
// NOTE: this file is for all the things that have to
// do with the database as well as what's shared between kmap
// and the converter.
//

#include "kmath.h"

#define TACellSide 128
#define WordsPerChunk 32
#define ChunkDim ((TACellSide)*40.f)

#define KanjiDefaultScreenWidth (f32)(TACellSide)
#define PermanentKanjiStorageSize 1024

#define IsKanji(Codepoint) ((((Codepoint) >= 0x4e00) && ((Codepoint) <= 0x9fff)) || \
                            (((Codepoint) >= 0x2A700) && ((Codepoint) <= 0x2B73F)) || \
                            (((Codepoint) >= 0x2B740) && ((Codepoint) <= 0x2B81F)) || \
                            (((Codepoint) >= 0x2B820) && ((Codepoint) <= 0x2CEAF)) || \
                            (((Codepoint) >= 0x2CEB0) && ((Codepoint) <= 0x2EBEF)) || \
                            (((Codepoint) >= 0x30000) && ((Codepoint) <= 0x3134F)))

typedef struct
{
    c32 Codepoint;
    u16 WordIndexCount;
    u8 OnReadingCount;
    u8 KunReadingCount;
    u8 MeaningCount;
    u8 RadicalCount;
    u8 SimilarKanjiCount;
    u8 FrequentlyUsedWordCount;

    size_t WordIndexOffset;
    size_t OnReadingOffset;
    size_t KunReadingOffset;
    size_t MeaningOffset;
    size_t RadicalOffset;
    size_t SimilarKanjiOffset;
    size_t FrequentlyUsedWordIndexOffset;
} kanji_info;

internal u16
GetKanjiHash(c32 Codepoint)
{
    u16 Hash = Codepoint & (PermanentKanjiStorageSize - 1);
    return Hash;
}

// NOTE: database file consists of 2 portions: header and data.
// header has references to everything we need,
// so if memory is a problem, we can just offload the actual data
// and only keep the header
//
#define KMDB_MAGIC (('K' << 24) | ('M' << 16) | ('D' << 8) | ('B'))
#define KMDB_MAKE_VERSION(Major, Minor) (((Major) << 16) | (Minor))

typedef struct
{
    u8 ReadingCount;
    u8 MeaningCount; 
    u8 FlagCount;
    size_t WritingOffset;
    size_t ReadingOffset;
    size_t MeaningOffset; 
    size_t FlagOffset;
} kmdb_word_description;

typedef struct
{
    u32 Magic;
    u32 Version;
    u32 WordCount;
    u32 KanjiCount;
    size_t KanjiHashtableOffset;
    size_t KanjiDataOffset;
    size_t WordDescriptionsOffset;
    size_t WordDataOffset;
    size_t ShaderDataOffset;
} kmdb_header;

typedef enum
{
    WF_None,

    WF_bra,
    WF_hob,
    WF_ksb,
    WF_ktb,
    WF_kyb,
    WF_kyu,
    WF_nab,
    WF_osb,
    WF_rkb,
    WF_thb,
    WF_tsb,
    WF_tsug,
    WF_agric,
    WF_anat,
    WF_archeol,
    WF_archit,
    WF_art,
    WF_astron,
    WF_audvid,
    WF_aviat,
    WF_baseb,
    WF_biochem,
    WF_biol,
    WF_bot,
    WF_Buddh,
    WF_bus,
    WF_chem,
    WF_Christn,
    WF_cloth,
    WF_comp,
    WF_cryst,
    WF_ecol,
    WF_econ,
    WF_elec,
    WF_electr,
    WF_embryo,
    WF_engr,
    WF_ent,
    WF_finc,
    WF_fish,
    WF_food,
    WF_gardn,
    WF_genet,
    WF_geogr,
    WF_geol,
    WF_geom,
    WF_go,
    WF_golf,
    WF_gramm,
    WF_grmyth,
    WF_hanaf,
    WF_horse,
    WF_law,
    WF_ling,
    WF_logic,
    WF_MA,
    WF_mahj,
    WF_math,
    WF_mech,
    WF_med,
    WF_met,
    WF_mil,
    WF_music,
    WF_ornith,
    WF_paleo,
    WF_pathol,
    WF_pharm,
    WF_phil,
    WF_photo,
    WF_physics,
    WF_physiol,
    WF_print,
    WF_psy,
    WF_psych,
    WF_rail,
    WF_Shinto,
    WF_shogi,
    WF_sports,
    WF_stat,
    WF_sumo,
    WF_telec,
    WF_tradem,
    WF_vidg,
    WF_zool,
    WF_ateji,
    WF_ke_ik,
    WF_ke_iK,
    WF_io,
    WF_oK,
    WF_rK,
    WF_abbr,
    WF_arch,
    WF_char,
    WF_chn,
    WF_col,
    WF_company,
    WF_creat,
    WF_dated,
    WF_dei,
    WF_derog,
    WF_doc,
    WF_ev,
    WF_fam,
    WF_fem,
    WF_fict,
    WF_form,
    WF_given,
    WF_group,
    WF_hist,
    WF_hon,
    WF_hum,
    WF_id,
    WF_joc,
    WF_leg,
    WF_m_sl,
    WF_male,
    WF_myth,
    WF_net_sl,
    WF_obj,
    WF_obs,
    WF_obsc,
    WF_on_mim,
    WF_organization,
    WF_oth,
    WF_person,
    WF_place,
    WF_poet,
    WF_pol,
    WF_product,
    WF_proverb,
    WF_quote,
    WF_rare,
    WF_relig,
    WF_sens,
    WF_serv,
    WF_sl,
    WF_station,
    WF_surname,
    WF_uk,
    WF_unclass,
    WF_vulg,
    WF_work,
    WF_X,
    WF_yoji,
    WF_adj_f,
    WF_adj_i,
    WF_adj_ix,
    WF_adj_kari,
    WF_adj_ku,
    WF_adj_na,
    WF_adj_nari,
    WF_adj_no,
    WF_adj_pn,
    WF_adj_shiku,
    WF_adj_t,
    WF_adv,
    WF_adv_to,
    WF_aux,
    WF_aux_adj,
    WF_aux_v,
    WF_conj,
    WF_cop,
    WF_ctr,
    WF_exp,
    WF_int,
    WF_n,
    WF_n_adv,
    WF_n_pr,
    WF_n_pref,
    WF_n_suf,
    WF_n_t,
    WF_num,
    WF_pn,
    WF_pref,
    WF_prt,
    WF_suf,
    WF_unc,
    WF_v_unspec,
    WF_v1,
    WF_v1_s,
    WF_v2a_s,
    WF_v2b_k,
    WF_v2b_s,
    WF_v2d_k,
    WF_v2d_s,
    WF_v2g_k,
    WF_v2g_s,
    WF_v2h_k,
    WF_v2h_s,
    WF_v2k_k,
    WF_v2k_s,
    WF_v2m_k,
    WF_v2m_s,
    WF_v2n_s,
    WF_v2r_k,
    WF_v2r_s,
    WF_v2s_s,
    WF_v2t_k,
    WF_v2t_s,
    WF_v2w_s,
    WF_v2y_k,
    WF_v2y_s,
    WF_v2z_s,
    WF_v4b,
    WF_v4g,
    WF_v4h,
    WF_v4k,
    WF_v4m,
    WF_v4n,
    WF_v4r,
    WF_v4s,
    WF_v4t,
    WF_v5aru,
    WF_v5b,
    WF_v5g,
    WF_v5k,
    WF_v5k_s,
    WF_v5m,
    WF_v5n,
    WF_v5r,
    WF_v5r_i,
    WF_v5s,
    WF_v5t,
    WF_v5u,
    WF_v5u_s,
    WF_v5uru,
    WF_vi,
    WF_vk,
    WF_vn,
    WF_vr,
    WF_vs,
    WF_vs_c,
    WF_vs_i,
    WF_vs_s,
    WF_vt,
    WF_vz,
    WF_gikun,
    WF_ik,
    WF_ok,
    WF_uK,
} word_flag;

typedef enum
{
    SP_TextBackground,
    SP_Debug,
    SP_Lines,
    SP_Text,
    SP_Background,

    //
    SP_Count
} shader_index;

#define MaxShaderIncludes 16

typedef struct
{
    unsigned int Handle;
    char *VertexPath;
    char *FragmentPath;

    char *VertexIncludes[MaxShaderIncludes],
         *FragmentIncludes[MaxShaderIncludes];
} shader_program_description;

shader_program_description ShaderPrograms[SP_Count] =
{
    [SP_Text] =
    {
        .VertexPath = "text.vert",
        .FragmentPath = "text.frag",
        .VertexIncludes = {"camera_spec.glsl"},
    },
    [SP_Background] =
    {
        .VertexPath = "background.vert",
        .FragmentPath = "background.frag",
        .FragmentIncludes = {"camera_spec.glsl"}
    },
    [SP_TextBackground] =
    {
        .VertexPath = "text_bg.vert",
        .FragmentPath = "text_bg.frag",
        .VertexIncludes = {"camera_spec.glsl"},
    },
    [SP_Debug] =
    {
        .VertexPath = "debug.vert",
        .FragmentPath = "debug.frag",
        .VertexIncludes = {"camera_spec.glsl"},
    },
    [SP_Lines] =
    {
        .VertexPath = "lines.vert",
        .FragmentPath = "lines.frag",
        .VertexIncludes = {"camera_spec.glsl"},
    },
};

internal iv2
GetChunkPosFromWorldPos(v2 WorldPos)
{
    iv2 Result = 
    {
        .x = (i32)Floor(WorldPos.x / ChunkDim),
        .y = (i32)Floor(WorldPos.y / ChunkDim)
    };
    return Result;
}

internal u32
GetChunkIndex(u32 ChunkCount, i32 ChunkX, i32 ChunkY)
{
    i32 ChunkTotalRows = (i32)Sqrt((f32)ChunkCount);
    i32 HalfChunkTotalRows = ChunkTotalRows / 2;

    u32 Result = ChunkTotalRows * (u32)(ChunkY + HalfChunkTotalRows) + (u32)(ChunkX + HalfChunkTotalRows);
    return Result;
}

internal u32
RandomU32(u64 *State)
{
    u64 NewState = *State * 23 + 50;
    *State = NewState;
    u32 Result = (NewState >> (29 - (NewState >> 61)));
    return Result;
}

internal f32
RandomF32InRange(u64 *State, f32 Min, f32 Max)
{
    Assert(Max >= Min);
    f32 Generated = (f32)RandomU32(State);
    f32 Range = Max - Min;
    f32 Result = (Generated / (f32)UINT32_MAX) * Range + Min;
    Assert(Result >= Min);
    Assert(Result <= Max);
    return Result;
}

internal u32
RandomU32InRange(u64 *State, u32 Min, u32 Max)
{
    Assert(Min < Max);
    u32 Generated = RandomU32(State);
    u32 Result = Min + (Generated % Max);
    return Result;
}

// return the box that doesn't collide with anything
internal rect
GetNewWordBoundingBox(u64 *Rng, i32 ChunkTotalRows, u32 ChunkIndex, v2 WordDim, u32 NumberOfWordsInChunk, rect *OtherWordsInChunk)
{
    rect Result;

    i32 ChunkRow = ((i32)ChunkIndex / ChunkTotalRows) - (ChunkTotalRows / 2); 
    i32 ChunkCol = ((i32)ChunkIndex % ChunkTotalRows) - (ChunkTotalRows / 2); 
    f32 ChunkX = ChunkCol * ChunkDim;
    f32 ChunkY = ChunkRow * ChunkDim;
    v2 WordBasePosition = V2(ChunkX + (ChunkDim / 2), ChunkY + (ChunkDim / 2));
    {
        v2 ChunkOffset;
FindNextSpot:
        ChunkOffset = V2(RandomF32InRange(Rng, -ChunkDim/2 + 100.f, ChunkDim/2 - 100.f),
                         RandomF32InRange(Rng, -ChunkDim/2 + 100.f, ChunkDim/2 - 100.f));
        Assert(V2Length(ChunkOffset) < ChunkDim);
        v2 NewBottomLeft = V2Add(WordBasePosition, ChunkOffset);
        Result = RectangleFromBottomLeftAndDim(NewBottomLeft, WordDim);

        for(u32 OtherRectIndex = 0;
            OtherRectIndex < NumberOfWordsInChunk;
            ++OtherRectIndex)
        {
            rect OtherWordRect = OtherWordsInChunk[OtherRectIndex];
            if(RectanglesIntersect(Result, OtherWordRect))
            {
                goto FindNextSpot;
            }
        }
    }
    return Result;
}

#endif // KMDB_H
