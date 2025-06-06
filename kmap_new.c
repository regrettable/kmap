#include "kmap.h"

#define MaxCharactersInFrame 2048
#define InvalidWordIndex UINT32_MAX
#define InvalidWorldPosition -9999999.f

global platform_functions Platform = {0};
global debug_queue GlobalDebugQueue = {0};
extern u32 GlobalDebugTimingCount;
global char GlVersionString[] = { '#', 'v', 'e', 'r', 's', 'i', 'o', 'n', ' ', OPENGL_MAJOR_RELEASE + '0', OPENGL_MINOR_RELEASE + '0', '0', ' ', 'c', 'o', 'r', 'e', '\n', 0 };
global char *GlobalWordFlagNames[] =
{
    [WF_bra] = "Brazilian",
    [WF_hob] = "Hokkaido-ben",
    [WF_ksb] = "Kansai-ben",
    [WF_ktb] = "Kantou_ben",
    [WF_kyb] = "Kyoto-ben",
    [WF_kyu] = "Kyuushuu-ben",
    [WF_nab] = "Nagano-ben",
    [WF_osb] = "Osaka-ben",
    [WF_rkb] = "Ryuukyuu-ben",
    [WF_thb] = "Touhoku-ben",
    [WF_tsb] = "Tosa-ben",
    [WF_tsug] = "Tsugaru-ben",
    [WF_agric] = "agriculture",
    [WF_anat] = "anatomy",
    [WF_archeol] = "archeology",
    [WF_archit] = "architecture",
    [WF_art] = "art, aesthetics",
    [WF_astron] = "astronomy",
    [WF_audvid] = "audiovisual",
    [WF_aviat] = "aviation",
    [WF_baseb] = "baseball",
    [WF_biochem] = "biochemistry",
    [WF_biol] = "biology",
    [WF_bot] = "botany",
    [WF_Buddh] = "Buddhism",
    [WF_bus] = "business",
    [WF_chem] = "chemistry",
    [WF_Christn] = "Christianity",
    [WF_cloth] = "clothing",
    [WF_comp] = "computing",
    [WF_cryst] = "crystallography",
    [WF_ecol] = "ecology",
    [WF_econ] = "economics",
    [WF_elec] = "electricity, elec. eng.",
    [WF_electr] = "electronics",
    [WF_embryo] = "embryology",
    [WF_engr] = "engineering",
    [WF_ent] = "entomology",
    [WF_finc] = "finance",
    [WF_fish] = "fishing",
    [WF_food] = "food, cooking",
    [WF_gardn] = "gardening, horticulture",
    [WF_genet] = "genetics",
    [WF_geogr] = "geography",
    [WF_geol] = "geology",
    [WF_geom] = "geometry",
    [WF_go] = "go (game)",
    [WF_golf] = "golf",
    [WF_gramm] = "grammar",
    [WF_grmyth] = "Greek mythology",
    [WF_hanaf] = "hanafuda",
    [WF_horse] = "horse racing",
    [WF_law] = "law",
    [WF_ling] = "linguistics",
    [WF_logic] = "logic",
    [WF_MA] = "martial arts",
    [WF_mahj] = "mahjong",
    [WF_math] = "mathematics",
    [WF_mech] = "mechanical engineering",
    [WF_med] = "medicine",
    [WF_met] = "meteorology",
    [WF_mil] = "military",
    [WF_music] = "music",
    [WF_ornith] = "ornithology",
    [WF_paleo] = "paleontology",
    [WF_pathol] = "pathology",
    [WF_pharm] = "pharmacy",
    [WF_phil] = "philosophy",
    [WF_photo] = "photography",
    [WF_physics] = "physics",
    [WF_physiol] = "physiology",
    [WF_print] = "printing",
    [WF_psy] = "psychiatry",
    [WF_psych] = "psychology",
    [WF_rail] = "railway",
    [WF_Shinto] = "Shinto",
    [WF_shogi] = "shogi",
    [WF_sports] = "sports",
    [WF_stat] = "statistics",
    [WF_sumo] = "sumo",
    [WF_telec] = "telecommunications",
    [WF_tradem] = "trademark",
    [WF_vidg] = "video games",
    [WF_zool] = "zoology",
    [WF_ateji] = "ateji (phonetic) reading",
    [WF_ke_ik] = "word containing irregular kana usage",
    [WF_ke_iK] = "word containing irregular kanji usage",
    [WF_io] = "irregular okurigana usage",
    [WF_oK] = "word containing out-dated kanji or kanji usage",
    [WF_rK] = "rarely-used kanji form",
    [WF_abbr] = "abbreviation",
    [WF_arch] = "archaism",
    [WF_char] = "character",
    [WF_chn] = "children's language",
    [WF_col] = "colloquialism",
    [WF_company] = "company name",
    [WF_creat] = "creature",
    [WF_dated] = "dated term",
    [WF_dei] = "deity",
    [WF_derog] = "derogatory",
    [WF_doc] = "document",
    [WF_ev] = "event",
    [WF_fam] = "familiar language",
    [WF_fem] = "female term or language",
    [WF_fict] = "fiction",
    [WF_form] = "formal or literary term",
    [WF_given] = "given name or forename, gender not specified",
    [WF_group] = "group",
    [WF_hist] = "historical term",
    [WF_hon] = "honorific or respectful (sonkeigo) language",
    [WF_hum] = "humble (kenjougo) language",
    [WF_id] = "idiomatic expression",
    [WF_joc] = "jocular, humorous term",
    [WF_leg] = "legend",
    [WF_m_sl] = "manga slang",
    [WF_male] = "male term or language",
    [WF_myth] = "mythology",
    [WF_net_sl] = "Internet slang",
    [WF_obj] = "object",
    [WF_obs] = "obsolete term",
    [WF_obsc] = "obscure term",
    [WF_on_mim] = "onomatopoeic or mimetic word",
    [WF_organization] = "organization name",
    [WF_oth] = "other",
    [WF_person] = "full name of a particular person",
    [WF_place] = "place name",
    [WF_poet] = "poetical term",
    [WF_pol] = "polite (teineigo) language",
    [WF_product] = "product name",
    [WF_proverb] = "proverb",
    [WF_quote] = "quotation",
    [WF_rare] = "rare",
    [WF_relig] = "religion",
    [WF_sens] = "sensitive",
    [WF_serv] = "service",
    [WF_sl] = "slang",
    [WF_station] = "railway station",
    [WF_surname] = "family or surname",
    [WF_uk] = "word usually written using kana alone",
    [WF_unclass] = "unclassified name",
    [WF_vulg] = "vulgar expression or word",
    [WF_work] = "work of art, literature, music, etc. name",
    [WF_X] = "rude or X-rated term (not displayed in educational software)",
    [WF_yoji] = "yojijukugo",
    [WF_adj_f] = "noun or verb acting prenominally",
    [WF_adj_i] = "adjective (keiyoushi)",
    [WF_adj_ix] = "adjective (keiyoushi) - yoi/ii class",
    [WF_adj_kari] = "'kari' adjective (archaic)",
    [WF_adj_ku] = "'ku' adjective (archaic)",
    [WF_adj_na] = "adjectival nouns or quasi-adjectives (keiyodoshi)",
    [WF_adj_nari] = "archaic/formal form of na-adjective",
    [WF_adj_no] = "nouns which may take the genitive case particle 'no'",
    [WF_adj_pn] = "pre-noun adjectival (rentaishi)",
    [WF_adj_shiku] = "'shiku' adjective (archaic)",
    [WF_adj_t] = "'taru' adjective",
    [WF_adv] = "adverb (fukushi)",
    [WF_adv_to] = "adverb taking the 'to' particle",
    [WF_aux] = "auxiliary",
    [WF_aux_adj] = "auxiliary adjective",
    [WF_aux_v] = "auxiliary verb",
    [WF_conj] = "conjunction",
    [WF_cop] = "copula",
    [WF_ctr] = "counter",
    [WF_exp] = "expressions (phrases, clauses, etc.)",
    [WF_int] = "interjection (kandoushi)",
    [WF_n] = "noun (common) (futsuumeishi)",
    [WF_n_adv] = "adverbial noun (fukushitekimeishi)",
    [WF_n_pr] = "proper noun",
    [WF_n_pref] = "noun, used as a prefix",
    [WF_n_suf] = "noun, used as a suffix",
    [WF_n_t] = "noun (temporal) (jisoumeishi)",
    [WF_num] = "numeric",
    [WF_pn] = "pronoun",
    [WF_pref] = "prefix",
    [WF_prt] = "particle",
    [WF_suf] = "suffix",
    [WF_unc] = "unclassified",
    [WF_v_unspec] = "verb unspecified",
    [WF_v1] = "Ichidan verb",
    [WF_v1_s] = "Ichidan verb - kureru special class",
    [WF_v2a_s] = "Nidan verb with 'u' ending (archaic)",
    [WF_v2b_k] = "Nidan verb (upper class) with 'bu' ending (archaic)",
    [WF_v2b_s] = "Nidan verb (lower class) with 'bu' ending (archaic)",
    [WF_v2d_k] = "Nidan verb (upper class) with 'dzu' ending (archaic)",
    [WF_v2d_s] = "Nidan verb (lower class) with 'dzu' ending (archaic)",
    [WF_v2g_k] = "Nidan verb (upper class) with 'gu' ending (archaic)",
    [WF_v2g_s] = "Nidan verb (lower class) with 'gu' ending (archaic)",
    [WF_v2h_k] = "Nidan verb (upper class) with 'hu/fu' ending (archaic)",
    [WF_v2h_s] = "Nidan verb (lower class) with 'hu/fu' ending (archaic)",
    [WF_v2k_k] = "Nidan verb (upper class) with 'ku' ending (archaic)",
    [WF_v2k_s] = "Nidan verb (lower class) with 'ku' ending (archaic)",
    [WF_v2m_k] = "Nidan verb (upper class) with 'mu' ending (archaic)",
    [WF_v2m_s] = "Nidan verb (lower class) with 'mu' ending (archaic)",
    [WF_v2n_s] = "Nidan verb (lower class) with 'nu' ending (archaic)",
    [WF_v2r_k] = "Nidan verb (upper class) with 'ru' ending (archaic)",
    [WF_v2r_s] = "Nidan verb (lower class) with 'ru' ending (archaic)",
    [WF_v2s_s] = "Nidan verb (lower class) with 'su' ending (archaic)",
    [WF_v2t_k] = "Nidan verb (upper class) with 'tsu' ending (archaic)",
    [WF_v2t_s] = "Nidan verb (lower class) with 'tsu' ending (archaic)",
    [WF_v2w_s] = "Nidan verb (lower class) with 'u' ending and 'we' conjugation (archaic)",
    [WF_v2y_k] = "Nidan verb (upper class) with 'yu' ending (archaic)",
    [WF_v2y_s] = "Nidan verb (lower class) with 'yu' ending (archaic)",
    [WF_v2z_s] = "Nidan verb (lower class) with 'zu' ending (archaic)",
    [WF_v4b] = "Yodan verb with 'bu' ending (archaic)",
    [WF_v4g] = "Yodan verb with 'gu' ending (archaic)",
    [WF_v4h] = "Yodan verb with 'hu/fu' ending (archaic)",
    [WF_v4k] = "Yodan verb with 'ku' ending (archaic)",
    [WF_v4m] = "Yodan verb with 'mu' ending (archaic)",
    [WF_v4n] = "Yodan verb with 'nu' ending (archaic)",
    [WF_v4r] = "Yodan verb with 'ru' ending (archaic)",
    [WF_v4s] = "Yodan verb with 'su' ending (archaic)",
    [WF_v4t] = "Yodan verb with 'tsu' ending (archaic)",
    [WF_v5aru] = "Godan verb - -aru special class",
    [WF_v5b] = "Godan verb with 'bu' ending",
    [WF_v5g] = "Godan verb with 'gu' ending",
    [WF_v5k] = "Godan verb with 'ku' ending",
    [WF_v5k_s] = "Godan verb - Iku/Yuku special class",
    [WF_v5m] = "Godan verb with 'mu' ending",
    [WF_v5n] = "Godan verb with 'nu' ending",
    [WF_v5r] = "Godan verb with 'ru' ending",
    [WF_v5r_i] = "Godan verb with 'ru' ending (irregular verb)",
    [WF_v5s] = "Godan verb with 'su' ending",
    [WF_v5t] = "Godan verb with 'tsu' ending",
    [WF_v5u] = "Godan verb with 'u' ending",
    [WF_v5u_s] = "Godan verb with 'u' ending (special class)",
    [WF_v5uru] = "Godan verb - Uru old class verb (old form of Eru)",
    [WF_vi] = "intransitive verb",
    [WF_vk] = "Kuru verb - special class",
    [WF_vn] = "irregular nu verb",
    [WF_vr] = "irregular ru verb, plain form ends with -ri",
    [WF_vs] = "noun or participle which takes the aux. verb suru",
    [WF_vs_c] = "su verb - precursor to the modern suru",
    [WF_vs_i] = "suru verb - included",
    [WF_vs_s] = "suru verb - special class",
    [WF_vt] = "transitive verb",
    [WF_vz] = "Ichidan verb - zuru verb (alternative form of -jiru verbs)",
    [WF_gikun] = "gikun (meaning as reading) or jukujikun (special kanji reading)",
    [WF_ik] = "word containing irregular kana usage",
    [WF_ok] = "out-dated or obsolete kana usage",
    [WF_uK] = "word usually written using kanji alone",
};

