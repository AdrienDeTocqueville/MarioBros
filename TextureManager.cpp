#include "TextureManager.h"
#include <iostream>

std::unordered_map<std::string, sf::Texture*> TextureManager::textures;

TextureManager::TextureManager()
{ }

TextureManager::~TextureManager()
{ }

void TextureManager::destroy()
{
    for (auto it(textures.begin()) ; it != textures.end() ; ++it)
        delete it->second;

    textures.clear();
}

sf::Texture* TextureManager::get(std::string _path)
{
    auto it = textures.find(_path);

    if (it == textures.end())
    {
        sf::Texture* t = new sf::Texture();

        if (!t->loadFromFile(_path))
        {
            std::cout << "Unable to load: " << _path << std::endl;
            return nullptr;
        }

        textures[_path] = t;
        return t;
    }
    else
        return it->second;
}
