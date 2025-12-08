#include "LanguageSelection.h"
#include "raylib.h"
#include "Button.h"
#include <stdlib.h>
#include "TextRender.h"

static Language currentLanguage = English;

static Button tamilButton;
static Button englishButton;

void SetLanguageAndLoadedFont(Language language)
{
    currentLanguage = language;
}

void InitializeLanguageSelection(Language language, Sound* clickSound)
{
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

Language GetLanguage()
{
    return currentLanguage;
}

//screen related 
void DrawLanguageSelection()
{
    ClearBackground(RAYWHITE);

    RenderEnglishText("Select Language", (Vector2) { 220, 200 }, 50, BLACK);
    //draw the buttons here
    bool drawButtonText = false;
    DrawButton(&tamilButton, drawButtonText);
    RenderTamilText("தமிழ்", (Vector2){tamilButton.rect.x + 26, tamilButton.rect.y}, 40, BLACK);
    DrawButton(&englishButton, drawButtonText);
    RenderEnglishText("English", (Vector2){englishButton.rect.x + 6, englishButton.rect.y}, 40, BLACK);
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
