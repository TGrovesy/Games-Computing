#include "wall.h"
#include "ofApp.h"

Wall::Wall(dWorldID w, dSpaceID s, float x, float y, float z) : Cube(w,s,x,y,z)
{
    dBodyDisable(body);
}

Wall::~Wall(){

}

void Wall::Update(float deltaTime){

    ofVec3f playerPos = ofApp::GetWorld()->GetPlayer()->GetPosition();
    float xDistance = position.x - playerPos.x;
    if(speed >= 1.5f)speed = 1.5f;
    if(xDistance > 150) speed = 4.0f;
    position.x += speed;//TODO add acceleration
    speed += 0.005f;
}

void Wall::Draw(){
    Cube::Draw();//SUPER!
}

