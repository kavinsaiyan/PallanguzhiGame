#include "raylib.h"
#include "Button.h"
#include "LanguageSelection.h"
#include "Translation.h"

void InitButton(Button* button,float x,float y, float width, float height, TextID textID, Sound* clickSound)
{
    button->rect.x = x;
    button->rect.y = y;
    button->rect.width = width;
    button->rect.height = height;
    button->clickSound = clickSound;
    button->color = LIGHTGRAY;
    button->textID = textID;
}

void DrawButton(Button* button, bool drawButtonText)
{
    DrawRectangleRec(button->rect, button->color);

    if(!drawButtonText)
        return;

    Vector2 textPos;
    textPos.x = button->rect.x+8;
    textPos.y = button->rect.y+8;
    DrawTextEx(*GetFont(),GetText(button->textID),textPos,32,0,BLACK);
}

bool IsButtonClicked(Button* button, Vector2 mousePosition)
{
    button->color = LIGHTGRAY;
    if(CheckCollisionPointRec(mousePosition, button->rect))
    {
        button->color = GRAY;
        if(IsMouseButtonPressed(0))
        {
            PlaySound(*(button->clickSound));
            return true;
        }
    }

    return false;
}
