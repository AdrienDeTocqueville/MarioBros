#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>

#include "TextureManager.h"

struct Animation
{
    Animation(sf::IntRect _src, int _tileCount, int _tileSet, bool _reversed, bool _loop, int _duration);

    sf::IntRect src;

    int timePerTile;

    int tileCount;
    int tileSet;

    bool reversed, loop;

    std::string name;
};

class Animations
{
    public:
        Animations(std::string _path);
        ~Animations();

        unsigned size()
        {
            return animations.size();
        }

        Animation* getAnimation(int i)
        {
            return animations[i];
        }

        sf::Sprite& getTileSet(int i)
        {
            return tileSets[i];
        }

    private:
        std::vector<sf::Sprite> tileSets;
        std::vector<Animation*> animations;
};

class TileSet
{
    public:
        TileSet(Animations* _animations);

        void setAnimation(unsigned _index);

        void tick();

        void draw(sf::Vector2f pos);

        static sf::RenderWindow* ecran;

    private:
        Animations* animations;
        Animation* animation;

        int tileNum;
        sf::IntRect src;

        sf::Clock timer;
};

#endif // TILESET_H
