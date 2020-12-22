#include "gameobject.h"

GameObject::GameObject() : name("UNNAMED"), position(0.0f, 10.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotationAxis(0.0f, 1.0f, 0.0f), rotationAmount(0.0f){

}

GameObject::~GameObject(){

}

void GameObject::Update(float deltaTime){
    //TODO: ODE STUFF
    const dReal* pos = dBodyGetPosition(body);
    position = glm::vec3(pos[0], pos[1], pos[2]);
}

void GameObject::Draw(){

}

void GameObject::SetPositon(glm::vec3 pos){
    this->position = pos;
}

void GameObject::SetScaling(glm::vec3 scale){
    this->scale = scale;
}

void GameObject::SetRotation(glm::vec3 rotation, float angleDegrees){
    this->rotationAmount = angleDegrees;
    this->rotationAxis = rotation;
}

void GameObject::SetWorldID(dWorldID id){
    this->world = id;
    //ODE

    body = dBodyCreate(world);
    dBodySetPosition(body, position.x,position.y,position.z);//set body position
    dMassSetBox(&mass, 1, scale.x, scale.y, scale.z);
    dMassAdjust(&mass, 1);
    dBodySetMass(body, &mass);
    geom = dCreateBox(0, scale.x, scale.y, scale.z);
    dGeomSetBody(geom, body);
}
