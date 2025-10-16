#include "raylib.h"
#include "SlotSelector.h"
#include "Board.h"

const char* SELECTOR_TEXTURE_PATH = "Resources/SlotSelector.png";

void DrawSlotSelector(SlotSelector* slotSelector, Texture2D* slotSelectorTexture, Vector2 position)
{
    if(slotSelector->renderState == DontRender)
        return;
    //Draw the selection Texture
    int yOffset = slotSelector->currentIndex < TOTAL_SLOTS / 2 ? 4 : -2;
    if(slotSelector->currentIndex >= 0 && slotSelector->currentIndex < TOTAL_SLOTS)
        DrawTexture(*slotSelectorTexture, position.x - 50, position.y-50-yOffset, BLUE);
}
