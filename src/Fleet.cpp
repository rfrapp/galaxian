#include "Fleet.h"
#include "SDL_rotozoom.h"


Fleet::Fleet(int n, int p, int w, int h) : num_aliens(n), per_row(p), alien_blue_image(Image("images/galaxian/GalaxianAquaAlien.gif")),
                 move_right(true), alien_speed(1), alien_dy(3),
                 attack_time(8), attack_timer(attack_time),
                 moved_aliens(0), width(w), height(h), level(0), num_firing(1),
                 alien_move_sound("sounds/galaxian_sound.wav"), attack_ms(5000),
                 times_attacked(0)
{
}

int Fleet::getNumLiveAliens() const
{
    int n = 0;
    for (int i = 0; i < aliens.size(); i++)
    {
        for (int j = 0; j < aliens[i].size(); j++)
        {
            n++;
        }
    }
    return n;
}

void Fleet::initFleet(int numAliens, int perRow, int lvl)
{ 
    alien_move_sound.stop();

    if (lvl > level)
    {
        level = lvl;

        if (level % 2 == 0)
        {
            if (attack_ms >= 1000)
                attack_ms -= 500;
            else 
            {
                if (attack_ms >= 20)
                    attack_ms -= 10;
            }
        }

        if (level % 5 == 0)
        {
            alien_dy++;
        }
    }

    attack_timer.stop();
    
    // clear the aliens and their positions
    aliens.clear();
    //alien_positions.clear();

    // set the number of aliens total and per row
    num_aliens = numAliens;
    per_row    = perRow;

    // set the width and height for the alien images
    int w = alien_blue_image.getRect().w;
    int h = alien_blue_image.getRect().h;

    // loop through columns
    for (int c = 0; c < per_row; c++)
    {
        // vectors to store the column of aliens and their positions
        std::vector<GameObject> alien_col;
        std::vector<Rect> alien_rect_col;

        // the x position of the column
        int x = (10 * c) + c * w;

        // loop througb rows
        for (int r = 0; r < num_aliens / per_row; r++)
        {
            if ((c == 0 || c == per_row - 1) && r <= 2)
                continue;
            else if ((c == 1 || c == per_row - 2) && r <= 1)
                continue;
            else if ((c == 2 || c == per_row - 3) && r == 0)
                continue;

            // the alien y position for the row
            int y = 80 + r * h;

            GameObject alien;

            if (r == 0)
            {
                alien.setType("flagship");
                alien.score_points = 25;
            }
            else if (r == 1)
            {
                alien.setType("red");
                alien.score_points = 20;
            }
            else if (r == 2)
            {
                alien.setType("purple");
                alien.score_points = 15;
            }
            else
            {
                alien.setType("blue");
                alien.score_points = 10;
            }

            Rect rect(0, 80, w, h);
            alien.set_state(Reset);
            alien.setSpeed(alien_speed, 0);
            alien.setRect(rect);
            
            rect.x = x;
            rect.y = y;

            alien.original_rect = rect;
            alien.set_cooldown(1000);
            alien_col.push_back(alien);
        }

        aliens.push_back(alien_col);
        //alien_positions.push_back(alien_rect_col);
    }
}

