#include "raylib.h"
#include "raymath.h"
#include "Board.h"
#include "Render.h"
#include "SlotSelector.h"
#include "Queue.h"
#include "LanguageSelection.h"
#include "Game.h"
#include "MainMenu.h"
#include "EndScreen.h"
#include "SaveData.h"
#include "TamilText.h"
#include <stdlib.h>
#include "TextRender.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 680;

    //Window related functions
    InitWindow(screenWidth, screenHeight, "Pallanguzhi");
    SetExitKey(0);
    bool exitWindow = false;
 
    //Save Data
    SaveData saveData;
    CheckAndLoadSaveData(&saveData);
   
#ifndef PLATFORM_ANDROID
	ChangeDirectory("assets");
#endif

    //Loading Textures
    Texture2D ballTexture = LoadTexture("Sample Bead.png");
    Texture2D boardTexture = LoadTexture("Board.png");
    Texture2D slotSelectorTexture = LoadTexture(SELECTOR_TEXTURE_PATH);
    Texture2D mainMenuBGTexture = LoadTexture("HomeBG.png");
    
    //Audio Init
    InitAudioDevice();
    Sound moveSound = LoadSound("move.wav");
    Sound clickSound = LoadSound("click.wav");

    //Load Fonts and init language selection
    InitText();
    InitializeLanguageSelection(saveData.languageSelected,&clickSound);

#ifndef PLATFORM_ANDROID
	ChangeDirectory("..");
