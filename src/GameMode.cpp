#include "GameMode.h"

GameMode::GameMode(int w, int h, Surface *s, Database *d, std::fstream *f) : width(w), height(h), screen(s),
                                        num_aliens(60), per_row(10), file(f),
                                        transition_timer(5),
                                        laser_sound("sounds/laser.wav"),
                                        explosion_sound("sounds/explosion.wav"),
                                        spaceship_image("images/galaxian/GalaxianGalaxip.gif"),
                                        COOLDOWN(1), 
                                        weapon_cooldown(COOLDOWN),
                                        alien_blue_image("images/galaxian/GalaxianAquaAlien.gif"),
                                        fleet(num_aliens, per_row, width, height), missile_speed(7),
                                        respawn_timer(2),
                                        alien_red_image("images/galaxian/GalaxianRedAlien.gif"),
                                        alien_flag_image("images/galaxian/GalaxianFlagship.gif"),
                                        alien_purple_image("images/galaxian/GalaxianPurpleAlien.gif"),
                                        explosion_sprites("images/galaxian/explosion_sprites.png"),
                                        dead_timer(1), current_explosion_clip(0), ANI_SWAP_TIME(50),
                                        SHIP_SPEED(2), initialized(false), db(d)
{
}

void GameMode::init()
{
    if (explosion_clips.size() == 0)
    {
        explosion_clips.push_back(Rect(77, 0, 27, 32));
        explosion_clips.push_back(Rect(36, 0, 28, 32));
        explosion_clips.push_back(Rect(0, 0, 26, 32));
    }

    weapon_cooldown.start();
    initialized = true;

    spaceship.rect = Rect(0, 0, spaceship_image.getRect().w, spaceship_image.getRect().h);
    spaceship.setPos(width / 2 - spaceship.rect.w, height - spaceship.rect.h  * 2 - 35); 
    spaceship.setSpeed(SHIP_SPEED, 0);

    fleet.initFleet(num_aliens, per_row);
}
