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
    
    PrintTextSize(button);
}

void PrintTextSize(Button* button)
{
    const char* text = GetText(button->textID);
    Vector2 size = MeasureTextEx(*GetFont(),text,32,0);
    TraceLog(LOG_INFO,"size of text is x : %f y : %f for %s",size.x,size.y,text);
    TraceLog(LOG_INFO,"pos is x : %f and y : %f",button->rect.x,button->rect.y);
    TraceLog(LOG_INFO,"width is : %f and height : %f",button->rect.width,button->rect.height);
}

void DrawButton(Button* button, bool drawButtonText)
{
    DrawRectangleRec(button->rect, button->color);

    if(!drawButtonText)
        return;

    const char* text = GetText(button->textID);
    Vector2 size = MeasureTextEx(*GetFont(),text,32,0);

    Vector2 textPos;
    textPos.x = ((button->rect.x*2+button->rect.width)/2.0f) - (size.x/2.0f);
    textPos.y = button->rect.y+8;
    DrawTextEx(*GetFont(),text,textPos,32,0,BLACK);
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
