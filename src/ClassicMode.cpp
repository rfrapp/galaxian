
#include "ClassicMode.h"
#include "SDL_rotozoom.h"
#include <sstream>
 
ClassicMode::ClassicMode(int w, int h, Surface *s, Database *d, std::fstream *f) : GameMode(w, h, s, d, f), level_complete(false),
                                                     game_over(false), lives(3), score(0), level(0),
                                                     message(""), back_to_menu(false), enter_name(false),
                                                     shots_fired(0), shots_hit(0), accuracy(0.0), paused(false),
                                                     shockwave_img("images/galaxian/shockwave.png"),
                                                     flag_img("images/galaxian/galaxianflag.png"),
                                                     shockwave_sound("sounds/electric_shot.wav")
{ 
}

// Used to reset the game
void ClassicMode::init()
{
    GameMode::init();
    score = 0;
    shots_hit = 0;
    shots_fired = 0;
    accuracy = 0.0;
    name_entered = false;
    enter_name = false;
    game_over = false;
    back_to_menu = false;
    level_complete = false;
    lives = 3;
    level = 1;

    shockwave_rect.w = shockwave_img.get_surface()->w;
    shockwave_rect.h = shockwave_img.get_surface()->h;
    shockwave_rect.y = spaceship.rect.y - shockwave_rect.h;
}

