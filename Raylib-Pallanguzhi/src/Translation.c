#include "LanguageSelection.h"
#include "Translation.h"

const char* GetText(TextID text)
{
    switch(GetLanguage())
    {
        case DefaultLanguage:
        case English:
            switch(text)
            {
                case Play: return "Play";
                case Exit: return "Exit";
                default:
                    return "";
            }
            break;
        case Tamil:
            switch(text)
            {
                case Play: return "விளையாடு";
                default:
                    return "";
            }
            break;
    }
    return "DefaultText";
}
