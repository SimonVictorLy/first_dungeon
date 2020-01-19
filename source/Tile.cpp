#include "def.h"

Tile::Tile(int x, int y, int tileType){
  mBox.x = x;
  mBox.y = y;

  mBox.w = TILE_WIDTH;
  mBox.h = TILE_HEIGHT;

  mType = tileType;
}

void Tile::render(SDL_Rect& camera)
{
  if(checkCollision(camera, mBox))
  {
    if(mType<12)gTileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
    else gCustomTexture.render(mBox.x - camera.x, mBox.y - camera.y, & gTileClips[mType]);
  }
}


int Tile::getType()
{
  return mType;
}

SDL_Rect Tile::getBox()
{
  return mBox;
}


