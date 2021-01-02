#include "world.h"
#include "cube.h"
#include "ofApp.h"

World::World()
{
    SetupWorld();
}

void World::SetupWorld(){
    SetupPhysics();

    //TODO: DEBUG TO TEST REMOVE!
    for(int i = 0; i < 2; i++){
        Cube* cube = new Cube(this->worldID, this->spaceID,contactGroup, 5,5,5);
        cube->SetPositon(ofVec3f((i * 2), 0, ((float)i * 20) + 20));
        gameObjects.push_back(cube);
    }

    ground = new Cube(this->worldID, this->spaceID, contactGroup,  10, 10, 1);
    ground->SetPositon(ofVec3f(0.f, 0.f, 0));
    ground->SetColour(ofColor::green);
    ground->Freeze();
    ground->SetMass(1000000);
    gameObjects.push_back(ground);
}

void World::SetupPhysics(){
    //ODE setup Create world
    dInitODE2(0);
    this->worldID = dWorldCreate();
    this->spaceID = dHashSpaceCreate(0);
    this->contactGroup = dJointGroupCreate(0);
    dWorldSetGravity(this->worldID, 0, 0, gravityValue);
    //DEBUG REMOVE AND REPLACE WITH PROPER GROUND
    //groundTMP = dCreatePlane(this->spaceID, 0, 0, 1, 0);//THIS IS WHAT COLLIDES

    dSpaceCollide(spaceID, 0, &nearCallback);
    dAllocateODEDataForThread(dAllocateMaskAll);
}


void World::SetWorldID(dWorldID id){
    this->worldID = id;
}

void World::SetSpaceID(dSpaceID id){
    this->spaceID = id;
}

static void nearCallback(void *, dGeomID o1, dGeomID o2){

    ofApp::GetWorld()->collide(o1, o2);

}


void World::collide(dGeomID o1, dGeomID o2){
    int i,n;

      // only collide things with the ground
      int g1 = (o1 == ground->GetGeom());
      int g2 = (o2 == ground->GetGeom());
      //if (!(g1 ^ g2)) return;

      const int N = 10;
      dContact contact[N];
      n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
      if (n > 0) {
        for (i=0; i<n; i++) {
          contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
            dContactSoftERP | dContactSoftCFM | dContactApprox1;
          contact[i].surface.mu = dInfinity;
          contact[i].surface.slip1 = 0.1;
          contact[i].surface.slip2 = 0.1;
          contact[i].surface.soft_erp = 0.5;
          contact[i].surface.soft_cfm = 0.3;
          dJointID c = dJointCreateContact (worldID,contactGroup,&contact[i]);
          dJointAttach (c,
                        dGeomGetBody(contact[i].geom.g1),
                        dGeomGetBody(contact[i].geom.g2));
        }
      }
}

void World::Draw(){
    for(int i = 0; i < (int)gameObjects.size(); i++){
        gameObjects.at(i)->Draw();
    }
}

void World::Update(float deltaTime){
    for(int i = 0; i < (int)gameObjects.size(); i++){
        gameObjects.at(i)->Update(deltaTime);
    }

    //ground->SetPositon(ofVec3f(0.f, 0.f, -15));//TODO REMOVE DEBUG

    dSpaceCollide (this->spaceID, 0, &nearCallback);
    dWorldStep (this->worldID, 0.05); //Simulates physics

    dJointGroupEmpty(contactGroup);//Remove all contact joints
}


