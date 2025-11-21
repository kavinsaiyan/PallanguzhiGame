#include "raylib.h"

#define TAMIL_CHARS_COUNT 128 // Range 0x0B80 to 0x0BFF

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib Tamil Text Example");

    // --- 1. Define the Tamil character range ---
    int tamil_chars[TAMIL_CHARS_COUNT];
    for (int i = 0; i < TAMIL_CHARS_COUNT; i++) {
        tamil_chars[i] = 0x0B80 + i;
    }

    // --- 2. Load the font with the custom range ---
    // Ensure "Kavivanar-Regular.ttf" is in the same directory as your executable
    Font tamilFont = LoadFontEx("../assets/Kavivanar-Regular.ttf", 40, tamil_chars, TAMIL_CHARS_COUNT);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // --- 3. Draw the Tamil Text ---
        // Ensure the font size (40) is passed correctly to DrawTextEx
        // The original code used 15 for font size AND spacing.
        DrawTextEx(tamilFont, "வணக்கம் உலகம்", (Vector2){100, 100}, 40, 4, BLACK);

        DrawText("If this text is in Tamil, it worked!", 10, 200, 20, DARKGRAY);

        EndDrawing();
    }

    UnloadFont(tamilFont);
    CloseWindow();

    return 0;
}
