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
                case Retry: return "Retry";
                case YourScore: return "Your Score : %d";
                case AIScore: return "AI Score : %d";
                case YourTurn: return "Your Turn";
                case AITurn: return "AI Turn";
                case YouWon: return "You Won!";
                case YouLost: return "You Lost!";
                default:
                    return "default text";
            }
            break;
        case Tamil:
            switch(text)
            {
                case Play: return "விளையாடு";
                case Exit: return "வெளியேறு";
                case Retry: return "மீண்டும் விளையாட முயற்சிக்கவும்";
                case YourScore: return "உங்கள் விதைகள்: %d";
                case AIScore: return "நண்பர் விதைகள்: %d";
                case YourTurn: return "உங்கள் முறை";
                case AITurn: return "நண்பரின் முறை";
                case YouWon: return "நீங்கள் வென்றீர்கள்!";
                case YouLost: return "நீ தோற்றுவிட்டாய்";
                default:
                    return "இயல்புநிலை உரை";
            }
            break;
    }
    return "DefaultText";
}
