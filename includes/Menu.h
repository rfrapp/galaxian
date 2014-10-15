
#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Includes.h"
#include "Event.h"
#include "GameObject.h"
#include "TextSurface.h"
#include "GameMode.h"
#include "Fleet.h"
#include "Star.h"
#include "Timer.h"
#include "Database.h"

class Menu : public GameMode
{
private:
    int current_page, num_scores;
    bool open, showMain, showHighScores, q;
    TextSurface play, quit, hs;
    Image title_img;
    Rect play_rect, hs_rect, quit_rect, previous_rect, next_rect;
    Timer switch_timer;
    Timer attack_timer;
    std::string high_scores;

public:
    Menu(int w, int h, Surface *s, Database *d, std::fstream *f);
    bool is_open() const { return open; }
    bool game_closed() const { return q; }
    void show_scores(bool b) { showHighScores = true; showMain = false; };
    void close() { open = false; }
    void init();
    void draw();
    void update();
    void handle_input(Event &);
    void update_scores(int start = 0);
};

#endif
