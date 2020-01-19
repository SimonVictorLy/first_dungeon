#include "def.h"

// Global Variables
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

LTexture gMainTexture;  // main focus
LTexture gTileTexture;  // tiles for floors and walls
LTexture gCustomTexture; // Custom tile Texture
LTexture gPersonTexture; // person

SDL_Rect gSpriteClips[ANIMATION_FRAMES];
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

bool init(){

  printf("Initializing...\n");
  if(SDL_Init(SDL_INIT_VIDEO)<0){printf("SDL Error: %s\n",SDL_GetError()); return false;}
   
  if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")){
    printf("Warning: Linear Texture Filtering not enabled");
  }

  gWindow = SDL_CreateWindow("Hip-hop Showdown", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if(gWindow == NULL){printf("Window Error: %s\n",SDL_GetError()); return false;}
  
  gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(gRenderer == NULL){printf("Renderer Error %s\n",SDL_GetError()); return false;}

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags)){printf("IMG Error %s\n",SDL_GetError()); return false;}

  return true;
}

bool loadMedia(Tile* tiles[]){
  printf("Loading Media...\n");

	//Load dot texture
	if( !gMainTexture.loadFromFile( "../imgs/dot.bmp"))
  {printf( "Failed to load Main texture!\n" ); return false;}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "../imgs/tiles.png" ) ) 
  {printf( "Failed to load tile set texture!\n" ); return false;}

  // load Custom tiles
  if( !gCustomTexture.loadFromFile( "../imgs/customTiles320x80.png"))
  {printf( "Failed to load Custom Tiles texture!\n" ); return false;}
  
  // person texture 
	if( !gPersonTexture.loadFromFile( "../imgs/char_fighter128x192.png"))
  {printf( "Failed to load Person texture!\n" ); return false;}
  
  gSpriteClips[0].x = 0;
  gSpriteClips[0].y = 32;
  gSpriteClips[0].w = 32;
  gSpriteClips[0].h = 32;

  gSpriteClips[1].x = 32;
  gSpriteClips[1].y = 32;
  gSpriteClips[1].w = 32;
  gSpriteClips[1].h = 32;

  gSpriteClips[2].x = 64;
  gSpriteClips[2].y = 32;
  gSpriteClips[2].w = 32;
  gSpriteClips[2].h = 32;

  gSpriteClips[3].x = 96;
  gSpriteClips[3].y = 32;
  gSpriteClips[3].w = 32;
  gSpriteClips[3].h = 32;

	//Load tile map
	if( !setTiles( tiles ) )
  {printf( "Failed to load tile set!\n" ); return false;}
 

	return true;
}

void close(Tile* tiles[]){
  printf("Deleting all tiles...\n");
  for(int i = 0; i < TOTAL_TILES; ++i){
    if(tiles[i] == NULL){
      delete tiles[i];
      tiles[i]= NULL;
    }
  }
  printf("Deleting all textures...\n");
 
  gMainTexture.free();
  gTileTexture.free();
  gPersonTexture.free();
  gCustomTexture.free();

  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  
  gWindow = NULL;
  gRenderer = NULL;

  IMG_Quit();
  SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b){
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  leftA = a.x;
  rightA = a.x + a.w;
  topA = a.y;
  bottomA = a.y + a.h;

  leftB = b.x;
  rightB = b.x + b.w;
  topB = b.y;
  bottomB = b.y + b.h;

  // Check extreme of one side of A exceeds the opposite in B
  if(bottomA <= topB) return false;
  if(topA >= bottomB) return false;
  if(rightA <= leftB) return false;
  if(leftA >= rightB) return false;

  return true;
}

