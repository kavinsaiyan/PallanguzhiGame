#ifndef LANGUAGE_ENUM_H
#define LANGUAGE_ENUM_H

typedef enum
{
    Tamil,
    English,
} Language;

void SetLanguage(Language language, Font* font);

Language GetLanguage();

Font* GetFont();
#endif
