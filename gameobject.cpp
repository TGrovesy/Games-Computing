#include "gameobject.h"

GameObject::GameObject() : name("UNNAMED"), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 1.0f, 0.0f), rotationAmount(0.0f){

}

GameObject::~GameObject(){

}

void GameObject::Update(float deltaTime){
    //TODO: ODE STUFF
    //gravity
    if(!isKinematic){
        const dReal* pos = dBodyGetPosition(body);
        position = ofVec3f(pos[0], pos[1], pos[2]);
    }
}

void GameObject::Draw(){

}

void GameObject::SetPositon(ofVec3f pos){
    std::cout << "pos set: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    this->position = pos;

    dBodySetPosition(body, position.x, position.y, position.z);//set body position ODE
}

void GameObject::SetScaling(ofVec3f scale){
    this->scale = scale;
}

void GameObject::SetRotation(ofVec3f rotation, float angleDegrees){
    this->rotationAmount = angleDegrees;
    this->rotationAxis = rotation;
}

void GameObject::SetWorldID(dWorldID id, dSpaceID collisionSpace){
    this->world = id;
    //ODE
    body = dBodyCreate(world);
    dBodySetPosition(body, position.x,position.y,position.z);//set body position
    dMassSetBox(&mass, 1, scale.x, scale.y, scale.z);
    dMassAdjust(&mass, 1);
    dBodySetMass(body, &mass);
    geom = dCreateBox(0, scale.x, scale.y, scale.z);
    dGeomSetBody(geom, body);

    dSpaceAdd(collisionSpace, geom);
}

void GameObject::SetKinematic(bool value){
    this->isKinematic = value;
    if(value) dBodyIsKinematic(body);
}

