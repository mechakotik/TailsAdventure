#include "font.h"
#include "tools.h"

void TA_Font::setMapping(std::string mappingString)
{
    mapping.clear();
    for(int pos = 0; pos < mappingString.length(); pos ++) {
        mapping[mappingString[pos]] = pos;
    }
}

void TA_Font::drawText(TA_Point position, std::string text, TA_Point offset)
{
    TA_Point currentPosition = position;
    for(char symbol : text) {
        if(mapping.count(symbol)) {
            setPosition(currentPosition);
            setFrame(mapping[symbol]);
            draw();
            currentPosition.x += getWidth() + offset.x;
        }
        else if(symbol == '\n') {
            currentPosition.x = position.x;
            currentPosition.x += getHeight() + offset.y;
        }
    }
}

void TA_Font::drawTextCentered(double y, std::string text, TA_Point offset)
{
    double currentWidth = 0, maxWidth = 0;
    for(char symbol : text) {
        if(symbol != '\n') {
            currentWidth += getWidth() + offset.x;
            maxWidth = std::max(maxWidth, currentWidth);
        }
        else {
            currentWidth = 0;
        }
    }
    drawText(TA_Point(TA::screenWidth / 2 - maxWidth / 2, y), text, offset);
}