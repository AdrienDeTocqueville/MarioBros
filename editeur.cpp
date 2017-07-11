#include "editeur.h"
#include "Input.h"
#include "Niveau.h"

void editer(unsigned world)
{
    sf::RenderWindow* ecran = Input::getWindow();

    Niveau niveau(world, "user.lvl");

	///Variables
        sf::Clock timer;


    while (Input::isOpen())
    {
        /// BACK TO MENU
        if (Input::getKeyPressed(sf::Keyboard::Escape))
            return;

        /// Limite de 60 FPS
            auto elapsed = timer.getElapsedTime().asMilliseconds();
            if (elapsed < 17) // 1 image toutes les 16.6 ms
            {
                sf::sleep(sf::milliseconds(17 - elapsed));
                continue;
            }
            timer.restart();

        /// Récuperation des evenements
            Input::update();

            if (Input::getKeyPressed(sf::Keyboard::S))
                niveau.saveMap();


            float speed = 1.5f;
            if (Input::getKeyDown(sf::Keyboard::LShift))
                speed *= 2.0f;

            if (Input::getKeyDown(sf::Keyboard::Left))
                niveau.setXOffset(niveau.cameraOffset.x-speed);
            if (Input::getKeyDown(sf::Keyboard::Right))
                niveau.setXOffset(niveau.cameraOffset.x+speed);

            if (Input::getKeyDown(sf::Keyboard::Up))
                niveau.setYOffset(niveau.cameraOffset.y-speed);
            if (Input::getKeyDown(sf::Keyboard::Down))
                niveau.setYOffset(niveau.cameraOffset.y+speed);

//            if (Input::getMouse)
//                niveau.setBloc((in->mousex + niveau.cameraOffset)/TAILLE_BLOC, in->mousey/TAILLE_BLOC, BLOC::CIEL);


        /// Draw
            ecran->clear(sf::Color(0, 204, 255));
            niveau.draw(ecran);
            ecran->display();
    }
}
