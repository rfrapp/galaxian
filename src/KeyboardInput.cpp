
#include "KeyboardInput.h"

KeyboardInput::KeyboardInput() : keypressed(false), str(""), s(" ", "fonts/FreeSansBold.ttf", 24, 255, 0, 0)
{
    //SDL_EnableUNICODE(SDL_ENABLE);
}

void KeyboardInput::add(char c)
{
    std::string tmp = str;

    if (str.length() <= 14)
        str += c;

    if (tmp != str)
    {
        // Render a new text surface
        s.set_text(str);
    }
}

void KeyboardInput::clear()
{
    str = "";
    s.set_text(" ");
}

void KeyboardInput::handle_input(Event & e)
{
    if (e.type() == SDL_KEYDOWN && (e.get_key()->keysym.sym != SDLK_LSHIFT
                                    && e.get_key()->keysym.sym != SDLK_RSHIFT))
    {
        keypressed = true;
    }

    if (e.type() == SDL_KEYUP && keypressed)
    {
        std::string temp = str;

        if (temp.length() <= 14)
        {
            char key = 0;
            keypressed = false;
            //std::cout << "this key was pressed: " << (char)e.get_event()->key.keysym.unicode << std::endl;

            // If the key is a space
            if ((char)e.get_key()->keysym.sym == ' ')
            {
                key = ' ';
            }

            // If the key is a number
            else if ( ( (char)e.get_key()->keysym.sym >= (int)'1' ) && ( (char)e.get_key()->keysym.sym <= (int)'9' ) )
            {
                // Append the character
                //str += (char)e.get_key()->keysym.sym;
                key = (char)e.get_key()->keysym.sym;
            }
            //If the key is a lowercase letter
            else if( ( (char)e.get_key()->keysym.sym >= (int)'a' ) && ( (char)e.get_key()->keysym.sym <= (int)'z' ) )
            {
                //Append the character
                key = (char)e.get_key()->keysym.sym;
            }

            KeyPressed k = get_keypressed();
            if (k[SDLK_LSHIFT] || k[SDLK_RSHIFT])
            {
                key = (char)((int)key - 32);
            }

            if (key != 0)
                str += key;

        }

        // If backspace was pressed and the string isn't blank
        if ( ( e.get_key()->keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
        {
            // Remove a character from the end
            str.erase( str.length() - 1 );
        }

        // If the string was changed
        if ( str != temp )
        {
            if (str.length() == 0)
                s.set_text(" ");
            else
            {
                // Render a new text surface
                s.set_text(str);
            }
        }
    }
}
