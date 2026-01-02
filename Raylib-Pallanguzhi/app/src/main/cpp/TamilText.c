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

// --- Simple Array Cache
#define MAX_GLYPHS 512 
Texture2D glyphCache[MAX_GLYPHS] = { 0 };

// --- Global Freetype/Harfbuzz objects ---
FT_Library ft_library;
FT_Face ft_face;
hb_font_t *hb_font;
static unsigned char* g_fontFileData = NULL;

static int fontSize = 40; // default font size

Image BitmapToImage(FT_Bitmap bitmap)
{
    Image image = { 0 };

    // Safety check for empty bitmaps
    if (bitmap.width == 0 || bitmap.rows == 0) {
        TraceLog(LOG_INFO, "the bitmap is empty");
        return GenImageColor(1, 1, BLANK);
    }

    image.width = bitmap.width;
    image.height = bitmap.rows;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; 

    Color *pixels = (Color*)RL_CALLOC(image.width*image.height,sizeof(Color));
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

void DrawTamilText(const char *text, Vector2 position, Color color)
{
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

    //TraceLog(LOG_INFO,"glyph count is %d",glyph_count);

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

Vector2 MeasureTamilText(const char *text)
{
    if (!hb_font) return (Vector2){0, 0};

    // --- 1. SHAPING SETUP (Same as DrawTamilText) ---
    hb_buffer_t *buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text, -1, 0, -1);
    hb_buffer_set_direction(buffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(buffer, HB_SCRIPT_TAMIL);
    hb_buffer_set_language(buffer, hb_language_from_string("ta", -1));
    hb_shape(hb_font, buffer, NULL, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buffer, &glyph_count);

    float pen_x = 0;
    float min_y = 0; // Tracks the highest point (most negative Y)
    float max_y = 0; // Tracks the lowest point (most positive Y)

    // --- 2. Iterate and Calculate Bounds ---
    for (unsigned int i = 0; i < glyph_count; i++)
    {
        FT_UInt glyph_index = glyph_info[i].codepoint;
        
        // HarfBuzz offsets (in 26.6 format)
        float y_offset = (float)(glyph_pos[i].y_offset / 64.0f);
        float x_advance = (float)(glyph_pos[i].x_advance / 64.0f);
        
        // Load the glyph metrics (we don't need the bitmap, just metrics)
        // FT_LOAD_NO_HINTING is faster when only metrics are needed
        if (FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_NO_HINTING) != 0) continue;

        // Calculate the top-most (min_y) and bottom-most (max_y) Y position
        // All values are calculated relative to the baseline (Y=0)

        // 1. Calculate the Y position of the glyph's top and bottom edge
        // Note: FreeType bitmap_top is distance UP from baseline (positive up).
        // The coordinate system needs to be flipped for min/max tracking.
        
        // The effective top of the glyph relative to the baseline (Screen Y-down)
        float glyph_top = -y_offset - ft_face->glyph->metrics.horiBearingY / 64.0f; 
        
        // The effective bottom of the glyph relative to the baseline (Screen Y-down)
        // horiBearingY - horiAdvance gives the height.
        float glyph_bottom = glyph_top + ft_face->glyph->metrics.height / 64.0f; 

        // 2. Track overall min/max bounds
        if (glyph_top < min_y) min_y = glyph_top;
        if (glyph_bottom > max_y) max_y = glyph_bottom;
        
        // 3. Accumulate total width
        pen_x += x_advance;
    }
    
    // --- 3. Final Cleanup and Return ---
    hb_buffer_destroy(buffer);

    return (Vector2){
        .x = pen_x,
        .y = max_y - min_y // Total vertical distance from top to bottom
    };
}

// ------------------------------------------------------------------------
// CLEANUP & INIT
// ------------------------------------------------------------------------
void InitComplexText(const char *fontPath, const int fntSize)
{
    fontSize = fntSize;
	// Clear cache
    for(int i=0; i<MAX_GLYPHS; i++) glyphCache[i].id = 0;

   // Initialize FreeType library
    if (FT_Init_FreeType(&ft_library)) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not init FreeType library.");
        return;
    }
    
    // Load font file into memory first (raylib handles Android assets)
    int fileSize = 0;
    unsigned char* fontFileData = LoadFileData(fontPath, &fileSize);
    
    if (fontFileData == NULL) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not load font file data: %s", fontPath);
        FT_Done_FreeType(ft_library);
        return;
    }
    
    TraceLog(LOG_INFO, "Font file loaded into memory: %u bytes", fileSize);
    
    // Create FreeType face from memory buffer instead of file path
    if (FT_New_Memory_Face(ft_library, fontFileData, fileSize, 0, &ft_face)) {
        TraceLog(LOG_FATAL, "FREETYPE: Could not load font face from memory: %s", fontPath);
        UnloadFileData(fontFileData);
        FT_Done_FreeType(ft_library);
        return;
    }
    
    // NOTE: We need to keep fontFileData alive as long as ft_face is in use
    // Store it in a static/global variable so it persists
    // DO NOT call UnloadFileData(fontFileData) here!
    
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
    
    // Destroy HarfBuzz font
    if (hb_font) {
        hb_font_destroy(hb_font);
        hb_font = NULL;
    }
    
    // Destroy FreeType face
    if (ft_face) {
        FT_Done_Face(ft_face);
        ft_face = NULL;
    }
    
    // Destroy FreeType library
    if (ft_library) {
        FT_Done_FreeType(ft_library);
        ft_library = NULL;
    }
    
    // Free the font file data that was loaded into memory
    if (g_fontFileData) {
        UnloadFileData(g_fontFileData);
        g_fontFileData = NULL;
    }
    
    TraceLog(LOG_INFO, "Complex Text Unloaded successfully.");
}