typedef struct
{
    u8 MinimumValue, MaximumValue;
    char *Description;
} word_flag_type;

global word_flag_type GlobalFlagTypes[] =
{
    { .MinimumValue = WF_adj_f, .MaximumValue = WF_vz, .Description = "Part of speech:" },
    { .MinimumValue = WF_bra, .MaximumValue = WF_tsug, .Description = "Dialect:" },
    { .MinimumValue = WF_agric, .MaximumValue = WF_zool, .Description = "Field:" },
    { .MinimumValue = WF_ateji, .MaximumValue = WF_rK, .Description = "Kanji info:" },
    { .MinimumValue = WF_gikun, .MaximumValue = WF_uK, .Description = "Reading info:" },
    { .MinimumValue = WF_abbr, .MaximumValue = WF_yoji,  .Description = "Other info:" },
};

// NOTE: used for debugging
#define WHITE V3(1.f, 1.f, 1.f)
#define BLACK V3(0.f, 0.f, 0.f)
#define BLUE V3(0.f, 0.f, 1.f)
#define RED V3(1.f, 0.f, 0.f)
#define GREEN V3(0.f, 1.f, 0.f)
#define CYAN V3(0.f, 1.f, 1.f)
#define PINK V3(1.f, 0.f, 1.f)
#define YELLOW V3(1.f, 1.f, 0.f)

#define IsPrintableAscii(Codepoint) ((Codepoint >= ' ') && (Codepoint <= '~'))
#define UiCharacterCount (('~' - ' ') + (L'ヽ' - L'ぁ') + 2)

// NOTE: TA = Texture Atlas
#define TACellsPerSide 64
#define TACellCount (TACellsPerSide*TACellsPerSide)

StaticAssert(TACellCount > UiCharacterCount);

#define TACellSize (TACellSide*TACellSide)
#define TASide (TACellSide*TACellsPerSide)

#define UseShader(ProgramName) glUseProgram(ShaderPrograms[ProgramName].Handle)

// NOTE: voronoi background. kinda slow...
// https://nullprogram.com/blog/2014/06/01/
#define USE_BACKGROUND_SHADER 0

#define REFERENCE_CACHE_MAGIC (('K' << 24) | ('R' << 16) | ('F' << 8) | ('C'))
#define REFERENCE_CACHE_VERSION 1

typedef struct
{
    u64 Rng;
    v2 CameraPos;
    u32 ReferenceCount;
} reference_layer;

#define ReferenceLayerCount 10

typedef struct
{
    u32 Magic;
    u32 Version;
    u32 WordCount;
    u32 FontNameHash;
    reference_layer Layers[ReferenceLayerCount];
} reference_cache_header;

typedef struct
{
    u32 WordIndex;
    v2 KanjiPosition;
} line_data;

typedef union
{
    void *Ptr;
    struct
    {
        u32 High;
        u32 Low;
    };
} button_data;

typedef struct kmap_state kmap_state;
typedef struct button button;
#define BUTTON_CALLBACK(Name) void Name(kmap_state *State, b32 IsRightClicked, b32 IsMiddleClicked, button_data Data)
typedef BUTTON_CALLBACK(button_callback);

struct button
{
    button_callback *Callback;
    rect BoundingBox;
    button_data Data;
};

struct kmap_state
{
    memory_arena Arena;
    memory_arena FrameArena;
    memory_arena WordArena;

    f32 LastFrameTime;
    v2 LastFrameMousePos;

    camera Camera;

    v2 CameraStartpoint;
    v2 CameraEndpoint;
    f32 CameraMovementTime;
    f32 CameraMovementStartTime;
    b32 CameraMovesTowardsEndpoint;

    load_file_result FontFile;
    stbtt_fontinfo Font;
    f32 LargestFontScale;
    f32 UiTextScale;

    load_file_result Database;

    u32 WordCount;
    word *Words;
    char *WordData;

    GLuint CameraUBO;
    GLuint ThemeUBO;

    GLuint TextVAO;
    GLuint TextVBO;

#if USE_BACKGROUND_SHADER
    GLuint BackgroundVAO;
    GLuint BackgroundVBO;
#endif

    struct
    {
        ta_cell *Cells;
        GLuint Texture;
    } TA;

    GLuint TextBackgroundVAO;
    GLuint TextBackgroundVBO;

    GLuint LineVAO;
    GLuint LineVBO;
    u32 LineCount;

    u32 SelectedWordPosition;
    line_data *Lines;
    c32 SelectedCodepoint;

    b32 UsingLightTheme;
    theme Theme;

    kanji_hashtable KanjiHashtable;

    u32 InputFieldCursorPosition;
    codepoints InputField;

    u32 SearchResultCount;
    u32 SearchResultPos;
    u32 SearchResultIndices[256];

    u32 CurrentMatchOffset;
    u32 ButtonCount;
    button Buttons[256];

    // NOTE: the bookmarks are implemented through the use of these reference layers.
    // This way the word can be positioned in multiple different places at the same time.
    u32 MainLayerIndex;
    u32 SecondaryLayerIndex;

    reference_cache_header *ReferenceHeader;
    word_reference *ReferenceData;

    b32 ShouldShowKanjiOverlay;
    b32 Initialized;
};

internal GLuint
DebugLoadShader(char *Path, GLenum Type, u32 IncludeCount, char **IncludePaths)
{
    char FullPath[512] = SHADERS_PATH;
    size_t PathLength = CopyStringLen(Path, FullPath + sizeof(SHADERS_PATH) - 1, ArrayCount(FullPath));
    Assert(PathLength < (ArrayCount(FullPath) - 1));

    GLuint Shader = 0;
    b32 LoadedSuccessfully = false;
    while(!LoadedSuccessfully)
    {
        u32 ShaderSourceCount = 2 + IncludeCount;
        load_file_result MainSource = Platform.LoadFile(FullPath, true);

        char *ShaderSources[ShaderSourceCount];
        load_file_result AdditionalSources[IncludeCount];
        ShaderSources[0] = GlVersionString;
        Foreach(IncludeIndex, IncludeCount)
        {
            char IncludeFullPath[512] = SHADERS_PATH;
            size_t PathLength = CopyStringLen(IncludePaths[IncludeIndex], IncludeFullPath + sizeof(SHADERS_PATH) - 1, ArrayCount(FullPath));
            Assert(PathLength < (ArrayCount(FullPath) - 1));

            AdditionalSources[IncludeIndex] = Platform.LoadFile(IncludeFullPath, true);
            ShaderSources[IncludeIndex + 1] = AdditionalSources[IncludeIndex].Data;
        }
        ShaderSources[ShaderSourceCount - 1] = MainSource.Data;

        Shader = glCreateShader(Type);
        glShaderSource(Shader, ShaderSourceCount, (const char * const *)ShaderSources, 0);
        glCompileShader(Shader);

        glGetShaderiv(Shader, GL_COMPILE_STATUS, &LoadedSuccessfully);
        if(!LoadedSuccessfully)
        {
            GLchar InfoLog[512];
            glGetShaderInfoLog(Shader, 512, 0, InfoLog);
            eprintf("(%s): %s\n", FullPath, InfoLog);
            glDeleteShader(Shader);
            Platform.Sleep(1);
        }

        Platform.UnloadFile(MainSource);
        Foreach(IncludeIndex, IncludeCount)
        {
            Platform.UnloadFile(AdditionalSources[IncludeIndex]);
        }
    }
    return Shader;
}

internal void
DebugCreateShaderProgram(shader_program_description *ProgramDescription)
{
    b32 LinkSuccess = false;
    while(!LinkSuccess)
    {
        GLuint ShaderProgram = glCreateProgram();

        GLuint VertexShader, FragmentShader, GeometryShader = 0;

        VertexShader = DebugLoadShader(ProgramDescription->VertexPath, GL_VERTEX_SHADER, ProgramDescription->VertexIncludeCount, ProgramDescription->VertexIncludes);
        glAttachShader(ShaderProgram, VertexShader);

        FragmentShader = DebugLoadShader(ProgramDescription->FragmentPath, GL_FRAGMENT_SHADER, ProgramDescription->FragmentIncludeCount, ProgramDescription->FragmentIncludes);
        glAttachShader(ShaderProgram, FragmentShader);

        if(ProgramDescription->GeometryPath)
        {
            GeometryShader = DebugLoadShader(ProgramDescription->GeometryPath, GL_GEOMETRY_SHADER, ProgramDescription->GeometryIncludeCount, ProgramDescription->GeometryIncludes);
            glAttachShader(ShaderProgram, GeometryShader);
        }

        glLinkProgram(ShaderProgram);

        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &LinkSuccess);
        if(!LinkSuccess)
        {
            GLchar InfoLog[512];
            glGetProgramInfoLog(ShaderProgram, 512, 0, InfoLog);
            eprintf("Linking the shaders failed with %s\n", InfoLog);
            glDeleteProgram(ShaderProgram);
            Platform.Sleep(1);
        }
        else
        {
            ProgramDescription->Handle = ShaderProgram;
        }

        if(ProgramDescription->GeometryPath)
        {
            glDeleteShader(GeometryShader);
        }

        glDeleteShader(FragmentShader);
        glDeleteShader(VertexShader);
    }
}

internal GLuint
ReleaseLoadShader(char *Data, GLenum Type)
{
    GLuint Result = glCreateShader(Type);
    char *Strings[2] = { GlVersionString, Data };
    glShaderSource(Result, 2, (const GLchar * const *)Strings, 0);
    glCompileShader(Result);
    b32 LoadedSuccessfully = true;
    glGetShaderiv(Result, GL_COMPILE_STATUS, &LoadedSuccessfully);
    Assert(LoadedSuccessfully);
    return Result;
}

internal void
DebugDrawRect(v2 PixelCoordinates, v2 PixelSize, v3 Color)
{
    Assert(GlobalDebugQueue.SquareCount < ArrayCount(GlobalDebugQueue.Squares));
    square Square =
    {
        .Coordinates = PixelCoordinates,
        .Size = PixelSize,
        .Color = Color
    };
    GlobalDebugQueue.Squares[GlobalDebugQueue.SquareCount] = Square;
    GlobalDebugQueue.SquareCount++;

    // TODO: we should try do the debug pass whenever queue gets filled up,
    // but that would interfere with the regular drawing...
}

#define DebugDrawPoint(PixelCoordinates, Color) DebugDrawRect((PixelCoordinates), V2(2.f, 2.f), (Color))

// same formula as in the shader
internal v2
WorldToScreenSpace(v2 WorldPos, camera Camera)
{
    v2 CameraCorrected = V2TimesScalar(V2Sub(WorldPos, Camera.Pos), Camera.Zoom);
    v2 ScreenSpace = V2Hadamard(CameraCorrected, V2Inv(V2TimesScalar(Camera.Dim, .5f)));
    return ScreenSpace;
}

internal v2
WorldToPixelSpace(v2 WorldPos, camera Camera)
{
    v2 CameraCorrected = V2TimesScalar(V2Sub(WorldPos, Camera.Pos), Camera.Zoom);
    v2 PixelSpace = V2Add(CameraCorrected, V2TimesScalar(Camera.Dim, .5f));
    return PixelSpace;
}

// inverse of the above
internal v2
PixelToWorldSpace(v2 PixelSpace, camera Camera)
{
    v2 Result = V2Sub(PixelSpace, V2TimesScalar(Camera.Dim, .5f));
    Result = V2TimesScalar(Result, 1.f / Camera.Zoom);
    Result = V2Add(Result, Camera.Pos);
    return Result;
}

// incorrect
internal v2
PixelToScreenSpace(v2 PixelSpace, camera Camera)
{
    v2 Result = V2Sub(V2TimesScalar(V2Hadamard(PixelSpace, V2Inv(Camera.Dim)),
                                    2.f),
                      V2(1.f, 1.f));
    return Result;
}

internal void
DebugDrawRectWorld(camera Camera, v2 WorldCoordinates, v2 Size, v3 Color)
{
    v2 PixelCoordinates = WorldToPixelSpace(WorldCoordinates, Camera);
    DebugDrawRect(PixelCoordinates, Size, Color);
}

#define DebugDrawPointWorld(Camera, WorldCoordinates, Color) DebugDrawRectWorld((Camera), (WorldCoordinates), V2(2.f, 2.f), (Color))

void APIENTRY 
GLDebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
                GLsizei length, const char *message, const void *userParam)
{
    eprintf("[OPENGL]: %.*s\n", length, message);
}

internal void
ChangeTheme(kmap_state *State, theme NewTheme)
{
    State->Theme = NewTheme;
    UseShader(SP_TextBackground);
    glClearColor(NewTheme.Colors[CT_Background].r,
                 NewTheme.Colors[CT_Background].g,
                 NewTheme.Colors[CT_Background].b, 1.f);
    glBindBuffer(GL_UNIFORM_BUFFER, State->ThemeUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(theme), &NewTheme, GL_STATIC_DRAW);
}

