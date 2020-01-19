#include "def.h"

Entity::Entity(){
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	  mBox.w = ENTITY_WIDTH;
	  mBox.h = ENTITY_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 10;

    dir = true;
    frame =0;

    resetX = 0;
    resetY = 0;
    inAir = true;
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
            case SDLK_SPACE: mVelY = inAir ? mVelY : -20; break;
        }
    }
	else if( e.type == SDL_KEYDOWN && e.key.repeat == 1 ){
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            //case SDLK_SPACE: mVelY = inAir ? mVelY : -15; break;
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
    if( ( mBox.x < 0 ) || ( mBox.x + ENTITY_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;
    
    // 10 is max fall speed. 
    
    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + ENTITY_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) ){
        //move back
        mBox.y -= mVelY;
        mVelY = 0;
        inAir = false;
    }
    else{
      inAir = true;
    }
    mVelY = mVelY < 10 ? mVelY + 2: mVelY;
    
    switch(checkBackground(mBox,tiles)){
      case TILE_RED: mBox.x = resetX; mBox.y = resetY; break;
      case TILE_BLUE: resetX = mBox.x; resetY = mBox.y; break;
    }
}

void Entity::setCamera( SDL_Rect& camera ){
	//Center the camera over the dot
	camera.x = ( mBox.x + ENTITY_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + ENTITY_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 ){camera.x = 0;}
	if( camera.y < 0 ){camera.y = 0;}
	if( camera.x > LEVEL_WIDTH - camera.w ){camera.x = LEVEL_WIDTH - camera.w;}
	if( camera.y > LEVEL_HEIGHT - camera.h ){camera.y = LEVEL_HEIGHT - camera.h;}
}

void Entity::render( SDL_Rect& camera ){
  //Show the dot
  SDL_Rect idle;
  idle.x = 0;
  idle.y = 0;
  idle.w = 32;
  idle.h = 32;
  
  if(mVelX==0){
    if(dir)gPersonTexture.render( mBox.x - camera.x, mBox.y - camera.y, &idle);
    else gPersonTexture.render( mBox.x - camera.x, mBox.y - camera.y, &idle,0.0,NULL, SDL_FLIP_HORIZONTAL);
  }
  else if(mVelX>0){
    gPersonTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gSpriteClips[frame/4], 0.0, NULL,  SDL_FLIP_NONE);
    dir = true;
  }
  else {
    gPersonTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gSpriteClips[frame/4], 0.0, NULL, SDL_FLIP_HORIZONTAL);
    dir = false;
  }
  
  frame++;
  if(frame/4==4) frame = 0;
}
