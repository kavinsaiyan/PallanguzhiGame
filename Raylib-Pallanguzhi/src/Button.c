#include "raylib.h"
#include "Button.h"
#include "LanguageSelection.h"
#include "Translation.h"

void DrawButton(Button* button, bool drawButtonText)
{
    DrawRectangleRec(button->rect, button->color);

    if(!drawButtonText)
        return;

    Vector2 textPos;
    textPos.x = button->rect.x+16;
    textPos.y = button->rect.y+10;
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