// NOTE: state has to be initialized before this is called!
RELOAD_SHADERS(ReloadShaders)
{
    kmap_state *State = (kmap_state *)Memory->Base;
    for(size_t ShaderProgramIndex = 0;
        ShaderProgramIndex < ArrayCount(ShaderPrograms);
        ++ShaderProgramIndex)
    {
        shader_program_description *Prog = ShaderPrograms + ShaderProgramIndex;
        glDeleteProgram(Prog->Handle);
        DebugCreateShaderProgram(Prog);
    }

    ChangeTheme(State, State->UsingLightTheme ? LightTheme : DarkTheme);
}

// NOTE: state has to be initialized before this is called!
FINISH_WORK(FinishWork)
{
    kmap_state *State = (kmap_state *)Memory->Base;
    for(size_t ShaderProgramIndex = 0;
        ShaderProgramIndex < ArrayCount(ShaderPrograms);
        ++ShaderProgramIndex)
    {
        glDeleteProgram(ShaderPrograms[ShaderProgramIndex].Handle);
    }
    glDeleteTextures(1, &State->TA.Texture);
    Platform.UnloadFile(State->FontFile);
    Platform.UnloadFile(State->Database);
    ResetArena(&State->Arena);
    State->Initialized = false;
}

internal rect
GetWordBoundingBox(kmap_state *State, u32 WordIndex)
{
    rect Result = {.BottomLeft = {.x = InvalidWorldPosition, .y = InvalidWorldPosition}};
    reference_layer *CurrentLayer = State->ReferenceHeader->Layers + State->MainLayerIndex; 
    word_reference *References = State->ReferenceData + (State->MainLayerIndex * State->WordCount);
    if(State->MainLayerIndex == 0)
    {
        Result = References[WordIndex].BoundingBox;
    }
    else
    {
        Foreach(ThisReferenceIndex, CurrentLayer->ReferenceCount)
        {
            word_reference *Reference = References + ThisReferenceIndex;
            if(Reference->WordIndex == WordIndex)
            {
                Result = Reference->BoundingBox;
                break;
            }
        }
    }
    return Result;
}

// open addressing scheme
internal kanji_data
GetKanjiData(kanji_hashtable *Hashtable, c32 Codepoint)
{
    StartTimingFunc();
    kanji_data Result = {0};
    u16 Hash = GetKanjiHash(Codepoint);
    for(u32 StorageIndex = Hash;
        StorageIndex < Hashtable->KanjiCount;
        ++StorageIndex)
    {
        kanji_info *Storage = Hashtable->KanjiInfos + StorageIndex;
        Assert((Storage->Codepoint == 0) || IsKanji(Storage->Codepoint));
        if(Storage->Codepoint == Codepoint)
        {
            Result.WordIndices = (u32 *)(Hashtable->Data + Storage->WordIndexOffset);
            Result.WordCount = Storage->WordIndexCount;
            Result.OnReadingCount = Storage->OnReadingCount;
            Result.KunReadingCount = Storage->KunReadingCount;
            Result.MeaningCount = Storage->MeaningCount;
            Result.RadicalCount = Storage->RadicalCount;
            Result.SimilarKanjiCount = Storage->SimilarKanjiCount;
            Result.OnReadings = (char *)(Hashtable->Data + Storage->OnReadingOffset);
            Result.KunReadings = (char *)(Hashtable->Data + Storage->KunReadingOffset);
            Result.Meanings = (char *)(Hashtable->Data + Storage->MeaningOffset);
            Result.Radicals = (c32 *)(Hashtable->Data + Storage->RadicalOffset);
            Result.SimilarKanji = (c32 *)(Hashtable->Data + Storage->SimilarKanjiOffset);
            break;
        }
    }
    EndTimingFunc();
    return Result;
}

#if 0
internal void
SetupGlyph(stbtt_fontinfo *Font, f32 LargestFontScale, memory_arena *Arena, u32 CellIndex, u32 Codepoint, ta_cell *Cell)
{
    temp_memory TempMemory = BeginTempMemory(Arena);

    u32 CellX = CellIndex % TACellsPerSide;
    u32 CellY = CellIndex / TACellsPerSide;

    u32 GlyphIndex = stbtt_FindGlyphIndex(Font, Codepoint);
    int X0, X1, Y0, Y1;
    stbtt_GetGlyphBitmapBox(Font, GlyphIndex, LargestFontScale, LargestFontScale, &X0, &Y0, &X1, &Y1);

    Cell->IsOccupied = true;
    Cell->IsLocked = true;
    Cell->Recency = 0;
    Cell->Codepoint = Codepoint;

    v2 AtlasTopLeft = V2(CellX * TACellSide, CellY * TACellSide);
    v2 GlyphDim = V2Sub(V2(X1, Y1), V2(X0, Y0));
    v2 OneOverTASide = V2(1.f / TASide, 1.f / TASide);

    Cell->Glyph.BoundingBox.BottomLeft = V2Hadamard(AtlasTopLeft, OneOverTASide);
    Cell->Glyph.BoundingBox.TopRight = V2Hadamard(V2Add(AtlasTopLeft, GlyphDim), OneOverTASide);

    for(u32 CellSide = TACellSide, MipmapLevel = 0;
        MipmapLevel < TAMipmapLevels;
        CellSide /= 2, MipmapLevel++)
    {
        int GlyphX = CellX * CellSide;
        int GlyphY = CellY * CellSide;
        // TODO: properly solve the problem with characters getting cut off at lower resolutions
        int CellSize = CellSide*CellSide;
        int Stride = CellSide;
        f32 Width = CellSide, Height = CellSide;
        u8 *GlyphData = PushSize(TempMemory.Arena, CellSize);
        f32 FontScale = stbtt_ScaleForPixelHeight(Font, CellSide);
        stbtt_GetGlyphBitmapBox(Font, GlyphIndex, FontScale, FontScale, &X0, &Y0, &X1, &Y1);
        stbtt_MakeGlyphBitmap(Font, GlyphData, CellSide, CellSide, Stride, FontScale, FontScale, GlyphIndex);
        glTexSubImage2D(GL_TEXTURE_2D, MipmapLevel, GlyphX, GlyphY, Width, Height, GL_RED, GL_UNSIGNED_BYTE, GlyphData);
    }

    EndTempMemory(TempMemory);
}
#else
internal void
SetupGlyph(stbtt_fontinfo *Font, f32 FontScale, memory_arena *Arena, u32 CellIndex, u32 Codepoint, ta_cell *Cell)
{
    glyph Glyph = {0};
    temp_memory TempMemory = BeginTempMemory(Arena);

    u32 CellX = CellIndex % TACellsPerSide;
    u32 CellY = CellIndex / TACellsPerSide;

    u32 GlyphIndex = stbtt_FindGlyphIndex(Font, Codepoint);

    stbtt_GetGlyphBitmapBox(Font, GlyphIndex, FontScale, FontScale, &Glyph.X0, &Glyph.Y0, &Glyph.X1, &Glyph.Y1);

    // NOTE: since we have multiple font sizes, we need to know the scale to render properly.
    Cell->Scale = FontScale;
    int AtlasX = CellX * TACellSide;
    int AtlasY = CellY * TACellSide;
    Glyph.AtlasTopLeft = V2(AtlasX, AtlasY);

    int Lsb;
    stbtt_GetGlyphHMetrics(Font, GlyphIndex, &Glyph.Advance, &Lsb);
    int Width = Glyph.X1 - Glyph.X0;
    int Height = Glyph.Y1 - Glyph.Y0;
    u8 *GlyphData = PushSize(TempMemory.Arena, TACellSize);
    stbtt_MakeGlyphBitmap(Font, GlyphData, TACellSide, TACellSide, Width, FontScale, FontScale, GlyphIndex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, AtlasX, AtlasY, Width, Height, GL_RED, GL_UNSIGNED_BYTE, GlyphData);

    EndTempMemory(TempMemory);

    Cell->IsOccupied = true;
    Cell->IsLocked = true;
    Cell->Recency = 0;
    Cell->Codepoint = Codepoint;
    Cell->Glyph = Glyph;
}
#endif

internal f32
GetCharacterCenter(stbtt_fontinfo *Font,
                   f32 FontScale,
                   codepoints *Codepoints,
                   u32 CharacterIndex)
{
    int XPos = 0;
    Assert(CharacterIndex < Codepoints->Count);
    int GlyphIndex = 0,
        NextGlyphIndex = stbtt_FindGlyphIndex(Font, Codepoints->Data[0]);
    int Advance = 0, LSB;
    for(u32 CodepointIndex = 0;
        CodepointIndex < CharacterIndex;
        ++CodepointIndex)
    {
        GlyphIndex = NextGlyphIndex;
        NextGlyphIndex = stbtt_FindGlyphIndex(Font, Codepoints->Data[CodepointIndex + 1]);
        int Kerning = stbtt_GetGlyphKernAdvance(Font, GlyphIndex, NextGlyphIndex);
        stbtt_GetGlyphHMetrics(Font, GlyphIndex, &Advance, &LSB);
        XPos += Kerning + Advance;
    }
    f32 Result = FontScale * (XPos + (Advance * .5f));
    return Result;
}

#if 0
internal rect
GetRealWordBoundingBox(stbtt_fontinfo *Font, f32 FontScale, word *Word)
{
    rect Result = 
    {
        .BottomLeft = Word->BoundingBox.BottomLeft,
        .TopRight = Word->BoundingBox.BottomLeft
    };

    i32 Baseline = Result.BottomLeft.y;
    i32 Left = Result.BottomLeft.x;
    codepoints Writing = ConvertToCodepoints(Word->Writing);
    int GlyphIndex = 0,
        NextGlyphIndex = stbtt_FindGlyphIndex(Font, Writing.Data[0]);
    Foreach(CodepointIndex, Writing.Count)
    {
        GlyphIndex = NextGlyphIndex;
        NextGlyphIndex = stbtt_FindGlyphIndex(Font, Writing.Data[CodepointIndex + 1]);

        int X0, X1, Y0, Y1;
        stbtt_GetGlyphBitmapBox(Font, GlyphIndex, FontScale, FontScale, &X0, &Y0, &X1, &Y1);

        v2 ScreenBottomLeft = V2(Left + X0, Baseline - Y1);
        v2 ScreenTopRight = V2(Left + X1, Baseline - Y0);

        Result.BottomLeft.y = Minimum(Result.BottomLeft.y, ScreenBottomLeft.y);
        Result.TopRight.y = Maximum(Result.TopRight.y, ScreenTopRight.y);

        i32 Kerning = stbtt_GetGlyphKernAdvance(Font, GlyphIndex, NextGlyphIndex);
        i32 Advance, Lsb;
        stbtt_GetGlyphHMetrics(Font, GlyphIndex, &Advance, &Lsb);

        Left += FontScale * (Kerning + Advance);
    }
    Result.TopRight.x = Left;
    return Result;
}
#else
#endif

internal glyph
GetOrMakeCodepointGlyph(kmap_state *State, c32 Codepoint, f32 Scale)
{
    StartTimingFunc();
    ta_cell *UsedCell = 0;
    if((Scale == State->UiTextScale) && (IsKana(Codepoint) || IsPrintableAscii(Codepoint)))
    {
        Foreach(CellIndex, UiCharacterCount)
        {
            ta_cell *Cell = State->TA.Cells + CellIndex;
            if(Cell->IsOccupied && Cell->Codepoint == Codepoint)
            {
                Assert(Cell->IsOccupied);
                //Cell->IsLocked = true;
                UsedCell = Cell;
                break;
            }
        }
        Assert(UsedCell);
    }
    else
    {
        // in case scale is too large, don't render correctly and use the largest size instead
        if(Scale > State->LargestFontScale)
        {
            Scale = State->LargestFontScale;
        }
        u32 UsedCellIndex = 0;

        for(u32 CellIndex = UiCharacterCount;
            CellIndex < TACellCount;
            ++CellIndex)
        {
            ta_cell *Cell = State->TA.Cells + CellIndex;
            if((Cell->IsOccupied) && (Cell->Codepoint == Codepoint) && (Cell->Scale == Scale))
            {
                Cell->IsLocked = true;
                UsedCellIndex = CellIndex;
                UsedCell = Cell;
                break;
            }
        }

        // not in the cache, rasterize and put in there
        if(!UsedCell)
        {
            for(u32 CellIndex = UiCharacterCount;
                CellIndex < TACellCount;
                ++CellIndex)
            {
                ta_cell *Cell = State->TA.Cells + CellIndex;
                if(!Cell->IsOccupied)
                {
                    UsedCell = Cell;
                    UsedCellIndex = CellIndex;
                    break;
                }
                else if(!Cell->IsLocked)
                {
                    Cell->Recency++;
                }
            }

            // no unoccupied cells, have to unload something
            if(!UsedCell)
            {
                u32 HighestRecency = 0;

                for(u32 CellIndex = UiCharacterCount;
                    CellIndex < TACellCount;
                    ++CellIndex)
                {
                    ta_cell *Cell = State->TA.Cells + CellIndex;
                    if(!Cell->IsLocked)
                    {
                        if(Cell->Recency > HighestRecency)
                        {
                            UsedCellIndex = CellIndex;
                            HighestRecency = Cell->Recency;
                            UsedCell = Cell;
                        }
                    }
                }
            }

            // we don't support the situation where everything is locked
            Assert(UsedCell);

            SetupGlyph(&State->Font, Scale, &State->FrameArena, UsedCellIndex, Codepoint, UsedCell);
        }
    }
    glyph Result = UsedCell->Glyph;
    EndTimingFunc();
    return Result;
}

