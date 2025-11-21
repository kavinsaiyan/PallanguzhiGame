#include "raylib.h"
#include "Translation.h"
#include "LanguageSelection.h"

#ifndef BUTTON_H
#define BUTTON_H

typedef struct 
{
    Rectangle rect;
    Color color;
    Sound* clickSound; 
    TextID textID;
} Button;

void DrawButton(Button* button, bool drawButtonText);

bool IsButtonClicked(Button* button, Vector2 mousePosition);
#endif
