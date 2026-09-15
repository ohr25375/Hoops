#include "SDL2_Sprite.hpp"

SPRITE::SPRITE(std::vector<std::string> sprite)
{
    int max = 0;
    for (int i = 0; i < sprite.size(); i++)
    {
        if (sprite[i].size() > max)
        {
            max = sprite[i].size();
        }
    }
    width = max;
    height = sprite.size();
    this->sprite = sprite;
    cleanSprite();
}

void SPRITE::cleanSprite()
{
    for (int i = 0; i < height; i++)
    {
        if (sprite[i].size() < width)
        {
            sprite[i].append(width - sprite[i].size(), ' ');
        }
    }
}