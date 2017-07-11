#include "Bloc.h"

Bloc::Bloc(bool _solid):
    solid(_solid), numTileSet(0), tiling(false), type(BLOC::CIEL), tile(3, 3, TAILLE_BLOC, TAILLE_BLOC),
    draw(0)
{ }

void Bloc::set_tile(const Map& map, unsigned x, unsigned y)
{
    if (type != BLOC::SOL)
        setType(type);

    else
    {
        solid = true;
        tiling = false;
        numTileSet = 0;

        tile.left = (x*y)%4 + 1;

        if (y == 0 || map[x][y-1].type != BLOC::SOL)
        {
            tile.top = 0;

            if (x == 0 || map[x-1][y].type != BLOC::SOL)
                tile.left = 0;

            else if (x == map.size()-1 || map[x+1][y].type != BLOC::SOL)
                tile.left = 5;
        }
        else // from here y != 0
        {
            tile.top = 1;

            if (x != 0 && map[x-1][y].type == BLOC::SOL)
            {
                if (map[x-1][y-1].type != BLOC::SOL)
                {
                    tile.left = 6;
                    tile.top = 0;
                }
            }
            else
                tile.left = 0;


            if (x != map.size()-1 && map[x+1][y].type == BLOC::SOL)
            {
                if (map[x+1][y-1].type != BLOC::SOL)
                    tile.left = 6;
            }
            else
                tile.left = 5;
        }

        tile.left *= TAILLE_BLOC;
        tile.top *= TAILLE_BLOC;
    }
}

void Bloc::setType(BLOC _type)
{
    type = _type;

    solid = true;
    tiling = false;
    numTileSet = 1;

    if (type == BLOC::CIEL)
    {
        tile.left = 3;
        tile.top = 3;

        solid = false;
    }

    else if (type == BLOC::MYSTERE)
    {
        tile.left = 0;
        tile.top = 0;

        tiling = true;
    }
    else if (type == BLOC::BRIQUE || type == BLOC::BRIQUEVIDE)
    {
        tile.left = 0;
        tile.top = 1;

        tiling = true;
    }
    else if (type == BLOC::PIECE)
    {
        tile.left = 0;
        tile.top = 2;

        solid = false;
        tiling = true;
    }
    else if (type == BLOC::CUBE)
    {
        tile.left = 0;
        tile.top = 3;
    }
    else if (type == BLOC::VIDE)
    {
        tile.left = 1;
        tile.top = 3;
    }

    tile.left *= TAILLE_BLOC;
    tile.top *= TAILLE_BLOC;
}

Bloc::~Bloc()
{
    //dtor
}
