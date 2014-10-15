#include "Menu.h"
#include "SDL_rotozoom.h"

const int SCORES_PER_PAGE = 5;

Menu::Menu(int w, int h, Surface *s, Database *d, std::fstream *f) : GameMode(w, h, s, d, f), showMain(true),
                                       showHighScores(false), open(true), q(false),
                                       play("Play", "fonts/FreeSansBold.ttf", 24, 255, 255, 102),
                                       hs("High Scores", "fonts/FreeSansBold.ttf", 24, 255, 255, 102),
                                       quit("Quit", "fonts/FreeSansBold.ttf", 24, 255, 255, 102),
                                       switch_timer(5), attack_timer(3), current_page(1), num_scores(0),
                                       title_img("images/galaxian/galaxiantitle.png")
{
    init();
    switch_timer.start();
    attack_timer.start();
    spaceship.setSpeed(2, 0);

    
}

void Menu::init()
{
    //std::cout << "initializing menu..." << std::endl;
    GameMode::init();

    open = true;
    showMain = true;
    showHighScores = false;
    current_page = 1;

    // create the menu options
    // TextSurface t0("Play", "fonts/FreeSansBold.ttf", 24, 255, 255, 102);
    // TextSurface t1("High Scores", "fonts/FreeSansBold.ttf", 24, 255, 255, 102);
    // TextSurface t2("Quit", "fonts/FreeSansBold.ttf", 24, 255, 255, 102);

    //std::cout << "added menu options" << std::endl;

    // create the menu rects
    play_rect = Rect(width / 2 - play.get_surface()->w / 2, 
            height / 2 - play.get_surface()->h / 2 - 50,
            play.get_surface()->w, play.get_surface()->h);

    hs_rect = Rect(width / 2 - hs.get_surface()->w / 2, 
            height / 2 - hs.get_surface()->h / 2 + 25,
            hs.get_surface()->w, hs.get_surface()->h);

    quit_rect = Rect(width / 2 - quit.get_surface()->w / 2, 
            height / 2 - quit.get_surface()->h / 2 + 100,
            quit.get_surface()->w, quit.get_surface()->h);

    //std::cout << "added menu rects" << std::endl;
}

