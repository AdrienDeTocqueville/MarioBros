#include "Personnage.h"
#include "Niveau.h"

#include <fstream>

using namespace std;


Animations animations[] = { Animations("res/TileSets/Mario"),
                            Animations("res/TileSets/Goomba"),
                            Animations("res/TileSets/Koopa") };

Niveau* Personnage::niveau = nullptr;

/// Constructeur
Personnage::Personnage(unsigned _x, unsigned _y, Type _type, bool _orientation):
    type(_type),
    tileSet(&(animations[_type])),
    orientation(_orientation),
    blocSize(1),
    maxHealth(1), health(1),
    dead(false),
    spawnPosition(_x, _y),
    position(_x, _y)
{
//    std::string path = "res/Personnages/"+_type+"/Infos.nsmb";
//    ifstream file(path.c_str());
//    if (!file)
//        std::cout << "Unable to open: " << path << std::endl;
//
//    string temp;
//    file >> temp >> blocSize >> temp >> lives;
//
//    life = lives;
}

Personnage::~Personnage()
{ }

void Personnage::draw()
{
    tileSet.tick();
    tileSet.draw(position - niveau->cameraOffset);
}

sf::FloatRect Personnage::getAABB()
{
    return sf::FloatRect(position, sf::Vector2f(TAILLE_BLOC, blocSize*TAILLE_BLOC));
}

sf::Vector2f Personnage::getPosition()
{
    return position;
}

void Personnage::respawn()
{
    position = spawnPosition;
}

void Personnage::takeDamage()
{
    if (dead)
        return;

    health--;

    if (!health)
    {
        dead = true;
        deathTimer.restart();
    }
}
