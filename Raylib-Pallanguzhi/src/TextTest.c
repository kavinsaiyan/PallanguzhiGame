#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FREETYPE & HARFBUZZ
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <hb.h>
#include <hb-ft.h>

// --- Global Freetype/Harfbuzz objects ---
FT_Library ft_library;
FT_Face ft_face;
hb_font_t *hb_font;

// Function Prototypes
void InitComplexText(const char *fontPath, int fontSize);
void DrawTamilText(const char *text, Vector2 position, Color color);
void UnloadComplexText(void);

Image BitmapToImage(FT_Bitmap bitmap)
{
    Image image = { 0 };

    // Safety check for empty bitmaps
    if (bitmap.width == 0 || bitmap.rows == 0) {
        TraceLog(LOG_INFO, "the bitmap is empty");
        return GenImageColor(1, 1, BLANK);
    }

    TraceLog(LOG_INFO, "bitmap width: %d , height: %d",bitmap.width,bitmap.rows);

    image.width = bitmap.width;
    image.height = bitmap.rows;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; 

    Color *pixels = (Color *)RL_CALLOC(image.width*image.height,sizeof(Color));
    TraceLog(LOG_INFO, "Size of pixels is %d", image.width*image.height);

    for (int y = 0; y < bitmap.rows; y++)
    {
        for (int x = 0; x < bitmap.width; x++)
        {
            int index = (x  + y * bitmap.width);
            //unsigned char intensity = bitmap.buffer[y * bitmap.pitch + x];
            unsigned char intensity = bitmap.buffer[index];
            //TraceLog(LOG_INFO, "intesity is %d, index is %d",intensity,index);
            pixels[index].r = intensity;
            pixels[index].g = intensity;
            pixels[index].b = intensity;
            pixels[index].a = intensity;
        }
    }

    image.data = pixels;
    return image;
}

// Example usage in your main game file (main.c)
int main(void)
{
    // Initialization
    InitWindow(800, 450, "Tamil HarfBuzz/FreeType Example");
    
    // Initialize the complex text system (Font must be compatible with Tamil, e.g., Kavivanar)
    InitComplexText("assets/Kavivanar-Regular.ttf", 40); // 40 is the pixel height

    SetTargetFPS(60);
    bool drawnonce=false;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        if(!drawnonce)
        {
            drawnonce = true;
            ClearBackground(RAYWHITE);

            // Draw the Tamil text using the custom function
            DrawTamilText("வணக்கம் உலகம்", (Vector2){100, 100}, RED);
            DrawText("This text is rendered with raylib's default font.", 100, 200, 20, DARKGRAY);
        }
        EndDrawing();
    }

    // Cleanup
    UnloadComplexText();
    CloseWindow();
    return 0;
}

void UnloadComplexText(void)
{
    if (hb_font) hb_font_destroy(hb_font);
    if (ft_face) FT_Done_Face(ft_face);
    if (ft_library) FT_Done_FreeType(ft_library);
}


void InitComplexText(const char *fontPath, int fontSize)
{
    // 1. Initialize FreeType
    if (FT_Init_FreeType(&ft_library)) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not init FreeType library.");
        return;
    }

    // 2. Load the font face
    if (FT_New_Face(ft_library, fontPath, 0, &ft_face)) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not load font face: %s", fontPath);
        return;
    }

    // 3. Set the font size
    // 64 is used because HarfBuzz and FreeType work in 1/64ths of a pixel internally
    if (FT_Set_Char_Size(ft_face, 0, fontSize * 64, 96, 96)) {
        TraceLog(LOG_WARNING, "FREETYPE: Could not set font size.");
    }
    
    // 4. Create HarfBuzz font object from the FreeType face
    hb_font = hb_ft_font_create_referenced(ft_face);
    
    TraceLog(LOG_INFO, "Complex Text Initialized successfully.");
}

void DrawTamilText(const char *text, Vector2 position, Color color)
{
    if (!hb_font) return;

    // --- A. SHAPING SETUP ---
    // 1. Create a HarfBuzz buffer
    hb_buffer_t *buffer = hb_buffer_create();

    // 2. Add text to the buffer and set script/direction
    hb_buffer_add_utf8(buffer, text, -1, 0, -1);
    hb_buffer_set_direction(buffer, HB_DIRECTION_LTR); // Tamil is Left-to-Right
    hb_buffer_set_script(buffer, HB_SCRIPT_TAMIL);    // Set the script to Tamil
    hb_buffer_set_language(buffer, hb_language_from_string("ta", -1)); // ta for Tamil

    // 3. Shape the text! (Applies ligatures, combining marks, etc.)
    hb_shape(hb_font, buffer, NULL, 0);

    // 4. Get the results
    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyph_count);
    
    float pen_x = position.x;
    float pen_y = position.y;

    TraceLog(LOG_INFO,"glyph count is %d",glyph_count);

    // --- B. RASTERIZATION AND DRAWING ---
    for (unsigned int i = 0; i < glyph_count; i++)
    {
        // 5. Calculate positioning using HarfBuzz output
        float x_offset = (float)(glyph_pos[i].x_offset / 64.0f);
        float y_offset = (float)(glyph_pos[i].y_offset / 64.0f);
        float x_advance = (float)(glyph_pos[i].x_advance / 64.0f);

        // 6. Load the glyph bitmap from FreeType
        FT_UInt glyph_index = glyph_info[i].codepoint;
        int errorCode = FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
        if (errorCode) continue;

        // 7. Render the glyph to a grayscale bitmap
        errorCode = FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL);
        if (errorCode) continue;

        // 8. Convert FreeType bitmap to raylib Image
        FT_Bitmap bitmap = ft_face->glyph->bitmap;
        Image glyphImage = BitmapToImage(bitmap);
        
        // 9. Create a Texture from the image (slow, should be optimized using a texture atlas!)
        Texture2D glyphTexture = LoadTextureFromImage(glyphImage);

        // 10. Draw the texture at the calculated position
        float draw_x = pen_x + x_offset + ft_face->glyph->bitmap_left;
        float draw_y = pen_y + y_offset - ft_face->glyph->bitmap_top; // Y is inverted in FreeType/Raylib context
        
        DrawTexture(glyphTexture, (int)draw_x, (int)draw_y, color);

        // 11. Cleanup (CRITICAL: Must be done after every glyph drawing)
        //UnloadTexture(glyphTexture);
        //UnloadImage(glyphImage);
        
        // 12. Advance the drawing pen position
        pen_x += x_advance;
    }
    
    // --- C. CLEANUP ---
    hb_buffer_destroy(buffer);
}
