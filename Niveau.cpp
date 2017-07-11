#include "Niveau.h"
#include <fstream>

#include "Joueur.h"
#include "Goomba.h"
#include "Koopa.h"

Niveau::Niveau(unsigned _world, std::string _levelFile)
    : cameraOffset{0.0f, 0.0f}, bgWidth(256),
      world(_world), level(_levelFile)
{
    std::string link = "res/Mondes/" + toString(world) + "/TileSet.png";

    for (int i(0) ; i < 2 ; i++)
    {
        sf::Texture* t = TextureManager::get(link.c_str());
        tileSets[i].setTexture(*t);

        link = "res/Images/CommonBlocs.png";
    }

    loadBackgrounds();
    loadMap();

    Personnage::niveau = this;
}

Niveau::~Niveau()
{
    for (unsigned i(0) ; i < personnages.size() ; i++)
        delete personnages[i];

    Personnage::niveau = nullptr;
}

void Niveau::loadBackgrounds()
{
    sf::Texture* bg0 = TextureManager::get("res/Mondes/" + toString(world) + "/BG0.png");
    sf::Texture* bg1 = TextureManager::get("res/Mondes/" + toString(world) + "/BG1.png");

    backgrounds[0].setTexture(*bg0);
    backgrounds[1].setTexture(*bg1);

//    bgWidth = bg0->getSize().x;
}

void Niveau::loadMap()
{
    if (level.size() < 5 || level.substr(level.size()-4) != ".lvl")
        level += ".lvl";

    bool foundMario = false;

    std::string path = "res/Mondes/"+toString(world) + "/" + level;
    std::ifstream file(path.c_str());
    if (!file)
        std::cout << "Unable to open: " << path << std::endl;

	if(file)
   	{
   	    std::string line;

   	    file >> width >> height;
        getline(file, line); // Take care of carriage return

        map.resize(width);
        for (unsigned i(0) ; i < map.size() ; i++)
            map[i].resize(height);

        for (int j(0) ; j < height ; j++)
        {
            getline(file, line);

            for (int i(0) ; i < width  ; i++)
            {
                map[i][j].type = getBLOC(line[i]);

                if (map[i][j].type == BLOC::GOOMBA)
                {
                    map[i][j].type = BLOC::CIEL;

                    personnages.push_back(new Goomba(i*TAILLE_BLOC, j*TAILLE_BLOC, LEFT));
                }
                else if (map[i][j].type == BLOC::KOOPA && map[i][j-1].type == BLOC::KOOPA)
                {
                    map[i][j-1].type = BLOC::CIEL;
                    map[i][j].type = BLOC::CIEL;

                    personnages.push_back(new Koopa(i*TAILLE_BLOC, (j-1)*TAILLE_BLOC, LEFT));
                }
                else if (map[i][j].type == BLOC::MARIO)
                {
                    map[i][j].type = BLOC::CIEL;

                    if (!foundMario)
                    {
                        foundMario = true;
                        playerPosX = i*TAILLE_BLOC;
                        playerPosY = j*TAILLE_BLOC;
                    }
                }

            }
        }

        refresh();
	}
}

bool Niveau::saveMap()
{
    std::ofstream file(("res/Mondes/" + toString(world) + "/user.lvl").c_str());

    if (!file)
        return 0;

    file << "64\n12\n";

    for (int i(0) ; i < width ; i++)
    {
        for (int j(0) ; j < height ; j++)
            file << getChar(map[i][j].type);

        file << '\n';
    }

    file.close();

    return 1;
}

void Niveau::setXOffset(float _offset)
{
    cameraOffset.x = std::max(std::min(_offset, (float)(width-NB_BLOC_LARGEUR)*TAILLE_BLOC), 0.0f);
}

void Niveau::setYOffset(float _offset)
{
    cameraOffset.y = std::max(std::min(_offset, (float)(height-NB_BLOC_HAUTEUR)*TAILLE_BLOC), 0.0f);
}

bool Niveau::setBloc(int _x, int _y, BLOC _type)
{
    if (0 > _x || _x >= width || 0 > _y || _y >= height)
        return 0;

    map[_x][_y].type = _type;
    return 1;
}

Bloc* Niveau::getBloc(int _x, int _y)
{
    if (0 > _x || _x >= width || 0 > _y || _y >= height)
        return &map[0][0];

    return &map[_x][_y];
}

