#include "LanguageSelection.h"
#include "raylib.h"
#include "Button.h"
#include <stdlib.h>

const int TAMIL_CHARS_COUNT = 128; // Range 0x0B80 to 0x0BFF

static Language currentLanguage = English;
static Font loadedFontTamil;
static Font loadedFontEnglish;
static Font loadedFont;

static Button tamilButton;
static Button englishButton;

void SetLanguageAndLoadedFont(Language language)
{
    currentLanguage = language;
    switch(language)
    {
        case Tamil:
            loadedFont = loadedFontTamil;
            break;
        case DefaultLanguage:
        case English:
            loadedFont = loadedFontEnglish;
            break;
    }
}

void Initialize(Language language, Sound clickSound)
{
    //Load tamil font
    int tamil_chars[TAMIL_CHARS_COUNT];
    for (int i = 0; i < TAMIL_CHARS_COUNT; i++)
        tamil_chars[i] = 0x0B80 + i;
    loadedFontTamil = (LoadFontEx("Kavivanar-Regular.ttf",40,tamil_chars,TAMIL_CHARS_COUNT));

    //Load English font
    loadedFontEnglish = (LoadFontEx("TASAExplorer-Regular.ttf",40,NULL,0)); 

    SetLanguageAndLoadedFont(language);

    //tamilButton.pos =  
}

void DeInitialize()
{
    UnloadFont(loadedFontEnglish);
    UnloadFont(loadedFontTamil);
}


Language GetLanguage()
{
    return currentLanguage;
}

Font* GetFont()
{
    return &loadedFont;
}


//screen related 
void DrawLanguageSelection()
{
    //draw the buttons here
    bool drawButtonText = false;
    DrawButton(&tamilButton, drawButtonText);
    DrawButton(&englishButton, drawButtonText);
}

Language UpdateLanguageSelection(Vector2 mousePosition)
{
    Language language = DefaultLanguage;
    if(IsButtonClicked(&tamilButton,mousePosition))
        language = Tamil;
    if(IsButtonClicked(&englishButton ,mousePosition))
        language = English; 
    return language;
}