bool setTiles(Tile* tiles[]){
  bool tilesLoaded = true;
  
  int x = 0, y = 0;
  std::ifstream map("../imgs/lazy.map");
  if(map.fail()){
    printf("Unable to load map file!\n");
    return false;
  }
  
  // 192 tiles
  for(int i = 0; i < TOTAL_TILES; ++i){
    int tileType = -1;
    map >> tileType;
    
    if (map.fail()){
      printf("Error loading map: Unexpected end of file!\n");
      return false;
    }
  

    // Place tile at x,y
    if((tileType >=0) && (tileType < TOTAL_TILE_SPRITES)){
      tiles[i] = new Tile(x,y, tileType);
    }
    else{
      printf("Error Loading Map: Invalid tile type at %d!\n", i);
      return false;
    }
    
    // keep track of x,y
    x+=TILE_WIDTH;
    if(x>= LEVEL_WIDTH){
      x = 0;
      y += TILE_HEIGHT;
    }
  }

  if( tilesLoaded ){
	  gTileClips[ TILE_RED ].x = 0;
    gTileClips[ TILE_RED ].y = 0;
    gTileClips[ TILE_RED ].w = TILE_WIDTH;
    gTileClips[ TILE_RED ].h = TILE_HEIGHT;

    gTileClips[ TILE_GREEN ].x = 0;
    gTileClips[ TILE_GREEN ].y = 80;
    gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
    gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

    gTileClips[ TILE_BLUE ].x = 0;
    gTileClips[ TILE_BLUE ].y = 160;
    gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
    gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

    gTileClips[ TILE_TOPLEFT ].x = 80;
    gTileClips[ TILE_TOPLEFT ].y = 0;
    gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
    gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

    gTileClips[ TILE_LEFT ].x = 80;
    gTileClips[ TILE_LEFT ].y = 80;
    gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
    gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

    gTileClips[ TILE_BOTTOMLEFT ].x = 80;
    gTileClips[ TILE_BOTTOMLEFT ].y = 160;
    gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
    gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

    gTileClips[ TILE_TOP ].x = 160;
    gTileClips[ TILE_TOP ].y = 0;
    gTileClips[ TILE_TOP ].w = TILE_WIDTH;
    gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

    gTileClips[ TILE_CENTER ].x = 160;
    gTileClips[ TILE_CENTER ].y = 80;
    gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
    gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

    gTileClips[ TILE_BOTTOM ].x = 160;
    gTileClips[ TILE_BOTTOM ].y = 160;
    gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
    gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

    gTileClips[ TILE_TOPRIGHT ].x = 240;
    gTileClips[ TILE_TOPRIGHT ].y = 0;
    gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
    gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

    gTileClips[ TILE_RIGHT ].x = 240;
    gTileClips[ TILE_RIGHT ].y = 80;
    gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
    gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

    gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
    gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
    gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
    gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
    
    gTileClips[ TILE_GRASS ].x = 0;
    gTileClips[ TILE_GRASS ].y = 0;
    gTileClips[ TILE_GRASS ].w = TILE_WIDTH;
    gTileClips[ TILE_GRASS ].h = TILE_HEIGHT;

    gTileClips[ TILE_FULL ].x = 80;
    gTileClips[ TILE_FULL ].y = 0;
    gTileClips[ TILE_FULL ].w = TILE_WIDTH;
    gTileClips[ TILE_FULL ].h = TILE_HEIGHT;

    gTileClips[ TILE_DIAG_DOWN ].x = 160;
    gTileClips[ TILE_DIAG_DOWN ].y = 0;
    gTileClips[ TILE_DIAG_DOWN ].w = TILE_WIDTH;
    gTileClips[ TILE_DIAG_DOWN ].h = TILE_HEIGHT;

    gTileClips[ TILE_DIAG_UP ].x = 240;
    gTileClips[ TILE_DIAG_UP ].y = 0;
    gTileClips[ TILE_DIAG_UP ].w = TILE_WIDTH;
    gTileClips[ TILE_DIAG_UP ].h = TILE_HEIGHT;
	}

  //Close the file
  map.close();

  //If the map was loaded fine
  return tilesLoaded;
}

// only check when the center is hovering a block
int checkBackground(SDL_Rect box, Tile* tiles[]){
  
  static int tilesPerRow = LEVEL_WIDTH/TILE_WIDTH;
  int centerX = box.x + box.w/2;
  int centerY = box.y + box.h/2;

  // at this point you don't even need the check collision code
  if(centerX >= 0 && centerY >=0 && centerX < LEVEL_WIDTH && centerY < LEVEL_HEIGHT){
      int k = centerX/TILE_WIDTH + tilesPerRow*(centerY/TILE_HEIGHT);
      return tiles[k]->getType();
  }

  return TILE_GREEN;
}



/*
bool checkTouchRed(SDL_Rect box, Tile* tiles[]){
  
  int tilesPerRow = LEVEL_WIDTH/TILE_WIDTH;
  
  int hits[9][2] = {{0,0},{1,0},{0,1},{-1,0},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
  int centerX = box.x + box.w/2;
  int centerY = box.y + box.h/2;
  for(int it = 0; it < 9; it++){
    int i = (centerX + hits[i][0]*TILE_WIDTH);
    int j = (centerY + hits[i][1]*TILE_HEIGHT);
    int k = i/TILE_WIDTH + tilesPerRow*(j/TILE_HEIGHT);

    if(i >= 0 && j >=0 && i < LEVEL_WIDTH && j < LEVEL_HEIGHT && tiles[k]->getType() == TILE_RED){ 
      if(checkCollision(box,tiles[k]->getBox())){
        return true;
      }
    }
  }

  return false;
}

*/
bool touchesWall(SDL_Rect box, Tile* tiles[]){
  for(int i = 0; i < TOTAL_TILES; ++i){
    if((tiles[i]->getType()>=TILE_CENTER) && (tiles[i]->getType()<=TILE_DIAG_UP)){
      if(checkCollision(box,tiles[i]->getBox())){
        return true; // touch
      }
    }
  }
  // no touch
  return false;
}

int main( int argc, char* args[]){
  // Start SDL
  if(!init()){ printf("Failed to Initialize!\n"); return 0;}
  
  // Create set of Tiles
  Tile* tileSet[TOTAL_TILES];
  
  // Load all media, calls set tiles to establish tile types (bg, floors, walls)
  if(!loadMedia(tileSet)){printf("Failed to load Media\n"); return 0;}

  bool quit = false;
  SDL_Event e;
  
  // Entities
  Entity dot;
  SDL_Rect camera = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};

  while(!quit){
    
    // Check inputs
    while(SDL_PollEvent(&e)!=0){
      // Close button on window was pressed
      if(e.type == SDL_QUIT)quit = true;
      // Entities effected by events
      dot.handleEvent(e);
    }

    dot.move(tileSet);
    dot.setCamera(camera);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF,0xFF,0xFF);
    SDL_RenderClear(gRenderer);
    for(int i = 0; i < TOTAL_TILES; ++i){
      tileSet[i]->render(camera);
    }
    dot.render(camera);
    SDL_RenderPresent(gRenderer);
  }
  close(tileSet);

  return 0;
}