internal void
ChangeLineCenter(v2 NewCenter)
{
    GLuint LineProgram = ShaderPrograms[SP_Lines].Handle;
    glUseProgram(LineProgram);
    glUniform2f(glGetUniformLocation(LineProgram, "FirstPositionWorld"), NewCenter.x, NewCenter.y);
}

internal void
SelectKanjiAndDrawLines(kmap_state *State, u32 WordIndex, c32 Codepoint)
{
    StartTimingFunc();
    Assert(IsKanji(Codepoint));
    reference_layer *CurrentLayer = State->ReferenceHeader->Layers + State->MainLayerIndex;

    // NOTE: we first save the indices of the words that have
    // our kanji, and then buffer their positions
    memory_arena *PermArena = &State->WordArena;
    ResetArena(PermArena);

    State->SelectedWordPosition = UINT32_MAX;
    State->SelectedCodepoint = 0;
    word_reference *References = State->ReferenceData + (State->MainLayerIndex * State->WordCount);
    State->Lines = StartArray(PermArena, line_data);
    u32 LineCount = 0;
    Foreach(ReferenceIndex, CurrentLayer->ReferenceCount)
    {
        word_reference *Reference = References + ReferenceIndex;

        u32 ThisWordIndex = Reference->WordIndex;
        word *ThisWord = State->Words + ThisWordIndex;
        codepoints Writing = ConvertToCodepoints(ThisWord->Writing);
        Foreach(CodepointIndex, Writing.Count)
        {
            if(Writing.Data[CodepointIndex] == Codepoint)
            {
                line_data *LineData = PushElement(PermArena);
                LineData->WordIndex = ThisWordIndex;
                if(ThisWordIndex == WordIndex)
                {
                    State->SelectedWordPosition = LineCount;
                }
                ++LineCount;

                rect BoundingBox = Reference->BoundingBox;
                //  0 1 2 3
                // | | | | |
                //  ^ ^ ^ ^
                //  1 3 5 7
                //  - - - -
                //  8 8 8 8
                f32 LerpC = (1.f / (Writing.Count * 2.f)) + ((f32)CodepointIndex / (f32)Writing.Count);
                v2 KanjiPosition =
                {
                    .x = Lerp(BoundingBox.BottomLeft.x, BoundingBox.TopRight.x, LerpC),
                    .y = (BoundingBox.BottomLeft.y + BoundingBox.TopRight.y) * .5f
                };
                LineData->KanjiPosition = KanjiPosition;

                //
                break;
            }
        }
    }
    EndArray(PermArena);

    // NOTE: we are drawing one extra line from the selected word to itself
    if(LineCount)
    {
        Assert(State->SelectedWordPosition != UINT32_MAX);

        v2 Center = State->Lines[State->SelectedWordPosition].KanjiPosition;
        ChangeLineCenter(Center);
        glBindVertexArray(State->LineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, State->LineVBO);
        glBufferData(GL_ARRAY_BUFFER, LineCount*sizeof(line_data), State->Lines, GL_STATIC_DRAW);

        State->LineCount = LineCount;
        State->SelectedCodepoint = Codepoint; 
    }
    EndTimingFunc();
}

internal void
MoveCamera(kmap_state *State, v2 NewPosition, f32 Time)
{
    f32 TravelTime = .1f;
    State->CameraStartpoint = State->Camera.Pos;
    State->CameraEndpoint = NewPosition;
    State->CameraMovementTime = TravelTime;
    State->CameraMovementStartTime = Time;
    State->CameraMovesTowardsEndpoint = true;
}

internal void
MoveCameraToWord(kmap_state *State, u32 WordIndex, f32 Time)
{
    rect WordBoundingBox = GetWordBoundingBox(State, WordIndex);
    MoveCamera(State, RectangleGetCenter(WordBoundingBox), Time);
}

typedef struct
{
    u32 Count;
    u32 *Indices;
} visible_word_indices;

// NOTE: this is how the chunking system works:
//
// when we initially place the words, they are organized inside our array in a semi-sorted manner:
// for every MaxWordsInChunk words, their x and y position are never outside of that chunk's x,y * ChunkDim:
// __________________________________________________________
// |        word2     | word4     word6 |     word8 word9   | ChunkDim
// | word1    word3   |          word5  |    word7          |
// __________________________________________________________
// | ChunkDim         | ChunkDim        | ChunkDim          |
//
// Because of that, we can later (here) fiter out only the words that could
// possibly be visible (eiter in the camera's chunk, or in one of the neighbouring chunks),
// and only these words are ever considered for testing for visibility
//
// Also (for now) since all chunks except the very last one are always full, we don't have to have any kind of
// variable step in the following function, all of the offsets are fixed,
// and we don't actually have to store any kind of chunk structure.
internal visible_word_indices
GetVisibleWords(kmap_state *State)
{
    StartTimingFunc();
    visible_word_indices Result = 
    {
        .Indices = StartArray(&State->FrameArena, u32),
    };
    reference_layer *Layer = State->ReferenceHeader->Layers + State->MainLayerIndex;
    u32 LayerChunkCount = (Layer->ReferenceCount / WordsPerChunk) + 1;
    u32 MaxChunkCount = (State->WordCount / WordsPerChunk);
    iv2 CameraChunkPos = GetChunkPosFromWorldPos(State->Camera.Pos);

    for(i32 ChunkY = CameraChunkPos.y - 1;
        ChunkY <= CameraChunkPos.y + 1;
        ++ChunkY)
    {
        for(i32 ChunkX = CameraChunkPos.x - 1;
            ChunkX <= CameraChunkPos.x + 1;
            ++ChunkX)
        {
            u32 ChunkIndex = GetChunkIndex(MaxChunkCount, ChunkX, ChunkY);
            if(ChunkIndex < LayerChunkCount)
            {
                word_reference *References = State->ReferenceData + (State->MainLayerIndex * State->WordCount) + (WordsPerChunk * ChunkIndex);
                u32 ChunkReferenceCount =
                    (ChunkIndex + 1) == LayerChunkCount ?
                    Layer->ReferenceCount % WordsPerChunk :
                    WordsPerChunk;
                Foreach(ReferenceIndex, ChunkReferenceCount)
                {
                    u32 *VisibleWordIndex = PushElement(&State->FrameArena);
                    *VisibleWordIndex = References[ReferenceIndex].WordIndex;
                    ++Result.Count;
                }
            }
        }
    }

    EndArray(&State->FrameArena);
    EndTimingFunc();

    return Result;
}

internal void
PushBackground(text_render_queue *RenderQueue, rect BoundingBox, color_type ColorType)
{
    Assert((RenderQueue->BackgroundCount + 1) < RenderQueue->MaxBackgroundCount);
    text_background Background =
    {
        .Rect = BoundingBox,
        .ColorType = ColorType,
        .Depth = (f32)RenderQueue->BackgroundCount / DepthRange
    };
    RenderQueue->Backgrounds[RenderQueue->BackgroundCount++] = Background;
}

// TODO: we are still not doing fonts correctly (try smaller fonts to see that better)
internal rect
PushMulticoloredLine(kmap_state *State, text_render_queue *RenderQueue, v2 BottomLeftInPixelSpace, f32 FontScale, u32 CodepointCount, c32 *Codepoints, color_type *ColorTypes)
{
    StartTimingFunc();
    Assert((RenderQueue->CharacterCount + CodepointCount) < RenderQueue->MaxCharacterCount);

    i32 Baseline = BottomLeftInPixelSpace.y;
    i32 Left = BottomLeftInPixelSpace.x;
    rect BoundingBox = 
    {
        .BottomLeft = V2(Left, Baseline),
        .TopRight = V2(Left, Baseline)
    };

    int GlyphIndex = 0,
        NextGlyphIndex = stbtt_FindGlyphIndex(&State->Font, Codepoints[0]);
    Foreach(CodepointIndex, CodepointCount)
    {
        GlyphIndex = NextGlyphIndex;
        c32 Codepoint = Codepoints[CodepointIndex];
        glyph Glyph = GetOrMakeCodepointGlyph(State, Codepoint, FontScale);

        if(Codepoint != ' ')
        {
            v2 ScreenBottomLeft = V2(Left + Glyph.X0, Baseline - Glyph.Y1);
            v2 ScreenTopRight = V2(Left + Glyph.X1, Baseline - Glyph.Y0);
            //DebugDrawPoint(ScreenBottomLeft, RED);
            //DebugDrawPoint(ScreenTopRight, GREEN);
            //DebugDrawPoint(V2(ScreenBottomLeft.x, ScreenTopRight.y), RED);
            //DebugDrawPoint(V2(ScreenTopRight.x, ScreenBottomLeft.y), GREEN);

            BoundingBox.BottomLeft.y = Minimum(BoundingBox.BottomLeft.y, ScreenBottomLeft.y);
            BoundingBox.TopRight.y = Maximum(BoundingBox.TopRight.y, ScreenTopRight.y);
            v2 GlyphDim = V2(Glyph.X1 - Glyph.X0, Glyph.Y1 - Glyph.Y0);
            v2 TextureBottomLeft = V2TimesScalar(Glyph.AtlasTopLeft, 1.f / TASide);
            v2 TextureTopRight = V2TimesScalar(V2Add(Glyph.AtlasTopLeft, GlyphDim), 1.f / TASide);

            ui_text_display_data CharacterDisplayData =
            {
                .ScreenBottomLeft = ScreenBottomLeft,
                .ScreenTopRight = ScreenTopRight,
                .TextureTopLeft = V2(TextureBottomLeft.x, TextureTopRight.y),
                .TextureBottomRight = V2(TextureTopRight.x, TextureBottomLeft.y),
                .ColorType = ColorTypes[CodepointIndex],
                .Depth = (f32)RenderQueue->BackgroundCount / DepthRange,
            };
            Assert(CharacterDisplayData.Depth >= 0.f);

            RenderQueue->Characters[RenderQueue->CharacterCount++] = CharacterDisplayData;
        }

        Left += FontScale * Glyph.Advance;

        if(CodepointIndex < (CodepointCount - 1))
        {
            NextGlyphIndex = stbtt_FindGlyphIndex(&State->Font, Codepoints[CodepointIndex + 1]);
            i32 Kerning = stbtt_GetGlyphKernAdvance(&State->Font, GlyphIndex, NextGlyphIndex);
            Left += FontScale * Kerning;
        }
    }
    BoundingBox.TopRight.x = Left;
    EndTimingFunc();
    return BoundingBox;
}

internal rect
PushSingleColoredLine(kmap_state *State, text_render_queue *RenderQueue, v2 BottomLeftInPixelSpace, f32 Scale, u32 CodepointCount, c32 *Codepoints, color_type ColorType)
{
    color_type Colors[CodepointCount];
    Foreach(ColorIndex, CodepointCount)
    {
        Colors[ColorIndex] = ColorType;
    }
    rect Result = PushMulticoloredLine(State, RenderQueue, BottomLeftInPixelSpace, Scale, CodepointCount, Codepoints, Colors);
    return Result;
}

// object oriented codebase™
typedef struct
{
    kmap_state *State;
    text_render_queue *RenderQueue;

    v2 FirstLineBottomLeft;
    v2 Position;
    f32 LineHeight;
    f32 TextScale;
    u32 MaxCharactersPerLine;
    u32 MaxLines;
    u32 CurrentLineLength;
    u32 CurrentLineCount;

    rect BoundingBox;
} text_printer;

internal text_printer
TextPrinter(kmap_state *State, text_render_queue *RenderQueue, v2 FirstLineBottomLeft, f32 LineHeight, f32 TextScale, u32 MaxCharactersPerLine, u32 MaxLines)
{
    text_printer Result =
    {
        .State = State,
        .RenderQueue = RenderQueue,
        .Position = FirstLineBottomLeft,
        .FirstLineBottomLeft = FirstLineBottomLeft,
        .LineHeight = LineHeight,
        .TextScale = TextScale,
        .MaxCharactersPerLine = MaxCharactersPerLine,
        .MaxLines = MaxLines,
        .CurrentLineLength = 0,
        .CurrentLineCount = 0,
        .BoundingBox = 
        {
            .BottomLeft = FirstLineBottomLeft,
            .TopRight = FirstLineBottomLeft
        }
    };
    return Result;
}

internal void
SetPrinterPos(text_printer *Printer, v2 FirstLineBottomLeft)
{
    Printer->FirstLineBottomLeft = FirstLineBottomLeft;
    Printer->Position = FirstLineBottomLeft;
    Printer->CurrentLineCount = 0;
    Printer->CurrentLineLength = 0;
}

internal void
PrintNewColumn(text_printer *Printer)
{
    SetPrinterPos(Printer, V2(Printer->BoundingBox.TopRight.x + Printer->State->Theme.DictionaryEntryPadding, Printer->FirstLineBottomLeft.y));
}

internal void
PrintNewline(text_printer *Printer)
{
    if(++Printer->CurrentLineCount > Printer->MaxLines)
    {
        PrintNewColumn(Printer);
    }
    else
    {
        Printer->Position.y -= Printer->LineHeight;
        Printer->Position.x = Printer->FirstLineBottomLeft.x;
        Printer->CurrentLineLength = 0;
    }
}

