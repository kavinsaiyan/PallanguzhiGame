#include "raylib.h"
#include "Button.h"
#include "LanguageSelection.h"
#include "Translation.h"
#include "TextRender.h"

void InitButton(Button* button,float x,float y, float width, float height, TextID textID, Sound* clickSound)
{
    button->center.x = x;
    button->center.y = y;
    button->clickSound = clickSound;
    button->color = LIGHTGRAY;
    button->textID = textID;
    ResizeButtonForText(button);
}

void PrintSize(Button* button)
{
    const char* text = GetText(button->textID);
    Vector2 size = MeasureTextDirect(text);
    Vector2 textPos = button->textPos;
    TraceLog(LOG_INFO,"size of text is x : %f y : %f for %s",size.x,size.y,text);
    TraceLog(LOG_INFO,"pos of text is x : %f y : %f for %s",textPos.x,textPos.y,text);
    TraceLog(LOG_INFO,"button center is x : %f and y : %f",button->center.x,button->center.y);
    TraceLog(LOG_INFO,"button pos is x : %f and y : %f",button->rect.x,button->rect.y);
    TraceLog(LOG_INFO,"button width is : %f and height : %f",button->rect.width,button->rect.height);
}

void ResizeButtonForText(Button* button)
{
    //need to get the Text size first
    const char* text = GetText(button->textID);
    button->textSize = MeasureTextDirect(text);

    //resize the button width and height
    float padding = 20;
    button->rect.width = button->textSize.x + padding;
    button->rect.x = button->center.x - button->rect.width / 2.0f;

    button->rect.height = button->textSize.y + padding;
    button->rect.y = button->center.y - button->rect.height / 2.0f;

    //Assigning Text pos
    Vector2 textPos;
    textPos.x = button->center.x - (button->textSize.x/2.0f) - padding/3.0f;
    textPos.y = button->center.y - (button->textSize.y/2.0f);
    button->textPos = textPos;

    //PrintSize(button);
}

void DrawButton(Button* button, bool drawButtonText)
{
    DrawRectangle(button->rect.x, button->rect.y, button->rect.width, button->rect.height, button->color);

    if(!drawButtonText)
        return;

    //draw the text
    RenderText(button->textID,button->textPos,BLACK);
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
