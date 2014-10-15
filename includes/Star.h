
#ifndef STAR_H
#define STAR_H

#include "Includes.h"
#include "GameObject.h"

class Star : public GameObject
{
public:
    Star() { color.r = 0; color.g = 0; color.b = 0; }
    Color color;
};

#endif