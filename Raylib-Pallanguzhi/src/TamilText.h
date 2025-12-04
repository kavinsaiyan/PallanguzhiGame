#ifndef TAMIL_TEXT_H
#define TAMIL_TEXT_H

void InitComplexText(const char *fontPath, int fontSize);
void DrawTamilText(const char *text, Vector2 position, Color color);
void UnloadComplexText(void);

#endif