internal rect
PrintCodepoints(text_printer *Printer, u32 CodepointCount, c32 *Codepoints, color_type ColorType)
{
    rect ThisCallBoundingBox = 
    {
        V2(99999999.f, 99999999.f),
        V2(-99999999.f, -99999999.f),
    };
    u32 TextIndex = 0;
    u32 LineOffset = 0;
    while(LineOffset < CodepointCount)
    {
        u32 LastBreakableCharacterIndex = -1;
        for(;(TextIndex < CodepointCount) && (Printer->CurrentLineLength <= Printer->MaxCharactersPerLine);
            ++Printer->CurrentLineLength, ++TextIndex)
        {
            c32 Character = Codepoints[TextIndex];

            if((Character == ' ') || (Character == '-'))
            {
                LastBreakableCharacterIndex = TextIndex;
            }
        }

        b32 FilledLine = Printer->CurrentLineLength > Printer->MaxCharactersPerLine;

        u32 CurrentCharacterCount = TextIndex - LineOffset;
        u32 CharactersToDraw = FilledLine ? Minimum(LastBreakableCharacterIndex - LineOffset, CurrentCharacterCount) : CurrentCharacterCount;
        rect LineBoundingBox = PushSingleColoredLine(Printer->State, Printer->RenderQueue, Printer->Position, Printer->TextScale, CharactersToDraw, Codepoints + LineOffset, ColorType);

        ThisCallBoundingBox.BottomLeft.x = Minimum(ThisCallBoundingBox.BottomLeft.x, LineBoundingBox.BottomLeft.x);
        ThisCallBoundingBox.BottomLeft.y = Minimum(ThisCallBoundingBox.BottomLeft.y, LineBoundingBox.BottomLeft.y);
        ThisCallBoundingBox.TopRight.x = Maximum(ThisCallBoundingBox.TopRight.x, LineBoundingBox.TopRight.x);
        ThisCallBoundingBox.TopRight.y = Maximum(ThisCallBoundingBox.TopRight.y, LineBoundingBox.TopRight.y);

        Printer->Position.x = LineBoundingBox.TopRight.x;

        LineOffset += CharactersToDraw;
        if(FilledLine)
        {
            PrintNewline(Printer);
        }
    }

    Printer->BoundingBox.BottomLeft.x = Minimum(Printer->BoundingBox.BottomLeft.x, ThisCallBoundingBox.BottomLeft.x);
    Printer->BoundingBox.BottomLeft.y = Minimum(Printer->BoundingBox.BottomLeft.y, ThisCallBoundingBox.BottomLeft.y);
    Printer->BoundingBox.TopRight.x = Maximum(Printer->BoundingBox.TopRight.x, ThisCallBoundingBox.TopRight.x);
    Printer->BoundingBox.TopRight.y = Maximum(Printer->BoundingBox.TopRight.y, ThisCallBoundingBox.TopRight.y);

    return ThisCallBoundingBox;
}

internal rect
PrintText(text_printer *Printer, char *Text, color_type ColorType)
{
    codepoints Codepoints = ConvertToCodepoints(Text);
    return PrintCodepoints(Printer, Codepoints.Count, Codepoints.Data, ColorType);
}

internal rect
PrintTextLn(text_printer *Printer, char *Text, color_type ColorType)
{
    rect Result = PrintText(Printer, Text, ColorType);
    PrintNewline(Printer);
    return Result;
}

internal size_t
U32ToString(u32 Value, char *Result)
{
    size_t Length = 0;
    b32 ShouldWrite = false;
    static u32 PowersOf10[] =
    {
        1000000000,
        100000000,
        10000000,
        1000000,
        100000,
        10000,
        1000,
        100,
        10,
        1
    };
    Foreach(Po10Index, ArrayCount(PowersOf10))
    {
        char Digit = (char)(Value / PowersOf10[Po10Index]);
        ShouldWrite = ShouldWrite || Digit;

        Value -= Digit * PowersOf10[Po10Index];
        if(ShouldWrite)
        {
            Result[Length++] = Digit + '0';
        }
    }
    return Length;
}

internal codepoints
U32ToCodepoints(u32 Value)
{
    codepoints Result = {0};
    b32 ShouldWrite = false;
    static u32 PowersOf10[] =
    {
        1000000000,
        100000000,
        10000000,
        1000000,
        100000,
        10000,
        1000,
        100,
        10,
        1
    };
    Foreach(Po10Index, ArrayCount(PowersOf10))
    {
        u32 Digit = Value / PowersOf10[Po10Index];
        ShouldWrite = ShouldWrite || Digit;

        Value -= Digit * PowersOf10[Po10Index];
        if(ShouldWrite)
        {
            Result.Data[Result.Count++] = (c32)Digit + '0';
        }
    }
    return Result;
}

typedef enum
{
    SF_None,
    SF_Readings,
    SF_Meanings,
} search_field;

typedef struct
{
    u32 MatchCount;
    u32 ClosestMatchIndex;
    u32 *Indices;
} search_result;

internal search_result 
SearchInWords(size_t QueryLength, char *Query, u32 WordCount, word *Words, search_field SearchField, memory_arena *Arena)
{
    Assert(QueryLength);
    size_t FieldOffset = 0;
    size_t FieldCountOffset = 0;
    switch(SearchField)
    {
        case SF_Readings:
        {
            FieldOffset = offsetof(word, Readings);
            FieldCountOffset = offsetof(word, ReadingCount);
        } break;
        case SF_Meanings:
        {
            FieldOffset = offsetof(word, Meanings);
            FieldCountOffset = offsetof(word, MeaningCount);
        } break;
        InvalidDefaultCase;
    }

    search_result Result = 
    {
        .Indices = StartArray(Arena, u32)
    };

    Foreach(WordIndex, WordCount)
    {
        word *Word = Words + WordIndex;
        char *Field = *(char **)((char *)Word + FieldOffset);
        u32 FieldCount = *(u32 *)((char *)Word + FieldCountOffset);
        Foreach(FieldIndex, FieldCount)
        {
            size_t QueryPos = 0;
            size_t ThisFieldLength = 0;
            char FieldChar;
            do
            {
                FieldChar = *Field++;
                if(QueryPos == (QueryLength - 1))
                {
                    // found a match
                    u32 *Index = PushElement(Arena);
                    *Index = WordIndex;
                    ++Result.MatchCount;
                    if(ThisFieldLength == QueryLength)
                    {
                        Result.ClosestMatchIndex = Result.MatchCount - 1;
                    }
                    goto NextWord;
                }
                else 
                {
                    char QueryChar = Query[QueryPos++];
                    if(QueryChar != FieldChar)
                    {
                        QueryPos = 0;
                    }
                }
                ++ThisFieldLength;
            }  while(FieldChar);
            ++Field;
        }
NextWord:
    }

    EndArray(Arena);
    return Result;
}

internal void
SelectWord(kmap_state *State, u32 WordIndex, f32 Time, memory_arena *Arena)
{
    word *Word = State->Words + WordIndex;
    MoveCameraToWord(State, WordIndex, Time);
    for(char *Writing = Word->Writing;
        *Writing;)
    {
        c32 Character;
        Writing += ConvertSingleUtf8(Writing, &Character);
        if(IsKanji(Character))
        {
            SelectKanjiAndDrawLines(State, WordIndex, Character);
            break;
        }
    }
}

internal search_result
SearchAndSelectClosestMatch(kmap_state *State, u32 QueryLength, char *Query, search_field Field, memory_arena *Arena, f32 Time)
{
    //word_reference_layer *MainLayer = State->ReferenceLayers + State->MainLayerIndex;
    search_result Matches = SearchInWords(QueryLength, Query, State->WordCount, State->Words, Field, Arena);
    if(Matches.MatchCount)
    {
        u32 ClosestMatchIndex = Matches.Indices[Matches.ClosestMatchIndex];
        SelectWord(State, ClosestMatchIndex, Time, Arena);
    }
    return Matches;
}

typedef struct
{
    b32 IsValid;
    codepoints Value;
} conversion_result;

internal conversion_result
ConvertToHiragana(codepoints Codepoints)
{
    conversion_result Result =
    {
        .IsValid = true
    };
#define IsVowel(Codepoint) (((Codepoint) == 'a') || ((Codepoint) == 'i') || ((Codepoint) == 'u') || ((Codepoint) == 'e') || ((Codepoint) == 'o'))
#define AUO(Codepoint) (((Codepoint) == 'a') || ((Codepoint) == 'u') || ((Codepoint) == 'o'))

    static c32 ARow[] = { ['a'] = L'あ', ['i'] = L'い', ['u'] = L'う', ['e'] = L'え', ['o'] = L'お' };

    static c32 TwoLetterSyllables[]['v'] = 
    {
        ['k'] = { ['a'] = L'か', ['i'] = L'き', ['u'] = L'く', ['e'] = L'け', ['o'] = L'こ' },
        ['g'] = { ['a'] = L'が', ['i'] = L'ぎ', ['u'] = L'ぐ', ['e'] = L'げ', ['o'] = L'ご' },
        ['h'] = { ['a'] = L'は', ['i'] = L'ひ', ['u'] = L'ふ', ['e'] = L'へ', ['o'] = L'ほ' },
        ['b'] = { ['a'] = L'ば', ['i'] = L'び', ['u'] = L'ぶ', ['e'] = L'べ', ['o'] = L'ぼ' },
        ['p'] = { ['a'] = L'ぱ', ['i'] = L'ぴ', ['u'] = L'ぷ', ['e'] = L'ぺ', ['o'] = L'ぽ' },
        ['t'] = { ['a'] = L'た', ['i'] = L'ち', ['u'] = L'つ', ['e'] = L'て', ['o'] = L'と' },
        ['d'] = { ['a'] = L'だ', ['i'] = L'ぢ', ['u'] = L'づ', ['e'] = L'で', ['o'] = L'ど' },
        ['n'] = { ['a'] = L'な', ['i'] = L'に', ['u'] = L'ぬ', ['e'] = L'ね', ['o'] = L'の' },
        ['m'] = { ['a'] = L'ま', ['i'] = L'み', ['u'] = L'む', ['e'] = L'め', ['o'] = L'も' },
        ['s'] = { ['a'] = L'さ', ['i'] = L'し', ['u'] = L'す', ['e'] = L'せ', ['o'] = L'そ' },
        ['z'] = { ['a'] = L'ざ', ['i'] = L'じ', ['u'] = L'ず', ['e'] = L'ぜ', ['o'] = L'ぞ' },
        ['r'] = { ['a'] = L'ら', ['i'] = L'り', ['u'] = L'る', ['e'] = L'れ', ['o'] = L'ろ' }
    };

    static c32 YRow[] = { ['a'] = L'や', ['u'] = L'ゆ', ['o'] = L'よ' };
    static c32 SmallYRow[] = { ['a'] = L'ゃ', ['u'] = L'ゅ', ['o'] = L'ょ'};

    Foreach(CodepointIndex, Codepoints.Count)
    {
        c32 Codepoint = Codepoints.Data[CodepointIndex];
        if(Codepoint >= 'A' && Codepoint <= 'Z')
        {
            Codepoints.Data[CodepointIndex] += 'a' - 'A';
        }
    }

    Foreach(CodepointIndex, Codepoints.Count)
    {
        c32 Codepoint = Codepoints.Data[CodepointIndex];
        if((Codepoint >= 'A') && (Codepoint <= 'Z'))
        {
            Codepoint += 'a' - 'Z';
        }
#define InsertChar(Character) Result.Value.Data[Result.Value.Count++] = (Character)

#define GetCodepoint(CodepointName) \
                if((++CodepointIndex) >= Codepoints.Count) \
                { \
                    goto Invalid; \
                } \
                c32 CodepointName = Codepoints.Data[CodepointIndex];

#define GetRepeated(CodepointName) \
        c32 CodepointName; \
        do \
        { \
            if((++CodepointIndex) > Codepoints.Count) \
            { \
                goto Invalid; \
            } \
            CodepointName = Codepoints.Data[CodepointIndex]; \
            if(CodepointName == Codepoint) \
            { \
                InsertChar(L'っ'); \
            } \
        } while(CodepointName == Codepoint);

        switch(Codepoint)
        {
            case 'a':
            case 'i':
            case 'u':
            case 'e':
            case 'o':
            {
                InsertChar(ARow[Codepoint]);
            } break;
            case 's': 
            case 'k':
            case 'g':
            case 'h':
            case 'b':
            case 'p':
            case 't':
            case 'd':
            case 'm':
            case 'z':
            case 'r':
            {
                GetRepeated(SecondCodepoint);

                if(IsVowel(SecondCodepoint))
                {
                    InsertChar(TwoLetterSyllables[Codepoint][SecondCodepoint]);
                }
                else if(SecondCodepoint == 'y' || ((Codepoint == 's') && (SecondCodepoint == 'h')))
                {
                    GetCodepoint(ThirdCodepoint);
                    if(AUO(ThirdCodepoint))
                    {
                        InsertChar(TwoLetterSyllables[Codepoint]['i']);
                        InsertChar(SmallYRow[ThirdCodepoint]);
                    }
                    else
                    {
                        goto Invalid;
                    }
                }
                else if(Codepoint == 't' && SecondCodepoint == 's')
                {
                    GetCodepoint(ThirdCodepoint);
                    if(ThirdCodepoint == 'u')
                    {
                        InsertChar(L'つ');
                    }
                    else
                    {
                        goto Invalid;
                    }
                }
                else
                {
                    goto Invalid;
                }
            } break;
            case 'f':
            {
                GetRepeated(SecondCodepoint);
                if(SecondCodepoint == 'u')
                {
                    InsertChar(L'ふ');
                }
                else
                {
                    goto Invalid;
                }
            } break;
            case 'w':
            {
                GetRepeated(SecondCodepoint);
                if(SecondCodepoint == 'a')
                {
                    InsertChar(L'わ');
                }
                else if(SecondCodepoint == 'o')
                {
                    InsertChar(L'を');
                }
                else
                {
                    goto Invalid;
                }
            } break;
            case 'y':
            {
                GetRepeated(SecondCodepoint);

                if(AUO(SecondCodepoint))
                {
                    InsertChar(YRow[SecondCodepoint]);
                }
                else
                {
                    goto Invalid;
                }
            } break;
            case 'c':
            {
                GetRepeated(SecondCodepoint);
                if(SecondCodepoint == 'h') 
                {
                    GetCodepoint(ThirdCodepoint);
                    if(AUO(ThirdCodepoint))
                    {
                        InsertChar(L'ち');
                        InsertChar(SmallYRow[ThirdCodepoint]);
                    }
                    else if(ThirdCodepoint == 'i')
                    {
                        InsertChar(L'ち');
                    }
                    else
                    {
                        goto Invalid;
                    }
                }
                else
                {
                    goto Invalid;
                }
            } break;
            case 'j':
            {
                GetRepeated(SecondCodepoint);
                if(AUO(SecondCodepoint))
                {
                    InsertChar(L'じ');
                    InsertChar(SmallYRow[SecondCodepoint]);
                }
                else if(SecondCodepoint == 'i')
                {
                    InsertChar(L'じ');
                }
                else
                {
                    goto Invalid;
                }
            } break;
            case 'n':
            {
                if((++CodepointIndex) < Codepoints.Count)
                {
                    c32 SecondCodepoint = Codepoints.Data[CodepointIndex];

                    if(IsVowel(SecondCodepoint))
                    {
                        Result.Value.Data[Result.Value.Count++] = TwoLetterSyllables[Codepoint][SecondCodepoint];
                    }
                    else if(SecondCodepoint == 'y')
                    {
                        GetCodepoint(ThirdCodepoint);
                        if(AUO(ThirdCodepoint))
                        {
                            InsertChar(L'に');
                            InsertChar(SmallYRow[ThirdCodepoint]);
                        }
                    }
                    else if(SecondCodepoint == 'n')
                    {
                        InsertChar(L'ん');
                        --CodepointIndex;
                    }
                    else
                    {
                        InsertChar(L'ん');
                        --CodepointIndex;
                    }
                }
                else
                {
                    InsertChar(L'ん');
                }
            } break;
            default:
            {
Invalid:
                Result.IsValid = false;
                return Result;
            } break;
        }
    }
    return Result;
}

