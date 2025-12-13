#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FREETYPE & HARFBUZZ Headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <hb.h>
#include <hb-ft.h>

// --- Global Freetype/Harfbuzz objects ---
FT_Library ft_library;
FT_Face ft_face;
hb_font_t *hb_font;

// --- FIXED-SIZE ARRAY CACHE (DANGEROUS but requested) ---
// This size must accommodate the largest glyph index in your font. 
// If a glyph index is >= 4096, the program will crash or corrupt memory.
#define MAX_GLYPHS 4096 
Texture2D glyphCache[MAX_GLYPHS] = { 0 }; 

// --- Function Prototypes ---
void InitComplexText(const char *fontPath, int fontSize);
void DrawTamilText(const char *text, Vector2 position, int fontSize, Color color);
void UnloadComplexText(void);

// ------------------------------------------------------------------------
// BitmapToImage (Converts FreeType Grayscale Bitmap to Raylib RGBA Image)
// ------------------------------------------------------------------------
Image BitmapToImage(FT_Bitmap bitmap)
{
    Image image = { 0 };

    if (bitmap.width == 0 || bitmap.rows == 0) {
        return GenImageColor(1, 1, BLANK);
    }

    image.width = bitmap.width;
    image.height = bitmap.rows;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; 

    // Allocate memory for the RGBA pixels
    Color *pixels = (Color *)RL_CALLOC(image.width * image.height, sizeof(Color));

    for (int y = 0; y < bitmap.rows; y++)
    {
        for (int x = 0; x < bitmap.width; x++)
        {
            int index = (x + y * bitmap.width);
            unsigned char intensity = bitmap.buffer[index];
            
            // Set R, G, B to max (to allow coloring via DrawTexture)
            // Set A (Alpha) to intensity (for transparency)
            pixels[index].r = 255; 
            pixels[index].g = 255;
            pixels[index].b = 255;
            pixels[index].a = intensity; // Use intensity as Alpha
        }
    }

    image.data = pixels;
    return image;
}

void DrawTamilText(const char *text, Vector2 position, int fontSize, Color color)
{
    FT_Set_Char_Size(ft_face, 0, fontSize * 64, 96, 96);

    if (!hb_font) return;

    // --- A. SHAPING SETUP ---
    hb_buffer_t *buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text, -1, 0, -1);
    hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(buffer, HB_SCRIPT_TAMIL);
    hb_buffer_set_language(buffer, hb_language_from_string("ta", -1));
    hb_shape(hb_font, buffer, NULL, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyph_count);
    
    float pen_x = position.x;
    float pen_y = position.y + fontSize;

    TraceLog(LOG_INFO,"glyph count is %d",glyph_count);

    // --- B. RASTERIZATION AND DRAWING ---
    for (unsigned int i = 0; i < glyph_count; i++)
    {
        // 5. Calculate positioning using HarfBuzz output
        float x_offset = (float)(glyph_pos[i].x_offset / 64.0f);
        float y_offset = (float)(glyph_pos[i].y_offset / 64.0f);
        float x_advance = (float)(glyph_pos[i].x_advance / 64.0f);

        FT_UInt glyph_index = glyph_info[i].codepoint;
        
        if (glyph_index >= MAX_GLYPHS) 
        {
            TraceLog(LOG_ERROR, "Glyph index %u exceeds MAX_GLYPHS.Skipping.", glyph_index);
            pen_x += x_advance;
            continue;
        }

        // --- CACHE CHECK & LOAD ---
        Texture2D glyphTexture = glyphCache[glyph_index];
        int errorCode = FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
        if (errorCode) {
            pen_x += x_advance;
            continue;
        }

        errorCode = FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL);
        if (errorCode) {
            pen_x += x_advance;
            continue;
        }

        if (glyphTexture.id == 0)
        {
            // Convert and Upload
            Image glyphImage = BitmapToImage(ft_face->glyph->bitmap);
            glyphTexture = LoadTextureFromImage(glyphImage);
            UnloadImage(glyphImage);
            
            // Store the new texture in the cache for future use
            glyphCache[glyph_index] = glyphTexture;
        }

        float draw_x = pen_x + x_offset + ft_face->glyph->bitmap_left;
        float draw_y = pen_y + y_offset - ft_face->glyph->bitmap_top; 
        
        DrawTexture(glyphTexture, (int)draw_x, (int)draw_y, color);

        pen_x += x_advance;
    }
    
    // --- C. CLEANUP ---
    hb_buffer_destroy(buffer);
}

// ------------------------------------------------------------------------
// INITIALIZATION AND CLEANUP
// ------------------------------------------------------------------------
void InitComplexText(const char *fontPath, int fontSize)
{
    // ... (Initialization remains the same) ...
    if (FT_Init_FreeType(&ft_library)) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not init FreeType library.");
        return;
    }
    if (FT_New_Face(ft_library, fontPath, 0, &ft_face)) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not load font face: %s", fontPath);
        return;
    }
    if (FT_Set_Char_Size(ft_face, 0, fontSize * 64, 96, 96)) {
        TraceLog(LOG_WARNING, "FREETYPE: Could not set font size.");
    }
    hb_font = hb_ft_font_create_referenced(ft_face);
    TraceLog(LOG_INFO, "Complex Text Initialized successfully.");
}

void UnloadComplexText(void)
{
    // CRITICAL: Unload ALL textures from the fixed array cache
    for(int i = 0; i < MAX_GLYPHS; i++) {
        if (glyphCache[i].id != 0) {
            UnloadTexture(glyphCache[i]);
            glyphCache[i].id = 0; // Reset
        }
    }
    
    if (hb_font) hb_font_destroy(hb_font);
    if (ft_face) FT_Done_Face(ft_face);
    if (ft_library) FT_Done_FreeType(ft_library);
    TraceLog(LOG_INFO, "Complex Text Unloaded successfully.");
}

// ------------------------------------------------------------------------
// EXAMPLE USAGE (main function)
// ------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Tamil HarfBuzz/FreeType Fixed Array Cache Example");
    InitComplexText("assets/Kavivanar-Regular.ttf", 40); 

    SetTargetFPS(60);
    bool drawOnce = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        if(!drawOnce)
        {
            ClearBackground(RAYWHITE);
            DrawTamilText("வணக்கம் உலகம்", (Vector2){100, 100}, 40, BLACK);
            DrawText("Using Fixed Array Cache. Watch MAX_GLYPHS!", 100, 200, 20, DARKGRAY);
            drawOnce = true;
        }

        EndDrawing();
    }

    UnloadComplexText();
    CloseWindow();
    return 0;
}
