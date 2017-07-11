#include "jeu.h"
#include "editeur.h"

#include "NEAT/Pool.h"

void afficherMenu(sf::RenderWindow& ecran);
void afficherSelecteur(sf::RenderWindow& ecran, int selection);

int main(int argc, char *argv[])
{
    sf::RenderWindow ecran(sf::VideoMode(NB_BLOC_LARGEUR*TAILLE_BLOC, NB_BLOC_HAUTEUR*TAILLE_BLOC + 160), "New Super Mario Bros.");

    TileSet::ecran = &ecran;
    Input::init(&ecran);

    int selection = 0;

    while(Input::isOpen())
    {
        // Draw
            ecran.clear(sf::Color(0, 204, 255));

            afficherMenu(ecran);
            afficherSelecteur(ecran, selection);

            ecran.display();

        // Evenements
            Input::update();

            if ((Input::getKeyPressed(sf::Keyboard::Down) || Input::getKeyPressed(sf::Keyboard::Right)) && selection != 3)
                selection++;

            if (Input::getKeyPressed(sf::Keyboard::Up) && selection)
                selection--;

            if (Input::getKeyPressed(sf::Keyboard::Return))
            {
                if (selection == 0)
                    jouer(1, "1.lvl");   // Demarrage du jeu

                if (selection == 1)
                {
                    editer(1);	        // Editeur de niveau


//                    Pool pool(0, NB_BLOC_HAUTEUR*NB_BLOC_LARGEUR +1, 3);
//                    std::string gs = "197 1606.5 13 0 2 0 0 0 0 38 195 -0.24136 554 1 16 193 0.80743 555 1 192 193 -0.68533 556 0 71 194 0.302737 1347 1 166 195 -0.36166 1348 0 192 194 1.65186 1349 1 166 196 1.05184 1350 1 196 195 -0.36531 1351 1 154 194 1.65121 2697 1 54 194 1.92199 2698 1 192 196 -1.27208 2699 1 67 194 0.701987 4030 1 59 195 0.067568 4031 1";
//                    Genome g; g.loadFromString(gs);
//                    Network net(&g);
//
//                    bool stop;
//                    fitnessEvaluation(net, stop);
                }

                if (selection == 2)         // NEAT
                {
                    Pool* pool = nullptr;

                    std::cout << "Select action: " << std::endl;
                    std::cout << "1. Build new" << std::endl;
                    std::cout << "2. Load existing" << std::endl;

                    int action;
                    std::cin >> action;

                    if (action == 1)
                    {
                        pool = new Pool(300, NB_BLOC_HAUTEUR*NB_BLOC_LARGEUR +1, 3);
                    }
                    else
                    {
                        std::cout << "Load generation: ";
                        unsigned gen;
                        std::cin >> gen;

                        pool = new Pool(gen);
                    }

                    std::cout << "Train to generation: ";
                    unsigned x = 0;
                    std::cin >> x;

                    while (pool->generation < x)
                    {
                        if (!pool->computeGenerationFitness(fitnessEvaluation))
                            break;

                        pool->buildNewGeneration(true);
                    }
                }

                if (selection == 3)
                    ecran.close();
            }
    }

    Input::destroy();

    return EXIT_SUCCESS;
}

void afficherMenu(sf::RenderWindow& ecran)
{
    /// Images
        static sf::Sprite logo   (*TextureManager::get("res/Images/Intro/logo.png"));
        static sf::Sprite gauche (*TextureManager::get("res/Images/Intro/gauche.png"));
        static sf::Sprite droite (*TextureManager::get("res/Images/Intro/droite.png"));
        static sf::Sprite chateau(*TextureManager::get("res/Images/Intro/castle.png"));

        static sf::Sprite tileSet(*TextureManager::get("res/Mondes/1/TileSet.png"));

    /// Positions
        unsigned solHeight = TAILLE_BLOC*(NB_BLOC_HAUTEUR-2);
        unsigned width = ecran.getSize().x;

        tileSet.setPosition(0, solHeight);

    /// Affichage
    // Chateau
        chateau.setPosition(width/2 + 10, 4*TAILLE_BLOC);
        ecran.draw(chateau);

    // Buisson
        gauche.setPosition(0, solHeight - gauche.getTexture()->getSize().y);
        ecran.draw(gauche);

    // Barriere
        droite.setPosition(width - droite.getTexture()->getSize().x, solHeight - droite.getTexture()->getSize().y);
        ecran.draw(droite);

    // Logo
        logo.setPosition(width/2 - logo.getTexture()->getSize().x/2, TAILLE_BLOC/2);
        ecran.draw(logo);

    /// Herbe et terre

        for(int i(0) ; i <= NB_BLOC_LARGEUR ; i++)
        {
            tileSet.setTextureRect(sf::IntRect(i%4*TAILLE_BLOC + TAILLE_BLOC, 0, TAILLE_BLOC, 2*TAILLE_BLOC));

            ecran.draw(tileSet);
            tileSet.move(TAILLE_BLOC, 0);
        }

    /// "Credits"
//        SDL_Write(ecran, (ecran->w - 24*8)/2, solHeight + TAILLE_BLOC, "BY ADRIEN DE TOCQUEVILLE", "font2", 8);
}

void afficherSelecteur(sf::RenderWindow& ecran, int selection)
{
    /// Image
        static sf::Sprite menu(*TextureManager::get("res/Images/Intro/menu.png"));
        static sf::Sprite background(*TextureManager::get("res/Images/HUD/background.png"));

    /// Positions
        unsigned y = ecran.getSize().y/2 + TAILLE_BLOC + 3;

        background.setPosition(0, ecran.getSize().y - 160);

    /// Affichage
        ecran.draw(background);

        for (int i(0) ; i < 4 ; i++)
        {
            unsigned srcw = 202 + 23*(selection == i);

            menu.setPosition(ecran.getSize().x - srcw, y + i*40);
            menu.setTextureRect( sf::IntRect(226 * (selection != i), i * 35, srcw, 35) );

            ecran.draw(menu);
        }
}
