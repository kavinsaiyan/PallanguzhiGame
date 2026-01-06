#include "raylib.h"
#include "Game.h"
#include "SaveData.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef PLATFORM_ANDROID
#include "raymob.h"
#endif

char* SAVE_FILE_PATH = "savefile.dat";

void CheckAndLoadSaveData(SaveData* saveData)
{
#ifdef PLATFORM_ANDROID
	if(!IsFileExistsInAppStorage(SAVE_FILE_PATH))
#else
    if(!FileExists(SAVE_FILE_PATH))
#endif
    {
        TraceLog(LOG_INFO, "Creating new save file");
        saveData->languageSelected = DefaultLanguage;
        saveData->playerHiScore = 0;
        WriteSaveData(saveData);
        return;
    }
    
    int size = 0;
#ifdef PLATFORM_ANDROID
	unsigned char* data = ReadFromAppStorage(SAVE_FILE_PATH, &size);
#else
    unsigned char* data = LoadFileData(SAVE_FILE_PATH, &size);
#endif

    if (data == NULL || size < 2)
    {
        TraceLog(LOG_ERROR, "Failed to load save data");
        saveData->languageSelected = DefaultLanguage;
        saveData->playerHiScore = 0;
        if (data != NULL) UnloadFileData(data);
        return;
    }

    saveData->languageSelected = (Language)data[0];
    saveData->playerHiScore = data[1];
    
    TraceLog(LOG_INFO, "Loaded: lang=%d, score=%d", 
             saveData->languageSelected, saveData->playerHiScore);

    UnloadFileData(data); 
}

void WriteSaveData(SaveData* saveData)
{
    unsigned char initDummyData[2];
    initDummyData[0] = (unsigned char)saveData->languageSelected;
    initDummyData[1] = saveData->playerHiScore;
    
#ifdef PLATFORM_ANDROID
	WriteToAppStorage(SAVE_FILE_PATH, initDummyData, sizeof(unsigned char) * 2);
#else
    SaveFileData(SAVE_FILE_PATH, initDummyData, sizeof(unsigned char) * 2);
#endif

    TraceLog(LOG_INFO, "Saved: lang=%d, score=%d", 
             saveData->languageSelected, saveData->playerHiScore);
}
