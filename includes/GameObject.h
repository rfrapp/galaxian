
#include <iostream>
#include <string>
#include <vector>
#include "Includes.h"
#include "Timer.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

const int ALIEN_ATTACK_UP = 32;
const int ALIEN_ATTACK_SIDE = 96;

enum States
{
    Normal = 0,                         // When the alien is moving in the fleet
    Attack_Move_Up,                     // When alien moves up to start attack
    Attack_Move_Side_Down,              // When alien moves to one side after moving up
    Attack_Seek,                        // When alien is attacking, locks on to ship
    Reset                               // When the alien is returning back to the fleet
};

class GameObject
{
private:
    std::string type;
    int xspeed, yspeed;
    States state;
    // bool attack_mode;
    // bool fire_missile;
    // bool reset;
    bool alive;

public:
    //Image image;
    Rect rect;
    Rect original_rect;
    int score_points;
    int hit_points;
    int times_moved;
    int current_clip;
    int angle;
    int fire_cooldown;
    bool rotate_left;
    Timer dead_timer;
    Timer cooldown_timer;
    std::vector<Rect> missiles;

    GameObject() : xspeed(0), yspeed(0), hit_points(1), type(""),
                   times_moved(0), alive(true), current_clip(0),
                   dead_timer(0), state(Normal), angle(0), rotate_left(false),
                   cooldown_timer(1), fire_cooldown(500)
    { 
        cooldown_timer.start();
    } 

    //void draw(Surface *dst) { dst->put_image(image, rect); }
    void move(int x, int y) { rect.x += x; rect.y += y; }
    void move() { rect.x += xspeed; rect.y += yspeed; }

    void setPos(int x, int y) { rect.x = x; rect.y = y; }
    void setSpeed(int x, int y) { xspeed = x; yspeed = y; }
    void set_cooldown(int c) { fire_cooldown = c; }
    //void setImage(const Image & img) { image = img; }
    void setRect(const Rect & r)  { rect  = r;   }
    void setType(const std::string s) { type = s; }
    // void setAttackMode(bool b) { attack_mode = b; }
    // void setReset(bool b) { reset = b; }
    // void setFireMissile(bool b) { fire_missile = b; }
    
    void setAlive(bool b) { alive = b; }
    void set_state(States s) { state = s; }

    int get_xspeed() const { return xspeed; }
    int get_yspeed() const { return yspeed; }
    std::string get_type() const { return type; }

    States get_state() const { return state; }
    // bool get_attack_mode() const { return attack_mode; }
    // bool get_reset() const { return reset; }
    // bool get_fire_missile() const { return fire_missile; }
    bool is_alive() const { return alive; }

    bool can_fire()
    {
        if (cooldown_timer.getTicks() >= fire_cooldown)
        {
            cooldown_timer.start();
            return true;
        }
        return false;
    }
};

#endif