internal v2
GetCodepointsDim(stbtt_fontinfo *Font, f32 FontScale, codepoints Writing)
{
    v2 Result = {0};

    i32 Baseline = 0;
    i32 Left = 0;
    int GlyphIndex = 0,
        NextGlyphIndex = stbtt_FindGlyphIndex(Font, Writing.Data[0]);
    Foreach(CodepointIndex, Writing.Count)
    {
        GlyphIndex = NextGlyphIndex;
        NextGlyphIndex = stbtt_FindGlyphIndex(Font, Writing.Data[CodepointIndex + 1]);

        int X0, X1, Y0, Y1;
        stbtt_GetGlyphBitmapBox(Font, GlyphIndex, FontScale, FontScale, &X0, &Y0, &X1, &Y1);

        Result.y = Maximum(Result.y, Y1 - Y0);

        i32 Kerning = stbtt_GetGlyphKernAdvance(Font, GlyphIndex, NextGlyphIndex);
        i32 Advance, Lsb;
        stbtt_GetGlyphHMetrics(Font, GlyphIndex, &Advance, &Lsb);

        Left += FontScale * (Kerning + Advance);
    }
    Result.x = Left;
    return Result;
}

internal void
AddButton(kmap_state *State, button_callback *Callback, rect BoundingBox, button_data Data)
{
    button Button =
    {
        .Callback = Callback,
        .BoundingBox = BoundingBox,
        .Data = Data
    };
    DebugDrawPoint(Button.BoundingBox.BottomLeft, GREEN);
    DebugDrawPoint(Button.BoundingBox.TopRight, RED);
    State->Buttons[State->ButtonCount++] = Button;
}

