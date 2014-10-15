
#ifndef GAME_MODE_H
#define GAME_MODE_H

#include <iostream>
#include <string>
#include <vector>
#include "Includes.h"
#include "Event.h"
#include "GameObject.h"
#include "Fleet.h"
#include "Timer.h"
#include "Database.h"
#include <fstream>

class GameMode
{
protected:
    Database *db;
    std::fstream *file;
    Surface *screen;
    Image spaceship_image;
    Image explosion_sprites;
    Image alien_blue_image;
    Image alien_purple_image;
    Image alien_red_image;
    Image alien_flag_image;
    std::vector<Rect> explosion_clips;
    Sound laser_sound;
    Sound explosion_sound;

    int width, height;
    int num_aliens, per_row;
    int missile_speed;
    int current_explosion_clip;
    int ANI_SWAP_TIME;
    int COOLDOWN;
    int SHIP_SPEED;
    bool initialized;

    Timer transition_timer;
    Timer weapon_cooldown;
    Timer dead_timer;
    Timer respawn_timer;
    Fleet fleet;
    std::vector<Rect> missiles;
    GameObject spaceship;

public:
    GameMode(int w, int h, Surface *s, Database *d, std::fstream *f);
    virtual void init();
    bool is_init() const { return initialized; }
    bool set_init(bool b) { initialized = b; }
};

#endif
