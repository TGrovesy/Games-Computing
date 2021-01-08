#include "flyingcar.h"

FlyingCar::FlyingCar(dWorldID worldID, dSpaceID spaceID) : GameObject(worldID, spaceID)
{
    SetupBody();
}

FlyingCar::~FlyingCar(){

}


void FlyingCar::SetupBody(){

}

void FlyingCar::Update(float deltaTime){
    GameObject::Update(deltaTime);

    if(position.z < fixedHeight) dBodyAddForce(body, 0, 0, 100);

    //Pitch Control
    if(rotation.getEuler().y < -22.5){
        //std::cout <<"tor\n";
        dBodyAddRelTorque(body, 0, 100, 0);
    }

    if(rotation.getEuler().y > 22.5){
        dBodyAddRelTorque(body, 0, -100, 0);
    }

    //Roll Control
    if(rotation.getEuler().x < -22.5){
        //std::cout <<"tor\n";
        dBodyAddRelTorque(body, 100, 0, 0);
    }

    if(rotation.getEuler().x > 22.5){
        dBodyAddRelTorque(body, -100, 0, 0);
    }
}

void FlyingCar::Draw(){
    GameObject::Draw();//SUPER!


    //COLOUR

    ofSetColor(ofColor::coral);

    ofPushMatrix();
    ofTranslate(position);
    ofRotateDeg(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    ofDrawBox(0,0,0, GetScale().x, GetScale().y, GetScale().z);
    ofPopMatrix();
}


void FlyingCar::ForceUpright(){

}
