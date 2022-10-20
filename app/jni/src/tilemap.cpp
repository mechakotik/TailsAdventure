#include <sstream>
#include <SDL.h>
#include "tilemap.h"
#include "tinyxml2.h"
#include "error.h"
#include "tools.h"
#include "globals.h"

void TA_Tilemap::load(std::string filename)
{
    tinyxml2::XMLDocument xmlFile;
    xmlFile.Parse(readStringFromFile(filename).c_str());
    tinyxml2::XMLElement *xmlRoot = xmlFile.FirstChildElement("map");

    width = xmlRoot->IntAttribute("width");
    height = xmlRoot->IntAttribute("height");
    layerCount = xmlRoot->IntAttribute("nextlayerid") - 1;
    tileWidth = xmlRoot->FirstChildElement("tileset")->IntAttribute("tilewidth");
    tileHeight = xmlRoot->FirstChildElement("tileset")->IntAttribute("tileheight");
    int tileCount = xmlRoot->FirstChildElement("tileset")->IntAttribute("tilecount");

    tilemap.resize(layerCount);
    for(int layer = 0; layer < layerCount; layer ++) {
        tilemap[layer].resize(width);
        for(int pos = 0; pos < width; pos ++) {
            tilemap[layer][pos].resize(height);
        }
    }
    tileset.assign(tileCount, TA_Tile());

    for(int pos = 0; pos < tileCount; pos ++) {
        tileset[pos].animation = std::vector<int>{pos};
        tileset[pos].animationDelay = 1;
    }

    auto loadTileset = [&](tinyxml2::XMLElement *tilesetElement)
    {
        std::string textureFilename = filename;
        while(!textureFilename.empty() && textureFilename.back() != '/') {
            textureFilename.pop_back();
        }
        textureFilename += tilesetElement->FirstChildElement("image")->Attribute("source");
        texture.load(textureFilename);

        for(tinyxml2::XMLElement *tileElement = tilesetElement->FirstChildElement("tile");
            tileElement != nullptr; tileElement = tileElement->NextSiblingElement("tile"))
        {
            int tileId = tileElement->IntAttribute("id");
            if(tileElement->FirstChildElement("animation") != nullptr) {
                tileset[tileId].animation.clear();
                int delayMs = tileElement->FirstChildElement("animation")->FirstChildElement("frame")->IntAttribute("duration");
                tileset[tileId].animationDelay = int(delayMs * 60 / 1000 + 0.5);
                for(tinyxml2::XMLElement *frameElement = tileElement->FirstChildElement("animation")->FirstChildElement("frame");
                    frameElement != nullptr; frameElement = frameElement->NextSiblingElement("frame")) {
                    tileset[tileId].animation.push_back(frameElement->IntAttribute("tileid"));
                }
            }
        }
    };

    auto loadLayer = [&](tinyxml2::XMLElement *layerElement)
    {
        int layer = layerCount - layerElement->IntAttribute("id");
        std::stringstream mapStream;
        mapStream << layerElement->FirstChildElement("data")->GetText();
        for(int tileY = 0; tileY < height; tileY ++ ) {
            for(int tileX = 0; tileX < width; tileX ++) {
                int tile;
                char temp;
                mapStream >> tile;
                if(tileX != width - 1 || tileY != height - 1) {
                    mapStream >> temp;
                }
                tile --;
                tilemap[layer][tileX][tileY].tileIndex = tile;
                tilemap[layer][tileX][tileY].sprite.loadFromTexture(&texture, tileWidth, tileHeight);
                tilemap[layer][tileX][tileY].sprite.setPosition(tileX * tileWidth, tileY * tileHeight);
                tilemap[layer][tileX][tileY].sprite.setAnimation(tileset[tile].animation, tileset[tile].animationDelay, -1);
            }
        }
    };

    loadTileset(xmlRoot->FirstChildElement("tileset"));
    tinyxml2::XMLElement *layerElement = xmlRoot->FirstChildElement("layer");
    for(int layer = 0; layer < layerCount; layer ++) {
        loadLayer(layerElement);
        layerElement = layerElement->NextSiblingElement("layer");
    }
}

void TA_Tilemap::draw(int layer)
{
    for(int tileX = 0; tileX < width; tileX ++) {
        for(int tileY = 0; tileY < height; tileY ++) {
            tilemap[layer][tileX][tileY].sprite.draw();
        }
    }
}

void TA_Tilemap::setCamera(TA_Camera *newCamera)
{
    newCamera->setBorder({TA_Point(0, 0), TA_Point(width * tileWidth - gScreenWidth, height * tileHeight - gScreenHeight)});
    for(int layer = 0; layer < layerCount; layer ++) {
        for(int tileX = 0; tileX < width; tileX ++) {
            for(int tileY = 0; tileY < height; tileY ++) {
                tilemap[layer][tileX][tileY].sprite.setCamera(newCamera);
            }
        }
    }
}
