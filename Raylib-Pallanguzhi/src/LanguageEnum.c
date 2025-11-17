#include "raylib.h"
#include "LanguageEnum.h"
#include <stdlib.h>

Language currentLanguage = English;
Font* currentFont = NULL;

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
