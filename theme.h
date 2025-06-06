#ifndef CONFIG_H
#define CONFIG_H

// NOTE: no capital letters!
#define HexColor(Hex) (v4) { \
    .r = (f32)((Hex) >> 16) / 255.f, \
    .g = (f32)(((Hex) >> 8) & 0xff) / 255.f, \
    .b = (f32)((Hex) & 0xff) / 255.f, \
    .a = 1.f \
}

global theme LightTheme =
{
    .Colors =
    {
        [CT_Text] = HexColor(0x1c1c1c),
        [CT_Highlight] = HexColor(0x5f0000),
        [CT_Background] = HexColor(0xdfdfaf),
        [CT_Line1] = HexColor(0x3a3a3a),
        [CT_Line2] = HexColor(0x3a3a3a),
    },
    .UiFontPixelHeight = 26.f,
    .LineWidth = 2.f,
    .DictionaryEntryPadding = 10.f,
    .UiBorderWidth = 2.f
};

global theme DarkTheme =
{
    .Colors =
    {
        [CT_Text] = HexColor(0xdfdfaf),
        [CT_Highlight] = HexColor(0xffdf87),
        [CT_Background] = HexColor(0x1c1c1c),
        [CT_Line1] = HexColor(0xdfdfaf),
        [CT_Line2] = HexColor(0xffdf87),
    },
    .UiFontPixelHeight = 26.f,
    .LineWidth = 2.f,
    .DictionaryEntryPadding = 10.f,
    .UiBorderWidth = 2.f
};

#endif // CONFIG_H
