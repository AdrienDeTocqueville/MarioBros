#include "jeu.h"
#include "Niveau.h"

// TODO: tester nouvel affichage niveau
// tester getInputs avec ennemis morts

void jouer(unsigned world, std::string level)
{
    sf::RenderWindow* ecran = Input::getWindow();

    Niveau niveau(1, "1.lvl");
    Joueur joueur(niveau.playerPosX, niveau.playerPosY, RIGHT);

    sf::Clock timer;

    auto b = TextureManager::get(("res/Images/HUD/background.png"));

    sf::RectangleShape sh({TAILLE_BLOC, 13});
    sf::Sprite background(*b);
    background.setPosition(0, ecran->getSize().y - 160);


    while (Input::isOpen())
    {
        /// BACK TO MENU
        if (Input::getKeyPressed(sf::Keyboard::Escape))
            return;

        /// GENERATE NN INPUTS
        auto inputs = getInputs(joueur.getPosition(), niveau);

        /// Limite de 60 FPS
            auto elapsed = timer.getElapsedTime().asMilliseconds();
            if (elapsed < 16) // 1 image toutes les 16.6 ms
            {
                sf::sleep(sf::milliseconds(16 - elapsed));
                continue;
            }
            timer.restart();

        /// Récuperation des evenements
            Input::update();

        /// Update game
            niveau.update();

            joueur.update();

            if (joueur.dead || joueur.getPosition().x >= 63*TAILLE_BLOC)
                break;

        /// Draw
            ecran->clear(sf::Color(0, 204, 255));

            /// DRAW GAME
            niveau.draw(ecran);
            joueur.draw();

            ecran->draw(background);
//            HUD.draw();

            /// DRAW INPUTS
            for (unsigned i(0) ; i < NB_BLOC_LARGEUR ; i++)
            {
                sh.setPosition(i*TAILLE_BLOC, NB_BLOC_HAUTEUR*TAILLE_BLOC);

                for (unsigned j(0) ; j < NB_BLOC_HAUTEUR ; j++)
                {
                    if (inputs[i + j*NB_BLOC_LARGEUR] == -1)
                        sh.setFillColor(sf::Color::Black);
                    else if (inputs[i + j*NB_BLOC_LARGEUR] == 0)
                        sh.setFillColor(sf::Color(127, 127, 127));
                    else if (inputs[i + j*NB_BLOC_LARGEUR] == 1)
                        sh.setFillColor(sf::Color::White);

                    ecran->draw(sh);

                    sh.move(0, 13);
                }
            }

            ecran->display();
    }
}


// Nice genome: 5 352 3 0 3 0.118839 39 1 1 3 0.202338 40 1 1 4 -0.512406 41 1

const float toc = 20.0f;

float fitnessEvaluation(Network& net, bool& _stop)
{
    sf::RenderWindow* ecran = Input::getWindow();

    sf::RectangleShape sh({TAILLE_BLOC, 13});

    Niveau niveau(1, "1.lvl");
    Joueur joueur(niveau.playerPosX, niveau.playerPosY, RIGHT);

    float rightMost = 0;
    float timeOut = toc;

    unsigned currentFrame = 0;

    sf::Clock timer;

    while (Input::isOpen())
    {
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

        /// Compute NN input
            auto inputs = getInputs(joueur.getPosition(), niveau);

        /// Get NN output
            auto output = net.evaluate( inputs );

            Input::setKeyState(sf::Keyboard::Left , (output[0] > 0.0f));
            Input::setKeyState(sf::Keyboard::Right, (output[1] > 0.0f));
            Input::setKeyState(sf::Keyboard::Up   , (output[2] > 0.0f));

        /// Update game
            niveau.update();

            joueur.update();

        /// Draw
            ecran->clear(sf::Color(0, 204, 255));

            /// DRAW INPUTS
            for (unsigned i(0) ; i < NB_BLOC_LARGEUR ; i++)
            {
                sh.setPosition(i*TAILLE_BLOC, NB_BLOC_HAUTEUR*TAILLE_BLOC);

                for (unsigned j(0) ; j < NB_BLOC_HAUTEUR ; j++)
                {
                    if (inputs[i + j*NB_BLOC_LARGEUR] == -1)
                        sh.setFillColor(sf::Color::Black);
                    else if (inputs[i + j*NB_BLOC_LARGEUR] == 0)
                        sh.setFillColor(sf::Color(127, 127, 127));
                    else if (inputs[i + j*NB_BLOC_LARGEUR] == 1)
                        sh.setFillColor(sf::Color::White);

                    ecran->draw(sh);

                    sh.move(0, 13);
                }
            }

            niveau.draw(ecran);
            joueur.draw();

            ecran->display();

        /// Check if org is inactive
            if (joueur.getPosition().x > rightMost)
            {
                rightMost = joueur.getPosition().x;
                timeOut = 0;
            }

            timeOut++;

            float tob = currentFrame * 0.25f;

            bool firstTimer = tob + toc <= timeOut && rightMost < 2.0f*niveau.playerPosX;
            bool secondTimer = tob + 2.0f*toc <= timeOut;

            bool finished = joueur.getPosition().x >= 63*TAILLE_BLOC;

            if (firstTimer || secondTimer || joueur.dead || finished)
            {
                float fitness = rightMost-niveau.playerPosX - currentFrame * 0.5f;

                if (finished)
                    fitness += 1000.0f;

                else if (fitness == 0.0f)
                    fitness = -1.0f;

                return fitness;
            }

        /// Frame counter
            currentFrame++;
    }

    _stop = true;

    return -1.0f;
}

vector<float> getInputs(sf::Vector2f pos, const Niveau& niveau)
{
    vector<float> inputs(NB_BLOC_LARGEUR * NB_BLOC_HAUTEUR, 0);

    int blocX = floor(pos.x / TAILLE_BLOC);
    int blocY = floor(pos.y / TAILLE_BLOC);

    int iMin = blocX - NB_BLOC_LARGEUR/2;
    int jMin = blocY - NB_BLOC_HAUTEUR/3;

    for (int i(iMin) ; i < iMin + NB_BLOC_LARGEUR; i++) //LARGEUR
    {
        if (i < 0 || i >= niveau.width)
            continue;

        for (int j(jMin) ; j < jMin + NB_BLOC_HAUTEUR ; j++) //HAUTEUR
        {
            if (j < 0 || j >= niveau.height)
                continue;

            if (niveau.map[i][j].solid)
                inputs[(i - iMin) + (j-jMin)*NB_BLOC_LARGEUR] = 1;
        }
    }

    for (Personnage* p: niveau.personnages)
    {
        if (p->dead)
            continue;

        // Get bloc
        int i = (p->getPosition().x) / TAILLE_BLOC;
        int j = (p->getPosition().y) / TAILLE_BLOC;

        if (i >= iMin && i < iMin + NB_BLOC_LARGEUR)
        {
            if (j >= jMin && j < jMin + NB_BLOC_HAUTEUR)
            {
                inputs[(i - iMin) + (j-jMin)*NB_BLOC_LARGEUR] = -1;
                if (p->blocSize == 2 && j+1 < jMin + NB_BLOC_HAUTEUR)
                    inputs[(i - iMin) + (j+1-jMin)*NB_BLOC_LARGEUR] = -1;
            }
        }
    }

    return inputs;
}
