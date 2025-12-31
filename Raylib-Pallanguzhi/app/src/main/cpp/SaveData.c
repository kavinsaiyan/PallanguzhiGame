#include "raylib.h"
#include "Game.h"
#include "SaveData.h"

const char* SAVE_FILE_DIRECTORY = "SaveData";
const char* SAVE_FILE_PATH = "SaveData/savefile.dat";

void CheckAndLoadSaveData(SaveData* saveData)
{
    if(!DirectoryExists(SAVE_FILE_DIRECTORY))
        MakeDirectory(SAVE_FILE_DIRECTORY);

    if(!FileExists(SAVE_FILE_PATH))
    {
        saveData->languageSelected = DefaultLanguage;
        saveData->playerHiScore = 0;
        WriteSaveData(saveData);
        return;
    }
    
    int size = (int)(sizeof(unsigned char) * 2);
    unsigned char* data = LoadFileData(SAVE_FILE_PATH,&size);

    saveData->languageSelected = (Language)data[0];
    saveData->playerHiScore = data[1];

    UnloadFileData(data); 
}

void WriteSaveData(SaveData* saveData)
{
    //unsigned char* initDummyData = (unsigned char*)malloc(sizeof(unsigned char) * 2);
    //if(initDummyData == NULL)
    //{
    //   TraceLog(LOG_ERROR, "[SaveData.c/CheckAndLoadSaveData]: Unable to allocate memory!");
    //   return;
    //}

    unsigned char initDummyData[2];
    initDummyData[0] = (unsigned char)saveData->languageSelected;
    initDummyData[1] = saveData->playerHiScore;

    SaveFileData(SAVE_FILE_PATH,&initDummyData,sizeof(unsigned char) * 2);
}
