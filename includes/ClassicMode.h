
#ifndef CLASSICMODE_H
#define CLASSICMODE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Includes.h"
#include "Event.h"
#include "GameObject.h"
#include "GameMode.h"
#include "Fleet.h"
#include "Star.h"
#include "Timer.h"
#include "Database.h"
#include "KeyboardInput.h"

const int SHIP_SPEED = 10;
const int LIVES      = 3;
const int COOLDOWN   = 1;

class ClassicMode : public GameMode
{
private:
    int lives, score, level;
    int shots_fired, shots_hit;
    double accuracy;

    bool game_over, back_to_menu;
    bool level_complete, ship_died;
    bool enter_name, name_entered;
    bool paused, shockwave_used;

    std::string score_text;
    std::string level_text;
    std::string message;

    KeyboardInput input;

    Image shockwave_img;
    Image flag_img;
    Sound shockwave_sound;
    Rect shockwave_rect;

public:
    ClassicMode(int w, int h, Surface *s, Database *d, std::fstream *f);

    void init();
    void update();
    void draw();
    void handle_input(Event &);

    bool is_paused() const { return paused; }
    bool is_over() const { return back_to_menu; }
};

#endif 
