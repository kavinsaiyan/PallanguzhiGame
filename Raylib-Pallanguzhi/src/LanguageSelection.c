#include "LanguageSelection.h"
#include "raylib.h"
#include "Button.h"
#include <stdlib.h>
#include "TamilText.h"

const int TAMIL_CHARS_COUNT = 128; // Range 0x0B80 to 0x0BFF

static Language currentLanguage = English;
static Font englishFont;
static Font tamilFont;
static Font selectedFont;

static Button tamilButton;
static Button englishButton;

void SetLanguageAndLoadedFont(Language language)
{
    TraceLog(LOG_INFO,"language set is %d",language);
    currentLanguage = language;
    switch(language)
    {
        case Tamil:
            selectedFont = tamilFont;
            break;
        case DefaultLanguage:
        case English:
            selectedFont = englishFont;
            break;
    }
}

void InitializeLanguageSelection(Language language, Sound* clickSound)
{
    //Load tamil font
    int tamil_chars[TAMIL_CHARS_COUNT];
    for (int i = 0; i < TAMIL_CHARS_COUNT; i++)
        tamil_chars[i] = 0x0B80 + i;
    tamilFont = (LoadFontEx("Kavivanar-Regular.ttf",40,tamil_chars,TAMIL_CHARS_COUNT));

    //Load English font
    englishFont = (LoadFontEx("TASAExplorer-Regular.ttf",40,NULL,0)); 

    SetLanguageAndLoadedFont(language);

    tamilButton.rect.x = 320;
    tamilButton.rect.y = 280;
    tamilButton.rect.width = 140;
    tamilButton.rect.height = 50;
    tamilButton.color = LIGHTGRAY;
    tamilButton.clickSound = clickSound;

    englishButton.rect.x = 320;
    englishButton.rect.y = 360;
    englishButton.rect.width = 140;
    englishButton.rect.height = 50;
    englishButton.color = LIGHTGRAY;
    englishButton.clickSound = clickSound;
}

void DeInitializeLanguageSelection()
{
    UnloadFont(englishFont);
    UnloadFont(tamilFont);
}


Language GetLanguage()
{
    return currentLanguage;
}

Font* GetFont()
{
    return &selectedFont;
}


//screen related 
void DrawLanguageSelection()
{
    ClearBackground(RAYWHITE);

    DrawTextEx(englishFont,"Select Language", (Vector2) { 220, 200 }, 50, 4 , BLACK);
    //draw the buttons here
    bool drawButtonText = false;
    DrawButton(&tamilButton, drawButtonText);
    DrawTamilText("தமிழ்", (Vector2){tamilButton.rect.x + 26, tamilButton.rect.y}, BLACK);
    DrawButton(&englishButton, drawButtonText);
    DrawTextEx(englishFont, "English", (Vector2){englishButton.rect.x + 6, englishButton.rect.y}, 40, 4, BLACK);
}

Language UpdateLanguageSelection(Vector2 mousePosition)
{
    Language language = DefaultLanguage;
    if(IsButtonClicked(&tamilButton,mousePosition))
        language = Tamil;
    if(IsButtonClicked(&englishButton,mousePosition))
        language = English; 
    if(language != DefaultLanguage)
        SetLanguageAndLoadedFont(language);
    return language;
}
