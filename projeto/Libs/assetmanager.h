#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>

class AssetManager
{
public:
    AssetManager()
    {
        assert(sInstance == nullptr);
        this->sInstance = this;
    }

    static sf::Texture& GetTexture(std::string const& fileName)
    {
        try
        {
            return sInstance->texMap.at(fileName);
        }
        catch (...)
        {
            sf::Texture& texture = sInstance->texMap[fileName];
            if (texture.loadFromFile(fileName))
                return texture;
            else
                throw;
        }
    }
    static sf::Font& GetFont(std::string const& fileName) {
        try {
            return sInstance->fontMap.at(fileName);
        } catch (...) {
            sf::Font& font = sInstance->fontMap[fileName];
            if (font.loadFromFile(fileName))
                return font;
            else
                throw;
        }
    }

private:
    std::map< std::string, sf::Texture > texMap;
    std::map< std::string, sf::Font > fontMap;
    static AssetManager* sInstance;
};

AssetManager *AssetManager::sInstance = nullptr;

#endif // ASSETMANAGER_H