void ClassicMode::update()
{
    if (lives == 0)
    {
        message = "You lost! Game over.";
        game_over = true;

        if (!transition_timer.isStarted())
            transition_timer.start();
    }

    if (shockwave_used)
    {
        shockwave_rect.y -= 15;

        if (shockwave_rect.y + shockwave_rect.h < 0)
        {
            shockwave_used = false;
            shockwave_rect.y = spaceship.rect.y - shockwave_rect.h;
            //std::cout << "killing first row aliens..." << std::endl;

            // kill all aliens in closest row
            for (int c = 0; c < fleet.aliens.size(); c++)
            {
                if (fleet.aliens[c].size() >= 1)
                {
                    fleet.aliens[c][fleet.aliens[c].size() - 1].setAlive(false);
                    fleet.aliens[c][fleet.aliens[c].size() - 1].dead_timer.start();
                    score -= fleet.aliens[c][fleet.aliens[c].size() - 1].score_points;
                }
            }
            //std::cout << "killed first row aliens..." << std::endl;

            //std::cout << "killing attacking aliens..." << std::endl;
            // kill all attacking aliens
            for (int c = 0; c < fleet.aliens.size(); c++)
            {
                for (int r = 0; r < fleet.aliens[c].size(); r++)
                {
                    if (fleet.aliens[c][r].get_state() != Normal &&
                        fleet.aliens[c][r].get_state() != Reset)
                    {
                        fleet.aliens[c][r].setAlive(false);
                        fleet.aliens[c][r].dead_timer.start();
                        score -= fleet.aliens[c][r].score_points;
                    }
                }
            }
            //std::cout << "killed attacking aliens..." << std::endl;

            explosion_sound.play(4);
        }
    }

    // move the missile_speed
    for (int i = 0; i < spaceship.missiles.size(); i++)
    {
        spaceship.missiles[i].y -= missile_speed;
        bool collided = false;

        if (!level_complete)
        {
            // check for collision with aliens
            for (int c = 0; c < fleet.aliens.size(); c++)
            {
                for (int r = 0; r < fleet.aliens[c].size(); r++)
                {
                    // Check if a missile hit an alien
                    if (spaceship.missiles[i].collide_rect(fleet.aliens[c][r].rect))
                    {
                        // Subtract 1 from the alien's hit points
                        fleet.aliens[c][r].hit_points--;

                        // Check if the alien's hit points are 0
                        // If so, kill the alien
                        if (fleet.aliens[c][r].hit_points == 0)
                        {
                            fleet.aliens[c][r].setAlive(false);
                            explosion_sound.play(4);
                            //fleet.alien_positions[c].erase(fleet.alien_positions[c].begin() + r);
                            //missiles.erase(missiles.begin() + i);
                            score += fleet.aliens[c][r].score_points;
                            collided = true;

                            if (!fleet.aliens[c][r].dead_timer.isStarted())
                                fleet.aliens[c][r].dead_timer.start();
                        }
                        continue;
                    }
                }
            }
        }

        // remove the missile if it hit an alien
        if (collided)
        {
            shots_hit++;
            spaceship.missiles.erase(spaceship.missiles.begin() + i);
        }
    }

    // remove the first missile in the list if
    // goes out of bounds
    if (spaceship.missiles.size() >= 1)
    {
        if (spaceship.missiles[0].y + spaceship.missiles[0].h <= 0)
            spaceship.missiles.erase(spaceship.missiles.begin());
    }

    // Move the fleet if the level is not complete
    if (!level_complete)
        fleet.move(spaceship);

    for (int c = 0; c < fleet.aliens.size(); c++)
    {
        for (int r = 0; r < fleet.aliens[c].size(); r++)
        {
            if (fleet.aliens[c][r].dead_timer.getTicks() >= ANI_SWAP_TIME)
            {
                fleet.aliens[c][r].current_clip++;
                dead_timer.start();
            }

            if (fleet.aliens[c][r].current_clip >= explosion_clips.size())
            {
                // The explosion has finished, remove
                // this alien from the list
                fleet.aliens[c].erase(fleet.aliens[c].begin() + r);

                // Check if the level is complete.
                // If so, start a new level
                if (fleet.getNumLiveAliens() == 0)
                {
                    message = "Fleet destroyed. Level complete!";
                    level_complete = true;
                    spaceship.rect.x = width / 2 - spaceship.rect.w / 2;
                    transition_timer.start();
                    fleet.initFleet(num_aliens, per_row, level + 1);
                    break;
                }
            }
        }
    }

    // check for if an alien hit the ship
    for (int c = 0; c < fleet.aliens.size(); c++)
    {
        for (int r = 0; r < fleet.aliens[c].size(); r++)
        {
            // check if an alien collided with the spaceship
            if (spaceship.rect.collide_rect(fleet.aliens[c][r].rect) && !game_over
                && spaceship.is_alive())
            {
                // Check if the alien is attacking
                if (fleet.aliens[c][r].get_state() != Normal)
                {
                    fleet.aliens[c][r].setAlive(false);
                    explosion_sound.play(4);
                    //fleet.aliens[c].erase(fleet.aliens[c].begin() + r);
                    //fleet.alien_positions[c].erase(fleet.alien_positions[c].begin() + r);

                    if (!fleet.aliens[c][r].dead_timer.isStarted())
                        fleet.aliens[c][r].dead_timer.start();
                }
                else // Fleet reached the bottom, reset level
                    fleet.initFleet(num_aliens, per_row, level);

                // Kill the ship
                spaceship.setAlive(false);
                explosion_sound.play(4);

                if (!spaceship.dead_timer.isStarted())
                    spaceship.dead_timer.start();
                break;
            }
        }
    }

    // check if alien missiles hit the spaceship
    for (int c = 0; c < fleet.aliens.size(); c++)
    {
        for (int r = 0; r < fleet.aliens[c].size(); r++)
        {
            for (int i = 0; i < fleet.aliens[c][r].missiles.size(); i++)
            {
                fleet.aliens[c][r].missiles[i].y += missile_speed;

                // Erase alien missile if it went off the screen
                if (fleet.aliens[c][r].missiles[i].y > height)
                {
                    fleet.aliens[c][r].missiles.erase(fleet.aliens[c][r].missiles.begin() + i);
                }

                // Check if an alien missile hit the ship
                if (fleet.aliens[c][r].missiles[i].collide_rect(spaceship.rect) && !game_over
                    && spaceship.is_alive())
                {
                    // Erase the missile and kill the ship
                    fleet.aliens[c][r].missiles.erase(fleet.aliens[c][r].missiles.begin() + i);
                    spaceship.setAlive(false);
                    
                    if (!spaceship.dead_timer.isStarted())
                        spaceship.dead_timer.start();
                    explosion_sound.play();
                }
            }
        }
    }
}

