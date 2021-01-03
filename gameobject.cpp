#include "gameobject.h"

GameObject::GameObject(dWorldID w, dSpaceID s) : name("UNNAMED"), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.f, 0.0f, 1.0f, 0.0f), rotationAmount(0.0f){
    SetupPhysics(w, s);
}

GameObject::~GameObject(){

}

/*
Used for updateing the physics
*/
void GameObject::Update(float deltaTime){
    const dReal* pos = dBodyGetPosition(body);
    const dReal* rot = dBodyGetQuaternion(body);
    position = ofVec3f(pos[0], pos[1], pos[2]);
    rotation = ofQuaternion(rot[1], rot[2], rot[3], rot[0]);
    rotation.getRotate(rotationAmount, rotationAngle);
}

/*
Drawing the GameObject
*/
void GameObject::Draw(){

}

/*
Set Position of Object
*/
void GameObject::SetPosition(ofVec3f pos){
    //std::cout << "pos set: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    this->position = pos;

    dBodySetPosition(body, position.x, position.y, position.z);//set body position ODE
}

/*
Set Object Scaleing
*/
void GameObject::SetScaling(ofVec3f scale){
    this->scale = scale;
    //ODE SCALE
    dGeomDestroy(geom);//destory current geom
    geom = dCreateBox(space, scale.x, scale.y, scale.z);
    dGeomSetBody(geom, body);//Link Geom To Body
}

/*
Set Rotation Of Object
*/
void GameObject::SetRotation(ofQuaternion rotation){
    rotation.getRotate(rotationAmount, rotationAngle);
    std::cout << rotationAngle.y << ", ";
    std::cout << rotationAngle.z << std::endl;
    const dReal newRot[4] = {rotation.w(), rotation.x(),rotation.y(), rotation.z()};
    dBodySetQuaternion(body, newRot);
}

/*
 * Rotate Object by amount on axis
 */
void GameObject::Rotate(ofVec3f rotationAxis, float amount){
    /*ofQuaternion newRot = rotation;
    newRot.makeRotate(rotationAmount + amount, rotationAxis);
    rotation = newRot;
    if(rotationAmount>=360){
        std::cout << "rest\n";
        rotationAmount = (int) rotationAmount % 360;
    }
    rotation.getRotate(rotationAmount, rotationAngle);*/
    rotation *= glm::angleAxis(glm::radians(amount), glm::vec3(rotationAxis));

    const dReal odeRot[4] = {rotation.w(), rotation.x(),rotation.y(), rotation.z()};
    dBodySetQuaternion(body, odeRot);
}


/*
Setup Physics Of Object
*/
void GameObject::SetupPhysics(dWorldID id, dSpaceID collisionSpace){
    world = id;
    space = collisionSpace;
    body = dBodyCreate(id);
    dBodySetPosition(body, position.x, position.y, position.z);
    dMassSetBox(&mass, 1, scale.x, scale.y, scale.z);//Default mass is 1 unit. Always edit in after object
    dMassAdjust(&mass, 1);//Update mass with density of 1
    dBodySetMass(body, &mass);//Set body mass
    geom = dCreateBox(collisionSpace, scale.x, scale.y, scale.z);
    dGeomSetBody(geom, body);//Link Geom To Body
}

/*
 * Sets Body To be kinematic
 */
void GameObject::SetKinematic(bool value){
    this->isKinematic = value;
    if(value) dBodySetKinematic (body);

}

/*
Disables Gravity
*/
void GameObject::Freeze(){
    dBodySetGravityMode(body, 0);
    //dJointCreateFixed (world, jointGroupID);
}

/*
 * Set mass of object
*/
void GameObject::SetMass(dReal newMass){
    dMassSetBox(&mass, newMass, scale.x, scale.y, scale.z);//Default mass is 1 unit. Always edit in after object
    dMassAdjust(&mass, 1);
    dBodySetMass(body, &mass);
}