#endif

    //Initialize Menus
    InitializeMainMenu(&clickSound);
    InitializeEndScreen(&clickSound);

    //Initialize the board
    Board board;
    InitializeBoard(&board);

    //Player Input
    SlotSelector slotSelector;
    slotSelector.currentIndex =(int)(TOTAL_SLOTS * 0.75f);
    slotSelector.renderState = Render;

    //Initialize Game State
    GameStateData gameStateData;
    InitializeGameStateData(&gameStateData,&saveData);
    Queue* animQ = CreateQueue();
    Language selected;

    //timer 
    float timer = 0;
    int indexOfBeadToMove;
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!exitWindow)    // Detect window close button or ESC key
    {
        // Update
        float dt = GetFrameTime();
        Vector2 mousePosition = GetMousePosition();

        // Check for window close
        if(IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) exitWindow = true;

        // Get user input from keys for moving slotSelector
        if(IsKeyPressed(KEY_RIGHT)) slotSelector.currentIndex -= 1;
        if(IsKeyPressed(KEY_LEFT)) slotSelector.currentIndex += 1;
        int mousePosOnSlot = CheckMouseHoverOnSlot(&board,mousePosition);
        if(mousePosOnSlot != -1 && mousePosOnSlot >= TOTAL_SLOTS/2 && mousePosOnSlot < TOTAL_SLOTS)
            slotSelector.currentIndex = mousePosOnSlot;
        slotSelector.currentIndex = (int)Wrap(slotSelector.currentIndex,TOTAL_SLOTS/2,TOTAL_SLOTS);
  
        // Get user input for actually selecting that slot
        if((IsKeyPressed(KEY_ENTER) || (mousePosOnSlot!=-1 && IsMouseButtonPressed(0)))
                && gameStateData.state == PlayerMove && gameStateData.playerTurn == Player1Turn
                && GetBeadCountInSlot(&board,slotSelector.currentIndex) > 0) 
        {
            slotSelector.renderState = DontRender;
            StartMove(&gameStateData.state,&board,animQ,slotSelector.currentIndex);
        }

        switch(gameStateData.state)
        {
            case PlayerMove: 
                gameStateData.playerWon = HasAnyPlayerWon(&board);
                if(gameStateData.playerWon != -1)
                {
                    gameStateData.state = GameOver;
                    PrepareEndScreenForDrawing();
                    break;
                }

                if(gameStateData.playerTurn == Player2Turn)
                {
                    //AI move range (0,TOTAL_SLOTS/2)
                    //Need to know all indices that have beads in them
                    Array* arr = GetSlotsThatHaveBeads(&board,0,TOTAL_SLOTS/2);
                    //for now choose randomly
                    int rand = GetRandomValue(0,arr->len-1);
                    board.currentMoveIndex = arr->arr[rand];
                    TraceLog(LOG_INFO,"AI chooses %d slot index randomly and rand index is %d", arr->arr[rand], rand);
                    DestroyArray(arr);
                    StartMove(&gameStateData.state,&board,animQ,board.currentMoveIndex);
                }
                break;
            case Animating:
                timer += dt;
                if(timer > 0.1f)
                {
                    timer = 0;
                    PlaySound(moveSound);
                    TraceLog(LOG_INFO,"next move index is %d",board.currentMoveIndex);
                    if(animQ->count > 0)
                    {
                        // move one bead to next slot
                        indexOfBeadToMove = Dequeue(animQ);
                        MoveBeadTo(&board,indexOfBeadToMove);

                        //TraceLog(LOG_INFO,"Animqueue count is not zero");
                    }
                    else
                    {
                        int beads = GetBeadCountInSlot(&board, board.currentMoveIndex);
                        TraceLog(LOG_INFO,"Animqueue count is zero and beads in curmovindex is %d",beads);
                        if(beads != 0)
                        {
                            SetBeadRenderStateInSlot(&board,board.currentMoveIndex,DontRender);
                            // Add to Queue
                            Array* tempArr = GetAllBeadsFrom(&board,board.currentMoveIndex);
                            EnqueueArray(animQ, tempArr->arr, tempArr->len);
                            board.currentMoveIndex = (int)Wrap(board.currentMoveIndex - 1,0,TOTAL_SLOTS);
                            DestroyArray(tempArr);

                            //TraceLog(LOG_INFO, "move intermediate end");
                        }
                        else
                        {
                            //Add the next slot score current player
                            TraceLog(LOG_INFO, "current player is %d",(int)gameStateData.playerTurn);
                            board.currentMoveIndex = (int)Wrap(board.currentMoveIndex - 1,0,TOTAL_SLOTS);
                            AddBeadsToPlayer(&board,gameStateData.playerTurn,board.currentMoveIndex);
                            UpdatePlayerScore(&board);

                            gameStateData.playerTurn = gameStateData.playerTurn == Player1Turn ? Player2Turn : Player1Turn;
                            gameStateData.state = PlayerMove;
                            if(gameStateData.playerTurn == Player1Turn)
                                slotSelector.renderState = Render;
                            TraceLog(LOG_INFO, "move end");
                        }
                    }
                }
                break;
            case GameOver: 
                if(IsRetryButtonClicked(mousePosition))
                {
                    gameStateData.state = PlayerMove;
                    gameStateData.playerTurn = Player1Turn;
                    slotSelector.currentIndex = (int)(TOTAL_SLOTS * 0.75f);
                    slotSelector.renderState = Render;
                    InitializeBoard(&board);
                }
                break;
            case MainMenu:
                if(IsPlayButtonClicked(mousePosition))
                    gameStateData.state = PlayerMove;
                if(IsExitButtonClicked(mousePosition))
                    exitWindow = true;
                break;
            case LanguageSelection:
                 selected = UpdateLanguageSelection(mousePosition);
                 if(selected != DefaultLanguage)
                 {
                    saveData.languageSelected = selected;
                    WriteSaveData(&saveData);
                    gameStateData.state = MainMenu;
                    InitializeMainMenuForDrawing();
                 }
                 break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        switch(gameStateData.state)
        {
            case Animating:
            case PlayerMove:
                DrawBoardGame(&board,&slotSelector,&boardTexture,&ballTexture,&slotSelectorTexture, gameStateData.playerTurn);
                break;
            case GameOver:
                DrawEndScreen(board.player1Score,board.player2Score,gameStateData.playerWon);
                break;
            case MainMenu:
                DrawMainMenu(&mainMenuBGTexture);
                break;
            case LanguageSelection:
                DrawLanguageSelection();
                break;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    DestroyQueue(animQ);

    UnloadComplexText();

    UnloadSound(moveSound);
    UnloadSound(clickSound);
    CloseAudioDevice();

    UnloadTexture(ballTexture);
    UnloadTexture(boardTexture);
    UnloadTexture(slotSelectorTexture);
    UnloadTexture(mainMenuBGTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitializeGameStateData(GameStateData* gameStateData, SaveData* saveData)
{
    gameStateData->state = PlayerMove;
    gameStateData->playerTurn = Player1Turn;
    gameStateData->playerWon = -1;

    //Set Main menu
    if(saveData->languageSelected == DefaultLanguage)
        gameStateData->state = LanguageSelection;
    else
    {
        InitializeMainMenuForDrawing();
        gameStateData->state = MainMenu;
    }
}

void StartMove(GameState* gameState,Board* board, Queue* animQ, int currentIndex)
{
    *gameState = Animating;
    SetBeadRenderStateInSlot(board,currentIndex,DontRender);
    Array* arr = GetAllBeadsFrom(board,currentIndex);
    EnqueueArray(animQ, arr->arr, arr->len);
    board->currentMoveIndex = (int)Wrap(currentIndex - 1,0,TOTAL_SLOTS);
    DestroyArray(arr);
}

void DrawBoardGame(Board* board,SlotSelector* slotSelector, Texture2D* boardTexture, Texture2D* ballTexture, Texture2D* slotSelectorTexture,PlayerTurn playerTurn)
{
    ClearBackground(RAYWHITE);
    DrawBoard(board, boardTexture, ballTexture);
    DrawSlotSelector(slotSelector, slotSelectorTexture, board->slots[slotSelector->currentIndex].position);

    if(playerTurn == Player1Turn)    
        RenderText(YourTurn,(Vector2){260,600},BLACK);
    else 
        RenderText(AITurn,(Vector2){260,120},BLACK);

    Vector2 playerScorePos = { 10, 30 };
    RenderTextDirect(TextFormat(GetText(YourScore),board->player1Score),playerScorePos,BLACK);
    
    playerScorePos.y += 40;
    RenderTextDirect(TextFormat(GetText(AIScore),board->player2Score),playerScorePos,BLACK);
}
