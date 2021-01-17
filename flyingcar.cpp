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

/*
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
    }*/

    //SetFixedPosition(ofVec3f(position.x + 0.25, position.y, position.z), 0.5);
    dBodySetLinearVel(body, 5,0,0);
/*
    //Pitch Control
    if(rotation.getEuler().y < -22.5){
        dBodyAddRelTorque(body, 0, 1, 0);

    }else if(rotation.getEuler().y > 22.5){
        dBodyAddRelTorque(body, 0, -1, 0);
    }else if(rotation.getEuler().x < -22.5){//Roll Control
        dBodyAddRelTorque(body, 1, 0, 0);
    }else  if(rotation.getEuler().x > 22.5){
        dBodyAddRelTorque(body, -1, 0, 0);
    }*/
}

void FlyingCar::Draw(){
    GameObject::Draw();//SUPER!



    //COLOUR
    ofSetColor(ofColor::green);

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

void FlyingCar::SetPosition(ofVec3f pos){
    this->position = pos;

    dBodySetPosition(body, position.x, position.y, position.z);//set body position ODE
    SetFixedPosition(pos, maxDeviation);
}

void FlyingCar::SetFixedPosition(ofVec3f pos, float maxDeviation){
    //this->SetPosition(pos);
    this->fixedPos = pos;
    this->maxDeviation = maxDeviation;
}

void FlyingCar::ForceUpright(){

}
