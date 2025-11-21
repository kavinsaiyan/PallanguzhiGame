#include "LanguageSelection.h"

#ifndef SAVEDATA_H
#define SAVEDATA_H

typedef struct
{
    Language languageSelected;
    unsigned char playerHiScore;
} SaveData;

void CheckAndLoadSaveData(SaveData* saveData);

void WriteSaveData(SaveData* saveData);

#endif