// https://sites.google.com/site/murmurhash/
internal u32
MurmurHash2(const void *Key, int Len, u32 Seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	u32 m = 0x5bd1e995;
	i32 r = 24;

	// Initialize the hash to a 'random' value

	u32 h = Seed ^ Len;

	// Mix 4 bytes at a time into the hash

	u8 *Data = (u8 *)Key;

	while(Len >= 4)
	{
		u32 k = *(u32 *)Data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		Data += 4;
		Len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(Len)
	{
        case 3: h ^= Data[2] << 16; // fallthrough
        case 2: h ^= Data[1] << 8; // fallthrough
        case 1: h ^= Data[0];
                h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

internal void
ZoomIn(camera *Camera, v2 LastFrameMousePos, f32 dTime)
{
    f32 MaximumZoom = Camera->DefaultZoom * 3.f;
    f32 NewZoom = Camera->Zoom + Camera->Zoom * dTime * 2.f;
    // clamping is very important!
    if(NewZoom < MaximumZoom)
    {
        Camera->Zoom = NewZoom;
        v2 LastFrameMouseWorldPos = PixelToWorldSpace(LastFrameMousePos, *Camera);
        // TODO: a correct formula 
        // also, it doesn't feel right to put this into zoom out code, but maybe it's correct?
        Camera->Pos = V2Lerp(Camera->Pos, LastFrameMouseWorldPos, .05f);
        //v2 dCamera = V2Sub(LastFrameMouseWorldPos, Camera->Pos);
        //Camera->Pos = V2Add(Camera->Pos, V2TimesScalar(dCamera, Camera->Zoom));
    }
}

internal void
ZoomOut(camera *Camera, f32 dTime)
{
    f32 MinimumZoom = Camera->DefaultZoom * .5f;
    Camera->Zoom = Maximum(Camera->Zoom - Camera->Zoom * dTime * 2.f, MinimumZoom);
}

internal
BUTTON_CALLBACK(BtnMatch)
{
    if(IsRightClicked)
    {
        State->CurrentMatchOffset = Data.High;
    }
}

internal
BUTTON_CALLBACK(BtnKanji)
{
    if(IsRightClicked)
    {
        State->CurrentMatchOffset = 0;
        State->SelectedCodepoint = Data.High;
    }
}

// New interface design:
// screen is divided into 2 parts: filter and matches, like this:
// ----------------------------------------------
// |    音   訓   Meaning  Radicals       <) (> |
// | 人 にん ひと ...      人                   |
// |    じん ...                                |
// |    ...                                     |
// ----------------------------------------------
// | ...   |(also written as ...)               |
// | 人気  | にんげん                           |
// | 人間 <| 1. person                          |
// | 人口  | 2. human                           |
// | ...   |    ...                             |
// ----------------------------------------------
// Then, the matches themselves are presented as writing + dictionary info.
// The top-right arrows allow the user to go to his previous/next query.
//
// The next thing to do would be to allow different types of filters:
// radicals, parts of speech, bookmarks etc.

// giant TODO list:
//
// - figure out a better way to navigate the map.
//   Right now we can move around manually or to the words that are connected to the currently selected character,
//   but we want to give the user some kind of preview of some of the words connected to our word.
//   The problem is, there's too many words containing some of the more popular characters
//   (the biggest offender, 人, is a part of over 2900 words!)
//
// Architecture/bugs
// - more intuitive zoom behavior (right now zoom is not absolute, but dependent on the size of the texture atlas)
// - smooth zooming
// - it looks like we have a memory leak?
// - rework the printer
// - get rid of ConvertToCodepoints everywhere
// - get the background inflation logic out of the shader
// - try out the "occupied regions" idea for the ui
// - get rid of the GetRealWordBoundingBox related cruft
// - get rid of geometry shaders completely
// - get rid of uniform buffers since they bump our opengl requirements for no reason
// - allow user to see kanji connections while inside bookmark layers
// - cleanup of the pre-true-ui cruft
// - polish button system (i really should've called them active
// regions after all...)
//
// Features
// - try "real" chunks
// - other dictionary formats
// - jumping to words connected to the selected via mouse (if the word count is less than N)
// - ability to see info for multiple kanji
// - a way to present words with more than one writing
// - try out distance fields for fonts
// - draw pronounciations as furigana
// - jump history
// - multi-selection
// - more filtering options (filter by flags, regex)
// - multiple languages
// - upgrade the converter to detect which word info corresponds to which reading/meaning
RENDER_FRAME(RenderFrame)
{
    StartTimingFunc();
    kmap_state *State = (kmap_state *)Memory->Base;

    if(!State->Initialized)
    {
        StartTiming(StateInit);

        StartTiming(Tests);
        {
#define TestUtf8String(CString) \
            { \
                char TestString[] = (CString); \
                codepoints TestCodepoints = ConvertToCodepoints(TestString); \
                char Encoded[256] = {0}; \
                size_t EncodedSize = EncodeCodepointsToUtf8(TestCodepoints, Encoded, sizeof(Encoded)); \
                Assert(EncodedSize == sizeof(TestString)); \
                Foreach(CharacterIndex, EncodedSize) \
                { \
                    Assert(Encoded[CharacterIndex] == TestString[CharacterIndex]); \
                } \
            }
            TestUtf8String("123 abc");
            TestUtf8String("Р");
            TestUtf8String("Работа");
            TestUtf8String("令和");
            TestUtf8String("😂");
        }
        EndTiming(Tests);

        State->Arena = InitializeArena(Memory->Base + sizeof(kmap_state), Memory->Size - sizeof(kmap_state));
        State->FrameArena = Subarena(&State->Arena, Megabytes(1));
        State->WordArena = Subarena(&State->Arena, Megabytes(1));
        // set the platform functions
        Platform = Memory->Platform;

        StartTiming(InitOpenGL);

#if BUILD_INTERNAL
        LoadOpenGlExtensions();
        // debugging
        int ContextFlags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &ContextFlags);
        if(!(ContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT))
        {
            Die("Couldn't initialize debugging...\n");
        }
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLDebugCallback, 0);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
#endif

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_GEQUAL);
        glClearDepth(0.f);

        EndTiming(InitOpenGL);

        State->Database = Platform.LoadFile(DATABASE_PATH, true);
        kmdb_header *KmdbHeader = (kmdb_header *)State->Database.Data;

        Assert(KmdbHeader->Magic == KMDB_MAGIC);
        Assert(KmdbHeader->Version <= KMDB_CURRENT_VERSION);

        State->WordCount = KmdbHeader->WordCount;
        State->KanjiHashtable.KanjiCount = KmdbHeader->KanjiCount;
        State->KanjiHashtable.KanjiInfos = (kanji_info *)(State->Database.Data + KmdbHeader->KanjiHashtableOffset);
        State->KanjiHashtable.Data = State->Database.Data + KmdbHeader->KanjiDataOffset;

        kmdb_word_description *WordDescriptions = (kmdb_word_description *)(State->Database.Data + KmdbHeader->WordDescriptionsOffset);
        char *WordData = (char *)(State->Database.Data + KmdbHeader->WordDataOffset);
        State->Words = PushArray(&State->Arena, word, State->WordCount);

        Foreach(WordIndex, State->WordCount)
        {
            kmdb_word_description *WordDescription = WordDescriptions + WordIndex;
            word *Word = State->Words + WordIndex;
            Word->Writing = WordData + WordDescription->WritingOffset;

            Word->ReadingCount = WordDescription->ReadingCount;
            Word->Readings = WordData + WordDescription->ReadingOffset;

            Word->MeaningCount = WordDescription->MeaningCount;
            Word->Meanings = WordData + WordDescription->MeaningOffset;

            Word->FlagCount = WordDescription->FlagCount;
            Word->Flags = (u8 *)(WordData + WordDescription->FlagOffset);
        }

        // create shader programs
        StartTiming(InitShaders);
#if BUILD_INTERNAL
        Foreach(ShaderProgramIndex, ArrayCount(ShaderPrograms))
        {
            shader_program_description *Program = ShaderPrograms + ShaderProgramIndex;
            DebugCreateShaderProgram(Program);
        }
#else
        // for the release, our shaders are stored in the same database as everything else
        // and stored in the same order as in the descriptions
        char *ShaderFileData = (char *)State->Database.Data + KmdbHeader->ShaderDataOffset;
        Foreach(ShaderProgramIndex, SP_Count)
        {
            char *VertexData = ShaderFileData;
            while(*ShaderFileData++) {}
            char *FragmentData = ShaderFileData;
            while(*ShaderFileData++) {}
            char *GeometryData = 0;
            if(ShaderPrograms[ShaderProgramIndex].GeometryPath)
            {
                GeometryData = ShaderFileData;
                while(*ShaderFileData++) {}
            }

            GLuint ShaderProgram = glCreateProgram();
            GLuint VertexShader = ReleaseLoadShader(VertexData, GL_VERTEX_SHADER);
            glAttachShader(ShaderProgram, VertexShader);
            GLuint FragmentShader = ReleaseLoadShader(FragmentData, GL_FRAGMENT_SHADER);
            glAttachShader(ShaderProgram, FragmentShader);
            
            GLuint GeometryShader = 0;
            if(GeometryData)
            {
                GeometryShader = ReleaseLoadShader(GeometryData, GL_GEOMETRY_SHADER);
                glAttachShader(ShaderProgram, GeometryShader);
            }

            glLinkProgram(ShaderProgram);
            b32 LinkSuccess = true;
            glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &LinkSuccess);
            Assert(LinkSuccess);
            ShaderPrograms[ShaderProgramIndex].Handle = ShaderProgram;
            glDeleteShader(VertexShader);
            glDeleteShader(FragmentShader);
            if(GeometryData)
            {
                glDeleteShader(GeometryShader);
            }
        }
#endif
        EndTiming(InitShaders);

        StartTiming(ConfiguringShaders);

        glGenBuffers(1, &State->ThemeUBO);
        State->UsingLightTheme = Memory->ShouldUseLightTheme;
        ChangeTheme(State, State->UsingLightTheme ? LightTheme : DarkTheme);

        // the default zoom in firefox seems more like 30 px, but i like 40 better
        f32 DefaultHeightAt1Zoom = 40.f;
        State->Camera.DefaultZoom = DefaultHeightAt1Zoom / (f32)TACellSide;
        State->Camera.Zoom = State->Camera.DefaultZoom;

        EndTiming(ConfiguringShaders);

        // this fixes the tilting issue
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // NOTE on world space:
        // everything is in pixels
        // goes from -n to n in both x and y,
        // 0 is the center,
        // x goes from left to right, y - from bottom to top
        // dimensions of the camera are just window dimensions
        // camera's zoom goes from 0 to m,
        // where larger values mean that the image is more zoomed in
        //
        // We do most everything in pixel space instead, the world space is only for storage
        State->FontFile = Platform.LoadFile(Memory->FontPath, true);
        stbtt_InitFont(&State->Font, State->FontFile.Data, stbtt_GetFontOffsetForIndex(State->FontFile.Data, 0));
        State->LargestFontScale = stbtt_ScaleForPixelHeight(&State->Font, TACellSide);
        State->UiTextScale = stbtt_ScaleForPixelHeight(&State->Font, State->Theme.UiFontPixelHeight);

        StartTiming(LoadingDatabase);

        i32 FontPathLength = 0;
        for(char *C = Memory->FontPath; *C; ++C, ++FontPathLength);

        u32 FontNameHash = MurmurHash2(Memory->FontPath, FontPathLength, 0xfeed5eed);
        State->MainLayerIndex = 0;
        State->SecondaryLayerIndex = 1;
        load_file_result ReferenceCache = Platform.LoadFile(Memory->CachePath, false);
        if(ReferenceCache.Data)
        {
            // validate the cache
            reference_cache_header *Header = (reference_cache_header *)ReferenceCache.Data;
            if((Header->Magic != REFERENCE_CACHE_MAGIC) ||
               (Header->Version != REFERENCE_CACHE_VERSION) ||
               (Header->WordCount != State->WordCount) ||
               (Header->FontNameHash != FontNameHash))
            {
                eprintf("Reference cache is invalid and needs to be recreated:\n"
                        "\t.Magic = %d (expected %d),\n"
                        "\t.Version = %d (expected %d),\n"
                        "\t.WordCount = %d (expected %d)\n"
                        "\t.FontNameHash = %d(expected %d)\n", 
                        Header->Magic, REFERENCE_CACHE_MAGIC,
                        Header->Version, REFERENCE_CACHE_VERSION,
                        Header->WordCount, State->WordCount,
                        Header->FontNameHash, FontNameHash);
                goto GenerateCache;
            }
            else
            {
                State->ReferenceHeader = (reference_cache_header *)PushSize(&State->Arena, ReferenceCache.Size);
                Memcpy(State->ReferenceHeader, Header, ReferenceCache.Size);
                State->ReferenceData = (word_reference *)(State->ReferenceHeader + 1);
                Platform.UnloadFile(ReferenceCache);
            }
        }
        else
        {
GenerateCache:
            // generate the cache
            size_t CacheSize = sizeof(reference_cache_header) + sizeof(rect)*State->WordCount*ReferenceLayerCount;
            State->ReferenceHeader = PushStruct(&State->Arena, reference_cache_header);
            *State->ReferenceHeader = (reference_cache_header)
            {
                .Magic = REFERENCE_CACHE_MAGIC,
                .Version = REFERENCE_CACHE_VERSION,
                .WordCount = State->WordCount,
                .FontNameHash = FontNameHash,
            };
            State->ReferenceData = PushArray(&State->Arena, word_reference, State->WordCount * ReferenceLayerCount);
            u64 InitialRngSeed = 17298136516911894399u;
            Foreach(LayerIndex, ReferenceLayerCount)
            {
                reference_layer *Layer = State->ReferenceHeader->Layers + LayerIndex;
                Layer->CameraPos = V2(0.f, 0.f);
                Layer->Rng = InitialRngSeed + LayerIndex;
            }
            reference_layer *FirstLayer = State->ReferenceHeader->Layers;
            FirstLayer->ReferenceCount = State->WordCount;
            u32 ChunkCount = State->WordCount / WordsPerChunk;
            i32 ChunkTotalRows = (i32)Sqrt((f32)ChunkCount);
            // setup the main layer's positions
            // TODO: proper thing here  
            rect OtherWordRects[WordsPerChunk] = {0};
            Foreach(WordIndex, State->WordCount)
            {
                word *Word = State->Words + WordIndex;
                word_reference *Reference = State->ReferenceData + WordIndex; 
                u32 ChunkIndex = WordIndex / WordsPerChunk;
                u32 WordChunkRelativeIndex = WordIndex % WordsPerChunk;
                v2 WordDim = GetCodepointsDim(&State->Font, State->LargestFontScale, ConvertToCodepoints(Word->Writing));
                rect BoundingBox = GetNewWordBoundingBox(&FirstLayer->Rng, ChunkTotalRows, ChunkIndex, WordDim, WordChunkRelativeIndex, OtherWordRects);
                Reference->BoundingBox = BoundingBox;
                Reference->WordIndex = WordIndex;
                OtherWordRects[WordChunkRelativeIndex] = Reference->BoundingBox;
            }

            Memset(State->ReferenceData + State->WordCount, 0, sizeof(word_reference)*State->WordCount*(ReferenceLayerCount - 1));
            Platform.WriteFile(Memory->CachePath, CacheSize, State->ReferenceHeader);
        }
        EndTiming(LoadingDatabase);


        // initialize camera uniform buffer
        glGenBuffers(1, &State->CameraUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, State->CameraUBO);
        {
            glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), 0, GL_STREAM_DRAW);
        }

        // initiallize debug queue
        glGenVertexArrays(1, &GlobalDebugQueue.VAO);
        glGenBuffers(1, &GlobalDebugQueue.VBO);
        glBindVertexArray(GlobalDebugQueue.VAO);
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, State->CameraUBO);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, State->ThemeUBO);
            glBindBuffer(GL_ARRAY_BUFFER, GlobalDebugQueue.VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GlobalDebugQueue.Squares), 0, GL_STREAM_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(square), (GLvoid *)offsetof(square, Coordinates));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(square), (GLvoid *)offsetof(square, Size));
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(square), (GLvoid *)offsetof(square, Color));

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
        }

        // initialize line pipeline
        glGenVertexArrays(1, &State->LineVAO);
        glGenBuffers(1, &State->LineVBO);

        glBindVertexArray(State->LineVAO);
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, State->CameraUBO);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, State->ThemeUBO);
            glBindBuffer(GL_ARRAY_BUFFER, State->LineVBO);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(line_data), (GLvoid *)offsetof(line_data, KanjiPosition));
            glEnableVertexAttribArray(0);
            glVertexAttribDivisor(0, 1);
        }

        StartTiming(ConfiguringFonts);

        State->TA.Cells = PushArray(&State->Arena, ta_cell, TACellCount);
        Memset(State->TA.Cells, 0, sizeof(ta_cell)*TACellCount);

        // texture atlas
        {
            glGenTextures(1, &State->TA.Texture);
            glBindTexture(GL_TEXTURE_2D, State->TA.Texture);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, TASide, TASide);

            StartTiming(SettingUpGlyphs);
            // setup characters used for the interface rendering
            {
                u32 CellIndex = 0;
                for(u32 Codepoint = ' ';
                    Codepoint <= '~';
                    ++Codepoint)
                {
                    ta_cell *Cell = State->TA.Cells + CellIndex;
                    SetupGlyph(&State->Font, State->UiTextScale, &State->Arena, CellIndex, Codepoint, Cell);
                    ++CellIndex;
                }

                for(u32 Codepoint = L'ぁ';
                    Codepoint <= L'ヽ';
                    ++Codepoint)
                {
                    ta_cell *Cell = State->TA.Cells + CellIndex;
                    SetupGlyph(&State->Font, State->UiTextScale, &State->Arena, CellIndex, Codepoint, Cell);
                    ++CellIndex;
                }

                Assert(CellIndex == UiCharacterCount);
            }
            EndTiming(SettingUpGlyphs);
        }
        EndTiming(ConfiguringFonts);

        glGenVertexArrays(1, &State->TextBackgroundVAO);
        glBindVertexArray(State->TextBackgroundVAO);
        {
            glGenBuffers(1, &State->TextBackgroundVBO);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, State->CameraUBO);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, State->ThemeUBO);
            glBindBuffer(GL_ARRAY_BUFFER, State->TextBackgroundVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(text_background)*MaxCharactersInFrame, 0, GL_STREAM_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(text_background), (GLvoid *)offsetof(text_background, Rect.BottomLeft));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(text_background), (GLvoid *)offsetof(text_background, Rect.TopRight));
            glVertexAttribIPointer(2, 1, GL_INT, sizeof(text_background), (GLvoid *)offsetof(text_background, ColorType));
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(text_background), (GLvoid *)offsetof(text_background, Depth));

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);

            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
            glVertexAttribDivisor(3, 1);
        }

        // initialize the text pipeline
        glGenVertexArrays(1, &State->TextVAO);
        glGenBuffers(1, &State->TextVBO);
        glBindVertexArray(State->TextVAO);
        {
            glBindTexture(GL_TEXTURE_2D, State->TA.Texture);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, State->CameraUBO);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, State->ThemeUBO);
            glBindBuffer(GL_ARRAY_BUFFER, State->TextVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(ui_text_display_data)*MaxCharactersInFrame, 0, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ui_text_display_data), (GLvoid *)offsetof(ui_text_display_data, TextureTopLeft));
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ui_text_display_data), (GLvoid *)offsetof(ui_text_display_data, TextureBottomRight));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ui_text_display_data), (GLvoid *)offsetof(ui_text_display_data, ScreenBottomLeft));
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(ui_text_display_data), (GLvoid *)offsetof(ui_text_display_data, ScreenTopRight));
            glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(ui_text_display_data), (GLvoid *)offsetof(ui_text_display_data, ColorType));
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(ui_text_display_data), (GLvoid *)offsetof(ui_text_display_data, Depth));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
            glEnableVertexAttribArray(5);

            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
            glVertexAttribDivisor(2, 1);
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
        }

#if USE_BACKGROUND_SHADER
        glGenVertexArrays(1, &State->BackgroundVAO);
        glBindVertexArray(State->BackgroundVAO);
        {
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, State->CameraUBO);
            glGenBuffers(1, &State->BackgroundVBO);
            glBindBuffer(GL_ARRAY_BUFFER, State->BackgroundVBO);
            u32 MaxVisibleWords = WordsPerChunk * 9;
            glBufferData(GL_ARRAY_BUFFER, sizeof(background_display_data)*MaxVisibleWords, 0, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(background_display_data), (GLvoid *)offsetof(background_display_data, Pos));
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(background_display_data), (GLvoid *)offsetof(background_display_data, Color));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribDivisor(0, 1);
            glVertexAttribDivisor(1, 1);
        }
#endif

        State->ShouldShowKanjiOverlay = true;
        if(!IsKanji(State->SelectedCodepoint))
        {
            State->SelectedCodepoint = L'人';
        }

        //
        State->Initialized = true;
        EndTiming(StateInit);
    }

    // frame start
    memory_arena *FrameArena = &State->FrameArena;
    mouse *Mouse = &Input->Mouse;
    v2 LastFrameMousePos = State->LastFrameMousePos;
    v2 MousePos = Mouse->Coordinates;
    f32 dTime = Input->Time - State->LastFrameTime;

    b32 BookmarksChanged = false;

    if(State->Camera.Dim.x != WindowDim.x ||
       State->Camera.Dim.y != WindowDim.y)
    {
        State->Camera.Dim = WindowDim;
        glViewport(0, 0, WindowDim.x, WindowDim.y);
    }
