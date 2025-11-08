#include "raylib.h"
#include "SlotSelector.h"
#include "Board.h"

const char* SELECTOR_TEXTURE_PATH = "SlotSelector.png";

void DrawSlotSelector(SlotSelector* slotSelector, Texture2D* slotSelectorTexture, Vector2 position)
{
    if(slotSelector->renderState == DontRender)
        return;
    //Draw the selection Texture
    if(slotSelector->currentIndex >= 0 && slotSelector->currentIndex < TOTAL_SLOTS)
        DrawTexture(*slotSelectorTexture, position.x - 50, position.y-50, BLUE);
}
