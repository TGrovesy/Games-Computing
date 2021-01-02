#include "gameobject.h"

GameObject::GameObject(dWorldID w, dSpaceID s, dJointGroupID j) : name("UNNAMED"), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.f, 0.0f, 1.0f, 0.0f), rotationAmount(0.0f){
    jointGroupID = j;
    SetupPhysics(w, s);
}

GameObject::~GameObject(){

}

void GameObject::Update(float deltaTime){
    //TODO: ODE STUFF
    //gravity
    if(!isKinematic){
        const dReal* pos = dBodyGetPosition(body);
        const dReal* rot = dBodyGetQuaternion(body);
        position = ofVec3f(pos[0], pos[1], pos[2]);
        rotation = ofQuaternion(rot[1], rot[2], rot[3], rot[0]);
        rotation.getRotate(rotationAmount, rotationAngle);
    }
}

void GameObject::Draw(){

}

void GameObject::SetPositon(ofVec3f pos){
    //std::cout << "pos set: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    this->position = pos;

    dBodySetPosition(body, position.x, position.y, position.z);//set body position ODE
}

void GameObject::SetScaling(ofVec3f scale){
    this->scale = scale;
    //ODE SCALE
    dGeomDestroy(geom);//destory current geom
    geom = dCreateBox(space, scale.x, scale.y, scale.z);
    dGeomSetBody(geom, body);
}

void GameObject::SetRotation(ofQuaternion rotation, float angleDegrees){
    this->rotationAmount = angleDegrees;
    this->rotation = rotation;
    const dReal* newRot = new dReal[]{rotation.w(), rotation.x(),rotation.y(), rotation.z()};
    dBodySetQuaternion(body, newRot);
}

void GameObject::SetupPhysics(dWorldID id, dSpaceID collisionSpace){
    world = id;
    space = collisionSpace;
    body = dBodyCreate(id);
    dBodySetPosition(body, position.x, position.y, position.z);
    dMassSetBox(&mass, 1, scale.x, scale.y, scale.z);
    dMassAdjust(&mass, 1);
    dBodySetMass(body, &mass);
    geom = dCreateBox(collisionSpace, scale.x, scale.y, scale.z);
    dGeomSetBody(geom, body);
}

void GameObject::SetKinematic(bool value){
    this->isKinematic = value;
    if(value) dBodyIsKinematic(body);

}

void GameObject::Freeze(){
    dBodySetGravityMode(body, 0);
    //dJointCreateFixed (world, jointGroupID);
}

void GameObject::SetMass(dReal newMass){
    dMassAdjust(&mass, newMass);
    dBodySetMass(body, &mass);
}

