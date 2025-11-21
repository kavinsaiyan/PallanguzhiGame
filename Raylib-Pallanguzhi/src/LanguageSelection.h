#include "raylib.h"

#ifndef LANGUAGE_SELECTION_H
#define LANGUAGE_SELECTION_H

typedef enum
{
    DefaultLanguage,
    Tamil,
    English,
} Language;

void SetLanguageAndLoadedFont(Language language);

Language GetLanguage();

Font* GetFont();

void Initialize(Language language, Sound clickSound);

void DeInitialize();

void DrawLanguageSelection();

Language UpdateLanguageSelection(Vector2 mousePosition);
#endif
