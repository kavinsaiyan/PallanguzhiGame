#include "Translation.h"

#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

void InitText();

void DeInitText();

void RenderText(TextID textID, Vector2 position, Color color);

void RenderTextDirect(const char* text, Vector2 position, Color color);

void RenderEnglishText(const char* text, Vector2 position, Color color);

void RenderTamilText(const char* text, Vector2 position, Color color);

Vector2 MeasureTextDirect(const char* text);
#endif
