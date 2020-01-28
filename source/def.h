#include <SDL2/SDL.h>         // Load SDL gui functions
#include <SDL2/SDL_image.h>   // Load Image Library
#include <stdio.h>            // standard IO
#include <string>             // string library
#include <fstream>            // stream libary

// Custom Classes
#include "LTexture.h"         // Texture Wrapper Class
#include "Tile.h"             // Tile Class
#include "Entity.h"           // Entity/Dot Class

// Main functions
bool init();
bool loadMedia(Tile* tiles[]);
void close(Tile* tiles[]);

// Interation Functions
bool checkCollision(SDL_Rect a, SDL_Rect b);
bool touchesWall(SDL_Rect box, Tile* tiles[]);
int checkBackground(SDL_Rect box, Tile* tiles[]);
bool setTiles(Tile* tiles[]);

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 16;

//The different tile sprites
enum TileTypes{
  TILE_RED = 0,
  TILE_GREEN = 1,
  TILE_BLUE = 2,
  TILE_CENTER = 3,
  TILE_TOP = 4,
  TILE_TOPRIGHT = 5,
  TILE_RIGHT = 6,
  TILE_BOTTOMRIGHT = 7,
  TILE_BOTTOM = 8,
  TILE_BOTTOMLEFT = 9,
  TILE_LEFT = 10,
  TILE_TOPLEFT = 11,
  TILE_GRASS = 12,
  TILE_FULL = 13,
  TILE_DIAG_DOWN = 14,
  TILE_DIAG_UP = 15
};

// Global Variables
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

extern LTexture gMainTexture;  // main focus
extern LTexture gTileTexture;  // tiles for floors and walls
extern LTexture gCustomTexture; // Custom tile Texture
extern LTexture gPersonTexture; // person

extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
