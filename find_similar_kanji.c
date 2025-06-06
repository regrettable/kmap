#include "find_similar_kanji.h"

global u32 GlobalJapaneseFontCount = 0;
global char *GlobalJapaneseFontPaths[512];

static void
GetJapaneseFontsRecursively(char *DirectoryPath)
{
    DIR *Directory = opendir(DirectoryPath);
    chdir(DirectoryPath);
    char *DirName = get_current_dir_name();
    size_t DirNameLength = strlen(DirName);
    for(struct dirent *Entry = readdir(Directory);
        Entry;
        Entry = readdir(Directory))
    {
        int FilePathLen = strlen(Entry->d_name);
        char *FilePath = Entry->d_name;
        if(Entry->d_type == DT_REG)
        {
            // check if it's a font we can open
            if((FilePathLen > 4) &&
               (FilePath[FilePathLen - 4] == '.') &&
               ((FilePath[FilePathLen - 3] == 't') || (FilePath[FilePathLen - 3] == 'o')) &&
               (FilePath[FilePathLen - 2] == 't') &&
               ((FilePath[FilePathLen - 1] == 'f') || (FilePath[FilePathLen - 1] == 'c')))
            {
                int FontHandle = open(FilePath, O_RDONLY);
                struct stat StatBuffer;
                if(fstat(FontHandle, &StatBuffer) == -1)
                {
                    ErrorOut(fstat);
                }
                size_t FontSize = StatBuffer.st_size;
                unsigned char *FontData = mmap(0, FontSize, PROT_READ, MAP_PRIVATE, FontHandle, 0);
                if(FontData != MAP_FAILED)
                {
                    stbtt_fontinfo Info;
                    int FontOffset = stbtt_GetFontOffsetForIndex(FontData, 0);
                    if(FontOffset != -1)
                    {
                        int InitSuccess = stbtt_InitFont(&Info, FontData, FontOffset);
                        if(InitSuccess)
                        {
                            // NOTE: use kokuji to differenciate between japanese and non-japanese fonts
                            c32 TestCodepoints[] = {L'込', L'峠'};
                            b32 AllTestCodepointsPresent = 1;
                            for(u32 Codepointi = 0; Codepointi < ArrayCount(TestCodepoints); ++Codepointi)
                            {
                                int Index = stbtt_FindGlyphIndex(&Info, TestCodepoints[Codepointi]);
                                AllTestCodepointsPresent = AllTestCodepointsPresent && (Index != 0);
                            }

                            if(AllTestCodepointsPresent)
                            {
                                char *Path = malloc(DirNameLength + FilePathLen + 2);
                                memcpy(Path, DirName, DirNameLength);
                                Path[DirNameLength] = '/';
                                memcpy(Path + DirNameLength + 1, FilePath, FilePathLen);
                                Path[DirNameLength + FilePathLen + 1] = 0;
                                GlobalJapaneseFontPaths[GlobalJapaneseFontCount++] = Path;
                            }
                        }
                    }
                    munmap(FontData, FontSize);
                }
                else
                {
                    ErrorOut(mmap);
                }
                close(FontHandle);
            }
        }
        else if(Entry->d_type == DT_DIR)
        {
            if(((FilePathLen == 1) && (FilePath[0] == '.')) ||
               ((FilePathLen == 2) && (FilePath[0] == '.') && (FilePath[1] == '.')))
            {
                // skip
            }
            else
            {
                // recursively open the directory
                GetJapaneseFontsRecursively(FilePath);
            }
        }
    }
    closedir(Directory);
    free(DirName);
    chdir("..");
}

int 
main(int Argc, char **Argv)
{
    GetJapaneseFontsRecursively("/usr/share/fonts");
    // TODO: allocate a grid of n by n pixels for each of the characters we care about.
    //
    for(u32 Pathi = 0; Pathi < GlobalJapaneseFontCount; ++Pathi)
    {
        printf("%s\n", GlobalJapaneseFontPaths[Pathi]);
    }
}
