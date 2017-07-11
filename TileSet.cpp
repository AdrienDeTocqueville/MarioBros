#include "TileSet.h"

#include <fstream>
#include <iostream>

sf::RenderWindow* TileSet::ecran = nullptr;


Animations::Animations(std::string _path)
{
    /// Open file
    std::ifstream file((_path+"/Anims.nsmb").c_str());
    if (!file)
        std::cout << "Unable to open: " << _path + "/Anims.nsmb" << std::endl;


    /// Read header
    std::string temp;

    unsigned animsCount, tileSetsCount;
    file >> temp >> tileSetsCount >> temp >> animsCount;

    tileSets.reserve(tileSetsCount);
    animations.reserve(animsCount);

    /// Load tilesets
    for (unsigned i(0) ; i < tileSetsCount ; i++)
    {
        file >> temp >> temp;

        sf::Texture* tex = TextureManager::get(_path + "/" + temp);
        tileSets.emplace_back(*tex);
    }

    /// Load Animations
    int tileWidth, tileHeight, tileCount, tileSet, x, y, duration;
    bool reversed, loop;
    std::string name;

    for (unsigned i(0) ; i < animsCount ; i++)
    {
        while (temp != "Anim")
        {
            file >> temp;
        }

        file >> name;
        file >> temp >> tileWidth >> temp >> tileHeight >> temp >> tileCount;
        file >> temp >> tileSet >> temp >> x >> temp >> y;
        file >> temp >> reversed >> temp >> loop >> temp >> duration;

        sf::IntRect src(x, y, tileWidth, tileHeight);

        animations.push_back( new Animation(src, tileCount, tileSet, reversed, loop, duration) );
        animations.back()->name = name;
    }
}

Animations::~Animations()
{
    for (unsigned i(0) ; i < animations.size() ; i++)
        delete animations[i];
}

TileSet::TileSet(Animations* _animations):
    animations(_animations),
    animation(nullptr), tileNum(0)
{
    setAnimation(0);
}

void TileSet::setAnimation(unsigned _index)
{
    if (_index >= animations->size())
        return;

    Animation* anim = animations->getAnimation(_index);

    if (anim == animation)
        return;

    animation = anim;
    timer.restart();

    tileNum = (animation->tileCount-1) * animation->reversed;
    src = animation->src;
}

void TileSet::tick()
{
    if (timer.getElapsedTime().asMilliseconds() < animation->timePerTile)
        return;

    timer.restart();

    if (animation->reversed)
    {
        if (tileNum == 0)
        {
            if (animation->loop)
                tileNum = animation->tileCount-1;
        }
        else
            tileNum--;
    }
    else
    {
        if (tileNum == animation->tileCount-1)
        {
            if (animation->loop)
                tileNum = 0;
        }
        else
            tileNum++;
    }

    src.left = animation->src.left + tileNum * animation->src.width;
}

void TileSet::draw(sf::Vector2f pos)
{
    sf::Sprite& tileSet = animations->getTileSet(animation->tileSet);

    tileSet.setPosition(pos);
    tileSet.setTextureRect(src);

    ecran->draw(tileSet);
}

Animation::Animation(sf::IntRect _src, int _tileCount, int _tileSet, bool _reversed, bool _loop, int _duration):
    src(_src), timePerTile(_duration / _tileCount),
    tileCount(_tileCount), tileSet(_tileSet), reversed(_reversed), loop(_loop)
{ }
