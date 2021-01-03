#include "cube.h"

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

    ofPushMatrix();
    ofTranslate(position);
    ofRotateDeg(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    ofDrawBox(0,0,0, GetScale().x, GetScale().y, GetScale().z);
    ofPopMatrix();
}

void Cube::SetColour(ofColor colour){
    this->colour = colour;
}
