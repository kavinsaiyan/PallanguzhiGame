#include "raylib.h"
#include "LanguageEnum.h"
#include <stdlib.h>

// File scope variables
static Language currentLanguage = English;
static Font* currentFont = NULL;

void SetLanguage(Language language, Font* font)
{
    currentLanguage = language;
}

Language GetLanguage()
{
    return currentLanguage;
}

Font* GetFont()
{
    return currentFont;
}

void SetFont(Font* font)
{
	currentFont = font;
}
