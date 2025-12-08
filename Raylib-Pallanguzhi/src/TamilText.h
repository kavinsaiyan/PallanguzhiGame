#ifndef TAMIL_TEXT_H
#define TAMIL_TEXT_H

void InitComplexText(const char *fontPath);
void DrawTamilText(const char *text, Vector2 position, int fontSize, Color color);
void UnloadComplexText(void);
Vector2 MeasureTamilText(const char *text, int fontSize);

#endif
