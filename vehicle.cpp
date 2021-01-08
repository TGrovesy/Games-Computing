#include "vehicle.h"
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

    //Create wheels
    for(int i = 0; i < 4; i++){
        wheels[i] = new Cylinder(world, space, 1.5, 2);
        wheels[i]->SetMass(WHEEL_MASS);
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

    speed = 2000.f;
    //speed stuff
    dJointSetHinge2Param(wheelJoint[2], dParamVel2, -1.5);//TODO CHANGE TO SPEED!
    dJointSetHinge2Param(wheelJoint[2], dParamFMax2, 10.1);
    dJointSetHinge2Param(wheelJoint[3], dParamVel2, -1.5);
    dJointSetHinge2Param(wheelJoint[3], dParamFMax2, 10.1);

    //std::cout<<name<<std::endl;
    // steering
    // Clamp vertical angle to [-85, 85] degrees
    //steer applyer
    /*srand((unsigned) time(0));
    int randomNumber;
    randomNumber = (rand() * rand()) % 2;
    if(randomNumber == 0){
        steer += 0.002f;
    }else{
        steer -= 0.002f;
    }*/

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

}

void Vehicle::Draw(){
    ofSetColor(ofColor::pink);

    //Draw chasis
    chasis->Draw();

    for(int i = 0; i < 4; i++){
        wheels[i]->Draw();
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


