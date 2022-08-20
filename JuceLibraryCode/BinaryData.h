/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   DSEG7ClassicBold_ttf;
    const int            DSEG7ClassicBold_ttfSize = 23040;

    extern const char*   fileline_png;
    const int            fileline_pngSize = 906;

    extern const char*   filelistline_png;
    const int            filelistline_pngSize = 788;

    extern const char*   folderline_png;
    const int            folderline_pngSize = 782;

    extern const char*   layoutgridline_png;
    const int            layoutgridline_pngSize = 781;

    extern const char*   pauseline_png;
    const int            pauseline_pngSize = 551;

    extern const char*   playline_png;
    const int            playline_pngSize = 982;

    extern const char*   skipbackline_png;
    const int            skipbackline_pngSize = 931;

    extern const char*   skipforwardline_png;
    const int            skipforwardline_pngSize = 917;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 9;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
