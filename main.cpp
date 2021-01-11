#include "ofMain.h"
#include "ofApp.h"
#include "ode/ode.h"

ofApp *myApp;

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.setSize(1024, 768);
    settings.setGLVersion(3,3);
    ofCreateWindow(settings);
    myApp = new ofApp();
    return ofRunApp(myApp);
}
