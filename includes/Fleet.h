#include "GameObject.h"
#include <vector>
#include "Includes.h"
#include "Timer.h"

#ifndef FLEET_H 
#define FLEET_H

class Fleet 
{
private:
    int   width, height;
    int   num_aliens;
    int   per_row;
    int   alien_speed;
    int   alien_dy;
    int   attack_time;
    int   level;
    bool  move_right;
    int   moved_aliens;
    int   num_firing;
    int   times_attacked;
    int   attack_ms;
    Image alien_blue_image;
    Timer attack_timer;
    Sound alien_move_sound;

public:
    //std::vector< std::vector<Rect> > alien_positions;
    std::vector< std::vector<GameObject> > aliens;
    std::vector<Rect> alien_missiles; 
    
    void initFleet(int numAliens, int perRow, int lvl = 0);
    void move(const GameObject & spaceship);
    int  getNumLiveAliens() const;
    
    Fleet(int, int, int, int);
};

#endif
