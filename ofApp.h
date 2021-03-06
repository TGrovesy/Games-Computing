#pragma once
#include "ofMain.h"
#include "ode/ode.h"

#include "cube.h"
#include "world.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


    void collide (dGeomID o1, dGeomID o2);

    //WORLD
    static World* world;
    static World* GetWorld() {return world;}
    static void SetWorld(World* world){ofApp::world = world;}

    //ODE experimentation
    dWorldID worldID;
    dSpaceID space;
    dJointGroupID contactGroup;
    dWorldID GetWorldID(){return worldID;}
    dSpaceID physicsSpace;//TEST TO REMOVE
    dGeomID groundtmp;

    //Keys TODO MOVE ELSEWHERE DONT BELONG HERE!
    static bool keyDown[255];
};

/* ODE requires a global function to use as the collision callback; this
 * function, combined with the ofApp pointer, allows us to put the collision
 * code within myApp. Look at the .cpp for details of how this works.
 */
//static void nearCallback (void *, dGeomID o1, dGeomID o2);
extern ofApp *myApp;
