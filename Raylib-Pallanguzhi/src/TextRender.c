#include "TextRender.h"
#include "TamilText.h"
#include "LanguageSelection.h"
#include "raylib.h"
#include <stdlib.h>

static Font englishFont;
static int fontSize = 40; // default font size // This cannot be changed afterwards for now

void InitText()
{
    InitComplexText("Kavivanar-Regular.ttf",fontSize);

    //Load English Font
    englishFont = (LoadFontEx("TASAExplorer-Regular.ttf", fontSize, NULL,0)); 
}

void DeInitText()
{
    UnloadComplexText();
    UnloadFont(englishFont);
}

void RenderText(TextID textID, Vector2 position, Color color)
{
    const char* text = GetText(textID);
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            DrawTextEx(englishFont, text, position, fontSize, 4, color);
            break;
        case Tamil:
            DrawTamilText(text,position,color);
            break;
    }
}

void RenderTextDirect(const char* text, Vector2 position, Color color)
{
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            DrawTextEx(englishFont, text, position, fontSize, 4, color);
            break;
        case Tamil:
            DrawTamilText(text,position,color);
            break;
    }
}

void RenderEnglishText(const char* text, Vector2 position, Color color)
{
    DrawTextEx(englishFont, text, position, fontSize, 4, color);
}

void RenderTamilText(const char* text, Vector2 position, Color color)
{
    DrawTamilText(text,position,color);
}

Vector2 MeasureTextDirect(const char* text)
{
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            return MeasureTextEx(englishFont,text,fontSize,0); // setting spacing to zero 
        case Tamil:
            return MeasureTamilText(text);
    }
    return (Vector2) { 0, 0 };
}
