
#include "Program.h"

const std::string SERVER_IP     = "127.0.0.1";
const std::string MYSQL_DB      = "galaxian";
const std::string MYSQL_USER    = "user";
const std::string MYSQL_PSWD    = "Galaxian123";

Program::Program(int w, int h, std::string t)  : width(w), height(h), title(t),
                                           screen(Surface(width, height)), main_game(w, h, &screen, &db, &file),
                                           menu(width, height, &screen, &db, &file), star_dy(3),
                                           background_music("sounds/GameLoop.ogg")
{
    try 
    {
        std::cout << "connecting to db..." << std::endl;
        db.connect(SERVER_IP, MYSQL_DB, MYSQL_USER, MYSQL_PSWD);
        std::cout << "coonnected!" << std::endl;
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

        db.set_connected(false);
        file.open("scores.txt");
    }
    
    background_music.play(1);
    background_music.on();

    // create the stars
    for (int i = 0; i < NUM_STARS; i++)
    {
        Star s;
        s.rect.w = 1;
        s.rect.h = 1;
        s.rect.x = rand() % width;
        s.rect.y = rand() % height;
        s.color.r = rand() % 255;
        s.color.g = rand() % 255;
        s.color.b = rand() % 255;
        stars.push_back(s);
    }
}

void Program::execute()
{
    SDL_EnableUNICODE(SDL_ENABLE);

    while (1)
    {
        Uint32 time = SDL_GetTicks();
        if (e.poll() && e.type() == QUIT)
            break;

        // Check if closed button was pressed
        if (menu.is_open() && menu.game_closed())
            break;

        // Check if the menu was just closed to play
        // the game
        if (!menu.is_open() && !main_game.is_init() && !main_game.is_over())
        {
            std::cout << "starting game" << std::endl;
            is_playing = true;
            main_game.init();
            menu.set_init(false);
            //menu.init();
        }

        // Check if the game was exitited to to menu
        else if (!menu.is_open() && main_game.is_over() && !menu.is_init())
        {
            is_playing = false;
            main_game.init();
            menu.init();
            main_game.set_init(false);
            menu.update_scores();
            menu.show_scores(true);
        }
        // else
        //     std::cout << !menu.is_open() << ' ' << !main_game.is_init() << ' ' << !main_game.is_over() << std::endl;

        // move stars
        for (int i = 0; i < stars.size(); i++)
        {
            stars[i].rect.y += star_dy;

            // reset the star if it goes out of 
            // bounds
            if (stars[i].rect.y >= height - 1)
            {
                stars[i].rect.x = rand() % width;
                stars[i].rect.y = 0;
            }
        }

        if (is_playing)
        {
            if (!main_game.is_paused())
            {
                main_game.update();
            }

            main_game.handle_input(e);
        }
        else
        {
            menu.update();
            menu.handle_input(e);
        }

        screen.lock();
        screen.fill(BLACK);

        // draw stars
        for (int i = 0; i < stars.size(); i++)
        {
            screen.put_pixel(stars[i].rect.x, stars[i].rect.y, stars[i].color);
        }

        if (is_playing)
            main_game.draw();
        else
            menu.draw();

        screen.unlock();
        screen.flip();
        if(20 > (SDL_GetTicks() - time))
        {
            SDL_Delay(20-(SDL_GetTicks()-time)); //SDL_Delay pauses the execution.
        }
    }

    SDL_EnableUNICODE(SDL_DISABLE);
}
