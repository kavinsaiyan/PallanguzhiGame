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

void InitializeLanguageSelection(Language language, Sound* clickSound);

void DeInitializeLanguageSelection();

void DrawLanguageSelection();

Language UpdateLanguageSelection(Vector2 mousePosition);
#endif
