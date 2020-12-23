#include "cube.h"

Cube::Cube(float x, float y, float z) : GameObject()
{
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << x << ", "<< y << ", "<< z << std::endl;
    SetScaling(glm::vec3(x,y,z));
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
    ofRotateDeg(rotationAmount, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ofDrawBox(0,0,0, GetScale().x, GetScale().y, GetScale().z);
    ofPopMatrix();
}

void Cube::SetColour(ofColor colour){
    this->colour = colour;
}