void ClassicMode::draw()
{
    // Draw the ship if the game isn't over
    if (!game_over)
    {
        if (shockwave_used)
        {
            screen->put_image(shockwave_img, shockwave_rect);
        }

        if (spaceship.is_alive())
        {
            // draws the spaceship
            screen->put_image(spaceship_image, spaceship.rect);
        }
        else // Ship is dead, draw explosion
        {
            if (spaceship.dead_timer.getTicks() >= ANI_SWAP_TIME)
            {
                current_explosion_clip++;
                spaceship.dead_timer.start();
            }

            if (current_explosion_clip < explosion_clips.size())
                screen->put_image(explosion_sprites, explosion_clips[current_explosion_clip], spaceship.rect);
            else
            {
                if (!respawn_timer.isStarted())
                    respawn_timer.start();
                
                if (respawn_timer.getTicks() >= 3000)
                {
                    respawn_timer.stop();
                    spaceship.dead_timer.reset();
                    spaceship.setAlive(true);
                    spaceship.rect.x = width / 2 - spaceship.rect.w / 2;
                    lives--;
                    current_explosion_clip = 0;

                    //std::cout << "lives " << lives << std::endl;
                }
            }
        }
    }

    // ------------------- Draw the life indicators ----------------------
    int startX = 15;
    int startY = height - spaceship.rect.h - 15;
    for (int i = 0; i < lives - 1; i++)
    {
        screen->put_image(spaceship_image, Rect(startX, startY, spaceship.rect.w, spaceship.rect.h));
        startX += spaceship.rect.w + 10;
    }
    // -------------------------------------------------------------------

    // ------------------- Draw missiles ---------------------------------
    for (int i = 0; i < spaceship.missiles.size(); i++)
    {
        screen->put_rect(spaceship.missiles[i], BRIGHT_YELLOW);
    }
    // -------------------------------------------------------------------

    // ------------------- Draw alien missiles ---------------------------
    for (int c = 0; c < fleet.aliens.size(); c++)
    {
        for (int r = 0; r < fleet.aliens[c].size(); r++)
        {
            for (int i = 0; i < fleet.aliens[c][r].missiles.size(); i++)
            {
                screen->put_rect(fleet.aliens[c][r].missiles[i], WHITE);
            }
        }
    }
    // -------------------------------------------------------------------

    // ------------------- Draw aliens -----------------------------------
    if (!level_complete)
    {
        for (int c = 0; c < fleet.aliens.size(); c++)
        {
            for (int r = 0; r < fleet.aliens[c].size(); r++)
            {
                // Draw an alien image if this particular
                // alien is alive
                if (fleet.aliens[c][r].is_alive())
                {
                    if (fleet.aliens[c][r].get_state() == Normal ||
                        fleet.aliens[c][r].get_state() == Reset)
                    {
                        // Draw the right image based on the alien's 
                        // type
                        if (fleet.aliens[c][r].get_type() == "blue")
                            screen->put_image(alien_blue_image, fleet.aliens[c][r].rect);
                        else if (fleet.aliens[c][r].get_type() == "purple")
                            screen->put_image(alien_purple_image, fleet.aliens[c][r].rect);
                        else if (fleet.aliens[c][r].get_type() == "flagship")
                            screen->put_image(alien_flag_image, fleet.aliens[c][r].rect);
                        else
                            screen->put_image(alien_red_image, fleet.aliens[c][r].rect);
                    }
                    else
                    {
                        // Draw the right image based on the alien's 
                        // type
                        if (fleet.aliens[c][r].get_type() == "blue")
                        {
                            Image rotated = Image(rotozoomSurface(alien_blue_image.get_surface(), 
                                                            fleet.aliens[c][r].angle, 1.0, 0));

                            screen->put_image(rotated, fleet.aliens[c][r].rect);
                        }
                        else if (fleet.aliens[c][r].get_type() == "purple")
                        {
                            Image rotated = Image(rotozoomSurface(alien_purple_image.get_surface(), 
                                                            fleet.aliens[c][r].angle, 1.0, 0));
                            screen->put_image(rotated, fleet.aliens[c][r].rect);
                        }
                        else if (fleet.aliens[c][r].get_type() == "flagship")
                        {
                            Image rotated = Image(rotozoomSurface(alien_flag_image.get_surface(), 
                                                            fleet.aliens[c][r].angle, 1.0, 0));
                            screen->put_image(rotated, fleet.aliens[c][r].rect);
                        }
                        else
                        {
                            Image rotated = Image(rotozoomSurface(alien_red_image.get_surface(), 
                                                            fleet.aliens[c][r].angle, 1.0, 0));
                            screen->put_image(rotated, fleet.aliens[c][r].rect);   
                        }
                    }
                }
                else // The alien is dead, draw explosion
                {
                    screen->put_image(explosion_sprites, explosion_clips[fleet.aliens[c][r].current_clip], fleet.aliens[c][r].rect);
                }
            }
        }
    }
    // -------------------------------------------------------------------

    // ------------------- Draw score ------------------------------------
    // create score string
    std::stringstream s;
    s << score;
    score_text = s.str();
    TextSurface t("High Score", "fonts/FreeSansBold.ttf", 18, 255, 255, 255);

    screen->put_text(t, width / 2 - t.get_surface()->w / 2, 10);

    TextSurface t1(score_text.c_str(), "fonts/FreeSansBold.ttf", 18, 255, 0, 0);
    screen->put_text(t1, width / 2 - t1.get_surface()->w / 2, 40);
    // -------------------------------------------------------------------
 
    // ------------------- Draw level ------------------------------------
    // create score string
    // std::stringstream str;
    // str << "Level: " << level;

    // level_text = str.str();

    // screen->put_text(level_text.c_str(), width / 2 + 150, startY - 10, 255, 255, 102, "fonts/FreeSansBold.ttf", spaceship.rect.h);
    for (int i = 0; i < level; i++)
    {
        int x = width - flag_img.get_surface()->w - 10 - (flag_img.get_surface()->w + 10) * i;
        screen->put_image(flag_img, Rect(x, height - flag_img.get_surface()->h - 10,
                                         flag_img.get_surface()->w, flag_img.get_surface()->h));
    }

    // -------------------------------------------------------------------

    // ------------------- Draw message ----------------------------------
    if (level_complete || game_over)
    {
        TextSurface t(message.c_str(), "fonts/FreeSansBold.ttf", 24, 255, 255, 255);
        
        if (transition_timer.getSeconds() > 0)
        {
            screen->put_text(t, width / 2 - t.get_surface()->w / 2, height / 2 - t.get_surface()->h);
            //screen->put_text(message.c_str(), width / 2 - 125, height / 2, 255, 255, 102, "fonts/FreeSansBold.ttf", 18);
        }
        else
        {
            if (level_complete)
            {
                level_complete = false;
                transition_timer.stop();
                level++;
            }
            else if (game_over)
            {
                //back_to_menu = true;
                enter_name = true;
                TextSurface n("Enter your name", "fonts/FreeSansBold.ttf", 24, 255, 255, 255);
                screen->put_text(n, width / 2 - n.get_surface()->w / 2, height / 2 - (n.get_surface()->h + 50));

                screen->put_text(input.surface(), width / 2 - input.surface().get_surface()->w / 2, height / 2 - input.surface().get_surface()->h);
            }
        }
    }
    // -------------------------------------------------------------------
}

