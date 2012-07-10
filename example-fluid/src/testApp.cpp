#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);
    
    // Initial Allocation
    //
    fluid.allocate(640, 480, 0.5);
    
    // Seting the gravity set up & injecting the background image
    //
    fluid.setGravity(ofVec2f(0.0,-0.098));
    fluid.setDissipation(0.99);
    fluid.setVelocityDissipation(0.99);
    
    //  Set obstacle
    //
    fluid.begin();
    ofSetColor(255);
    ofCircle(640*0.5, 480*0.7, 10);
    ofCircle(640*0.5-30, 480*0.7-60, 5);
    ofCircle(640*0.5+30, 480*0.7-60, 5);
    ofCircle(640*0.5+50, 480*0.7-120, 2);
    ofCircle(640*0.5-50, 480*0.7-120, 2);
    fluid.end();
    
    // Adding constant forces
    //
    fluid.addConstantForce(ofPoint(640*0.5,480*0.85), ofPoint(0,-3.), ofFloatColor(0.5,0.1,0.0), 5.f);
    //fluid.addConstantForce(ofPoint(640*0.5,480*0.85), ofPoint(0,-1), ofFloatColor(0.0,0.3,0.0), 3.f);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Adding temporal Force
    //
    ofPoint m = ofPoint(mouseX,mouseY);
    ofPoint d = (m - oldM)*10.0;
    oldM = m;
    ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
    c.normalize();
    fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5),3.0f);

    //  Update
    //
    fluid.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    fluid.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if( key == 'p')
        bPaint = !bPaint;
    if( key == 'o')
        bObstacle = !bObstacle;
    if( key == 'b')
        bBounding = !bBounding;
    if( key == 'c')
        bClear = !bClear;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}