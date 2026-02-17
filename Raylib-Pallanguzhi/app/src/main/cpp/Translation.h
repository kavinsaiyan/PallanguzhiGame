#include "LanguageSelection.h"

#ifndef TRANSLATION_H
#define TRANSLATION_H

typedef enum 
{
    Play,
    Exit,
    YourScore,
    AIScore,
    Retry,
    LanguageSelect,
    YourTurn,
    AITurn,
    YouWon,
    YouLost,
    OnlinePlay,
    WaitingForOtherPlayer,
    GoToHomePage
} TextID;

const char* GetText(TextID textID);
#endif
