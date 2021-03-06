#include "player.h"

Player::Player(dWorldID w, dSpaceID s) : GameObject(w, s), horizontalAngle(0.0f), verticalAngle(0.0f), angularSpeed(2.5f)
{
    SetupCamera();
}

Player::~Player(){

}

void Player::SetupCamera(){
    // Set up the OpenFrameworks camera
    camera.setGlobalPosition(position.x, position.y, position.z);
    camera.lookAt(ofVec3f(0,1,0));

    camera.setFov(90);
    camera.setNearClip(0.01f);
    camera.setFarClip(51000);
}

void Player::Update(float deltaTime){
    //GameObject::Update(deltaTime);//SUPER
    //Update Camera Pos
    const dReal* pos = dBodyGetPosition(body);
    position = ofVec3f(pos[0], pos[1], pos[2]);
    const dReal newRot[4] = {1, 0, 0, 0};
    dBodySetQuaternion(body, newRot);//keep player upright
    camera.setPosition(position.x, position.y, position.z + (scale.z));
}

void Player::SetRotation(ofQuaternion rotation){
    GameObject::SetRotation(rotation);
    camera.setOrientation(rotation);
}

/*
 * Rotate Object by amount on axis
 */
void Player::Rotate(ofVec3f rotationAxis, float amount){
    rotation *= glm::angleAxis(glm::radians(amount), glm::vec3(rotationAxis));
    //TODO CLAMP VERTICAL AXIS
    camera.setGlobalOrientation(rotation);
}

void Player::Draw(){
    GameObject::Draw();
}

void Player::SetPosition(ofVec3f position){
    GameObject::SetPosition(position);
    camera.setPosition(position.x, position.y, position.z + (scale.z));
}

void Player::FrameBegin(){
    //background colour
    ofBackground(20);
    camera.begin();
    ofEnableDepthTest();
    ofPushMatrix();
}

void Player::FrameEnd(){
    ofDisableDepthTest();
    camera.end();
    ofPopMatrix();
}