void ClassicMode::handle_input(Event & event)
{
    KeyPressed keypressed = get_keypressed();

    // Only move the ship if the game is not over
    // and if the level is not complete
    if (!level_complete && !game_over && spaceship.is_alive())
    {
        // move the ship left when the left arrow
        // is pressed
        if (keypressed[LEFTARROW])
        {
            spaceship.move(-spaceship.get_xspeed(), 0);

            // keep the ship in bounds
            if (spaceship.rect.x < 0)
                spaceship.rect.x = 0;
        }

        // move the ship right when the 
        // right arrow key is pressed
        else if (keypressed[RIGHTARROW])
        {
            spaceship.move();

            // keep the ship in bounds
            if (spaceship.rect.right() > width)
                spaceship.rect.x = width - spaceship.rect.w;
        }

        // shoot a laser if the weapon cooldown 
        // is finished
        if (keypressed[SPACE])
        {
            //std::cout << "weapon_cooldown: " << weapon_cooldown.getTicks() << std::endl;
            if (spaceship.can_fire())
            {
                //laser_sound.play();
                shots_fired++;
                Rect r(spaceship.rect.x + spaceship.rect.w / 2 - 1, spaceship.rect.y - 6, 2, 6);
                spaceship.missiles.push_back(r);
                laser_sound.play(3);
            }
        }

        if (keypressed[SDLK_k])
        {
            for (int c = 0; c < fleet.aliens.size(); c++)
            {
                for (int r = 0; r < fleet.aliens[c].size(); r++)
                {
                    fleet.aliens[c][r].setAlive(false);
                    fleet.aliens[c][r].dead_timer.start();
                }
            }
        }

        if (keypressed[SDLK_UP])
        {
            shockwave_used = true;
            shockwave_sound.play(5);
        }

        if (keypressed[SDLK_p])
        {
            // paused = (paused) ? false : true;
            if (paused)
                paused = false;
            else
                paused = true;
        }
    }

    if (game_over && enter_name)
    {
        if (event.get_key()->keysym.sym != SDLK_RETURN)
            input.handle_input(event);
        else
        {
            if (!name_entered)
            {
                if (shots_fired != 0)
                    accuracy = (double) shots_hit / shots_fired;
                
                if (db->connected())
                {
                    std::stringstream q;

                    // build the query
                    q << "INSERT INTO scores (name, score, accuracy) VALUES ('" << input.text()
                      << "', '" << score << "', '" << accuracy * 100 << "')"; 

                    try 
                    {
                        //std::cout << "inserting row..." << std::endl;
                        db->query(q.str());
                        name_entered = true;
                        back_to_menu = true;
                        //std::cout << "row inserted!" << std::endl;
                    }
                    catch (sql::SQLException &e)
                    {
                        std::cout << "# ERR: SQLException in " << __FILE__;
                        std::cout << "(" << __FUNCTION__ << ") on line "
                        << __LINE__ << std::endl;
                        std::cout << "# ERR: " << e.what();
                        std::cout << " (MySQL error code: " << e.getErrorCode();
                        std::cout << ", SQLState: " << e.getSQLState() <<
                        " )" << std::endl;
                        //std::cout << e << std::endl;
                    }
                }
                else 
                {
                    int insert_index = 0;
                    std::ofstream f;
                    std::ifstream in;
                    in.open("scores.txt");

                    std::vector<std::string> score_lines;
                    std::string line;

                    // get the lines in the file
                    while (getline(in, line))
                    {
                        score_lines.push_back(line);
                    }

                    in.close();
                    f.open("scores.txt");

                    //std::cout << "scores before: " << std::endl;

                    // output score before manipulation
                    // for (int i = 0; i < score_lines.size(); i++)
                    // {
                    //     std::cout << score_lines[i] << std::endl;
                    // }

                    // search through the lines from top down
                    for (int i = score_lines.size() - 1; i >= 0; i--)
                    {
                        bool found = false;
                        std::stringstream s(score_lines[i]);
                        std::string item;
                        int item_count = 0;

                        // loop through each item
                        // Name;Score;Accuracy
                        while (getline(s, item, ';'))
                        {
                            // look at the score section of the line
                            if (item_count == 1)
                            {
                                if (score <= atoi(item.c_str()))
                                {
                                    insert_index = i + 1;
                                    found = true;
                                    break;
                                }
                            }
                            item_count++;
                        }

                        if (found)
                            break;
                    }

                    std::stringstream str;
                    str << input.text() << ';' << score << ';' << accuracy * 100;
                    score_lines.insert(score_lines.begin() + insert_index, str.str());

                    // std::cout << "scores after: " << std::endl;
                    // // output score after manipulation
                    // for (int i = 0; i < score_lines.size(); i++)
                    // {
                    //     std::cout << score_lines[i] << std::endl;
                    // }

                    // append the new entry to the file
                    for (int i = 0; i < score_lines.size(); i++)
                    {
                        f << score_lines[i].c_str() << '\n';
                    }

                    f.close();

                    name_entered = true;
                    back_to_menu = true;
                }

                initialized = false;
            }
        }

        //std::cout << (char)event.get_key()->keysym.unicode << std::endl;
        //std::cout << "keypressed: " << (char)event.get_event()->key.keysym.unicode << std::endl;
    }
}
