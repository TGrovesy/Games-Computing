#include "wall.h"

Wall::Wall(dWorldID w, dSpaceID s, float x, float y, float z) : Cube(w,s,x,y,z)
{
    dBodyDisable(body);
}

Wall::~Wall(){

}

void Wall::Update(float deltaTime){
    position.x += 0.15;
}

void Wall::Draw(){
    Cube::Draw();//SUPER!
}

