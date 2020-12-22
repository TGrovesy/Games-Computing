#include "ofApp.h"
#include <ode/ode.h>
#define LENGTH 0.7      // chassis length
#define WIDTH 0.5       // chassis width
#define HEIGHT 0.2      // chassis height
#define RADIUS 0.18     // wheel radius
#define STARTZ 0.5      // starting height of chassis
#define CMASS 1         // chassis mass
#define WMASS 0.2       // wheel mass

static const dVector3 yunit = { 0, 1, 0 }, zunit = { 0, 0, 1 };


//--------------------------------------------------------------
void ofApp::setup(){

    // Set up the OpenFrameworks camera
    ofVec3f upVector;
    upVector.set(0, 0, 1);
    cam.setAutoDistance(false);
    cam.setPosition(10,10,10);
    cam.lookAt({0,0,0},upVector);
    cam.setUpAxis(upVector);

    //TODO Debug Remove
    glm::vec3 size(5,5,5);
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    std::cout << size.x << ", "<< size.y << ", "<< size.z << std::endl;

    //ODE setup Create world
    dInitODE2(0);

    world = dWorldCreate();
    space = dHashSpaceCreate(0);
    contactGroup = dJointGroupCreate(0);
    dWorldSetGravity(world, 0,0, -0.5);


    cube = new Cube(5,5,5);
    cube->SetWorldID(world);
}

//--------------------------------------------------------------
void ofApp::update(){
    // TODO: PASTE IN THE REQUIRED CODE TO UPDATE ODE
    //       (THE FIRST PART OF SIMLOOP)
    //  |    FROM DEMO_BUGGY.CPP
    //  |
    //  |
    //  v
    // dSpaceCollide (space,0, &nearCallback);
    dWorldStep (world,0.05);

    cube->Update(3);

    // /////////////////////////////////

}
//--------------------------------------------------------------
void ofApp::draw(){

    // draw the scene
    ofBackground(20);
    cam.begin();

    ofEnableDepthTest();

    ofPushMatrix();

    ofSetColor(ofColor::lightGrey);
    ofDrawGrid(0.5f, 100, false, false,false,true);
    ofDrawAxis(10);

    cube->Draw();

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
