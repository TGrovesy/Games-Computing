#include "gameobject.h"

GameObject::GameObject(){

}

GameObject::GameObject(dWorldID w, dSpaceID s) : name("UNNAMED"), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.f, 0.0f, 1.0f, 0.0f), rotationAmount(0.0f){
    SetupPhysics(w, s);
}


GameObject::~GameObject(){
    dBodyDestroy(body);
    dGeomDestroy(geom);
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
    this->rotation = rotation;
    rotation.getRotate(rotationAmount, rotationAngle);
    const dReal newRot[4] = {rotation.w(), rotation.x(),rotation.y(), rotation.z()};
    dBodySetQuaternion(body, newRot);

}

/*
 * Rotate Object by amount on axis
 */
void GameObject::Rotate(ofVec3f rotationAxis, float amount){

    rotation *= glm::angleAxis(glm::radians(amount), glm::vec3(rotationAxis));

    const dReal newRot[4] = {rotation.w(), rotation.x(),rotation.y(), rotation.z()};
    dBodySetQuaternion(body, newRot);

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
void GameObject::DisableGravity(){
    dBodySetGravityMode(body, 0);
}

/*
 * Set mass of object
*/
void GameObject::SetMass(dReal newMass){
    dMassSetBox(&mass, newMass, scale.x, scale.y, scale.z);//Default mass is 1 unit. Always edit in after object
    dMassAdjust(&mass, 1);
    dBodySetMass(body, &mass);
}

/*
 * Set Texture
 */
void GameObject::SetTexture(std::string path){
    texture = *new ofTexture();
    if(!ofLoadImage(texture, path)) { std::cerr << "Failed to load ground texture." << std::endl; }

    texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    isTextured = true;
}

/*
 * Set Texture
 */
void GameObject::SetTexture(ofTexture tex){
    texture = tex;

    texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
    isTextured = true;
}

void GameObject::SetMaterial(ofMaterial* material){
    this->material = material;
    hasMaterial = true;
}
