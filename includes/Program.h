
#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <fstream>
#include <string>
#include "Includes.h"
#include "ClassicMode.h"
#include "Menu.h"
#include "Star.h"
#include "Database.h"

const int NUM_STARS = 200;

class Program
{
private:
    Database db;
    int width, height;
    int star_dy;
    bool is_playing = false;

    ClassicMode main_game;
    Menu menu;

    std::string title;
    Surface screen;
    SDL_Event event;
    Event e;
    Sound background_music;
    std::vector<Star> stars;
    std::fstream file;

public: 
    Program(int w, int h, std::string t);
    ~Program() { file.close(); }


    void execute();
};

#endif
