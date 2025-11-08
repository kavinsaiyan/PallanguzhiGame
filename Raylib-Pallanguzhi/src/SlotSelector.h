#include "Render.h"
#include "raylib.h"

#ifndef SLOT_SELECTOR_H
#define SLOT_SELECTOR_H

extern const char* SELECTOR_TEXTURE_PATH;

typedef struct 
{
    int currentIndex;
    RenderState renderState;
} SlotSelector;

void DrawSlotSelector(SlotSelector* slotSelector, Texture2D* slotSelectorTexture, Vector2 position);

#endif