void Fleet::move(const GameObject & spaceship)
{
    if (!attack_timer.isStarted())
        attack_timer.start();

    bool bounce = false;
    int furthest_col_left  = -1;
    int furthest_col_right = -1;
    int attacking_count = 0;

    // check if there are no aliens attacking
    for (int c = 0; c < aliens.size(); c++)
    {
        for (int r = 0; r < aliens[c].size(); r++)
        {
            if (aliens[c][r].get_state() != Normal && 
                aliens[c][r].get_state() != Reset)
                attacking_count++;
        }
    }

    if (attacking_count == 0)
        alien_move_sound.stop();

    // get the column with aliens furthest to 
    // the left
    for (int i = 0; i < aliens.size(); i++)
    {
        if (aliens[i].size() > 0)
        {
            furthest_col_left = i;
            break;
        }
    }

    // get the column with aliens furthest to
    // the right
    for (int i = aliens.size() - 1; i >= 0; i--)
    {
        if (aliens[i].size() > 0)
        {
            furthest_col_right = i;
            break;
        }
    }

    if (furthest_col_left != -1 && furthest_col_right != -1)
    {
        // check if the furthet column to the right has hit the width
        if (move_right && aliens[furthest_col_right][0].original_rect.right() >= width)
        {
            move_right = false;
            bounce = true;
        }

        // check if the furthest column to the left has hit th width
        else if (!move_right && aliens[furthest_col_left][0].original_rect.x <= 0)
        {
            move_right = true;
            bounce = true;
        }
    }

    // move aliens down if they hit a boundary
    if (bounce)
    {
        for (int i = aliens.size() - 1; i >= 0; i--)
        {
            for (int j = 0; j < aliens[i].size(); j++)
            {
                if (aliens[i][j].get_state() == Normal)
                    aliens[i][j].move(0, alien_dy);
                
                aliens[i][j].original_rect.y += alien_dy;
            }
        }
    }

    // move aliens
    for (int i = aliens.size() - 1; i >= 0; i--)
    {
        for (int j = 0; j < aliens[i].size(); j++)
        {
            if (aliens[i][j].is_alive())
            {
                if (aliens[i][j].get_state() == Normal)
                {
                    if (move_right)
                    {
                        aliens[i][j].move();
                        aliens[i][j].original_rect.x += aliens[i][j].get_xspeed();
                    }
                    else
                    {
                        aliens[i][j].move(-aliens[i][j].get_xspeed(), 0);
                        aliens[i][j].original_rect.x -= aliens[i][j].get_xspeed();
                    }
                }
                else
                {
                    if (aliens[i][j].can_fire() && aliens[i][j].get_state() != Reset)
                    {
                        int x = aliens[i][j].rect.x + aliens[i][j].rect.w / 2 - 1;
                        int y = aliens[i][j].rect.y + aliens[i][j].rect.h / 2;
                        Rect r(x, y, 3, 6);
                        aliens[i][j].missiles.push_back(r);
                    }

                    if (move_right)
                        aliens[i][j].original_rect.x += aliens[i][j].get_xspeed();
                    else
                        aliens[i][j].original_rect.x -= aliens[i][j].get_xspeed();

                    if (aliens[i][j].get_state() == Attack_Move_Up)
                    {
                        // move alien up
                        aliens[i][j].move(0, -1);

                        // if the alien has moved up as much as it should
                        if (aliens[i][j].original_rect.y >= aliens[i][j].rect.y + ALIEN_ATTACK_UP
                            || aliens[i][j].rect.y < 0)
                        {
                            // change the alien's state to attack while moving
                            // to the side
                            aliens[i][j].set_state(Attack_Move_Side_Down);

                            // determines which way the alien will rotate
                            if (aliens[i][j].rect.x < width / 2)
                                aliens[i][j].rotate_left = true;
                            else
                                aliens[i][j].rotate_left = false;
                        }
                    }
                    else if (aliens[i][j].get_state() == Attack_Move_Side_Down)
                    {
                        // rotate alien, keep at 180 or -180 degrees
                        if (aliens[i][j].rotate_left && aliens[i][j].angle != -180)
                            aliens[i][j].angle -= 5;
                        else
                        {
                            if (aliens[i][j].angle != 180)
                                aliens[i][j].angle += 5;
                        }

                        // move the alien, switch state seek the ship when moved the correct amound
                        if (aliens[i][j].rotate_left)
                        {
                            aliens[i][j].move(-1, alien_dy);

                            if (aliens[i][j].original_rect.x >= aliens[i][j].rect.x + ALIEN_ATTACK_SIDE)
                                aliens[i][j].set_state(Attack_Seek);
                        }
                        else 
                        {
                            aliens[i][j].move(1, alien_dy);

                            if (aliens[i][j].original_rect.x <= aliens[i][j].rect.x - ALIEN_ATTACK_SIDE)
                                aliens[i][j].set_state(Attack_Seek);
                        }
                    }
                    else if (aliens[i][j].get_state() == Attack_Seek)
                    {
                        // moves the alien towards the ships x
                        if (aliens[i][j].rect.x < spaceship.rect.x)
                            aliens[i][j].rect.x += 2;
                        else if (aliens[i][j].rect.x > spaceship.rect.x)
                            aliens[i][j].rect.x -= 2;

                        aliens[i][j].move(0, alien_dy);
                    }
                }
            }
        }
    }

    for (int c = 0; c < aliens.size(); c++)
    {
        for (int r = 0; r < aliens[c].size(); r++)
        {
            if (aliens[c][r].get_state() != Normal && 
                aliens[c][r].get_state() != Reset)
            {
                if (aliens[c][r].rect.y > height)
                {
                    aliens[c][r].set_state(Reset);
                    aliens[c][r].rect.y = -aliens[c][r].rect.h;
                }
            }

            if (aliens[c][r].get_state() == Reset)
            {
                if (aliens[c][r].rect.x < aliens[c][r].original_rect.x)
                {
                    if (aliens[c][r].original_rect.x - aliens[c][r].rect.x < aliens[c][r].get_xspeed() * 4)
                        aliens[c][r].rect.x += aliens[c][r].original_rect.x - aliens[c][r].rect.x;
                    else
                        aliens[c][r].rect.x += aliens[c][r].get_xspeed() * 4;
                }
                else if (aliens[c][r].rect.x > aliens[c][r].original_rect.x)
                {
                    if (aliens[c][r].rect.x - aliens[c][r].original_rect.x < aliens[c][r].get_xspeed() * 4)
                        aliens[c][r].rect.x -= aliens[c][r].rect.x - aliens[c][r].original_rect.x;
                    else
                        aliens[c][r].rect.x -= aliens[c][r].get_xspeed() * 4;
                }

                if (aliens[c][r].rect.y < aliens[c][r].original_rect.y)
                {
                    if (aliens[c][r].original_rect.y - aliens[c][r].rect.y < alien_dy)
                        aliens[c][r].rect.y += aliens[c][r].original_rect.y - aliens[c][r].rect.y;
                    else
                        aliens[c][r].rect.y += alien_dy;
                }

                if (aliens[c][r].rect.x == aliens[c][r].original_rect.x && 
                    aliens[c][r].rect.y == aliens[c][r].original_rect.y)
                {
                    // std::cout << "alien back (" << aliens[c][r].rect.x
                    //           << " , " << aliens[c][r].rect.y << ')' 
                    //           << ' ' << '(' << aliens[c][r].original_rect.x 
                    //           << " , " << aliens[c][r].original_rect.y <<  ')' << std::endl;
                    aliens[c][r].set_state(Normal);
                    aliens[c][r].angle = 0;
                    aliens[c][r].times_moved = 0;
                    alien_move_sound.off();
                }
            }
        }
    }

    if (attack_timer.getTicks() >= attack_ms)
    {
        attack_timer.reset();
        int rand_c = -1;
        int rand_r = -1;
        int num_moving = 1;

        while (1)
        {
            // pick random column of aliens
            if (aliens.size() > 1)
                rand_c = rand() % aliens.size();
            else
                rand_c = 0;

            if (aliens[rand_c].size() > 1)
                rand_r = rand() % aliens[rand_c].size();
            else
                rand_r = 0;

            if (aliens[rand_c][rand_r].is_alive() && aliens[rand_c][rand_r].get_state() == Normal
                || aliens[rand_c][rand_r].get_state() == Reset)
                break;

            if (getNumLiveAliens() == 1)
            {
                rand_c = 0;
                rand_r = 0;
                break;
            }
        }

        aliens[rand_c][rand_r].set_state(Attack_Move_Up);
        alien_move_sound.on();
        alien_move_sound.play();

        if (times_attacked % 3 == 0 && times_attacked != 0)
        {
            if (rand_c - 1 >= 0)
            {
                aliens[rand_c - 1][rand_r].set_state(Attack_Move_Up);
                num_moving++;
            }

            if (rand_c + 1 < aliens.size())
            {
                num_moving++;
                aliens[rand_c + 1][rand_r].set_state(Attack_Move_Up);
            }

            if (num_moving == 3)
            {
                if (rand_r - 1 >= 0)
                    aliens[rand_c][rand_r - 1].set_state(Attack_Move_Up);
                if (rand_r + 1 < aliens[rand_c].size())
                    aliens[rand_c][rand_r + 1].set_state(Attack_Move_Up);
            }
        }

        times_attacked++;
    }
}
