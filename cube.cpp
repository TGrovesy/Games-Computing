#include "cube.h"

Cube::Cube(float x, float y, float z) : GameObject(){
    SetScaling(ofVec3f(x,y,z));
}

Cube::Cube(dWorldID w, dSpaceID s, float x, float y, float z) : GameObject(w,s)
{
    SetScaling(ofVec3f(x,y,z));
}

Cube::~Cube(){
}

void Cube::Update(float deltaTime){
    GameObject::Update(deltaTime);//SUPER!
    //ODE STUFF!
}

void Cube::Draw(){
    GameObject::Draw();//SUPER!


    //COLOUR

    ofSetColor(this->colour);

    ofBoxPrimitive b;

    b.set(scale.x, scale.y, scale.z);


    b.setGlobalOrientation(rotation);

    // Now set the box's position according to ODE physics:
    b.setGlobalPosition(position);


    if(isTextured){
        //texture.draw(texturePoints[0],texturePoints[1],texturePoints[2],texturePoints[3]);
        b.mapTexCoords(0,0,textureScale.x,textureScale.y);
        texture->bind();
    }
    if(hasMaterial) material->begin();
    // Draw it:
    b.draw();
    if(hasMaterial)material->end();
    if(isTextured)texture->unbind();
}

void Cube::SetColour(ofColor colour){
    this->colour = colour;
}
