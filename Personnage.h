#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include "constantes.h"

#include "TileSet.h"


enum Type {MARIO, GOOMBA, KOOPA, MUSHROOM};
enum State {IDLE, WALK, RUN, JUMP, FALL};

class Niveau;
class Personnage
{
    friend class Niveau;

    public:
        Personnage(unsigned _x, unsigned _y, Type _type, bool _orientation);
        virtual ~Personnage();

        virtual void update() = 0;
        virtual void draw();

        sf::FloatRect getAABB();
        sf::Vector2f getPosition();

        void respawn();
        void damage();


//    protected:
        static Niveau* niveau;

        Type type;

        TileSet tileSet;

        int orientation;
        unsigned blocSize;
        int maxHealth, health;
        bool dead;

        sf::Clock deathTimer;

        sf::Vector2f spawnPosition, position;
};

#endif // PERSONNAGE_H
