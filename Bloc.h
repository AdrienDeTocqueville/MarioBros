#ifndef BLOC_H
#define BLOC_H

#include "constantes.h"

class Bloc
{
    public:
        Bloc(bool _solid = false);

        void set_tile(const Map& map, unsigned x, unsigned y);

        void setType(BLOC _type);

        ~Bloc();

        bool solid;
        bool numTileSet;
        bool tiling;
        BLOC type;

        sf::IntRect tile;

        int draw;
};

#endif // BLOC_H