std::vector<Hit> Niveau::getHits(sf::FloatRect rect)
{
    std::vector<Hit> hits;

    float blocOffsetX = rect.left / TAILLE_BLOC;
    float blocOffsetY = rect.top / TAILLE_BLOC;

    int iMin = std::max((int)floor(blocOffsetX), 0);
    int iMax = std::min((int)ceil (blocOffsetX + rect.width/TAILLE_BLOC), width);

    int jMin = std::max((int)floor(blocOffsetY), 0);
    int jMax = std::min((int)ceil (blocOffsetY + rect.height/TAILLE_BLOC), height);

    sf::FloatRect mapRect(0, 0, TAILLE_BLOC, TAILLE_BLOC);
    sf::FloatRect intersection;

    for (int i(iMin) ; i < iMax ; i++) //LARGEUR
    {
        for (int j(jMin) ; j < jMax ; j++) //HAUTEUR
        {
            mapRect.left = i*TAILLE_BLOC;
            mapRect.top = j*TAILLE_BLOC;

            if (mapRect.intersects(rect, intersection))
            {
                hits.emplace_back( mapRect.left, mapRect.top, &(map[i][j]) );

                if (intersection.width < intersection.height)
                    hits.back().sides = true;
                else if (intersection.top == rect.top)
                    hits.back().top = true;
            }
        }
    }

    return hits;
}

void Niveau::removePersonnage(Personnage* p)
{
    for (unsigned i(0) ; i < personnages.size() ; i++)
    {
        if (personnages[i] == p)
        {
            delete personnages[i];
            personnages.erase(personnages.begin() + i);
            return;
        }
    }
}

void Niveau::refresh()
{
    for (int i(0) ; i < width ; i++)
    for (int j(0) ; j < height ; j++)
        map[i][j].set_tile(map, i, j);
}

void Niveau::update()
{
    for (Personnage* p: personnages)
        p->update();

    for (unsigned i(0) ; i < personnages.size() ; i++)
    {
        for (unsigned j(i+1) ; j < personnages.size() ; j++)
        {
            if (personnages[i]->getAABB().intersects(personnages[j]->getAABB()))
            {
                personnages[i]->orientation = !personnages[i]->orientation;
                personnages[j]->orientation = !personnages[j]->orientation;
            }
        }
    }
}

void Niveau::draw(sf::RenderWindow* ecran)
{
    for (int i(0) ; i < 2 ; i++)
    {
        backgrounds[i].setPosition(-fmod((i+1)*cameraOffset.x / 10.0f, bgWidth), 0);
        ecran->draw(backgrounds[i]);

        backgrounds[i].move(bgWidth, 0);
        ecran->draw(backgrounds[i]);
    }

    auto blocOffset = cameraOffset * (1.0f/TAILLE_BLOC);

    int iMin = std::max((int)floor(blocOffset.x), 0);
    int iMax = std::min((int)ceil (blocOffset.x) + NB_BLOC_LARGEUR, width);

    int jMin = std::max((int)floor(blocOffset.y), 0);
    int jMax = std::min((int)ceil (blocOffset.y) + NB_BLOC_HAUTEUR, height);

    for (int i(iMin) ; i < iMax ; i++) //LARGEUR
    {
        for (int j(jMin) ; j < jMax ; j++) //HAUTEUR
        {
            if (map[i][j].draw)
            {
                sf::Vector2f blocPos(i*TAILLE_BLOC, j*TAILLE_BLOC);

                tileSets[1].setPosition(blocPos - cameraOffset);
                tileSets[1].setTextureRect(sf::IntRect((map[i][j].draw-1)*TAILLE_BLOC, 4*TAILLE_BLOC, TAILLE_BLOC, TAILLE_BLOC));

                ecran->draw(tileSets[1]);
                map[i][j].draw = 0;
                continue;
            }

            if (map[i][j].type == BLOC::CIEL)     continue;

            sf::Vector2f blocPos(i*TAILLE_BLOC, j*TAILLE_BLOC);

            sf::Sprite& sprite(tileSets[map[i][j].numTileSet]);

            sprite.setPosition(blocPos - cameraOffset);
            tileSets[map[i][j].numTileSet].setTextureRect(map[i][j].tile);

            ecran->draw(tileSets[map[i][j].numTileSet]);
        }
    }

    for (Personnage* p: personnages)
    {
        p->draw();
    }
}