void Menu::draw()
{
    if (!showHighScores)
    {
        if (spaceship.is_alive())
        {
            // draws the spaceship
            screen->put_image(spaceship_image, spaceship.rect);
        }
        else // Ship is dead, draw explosion
        {
            screen->put_image(explosion_sprites, explosion_clips[current_explosion_clip], spaceship.rect);
        }

        // ------------------- Draw missiles ---------------------------------
        for (int i = 0; i < missiles.size(); i++)
        {
            screen->put_rect(missiles[i], BRIGHT_YELLOW);
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
        for (int c = 0; c < fleet.aliens.size(); c++)
        {
            for (int r = 0; r < fleet.aliens[c].size(); r++)
            {
                // Draw an alien image if this particular
                // alien is alive
                if (fleet.aliens[c][r].is_alive())
                {
                    if (fleet.aliens[c][r].get_state() == Normal
                        || fleet.aliens[c][r].get_state() == Reset)
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
        // -------------------------------------------------------------------

        // draw the title image
        screen->put_image(title_img, Rect(width / 2 - title_img.get_surface()->w / 2, 20, 
                          title_img.get_surface()->w, title_img.get_surface()->h));
    }

    // ------------------- Draw menu items -------------------------------
    if (showMain)
    {
        // draw play button
        screen->put_text(play, play_rect.x, play_rect.y);

        // draw high scores button
        screen->put_text(hs, hs_rect.x, hs_rect.y);

        // draw quit button
        screen->put_text(quit, quit_rect.x, quit_rect.y);
    }
    else if (showHighScores)
    {
        TextSurface t0("High Scores:", "fonts/FreeSansBold.ttf", 24, 255, 255, 102);
        screen->put_text(t0, width / 2 - t0.get_surface()->w / 2, 80);

        TextSurface t1("Press escape to return to the main menu", "fonts/FreeSansBold.ttf", 14, 255, 255, 102);
        screen->put_text(t1, 20, 20);

        TextSurface t2("Player", "fonts/FreeSansBold.ttf", 14, 255, 255, 102);
        screen->put_text(t2, 40, 140);

        TextSurface t3("Score", "fonts/FreeSansBold.ttf", 14, 255, 255, 102);
        screen->put_text(t3, 280, 140);

        TextSurface t4("Accuracy", "fonts/FreeSansBold.ttf", 14, 255, 255, 102);
        screen->put_text(t4, 520, 140);

        // draw the sores
        std::stringstream in(high_scores);
        std::string line;
        int row = 0;
        int i = 0;
        int y = 0;
        int x = 40;

        if (db->connected())
        {
            // loop through row result str by line
            while (std::getline(in, line))
            {
                // loop through row line by space
                std::string item;
                std::stringstream in1(line);
                while (std::getline(in1, item, ';'))
                {
                    y = (row + 1) * 40 + 140;
                    TextSurface t5(item.c_str(), "fonts/FreeSansBold.ttf", 14, 255, 255, 102);
                    screen->put_text(t5, x, y);

                    i++;
                    x += 240;
                }
                x = 40;
                row++;
            }
        }
        else
        {
            while (getline(in, line))
            {
                std::string item;
                std::stringstream in1(line);
                
                while (getline(in1, item, ';'))
                {
                    y = (row + 1) * 40 + 140;
                    TextSurface t5(item.c_str(), "fonts/FreeSansBold.ttf", 14, 255, 255, 102);
                    screen->put_text(t5, x, y);

                    i++;
                    x += 240;
                }   
                x = 40;
                row++;
            }
        }

        // display the current page of high scores
        std::stringstream pg_stream;
        pg_stream << current_page;
        TextSurface pg(pg_stream.str().c_str(), "fonts/FreeSansBold.ttf", 32, 255, 255, 102);

        // calculate the x to put the pg # at
        x = width / 2 - pg.get_surface()->w / 2;
        y = height - (pg.get_surface()->h + 40);

        // draw the page number
        screen->put_text(pg, x, y);

        // draw the back arrow if applicable
        if (current_page > 1)
        {
            TextSurface back("<", "fonts/FreeSansBold.ttf", 32, 255, 255, 102);
            screen->put_text(back, x - (back.get_surface()->w + 5), y);

            if (previous_rect.x != x - (back.get_surface()->w + 5))
                previous_rect.x = x - (back.get_surface()->w + 5);
            if (previous_rect.y != y)
                previous_rect.y = y;
            if (previous_rect.w != back.get_surface()->w)
                previous_rect.w = back.get_surface()->w;
            if (previous_rect.h != back.get_surface()->h)
                previous_rect.h = back.get_surface()->h;
        }
        else
        {
            previous_rect.x = -1;
            previous_rect.y = -1;
            previous_rect.w = 0;
            previous_rect.h = 0;
        }

        if (db->connected())
        {
            if (db->rows() > SCORES_PER_PAGE * current_page)
            {
                TextSurface forward(">", "fonts/FreeSansBold.ttf", 32, 255, 255, 102);
                screen->put_text(forward, x + (forward.get_surface()->w + 5), y);
                
                if (next_rect.x != x + forward.get_surface()->w + 5)
                    next_rect.x = x + forward.get_surface()->w + 5;
                if (next_rect.y != y)
                    next_rect.y = y;
                if (next_rect.w != forward.get_surface()->w)
                    next_rect.w = forward.get_surface()->w;
                if (next_rect.h != forward.get_surface()->h)
                    next_rect.h = forward.get_surface()->h;
            }
            else
            {
                next_rect.x = -1;
                next_rect.y = -1;
                next_rect.w = 0;
                next_rect.h = 0;
            }
        }
        else
        {
            if (num_scores > current_page * SCORES_PER_PAGE)
            {
                TextSurface forward(">", "fonts/FreeSansBold.ttf", 32, 255, 255, 102);
                screen->put_text(forward, x + (forward.get_surface()->w + 5), y);
                
                if (next_rect.x != x + forward.get_surface()->w + 5)
                    next_rect.x = x + forward.get_surface()->w + 5;
                if (next_rect.y != y)
                    next_rect.y = y;
                if (next_rect.w != forward.get_surface()->w)
                    next_rect.w = forward.get_surface()->w;
                if (next_rect.h != forward.get_surface()->h)
                    next_rect.h = forward.get_surface()->h;
            }
            else
            {
                next_rect.x = -1;
                next_rect.y = -1;
                next_rect.w = 0;
                next_rect.h = 0;
            }
        }
    }
    // -------------------------------------------------------------------

}

void Menu::update()
{
    if (!showHighScores)
    {
        if (!spaceship.is_alive())
        {
            if (spaceship.dead_timer.getTicks() >= ANI_SWAP_TIME)
            {
                current_explosion_clip++;
                if (spaceship.dead_timer.isStarted())
                    spaceship.dead_timer.start();
            }

            // reset the ship
            if (current_explosion_clip >= explosion_clips.size())
            {
                spaceship.setAlive(true);
                spaceship.rect.x = width / 2 - spaceship.rect.w / 2;
                current_explosion_clip = 0;
            }
        }

        // move the spaceship
        if (switch_timer.getSeconds() <= 0)
        {
            spaceship.setSpeed(-spaceship.get_xspeed(), 0);
            switch_timer.start();
        }

        spaceship.move();
        
        if (spaceship.rect.right() >= width || spaceship.rect.x < 0)
        {
            spaceship.setSpeed(-spaceship.get_xspeed(), 0);
            switch_timer.start();
        }

        if (attack_timer.getSeconds() <= 0)
        {
            Rect r(spaceship.rect.x + spaceship.rect.w / 2 - 1, 
                   spaceship.rect.y, 2, 6);
            missiles.push_back(r);
            attack_timer.start();
            laser_sound.play();
        }

        // move the missiles
        for (int i = 0; i < missiles.size(); i++)
        {
            missiles[i].y -= missile_speed;
            bool collided = false;

            // check for collision with aliens
            for (int c = 0; c < fleet.aliens.size(); c++)
            {
                for (int r = 0; r < fleet.aliens[c].size(); r++)
                {
                    // Check if a missile hit an alien
                    if (missiles[i].collide_rect(fleet.aliens[c][r].rect))
                    {
                        // Subtract 1 from the alien's hit points
                        fleet.aliens[c][r].hit_points--;

                        // Check if the alien's hit points are 0
                        // If so, kill the alien
                        if (fleet.aliens[c][r].hit_points == 0)
                        {
                            fleet.aliens[c][r].setAlive(false);
                            explosion_sound.play();
                            //fleet.alien_positions[c].erase(fleet.alien_positions[c].begin() + r);
                            //missiles.erase(missiles.begin() + i);
                            collided = true;

                            if (!fleet.aliens[c][r].dead_timer.isStarted())
                                fleet.aliens[c][r].dead_timer.start();
                        }
                        continue;
                    }
                }
            }

            // remove the missile if it hit an alien
            if (collided)
                missiles.erase(missiles.begin() + i);
        }

        // remove the first missile in the list if
        // goes out of bounds
        if (missiles.size() >= 1)
        {
            if (missiles[0].y + missiles[0].h <= 0)
                missiles.erase(missiles.begin());
        }

        // Move the fleet if the level is not complete
        fleet.move(spaceship);

        // check if an alien is alive
        for (int c = 0; c < fleet.aliens.size(); c++)
        {
            for (int r = 0; r < fleet.aliens[c].size(); r++)
            {
                if (!fleet.aliens[c][r].is_alive())
                {
                    if (fleet.aliens[c][r].dead_timer.getTicks() >= ANI_SWAP_TIME)
                    {
                        fleet.aliens[c][r].current_clip++;
                        if (fleet.aliens[c][r].dead_timer.isStarted())
                            fleet.aliens[c][r].dead_timer.start();
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
                            spaceship.rect.x = width / 2 - spaceship.rect.w / 2;
                            transition_timer.start();
                            fleet.initFleet(num_aliens, per_row);
                            break;
                        }
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
                if (spaceship.rect.collide_rect(fleet.aliens[c][r].rect)
                    && spaceship.is_alive())
                {
                    // Check if the alien is attacking
                    if (fleet.aliens[c][r].get_state() != Normal)
                    {
                        fleet.aliens[c][r].setAlive(false);
                        explosion_sound.play();
                        //fleet.aliens[c].erase(fleet.aliens[c].begin() + r);
                        //fleet.alien_positions[c].erase(fleet.alien_positions[c].begin() + r);

                        if (!fleet.aliens[c][r].dead_timer.isStarted())
                            fleet.aliens[c][r].dead_timer.start();
                    }
                    else // Fleet reached the bottom, reset level
                        fleet.initFleet(num_aliens, per_row);

                    // Kill the ship
                    if (!spaceship.dead_timer.isStarted())
                        spaceship.dead_timer.start();
                    spaceship.setAlive(false);
                    explosion_sound.play();
                    break;
                }
            }
        }

        // check if alien missiles hit the spaceship
        for (int i = 0; i < fleet.alien_missiles.size(); i++)
        {
            fleet.alien_missiles[i].y += missile_speed;

            // Erase alien missile if it went off the screen
            if (fleet.alien_missiles[i].y > height)
            {
                fleet.alien_missiles.erase(fleet.alien_missiles.begin() + i);
            }

            // Check if an alien missile hit the ship
            if (fleet.alien_missiles[i].collide_rect(spaceship.rect)
                && spaceship.is_alive())
            {
                // Erase the missile and kill the ship
                fleet.alien_missiles.erase(fleet.alien_missiles.begin() + i);
                spaceship.setAlive(false);
                explosion_sound.play();
                if (!spaceship.dead_timer.isStarted())
                    spaceship.dead_timer.start();
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
                    if (fleet.aliens[c][r].missiles[i].collide_rect(spaceship.rect) && spaceship.is_alive())
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
}

void Menu::update_scores(int start)
{
    if (db->connected())
    {
        // get high scores from database
        try 
        {
            //std::cout << "querying..." << std::endl;
            std::stringstream str;

            // build the select query
            str << "SELECT * FROM scores ORDER BY score DESC, accuracy DESC LIMIT " << start << ", "
                << SCORES_PER_PAGE;
            
            db->query(str.str());
            //std::cout << "queried!" << std::endl;
            //std::cout << "getting results..." << std::endl;
            high_scores = db->getQueryRows();
            //std::cout << "results stored!" << std::endl;

            db->query("SELECT * FROM scores ORDER BY score DESC, accuracy DESC");
        }
        catch (const char *e)
        {
            std::cout << e << std::endl;
        }
    }
    else
    {
        high_scores = "";

        if (!file->is_open())
            file->open("scores.txt");

        num_scores = 0;

        //std::cout << "getting scores from db" << std::endl;

        // load scores from a file
        std::string line;
        
        // get each line from the file
        while (getline(*file, line))
        {
            if (line != "")
            {
                num_scores++;

                if (num_scores >= start && num_scores < start + SCORES_PER_PAGE)
                {
                    high_scores += line + '\n';
                }
            }
        }

        //std::cout << num_scores << std::endl;
    }
}

void Menu::handle_input(Event & event)
{
    if (event.type() == MOUSEBUTTONDOWN)
    {
        int x = mouse_x();
        int y = mouse_y();

        //std::cout << "here" << x << ' ' << y << std::endl;
        //std::cout << "quit_rect: " << quit_rect << std::endl;

        if (showMain)
        {
            if (play_rect.collide_point(x, y))
            {
                open = false;
                showMain = false;
                showHighScores = false;
                file->close();
            }
            else if (hs_rect.collide_point(x, y))
            {
                showHighScores = true;
                showMain = false;
                update_scores();
            }
            else if (quit_rect.collide_point(x, y))
            {
                open = true;
                q = true;
            }
        }

        else if (showHighScores)
        {
            if (previous_rect.collide_point(x, y))
            {
                current_page--;
                update_scores((current_page - 1) * SCORES_PER_PAGE);
            }

            else if (next_rect.collide_point(x, y))
            {
                current_page++;
                update_scores((current_page - 1) * SCORES_PER_PAGE);
            }
        }
    }
    
    if (showHighScores)
    {
        KeyPressed KeyPressed = get_keypressed();

        if (KeyPressed[SDLK_ESCAPE])
        {
            showMain = true;
            showHighScores = false;
        }
    }
}
