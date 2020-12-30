#include "ofApp.h"
#include <ode/ode.h>
#include <glm/glm.hpp>
#define LENGTH 0.7      // chassis length
#define WIDTH 0.5       // chassis width
#define HEIGHT 0.2      // chassis height
#define RADIUS 0.18     // wheel radius
#define STARTZ 0.5      // starting height of chassis
#define CMASS 1         // chassis mass
#define WMASS 0.2       // wheel mass

static const dVector3 yunit = { 0, 1, 0 }, zunit = { 0, 0, 1 };
World* ofApp::world;

//--------------------------------------------------------------
void ofApp::setup(){

    // Set up the OpenFrameworks camera
    ofVec3f upVector;
    upVector.set(0, 0, 1);
    cam.setAutoDistance(false);
    cam.setPosition(10,10,10);
    cam.lookAt({0,0,0},upVector);
    cam.setUpAxis(upVector);


/*
    //ODE setup Create world
    dInitODE2(0);

    worldID = dWorldCreate();
    space = dHashSpaceCreate(0);
    contactGroup = dJointGroupCreate(0);
    dWorldSetGravity(worldID, 0,0, -0.5);
    groundtmp = dCreatePlane(space, 0, 0, 1, 0);//THIS IS WHAT COLLIDES
    dSpaceCollide(space, 0, &nearCallback);

    //DEBUG TO REMOVE ODE SPACES
    physicsSpace = dSimpleSpaceCreate(space);
    dSpaceSetCleanup(physicsSpace, 0);

    cube = new Cube(5,5,5);
    cube->SetWorldID(worldID, physicsSpace);
    ofVec3f cubPos(0.f, 0.f, 15.f);
    cube->SetPositon(cubPos);

    ground = new Cube(10, 10, 0.25);
    ground->SetWorldID(worldID, physicsSpace);
    //ground->SetKinematic(true);
    ground->SetPositon(ofVec3f(0.0f, 0.f, 50.f));
    ground->SetColour(ofColor::green);
*/

    ofApp::world = new World();
    //dAllocateODEDataForThread(dAllocateMaskAll);
}

//--------------------------------------------------------------
void ofApp::update(){
    // TODO: PASTE IN THE REQUIRED CODE TO UPDATE ODE
    //       (THE FIRST PART OF SIMLOOP)
    //  |    FROM DEMO_BUGGY.CPP
    //  |
    //  |
    //  v

    /*
    cube->Update(3);//TODO: replace 3 with delta time
    ground->Update(3);


    dSpaceCollide (space, 0, &nearCallback);
    dWorldStep (world->GetWorldID(),0.05); //Simulates physics

    dJointGroupEmpty(contactGroup);//Remove all contact joints*/
    ofApp::world->Update(3);
}
//--------------------------------------------------------------
void ofApp::draw(){

    // draw the scene
    ofBackground(20);
    cam.begin();

    ofEnableDepthTest();

    ofPushMatrix();

    ofSetColor(ofColor::lightGrey);
    //ofDrawGrid(0.5f, 100, false, false,false,true);
    ofDrawAxis(10);

    /*cube->Draw();
    ground->Draw();*/
    ofApp::world->Draw();

    ofDisableDepthTest();
    cam.end();
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::exit() {
    // TODO: CLEAN UP ODE ON EXIT (END OF MAIN IN DEMO_BUGGY.CPP)
    //
    //  |
    //  |
    //  |
    //  v




    // /////////////////////////////////
}
//--------------------------------------------------------------

/*
static void nearCallback(void *, dGeomID o1, dGeomID o2){
    myApp->collide(o1, o2);
}*/

void ofApp::collide(dGeomID o1, dGeomID o2)
{
  int i,n;

  // only collide things with the ground
  int g1 = (o1 == ground->GetGeom() || o1 == groundtmp);
  int g2 = (o2 == ground->GetGeom()|| o2 == groundtmp);
  if (!(g1 ^ g2)) return;

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
      dJointID c = dJointCreateContact (worldID, contactGroup, &contact[i]);
      dJointAttach (c,
                    dGeomGetBody(contact[i].geom.g1),
                    dGeomGetBody(contact[i].geom.g2));
    }
  }
}



void ofApp::drawODEDemoBox(const dReal*pos_ode, const dQuaternion rot_ode, const dReal*sides_ode){


    dReal siz[3] = {sides_ode[0],sides_ode[1],sides_ode[2]};

    ofVec3f      position(pos_ode[0], pos_ode[1], pos_ode[2]);

    // ODEs quaternions are stored in a different order to openFrameworks:
    ofQuaternion rotation(rot_ode[1], rot_ode[2], rot_ode[3], rot_ode[0]);

    float rotationAmount;
    ofVec3f rotationAngle;
    rotation.getRotate(rotationAmount, rotationAngle);

    ofPushMatrix();
    ofTranslate(position);

    ofRotateDeg(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    ofDrawBox(0,0,0, siz[0],siz[1],siz[2]);
    ofPopMatrix();
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key) {
    // TODO: EMULATE command(..) from DEMO_BUGGY.CPP
    //       MAKE THE KEYS CONTROL THE SPEED AND STEER
    //  |    HINT: IT'S EXACTLY THE SAME AS DEMO_BUGGY
    //  |    WITH A DIFFERENT VARIABLE NAME!
    //  |
    //  v




    // /////////////////////////////////
    case 'q':
        ofExit();
        break;
    }
//    cout<<speed<<endl;
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
