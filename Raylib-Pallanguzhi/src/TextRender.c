#include "TextRender.h"
#include "TamilText.h"
#include "LanguageSelection.h"
#include "raylib.h"
#include <stdlib.h>

static Font englishFont;

void InitText()
{
    InitComplexText("Kavivanar-Regular.ttf");

    //Load English Font
    englishFont = (LoadFontEx("TASAExplorer-Regular.ttf",40,NULL,0)); 
}

void DeInitText()
{
    UnloadComplexText();
    UnloadFont(englishFont);
}

void RenderText(TextID textID, Vector2 position, int fontSize, Color color)
{
    const char* text = GetText(textID);
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            DrawTextEx(englishFont, text, position, fontSize, 4, color);
            break;
        case Tamil:
            DrawTamilText(text,position,fontSize,color);
            break;
    }
}

void RenderTextDirect(const char* text, Vector2 position, int fontSize, Color color)
{
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            DrawTextEx(englishFont, text, position, fontSize, 4, color);
            break;
        case Tamil:
            DrawTamilText(text,position,fontSize,color);
            break;
    }
}

void RenderEnglishText(const char* text, Vector2 position, int fontSize, Color color)
{
    DrawTextEx(englishFont, text, position, fontSize, 4, color);
}

void RenderTamilText(const char* text, Vector2 position, int fontSize, Color color)
{
    DrawTamilText(text,position,fontSize,color);
}

Vector2 MeasureTextDirect(const char* text, int fontSize)
{
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            return MeasureTextEx(englishFont,text,fontSize,0); // setting spacing to zero 
        case Tamil:
            return MeasureTamilText(text,fontSize);
    }
    return (Vector2) { 0, 0 };
}
