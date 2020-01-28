#ifndef _Entity_H
#define _Entity_H

class Entity
{
  public:
    static const int ENTITY_HEIGHT = 32; // height
    static const int ENTITY_WIDTH = 32; // width
    static const int ENTITY_VEL = 10;   // velocity

    Entity(LTexture *loadTexture);
    Entity(
        int x, int y, int width, int height, 
        LTexture *loadTexture, 
        int velocityX, 
        int velocityY);

    void handleEvent(SDL_Event& e);
    void move (Tile *tiles[]);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera,bool center);
    void getSpriteSheet();

  private:
    int mVelX, mVelY, resetX, resetY;
    SDL_Rect mBox;
    bool airborn = false;
    int frame;
    bool faceRight;
    bool attack;
    int TOTAL_ANIMATIONS;
    int TOTAL_FRAMES;
    int FRAME_LENGTH;

    LTexture *mTexture;
    SDL_Rect **mSpriteClips;
};

#endif
