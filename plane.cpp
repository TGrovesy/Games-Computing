#include "plane.h"

Plane::Plane(float x, float y, float z) : GameObject()
{
    SetScaling(ofVec3f(x,y,z));
}

void Plane::Draw(){
    ofPlanePrimitive plane;
    plane.set(GetScale().x, GetScale().y);
    plane.setGlobalOrientation(rotation);
    plane.setPosition(position);
    ofSetColor(ofColor::white);
    if(isTextured){
        //texture.draw(texturePoints[0],texturePoints[1],texturePoints[2],texturePoints[3]);
        plane.mapTexCoords(0,0,textureScale.x,textureScale.y);
        texture->bind();
    }
    if(hasMaterial) material->begin();
    // Draw it:
    plane.draw();
    if(hasMaterial)material->end();
    if(isTextured)texture->unbind();
}

void Plane::Update(float deltaTime){

}