#define PressedNotHeld(Key) ((Input->PressedKeys & (Key)) && !(PreviousInput->PressedKeys & (Key)))

    // input handling
    StartTiming(InputHandling);
    EndTiming(InputHandling);

    // rendering
    StartTiming(Rendering);

    text_render_queue RenderQueue =
    {
        .MaxCharacterCount = MaxCharactersInFrame,
        .MaxBackgroundCount = MaxCharactersInFrame,
        .CharacterCount = 0,
        .BackgroundCount = 0,
        .Characters = PushArray(FrameArena, ui_text_display_data, MaxCharactersInFrame),
        .Backgrounds = PushArray(FrameArena, text_background, MaxCharactersInFrame)
    };
    int Ascent, Descent, LineGap;
    stbtt_GetFontVMetrics(&State->Font, &Ascent, &Descent, &LineGap);

    f32 UiLineHeight = State->UiTextScale * (Ascent - Descent + LineGap);
    // drawing the filter
    c32 SelectedKanji = State->SelectedCodepoint;
    kanji_data SelectedKanjiData = GetKanjiData(&State->KanjiHashtable, SelectedKanji);
    f32 TextPadding = 25.f;
    f32 BigKanjiWidth = 100.f; 
    f32 BigKanjiPadding = 20.f;
    f32 FilterWindowBottom = State->Camera.Dim.y - BigKanjiWidth - BigKanjiPadding * 2.f;
    {
        v2 FirstColumnTop = V2(BigKanjiWidth + 2.f * BigKanjiPadding, State->Camera.Dim.y - TextPadding);
        text_printer Printer = TextPrinter(State, &RenderQueue, FirstColumnTop, UiLineHeight, State->UiTextScale, -1, -1);
        if(SelectedKanjiData.MeaningCount)
        {
            PrintTextLn(&Printer, "Meaning", CT_Highlight);
            char *Meaning = SelectedKanjiData.Meanings;
            Foreach(MeaningIndex, SelectedKanjiData.MeaningCount)
            {
                PrintTextLn(&Printer, Meaning, CT_Text);
                while(*Meaning)
                {
                    ++Meaning;
                }
                ++Meaning;
            }

            FilterWindowBottom = Minimum(FilterWindowBottom, Printer.BoundingBox.BottomLeft.y);
            PrintNewColumn(&Printer);
        }

        if(SelectedKanjiData.OnReadingCount)
        {
            PrintTextLn(&Printer, "音", CT_Highlight);
            char *OnReading = SelectedKanjiData.OnReadings;
            Foreach(OnReadingIndex, SelectedKanjiData.OnReadingCount)
            {
                PrintTextLn(&Printer, OnReading, CT_Text);
                while(*OnReading)
                {
                    ++OnReading;
                }
                ++OnReading;
            }

            FilterWindowBottom = Minimum(FilterWindowBottom, Printer.BoundingBox.BottomLeft.y);
            PrintNewColumn(&Printer);
        }

        if(SelectedKanjiData.KunReadingCount)
        {
            PrintTextLn(&Printer, "訓", CT_Highlight);
            char *KunReading = SelectedKanjiData.KunReadings;
            Foreach(KunReadingIndex, SelectedKanjiData.KunReadingCount)
            {
                PrintTextLn(&Printer, KunReading, CT_Text);
                while(*KunReading)
                {
                    ++KunReading;
                }
                ++KunReading;
            }
            FilterWindowBottom = Minimum(FilterWindowBottom, Printer.BoundingBox.BottomLeft.y);
            PrintNewColumn(&Printer);
        }

        if(SelectedKanjiData.RadicalCount)
        {
            PrintTextLn(&Printer, "Radicals", CT_Highlight);
            c32 Space = ' ';
            Foreach(RadicalIndex, SelectedKanjiData.RadicalCount)
            {
                PrintCodepoints(&Printer, 1, SelectedKanjiData.Radicals + RadicalIndex, CT_Text);
                PrintCodepoints(&Printer, 1, &Space, CT_Text);
            }
            FilterWindowBottom = Minimum(FilterWindowBottom, Printer.BoundingBox.BottomLeft.y);
            PrintNewColumn(&Printer);
        }

        if(SelectedKanjiData.SimilarKanjiCount)
        {
            u32 SimilarKanjiPerLine = 10;
            PrintTextLn(&Printer, "Similar kanji", CT_Highlight);
            c32 Space = ' ';
            Foreach(SimilarKanjiIndex, SelectedKanjiData.SimilarKanjiCount)
            {
                c32 SimilarKanji = SelectedKanjiData.SimilarKanji[SimilarKanjiIndex];
                rect SimilarKanjiRect = PrintCodepoints(&Printer, 1, &SimilarKanji, CT_Text);
                PrintCodepoints(&Printer, 1, &Space, CT_Text);
                if(((SimilarKanjiIndex + 1) % (SimilarKanjiPerLine + 1)) == 0)
                {
                    PrintNewline(&Printer);
                }

                AddButton(State, BtnKanji, SimilarKanjiRect, (button_data){.High = SimilarKanji});
            }
            FilterWindowBottom = Minimum(FilterWindowBottom, Printer.BoundingBox.BottomLeft.y);
            //PrintNewColumn(&Printer);
        }

        //  |p
        //  |o
        // s|k
        //  |o
        //  |p
        //  h
        // s = k + 2o + 2p,
        // o = (s - k - 2p)/2
        // o + p = (s - k)/2
        f32 FilterWindowHeight = State->Camera.Dim.y - FilterWindowBottom;
        v2 BigKanjiBottomLeft = V2(BigKanjiPadding,
                                   FilterWindowBottom + (FilterWindowHeight - BigKanjiWidth) / 2.f);
        PushSingleColoredLine(State, &RenderQueue, BigKanjiBottomLeft, stbtt_ScaleForPixelHeight(&State->Font, BigKanjiWidth), 1, &SelectedKanji, CT_Text);
    }

    // drawing the matches
    {
        f32 MatchWindowHeight = FilterWindowBottom;
        f32 MatchWindowTop = FilterWindowBottom - UiLineHeight;
        u32 MaxNumberOfMatchesToDisplay = (u32)(MatchWindowHeight / UiLineHeight);
        u32 DisplayedMatchCount = Minimum(MaxNumberOfMatchesToDisplay,
                                          SelectedKanjiData.WordCount);
        text_printer Printer = TextPrinter(State, &RenderQueue, V2(TextPadding, MatchWindowTop), UiLineHeight, State->UiTextScale, 40, MaxNumberOfMatchesToDisplay);
        u32 CurrentWordOffset = State->CurrentMatchOffset;
        // this is incorrect! too many matches
        for(u32 WordOffset = CurrentWordOffset;
            WordOffset < CurrentWordOffset + DisplayedMatchCount;
            ++WordOffset)
        {
            u32 WordIndex = SelectedKanjiData.WordIndices[WordOffset % SelectedKanjiData.WordCount];
            word *Word = State->Words + WordIndex;
            rect WordBoundingBox = PrintTextLn(&Printer, Word->Writing, WordOffset == CurrentWordOffset ? CT_Highlight : CT_Text);
            AddButton(State, BtnMatch, WordBoundingBox, (button_data){.High = WordOffset});
        }
        // TODO: add scrolling area here
        SetPrinterPos(&Printer, V2(Printer.BoundingBox.TopRight.x + TextPadding, Printer.FirstLineBottomLeft.y));
        // print match data
        word *Match = State->Words + SelectedKanjiData.WordIndices[CurrentWordOffset];
        char CountDigits[8] = {0};
        char *NumberDelimiter = ". ";
        PrintTextLn(&Printer, "Kanji:", CT_Highlight);
        codepoints WritingCodepoints = ConvertToCodepoints(Match->Writing);
        Foreach(CodepointIndex, WritingCodepoints.Count)
        {
            c32 Codepoint = WritingCodepoints.Data[CodepointIndex];
            if(IsKanji(Codepoint))
            {
                rect KanjiRect = PrintCodepoints(&Printer, 1, &Codepoint, Codepoint == SelectedKanji ? CT_Highlight : CT_Text);
                AddButton(State, BtnKanji, KanjiRect, (button_data){.High = Codepoint});
                c32 Space = ' ';
                PrintCodepoints(&Printer, 1, &Space, CT_Text);
            }
        }
        PrintNewline(&Printer);
        if(Match->ReadingCount)
        {
            PrintTextLn(&Printer, "Readings:", CT_Highlight);
            char *Reading = Match->Readings;
            Foreach(ReadingIndex, Match->ReadingCount)
            {
                if(Match->ReadingCount > 1)
                {
                    U32ToString(ReadingIndex + 1, CountDigits);
                    PrintText(&Printer, CountDigits, CT_Text);
                    PrintText(&Printer, NumberDelimiter, CT_Text);
                }
                PrintTextLn(&Printer, Reading, CT_Text);
                while(*Reading)
                {
                    ++Reading;
                }
                ++Reading;
            }
        }

        if(Match->MeaningCount)
        {
            PrintTextLn(&Printer, "Meanings:", CT_Highlight);
            char *Meaning = Match->Meanings;
            Foreach(MeaningIndex, Match->MeaningCount)
            {
                if(Match->MeaningCount > 1)
                {
                    U32ToString(MeaningIndex + 1, CountDigits);
                    PrintText(&Printer, CountDigits, CT_Text);
                    PrintText(&Printer, NumberDelimiter, CT_Text);
                }
                PrintTextLn(&Printer, Meaning, CT_Text);
                while(*Meaning)
                {
                    ++Meaning;
                }
                ++Meaning;
            }
        }
    }

    // handle mouse buttons
    Foreach(ButtonIndex, State->ButtonCount)
    {
        button *Button = State->Buttons + ButtonIndex;
        b32 Hovered = 
            ((MousePos.x >= Button->BoundingBox.BottomLeft.x) && (MousePos.x <= Button->BoundingBox.TopRight.x) &&
             (MousePos.y >= Button->BoundingBox.BottomLeft.y) && (MousePos.y <= Button->BoundingBox.TopRight.y));
        if(Hovered)
        {
            b32 IsRightClicked = (Mouse->Buttons & MB_Right) && !(PreviousInput->Mouse.Buttons & MB_Right);
            b32 IsMiddleClicked = (Mouse->Buttons & MB_Middle) && !(PreviousInput->Mouse.Buttons & MB_Middle);
            Button->Callback(State, IsRightClicked, IsMiddleClicked, Button->Data);
        }
    }

    StartTiming(RenderPasses);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_UNIFORM_BUFFER, State->CameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(camera), &State->Camera);
    if(RenderQueue.BackgroundCount)
    {
        glBindVertexArray(State->TextBackgroundVAO);
        glBindBuffer(GL_ARRAY_BUFFER, State->TextBackgroundVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(text_background)*RenderQueue.BackgroundCount, RenderQueue.Backgrounds);
        UseShader(SP_TextBackground);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, RenderQueue.BackgroundCount);
    }
    if(RenderQueue.CharacterCount)
    {
        glBindVertexArray(State->TextVAO);
        glBindBuffer(GL_ARRAY_BUFFER, State->TextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ui_text_display_data)*RenderQueue.CharacterCount, RenderQueue.Characters);
        UseShader(SP_Text);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, RenderQueue.CharacterCount);
    }

#if BUILD_INTERNAL
    if(GlobalDebugQueue.SquareCount)
    {
        UseShader(SP_Debug);
        glBindVertexArray(GlobalDebugQueue.VAO);
        {
            glBindBuffer(GL_ARRAY_BUFFER, GlobalDebugQueue.VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(square)*GlobalDebugQueue.SquareCount, GlobalDebugQueue.Squares);

            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, GlobalDebugQueue.SquareCount);
        }
    }
#endif
    EndTiming(RenderPasses);

    EndTiming(Rendering);

    // frame end
    StartTiming(ResetLocking);
    for(u32 CellIndex = UiCharacterCount;
        CellIndex < TACellCount;
        ++CellIndex)
    {
        ta_cell *Cell = State->TA.Cells + CellIndex;
        Cell->IsLocked = false;
    }
    EndTiming(ResetLocking);

    if(BookmarksChanged)
    {
        size_t CacheSize = sizeof(reference_cache_header) + sizeof(rect)*State->WordCount*ReferenceLayerCount;
        Platform.WriteFile(Memory->CachePath, CacheSize, State->ReferenceHeader);
    }

    State->ButtonCount = 0;
    State->LastFrameTime = Input->Time;
    State->LastFrameMousePos = MousePos;
    GlobalDebugQueue.SquareCount = 0;
    ResetArena(FrameArena);

    u32 TimingCount = GlobalDebugTimingCount;
#if DEBUG_INFO
    StartTiming(CopyingPreviousTimings);
    Foreach(TimingIndex, TimingCount)
    {
        PreviousFrameTimings[TimingIndex] = GlobalTimings[TimingIndex];
        GlobalTimings[TimingIndex] = (timing){0};
    }
    EndTiming(CopyingPreviousTimings);
#endif
    EndTimingFunc();
}

// IMPORTANT NOTE: do not put anything past this line
u32 GlobalDebugTimingCount = __COUNTER__;
