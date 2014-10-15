
#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <string>
#include "Includes.h"

class KeyboardInput
{
private:
    std::string str;
    TextSurface s;
    bool keypressed;

public:
    KeyboardInput();
    ~KeyboardInput()
    {
        //SDL_EnableUNICODE(SDL_DISABLE);
    }

    void add(char c);
    void handle_input(Event &);
    TextSurface & surface() { return s; }
    std::string text() const { return str; }
    void erase() { str.erase(str.length() - 1); }
    int length() const { return str.length(); }
    void clear();
};

#endif 
