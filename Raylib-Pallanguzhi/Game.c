#include "raylib.h"
#include "raymath.h"
#include "Board.h"
#include "Render.h"
#include "SlotSelector.h"
#include "Queue.h"
#include "Game.h"
#include "MainMenu.h"

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
    
    Texture2D ballTexture = LoadTexture("Resources/Sample Bead.png");
    Texture2D boardTexture = LoadTexture("Resources/Board.png");
    Texture2D slotSelectorTexture = LoadTexture(SELECTOR_TEXTURE_PATH);

    //Initialize Game State
    GameStateData gameStateData;
    InitializeGameStateData(&gameStateData);
    Queue* animQ = CreateQueue();
    MainMenuData mainMenu;
    InitializeMainMenu(&mainMenu);

    //Test Main menu
    gameStateData.state = MainMenu;

    //Initialize the board
    Board board;
    InitializeBoard(&board);

    //Player Input
    SlotSelector slotSelector;
    slotSelector.currentIndex =(int)(TOTAL_SLOTS * 0.75f);
    slotSelector.renderState = Render;

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

        // Check for window close
        if(IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) exitWindow = true;

        // Get user input from keys for moving slotSelector
        if(IsKeyPressed(KEY_RIGHT)) slotSelector.currentIndex -= 1;
        if(IsKeyPressed(KEY_LEFT)) slotSelector.currentIndex += 1;
        slotSelector.currentIndex = (int)Wrap(slotSelector.currentIndex,TOTAL_SLOTS/2,TOTAL_SLOTS);
        
        // Get user input for actually selecting that slot
        if(IsKeyPressed(KEY_ENTER) && gameStateData.state == PlayerMove && gameStateData.playerTurn == Player1Turn) 
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
            case GameOver: break;
            case MainMenu:
                 if(IsPlayButtonClicked(&mainMenu))
                    gameStateData.state = PlayerMove;
                 if(IsExitButtonClicked(&mainMenu))
                    exitWindow = true;
                 break;
            case PauseMenu: break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        switch(gameStateData.state)
        {
            case Animating:
            case PlayerMove:
                DrawBoardGame(&board,&slotSelector,&boardTexture,&ballTexture,&slotSelectorTexture);
                break;
            case GameOver:
                DrawText(TextFormat("Player 1 Score : %d",board.player1Score),300,100,16,BLACK);
                DrawText(TextFormat("Player 2 Score : %d",board.player2Score),300,120,16,BLACK);
                if(gameStateData.playerWon == 0)
                    DrawText("You Won!",300,160,16,BLACK);
                else 
                    DrawText("You lost!",300,160,16,BLACK);
                break;
            case MainMenu:
                DrawMainMenu(&mainMenu);
                break;
            case PauseMenu:
                break;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    DestroyQueue(animQ);

    UnloadTexture(ballTexture);
    UnloadTexture(boardTexture);
    UnloadTexture(slotSelectorTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitializeGameStateData(GameStateData* gameStateData)
{
    gameStateData->state = PlayerMove;
    gameStateData->playerTurn = Player1Turn;
    gameStateData->playerWon = -1;
}

void StartMove(GameState* gameState,Board* board, Queue* animQ, int currentIndex)
{
    //*gameState = PauseMenu;
    *gameState = Animating;
    SetBeadRenderStateInSlot(board,currentIndex,DontRender);
    Array* arr = GetAllBeadsFrom(board,currentIndex);
    EnqueueArray(animQ, arr->arr, arr->len);
    board->currentMoveIndex = (int)Wrap(currentIndex - 1,0,TOTAL_SLOTS);
    DestroyArray(arr);
}

void DrawBoardGame(Board* board,SlotSelector* slotSelector, Texture2D* boardTexture, Texture2D* ballTexture, Texture2D* slotSelectorTexture)
{
    ClearBackground(RAYWHITE);
    DrawBoard(board, boardTexture, ballTexture);
    DrawSlotSelector(slotSelector, slotSelectorTexture, board->slots[slotSelector->currentIndex].position);
    DrawText(TextFormat("Player 1 Score : %d",board->player1Score),0,100,16,BLACK);
    DrawText(TextFormat("Player 2 Score : %d",board->player2Score),0,120,16,BLACK);
}
