#include "flyingcar.h"

FlyingCar::FlyingCar(dWorldID worldID, dSpaceID spaceID): GameObject(worldID, spaceID)
{

}

FlyingCar::~FlyingCar(){

}


void FlyingCar::SetupBody(dWorldID worldID, dSpaceID spaceID){
    world = worldID;
    space = spaceID;
}

void FlyingCar::Update(float deltaTime){
    GameObject::Update(deltaTime);

    if(position.z < (fixedPos.z - maxDeviation)){
        dBodyAddForce(body, 0, 0, 10);
    }else if(position.z > (fixedPos.z + maxDeviation)){
        dBodyAddForce(body, 0, 0, -10);
    }else if(position.y < (fixedPos.y - maxDeviation)) {
        dBodyAddForce(body, 0, 10, 0);
    }else if(position.y > (fixedPos.y + maxDeviation)) {
        dBodyAddForce(body, 0, -10, 0);
    }else if(position.x < (fixedPos.x - maxDeviation)) {
        dBodyAddForce(body, 10, 0, 0);
    }else if(position.x > (fixedPos.x + maxDeviation)) {
        dBodyAddForce(body, -10, 0, 0);
    }


    //Pitch Control
    if(rotation.getEuler().y < -22.5){
        dBodyAddRelTorque(body, 0, 1, 0);
    }else if(rotation.getEuler().y > 22.5){
        dBodyAddRelTorque(body, 0, -1, 0);
    }else if(rotation.getEuler().x < -22.5){//Roll Control
        dBodyAddRelTorque(body, 1, 0, 0);
    }else  if(rotation.getEuler().x > 22.5){
        dBodyAddRelTorque(body, -1, 0, 0);
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


void FlyingCar::SetFixedPosition(ofVec3f pos, float maxDeviation){
    this->SetPosition(pos);
    this->fixedPos = pos;
    this->maxDeviation = maxDeviation;
}

void FlyingCar::ForceUpright(){

}
