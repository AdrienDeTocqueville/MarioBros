#ifndef NIVEAU_H
#define NIVEAU_H

#include "Bloc.h"
#include "Personnage.h"

#include <vector>

struct Hit
{
    Hit(float l, float t, Bloc* b):
        x(l), y(t), bloc(b),
        top(false), sides(false)
    { }

    float x;
    float y;

    Bloc* bloc;

    bool top, sides;
};

class Niveau
{
/// -----   METHODES    -----

    public:
        Niveau(unsigned _world, std::string _levelFile);
        ~Niveau();

        void loadBackgrounds();
        void loadMap();
        bool saveMap();

        void setXOffset(float _offset);
        void setYOffset(float _offset);

        bool setBloc(int _x, int _y, BLOC _type);
        Bloc* getBloc(int _x, int _y);

        std::vector<Hit> getHits(sf::FloatRect rect);

        void removePersonnage(Personnage* p);

        void refresh();
        void update();

        void draw(sf::RenderWindow* ecran);

/// -----   ATTRIBUTS   -----

        sf::Vector2f cameraOffset;
        sf::Sprite tileSets[2];

        sf::Sprite backgrounds[2];
        unsigned bgWidth;
//            SDL_Rect srcBackgrounds[2];
//            SDL_Rect dst_backgrounds[2];

        unsigned playerPosX, playerPosY;

//    private:
        /// Niveau
            int width, height;
            Map map;

            unsigned world;
            std::string level;

        /// Ennemis
            std::vector<Personnage*> personnages;
};

#endif // NIVEAU_H
