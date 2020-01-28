#include "def.h"

Entity::Entity(LTexture *loadTexture){
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	  mBox.w = ENTITY_WIDTH;
	  mBox.h = ENTITY_HEIGHT;
    mTexture = loadTexture;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 10;

    faceRight = true;
    frame =0;


    attack = false;
    resetX = 0;
    resetY = 0;
    airborn = true;
    TOTAL_FRAMES = 4;
    FRAME_LENGTH = 6;
    TOTAL_ANIMATIONS = 2;
    getSpriteSheet();
}

Entity::Entity(int x, int y, int width, int height, LTexture *loadTexture, int velocityX, int velocityY){
  mBox.x = x; 
  mBox.y = y;
  mBox.w = width; 
  mBox.h = height;
  mVelX = velocityX;
  mVelY = velocityY;
  mTexture = loadTexture;

  // facing direction
  faceRight = true;
  attack = false;
  // first animation frame
  frame = 0;
  
  // reset position
  resetX = x;
  resetY = y;
  
  airborn = true;

  TOTAL_FRAMES = 4;
  FRAME_LENGTH = 6;
  TOTAL_ANIMATIONS = 3;
  
  getSpriteSheet();
}

void Entity::getSpriteSheet(){
  // Dynamically splits up square sprite sheet into clips
  mSpriteClips = new SDL_Rect *[TOTAL_ANIMATIONS];
  
  for(int i = 0; i < TOTAL_ANIMATIONS; i++){
    mSpriteClips[i] = new SDL_Rect[TOTAL_FRAMES];
    for(int j = 0; j < TOTAL_FRAMES; j++){
      mSpriteClips[i][j] = {j*mBox.w, i*mBox.h,mBox.w,mBox.h};
    }
  }
}

void Entity::handleEvent( SDL_Event& e ){
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= ENTITY_VEL; break;
            case SDLK_DOWN: mVelY += ENTITY_VEL; break;
            case SDLK_LEFT: mVelX -= ENTITY_VEL; break;
            case SDLK_RIGHT: mVelX += ENTITY_VEL; break;
            case SDLK_a: attack = true; frame = 0; break;
            case SDLK_SPACE: mVelY = airborn ? mVelY : -20; break; // if pressed space while airborn
        }
    }
  //If a key was released
  else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += ENTITY_VEL; break;
            case SDLK_DOWN: mVelY -= ENTITY_VEL; break;
            case SDLK_LEFT: mVelX += ENTITY_VEL; break;
            case SDLK_RIGHT: mVelX -= ENTITY_VEL; break;
        }
    }
}

void Entity::move( Tile *tiles[] ){
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + mBox.w > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;
    
    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + mBox.h > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) ){
        
        //move back
        mBox.y -= mVelY;

        // try to get as close to the wall as possible
        mVelY /= 2;
        airborn = false;
    }
    else{
      airborn = true;
    }

    // Max fall speed
    mVelY = mVelY < 10 ? mVelY + 2: mVelY;
   
    // checks the overlapping tiles 
    switch(checkBackground(mBox,tiles)){
      case TILE_RED: mBox.x = resetX; mBox.y = resetY; break;
      case TILE_BLUE: resetX = mBox.x; resetY = mBox.y; break;
    }
}

// place camera over this object
void Entity::setCamera( SDL_Rect& camera ){
	
  //Center the camera
	camera.x = ( mBox.x + mBox.w / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + mBox.h / 2 ) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 ){camera.x = 0;}
	if( camera.y < 0 ){camera.y = 0;}
	if( camera.x > LEVEL_WIDTH - camera.w ){camera.x = LEVEL_WIDTH - camera.w;}
	if( camera.y > LEVEL_HEIGHT - camera.h ){camera.y = LEVEL_HEIGHT - camera.h;}
}

void Entity::render( SDL_Rect& camera, bool center){
 
  // center camera otherwise check if the entity is within the camera
  if(center) this->setCamera(camera);
  else if(!checkCollision(camera,mBox)) return;

  if (attack){
    mTexture->render( mBox.x - camera.x, mBox.y - camera.y,
        &mSpriteClips[2%TOTAL_ANIMATIONS][frame/FRAME_LENGTH], 0.0, NULL, 
        faceRight? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL); 
  }
  else{
    // Character is not moving 
    // Ensure clip does not exceed total animations in sprite sheet or # of
    // frames
    if(mVelX==0){
      mTexture->render( mBox.x - camera.x, mBox.y - camera.y, 
          &mSpriteClips[0%TOTAL_ANIMATIONS][frame/FRAME_LENGTH], 0.0, NULL, 
          faceRight? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL); 
    }
    else if(mVelX>0){
      // Character moving right
      mTexture->render( mBox.x - camera.x, mBox.y - camera.y, 
          &mSpriteClips[1%TOTAL_ANIMATIONS][frame/FRAME_LENGTH], 0.0, NULL,  SDL_FLIP_NONE);
      faceRight = true;
    }
    else {
      // Character moving left
      mTexture->render( mBox.x - camera.x, mBox.y - camera.y, 
          &mSpriteClips[1%TOTAL_ANIMATIONS][frame/FRAME_LENGTH], 0.0, NULL, SDL_FLIP_HORIZONTAL);
      faceRight = false;
    }
  }
  // Increase Frames
  frame++;
  if(frame/FRAME_LENGTH>=TOTAL_FRAMES){
    frame = 0;
    attack = false;
  }
}
