#include "vehicle.h"
#include "material.h"
#include "texture.h"
#include<time.h>

#define VEHICLE_LENGTH 10
#define VEHICLE_WIDTH 5
#define VEHICLE_HEIGHT 5
#define WHEEL_RADIUS 1.5
#define WHEEL_MASS 100
#define VMASS 1000

static const dVector3 yunit = { 0, 1, 0 }, zunit = { 0, 0, 1 };


Vehicle::Vehicle(dWorldID worldID, dSpaceID spaceID, dJointGroupID jointGroup) : GameObject(worldID, spaceID)
{
    group = jointGroup;
    SetupBody(worldID, spaceID);
}




Vehicle::~Vehicle(){
    chasis->~Cube();
    for(int i = 0; i < 4; i++){
        wheels[i]->~Cylinder();
    }
}


void Vehicle::SetupBody(dWorldID worldID, dSpaceID spaceID){
    //general physics stuff
    world = worldID;
    space = spaceID;


    //chasis body
    chasis = new Cube(world, space, VEHICLE_LENGTH, VEHICLE_WIDTH, VEHICLE_HEIGHT);
    chasis->SetColour(ofColor::pink);
    chasis->SetMass(VMASS);
    chasis->SetName("Truck");
    chasis->SetTexture("metal.jpg");
    chasis->SetMaterial(Material::GetMaterial("chrome"));

    if(lightsEnabled){
        //headlights
        for(int i = 0; i < 2; i++){
            headlights[i] = new ofLight();
            headlights[i]->rotateDeg(-90, glm::vec3(0, 1, 0));

            headlights[i]->setPosition(ofVec3f(chasis->GetPosition().x + (VEHICLE_LENGTH / 2) + 1, chasis->GetPosition().y, chasis->GetPosition().z - 2.5));
            if(i == 1)headlights[i]->setPosition(ofVec3f(chasis->GetPosition().x + (VEHICLE_LENGTH / 2) + 1, chasis->GetPosition().y, chasis->GetPosition().z + 2.5));
            headlights[i]->setSpotlight(20, 64);
            headlights[i]->setDiffuseColor(ofFloatColor::yellow);
            headlights[i]->setup();
            headlights[i]->enable();
        }
    }

    //Create wheels
    for(int i = 0; i < 4; i++){
        wheels[i] = new Cylinder(world, space, 1.5, 2);
        wheels[i]->SetMass(WHEEL_MASS);
        wheels[i]->SetName("Wheel");
        wheels[i]->SetTexture("adam.jpg");
        wheels[i]->SetMaterial(Material::GetMaterial("rubber"));
    }

    wheels[0]->SetColour(ofColor::brown);
    wheels[1]->SetColour(ofColor::brown);
    //move wheels to correct pos
    //front left wheel
    wheels[0]->SetPosition(ofVec3f(position.x + (0.5 * VEHICLE_LENGTH), position.y - (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
    //front right wheel
    wheels[1]->SetPosition(ofVec3f(position.x + (0.5 * VEHICLE_LENGTH), position.y + (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
    //back left wheel
    wheels[2]->SetPosition(ofVec3f(position.x - (0.5 * VEHICLE_LENGTH), position.y - (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
    //back right wheel
    wheels[3]->SetPosition(ofVec3f(position.x - (0.5 * VEHICLE_LENGTH), position.y + (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));

    for(int i = 0; i < 4; i++){
        wheels[i]->Rotate(ofVec3f(0, 1, 0), 90);
    }

    //Wheel Hinges
    for(int i = 0; i < 4; i++){
        wheelJoint[i] = dJointCreateHinge2(world, 0);//TODO 0 should be our joint contactgroup
        dJointAttach(wheelJoint[i], chasis->GetBody(), wheels[i]->GetBody());//Attach our wheel body to our chasis body
        const dReal* wheelPos = dBodyGetPosition(wheels[i]->GetBody());
        dJointSetHinge2Anchor(wheelJoint[i], wheelPos[0], wheelPos[1], wheelPos[2]);
        dJointSetHinge2Axes(wheelJoint[i], zunit, yunit);
    }

    //Set suspension
    for(int i = 0; i < 4; i++){
        dJointSetHinge2Param(wheelJoint[i], dParamSuspensionERP, 0.4);
        dJointSetHinge2Param(wheelJoint[i], dParamSuspensionCFM, 0.8);
    }

    //Lock back wheels
    for(int i = 2; i < 4; i++){
        dJointSetHinge2Param(wheelJoint[i], dParamLoStop, 0);
        dJointSetHinge2Param(wheelJoint[i], dParamHiStop, 0);
    }

    SetSpace(space);

    //Collision fix
    for(int i = 0; i < 4; i++){
        //dBodySetLinearDampingThreshold(wheels[i]->GetBody(), 0.02);
    }
}


void Vehicle::Update(float deltaTime){

    int randomNumber;
    randomNumber = (rand()) % 2;
    if(randomNumber == 0){
        speed += 0.1f;
    }else{
        speed -= 0.05f;
    }

    if(speed < 1) speed =1;//ensures minimum speed
    if(speed > 15) speed = 15;

    //speed stuff
    dJointSetHinge2Param(wheelJoint[2], dParamVel2, -speed);//TODO CHANGE TO SPEED!
    dJointSetHinge2Param(wheelJoint[2], dParamFMax2, 10.1);
    dJointSetHinge2Param(wheelJoint[3], dParamVel2, -speed);
    dJointSetHinge2Param(wheelJoint[3], dParamFMax2, 10.1);

    //std::cout<<name<<std::endl;
    // steering
    // Clamp vertical angle to [-85, 85] degrees
    //steer applyer
    //srand((unsigned) time(0));
    randomNumber = (rand()) % 2;
    if(randomNumber == 0){
        steer += 0.01f;
    }else{
        steer -= 0.01f;
    }

    if(steer > 1)steer = 1;
    if(steer < -1) steer = -1;

    steer = std::max(-4.0f, std::min(4.0f, steer));
    dReal v = steer - dJointGetHinge2Angle1 (wheelJoint[0]);
    if (v > 0.1) v = 0.1;
    if (v < -0.1) v = -0.1;
    v *= 10.0;
    dJointSetHinge2Param (wheelJoint[0],dParamVel,v);
    dJointSetHinge2Param (wheelJoint[0],dParamFMax,0.2);
    dJointSetHinge2Param (wheelJoint[0],dParamLoStop,-0.75);
    dJointSetHinge2Param (wheelJoint[0],dParamHiStop, 0.75);
    dJointSetHinge2Param (wheelJoint[0],dParamFudgeFactor,0.1);

    wheels[1]->SetRotation(wheels[0]->GetRotation());

    //steering

    chasis->Update(deltaTime);
    for(int i = 0; i < 4; i++){
        wheels[i]->Update(deltaTime);
    }




    this->position = chasis->GetPosition();
    dBodySetPosition(body, position.x, position.y, position.z);
}

void Vehicle::Draw(){
    ofSetColor(ofColor::pink);

    //Draw chasis
    chasis->Draw();

    for(int i = 0; i < 4; i++){
        wheels[i]->Draw();
    }

    if(lightsEnabled){
        for(int i =0; i < 2;  i++){
            headlights[i]->setPosition(ofVec3f(chasis->GetPosition().x + (VEHICLE_LENGTH / 2) + 1, chasis->GetPosition().y, chasis->GetPosition().z - 2.5));
            if(i == 1)headlights[i]->setPosition(ofVec3f(chasis->GetPosition().x + (VEHICLE_LENGTH / 2) + 1, chasis->GetPosition().y, chasis->GetPosition().z + 2.5));
            //headlights[i]->draw();
        }
    }
}

void Vehicle::SetPosition(ofVec3f pos){
    this->position = pos;
    chasis->SetPosition(position);
    //front left wheel
    wheels[0]->SetPosition(ofVec3f(position.x + (0.5 * VEHICLE_LENGTH), position.y - (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
    //front right wheel
    wheels[1]->SetPosition(ofVec3f(position.x + (0.5 * VEHICLE_LENGTH), position.y + (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
    //back left wheel
    wheels[2]->SetPosition(ofVec3f(position.x - (0.5 * VEHICLE_LENGTH), position.y - (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
    //back right wheel
    wheels[3]->SetPosition(ofVec3f(position.x - (0.5 * VEHICLE_LENGTH), position.y + (VEHICLE_WIDTH * 0.5), position.z - (VEHICLE_HEIGHT * 0.5)));
}


void Vehicle::Rotate(ofVec3f rot, float amount){
    chasis->Rotate(rot, amount);
    for(int i = 0; i < 4; i++){
        wheels[i]->Rotate(rot, amount);
    }
}

void Vehicle::SetScaling(ofVec3f scale){
    GameObject::SetScaling(scale);
    this->scale = scale;
    chasis->SetScaling(chasis->GetScale() * scale);
    for(int i = 0; i < 4; i++){
        wheels[i]->SetScaling(wheels[i]->GetScale() * scale);
    }
}

/*
 * Sets Body To be kinematic
 */
void Vehicle::SetKinematic(bool value){
    this->isKinematic = value;
    if(value){
        dBodySetKinematic(body);
        dBodySetKinematic(chasis->GetBody());
        dBodySetKinematic(wheels[0]->GetBody());
        dBodySetKinematic(wheels[1]->GetBody());
        dBodySetKinematic(wheels[2]->GetBody());
        dBodySetKinematic(wheels[3]->GetBody());
    }

}

void Vehicle::SetSpace(dSpaceID sp){
    dSpaceAdd(sp, chasis->GetGeom());
    for(int i = 0; i < 4; i++){
        dSpaceAdd(sp, wheels[i]->GetGeom());
    }
}


