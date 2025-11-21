#include "LanguageSelection.h"

#ifndef TRANSLATION_H
#define TRANSLATION_H

typedef enum 
{
    Play,
    Exit,
    YourScore,
    AIScore,
    Retry
} TextID;

const char* GetText(TextID textID);
#endif
