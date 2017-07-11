#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

class TextureManager
{
    public:
        static void destroy();

        static sf::Texture* get(std::string _path);

    private:
        TextureManager();
        ~TextureManager();

        static std::unordered_map<std::string, sf::Texture*> textures;
};

#endif // TEXTUREMANAGER_H
