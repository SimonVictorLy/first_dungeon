#ifndef _Entity_H
#define _Entity_H

class Entity
{
  public:
    /*
    static const int ENTITY_HEIGHT = 32; // height
    static const int ENTITY_WIDTH = 32; // width
    static const int ENTITY_VEL = 10;   // velocity
    */

    Entity();

    void handleEvent(SDL_Event& e);
    void move (Tile *tiles[]);
    void setCamera(SDL_Rect& camera);
    void render(SDL_Rect& camera);


  private:
    int mVelX, mVelY, resetX, resetY;
    SDL_Rect mBox;
    bool inAir = false;
    int frame;
    bool dir;
};

#endif
