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

void InitButton(Button* button,float x,float y, float width, float height, TextID text, Sound* clickSound);

void DrawButton(Button* button, bool drawButtonText);

void PrintTextSize(Button* button);

bool IsButtonClicked(Button* button, Vector2 mousePosition);
#endif
