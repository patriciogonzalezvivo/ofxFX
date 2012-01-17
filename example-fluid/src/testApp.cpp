#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);
	image.loadImage("logo.jpg");
    
    // Initial Allocation
    fluid.allocate(640, 480, 0.5);
    // Seting the gravity set up & injecting the background image
    fluid.setGravity(ofVec2f(0.0,0.0098));
    fluid.setTexture(image.getTextureReference());
    
    // Adding constant forces
    //fluid.addConstantForce(ofVec2f(640*0.5,480*0.85), ofVec2f(0,-3), ofFloatColor(0.5,0.0,0.0), 6.f);
    //fluid.addConstantForce(ofVec2f(640*0.5,480*0.85), ofVec2f(0,-1), ofFloatColor(0.0,0.3,0.0), 3.f);
    
    bPaint = true;
    bObstacle = false;
    bBounding = true;
    bClear = true;
    bLogo = false;
}

//--------------------------------------------------------------
void testApp::update(){
    ofVec2f m = ofVec2f(mouseX,mouseY);
    
    if (bPaint){
        ofVec2f center = ofVec2f(640*0.5, 480*0.5);
        ofVec2f d = m - oldM;
        ofVec2f c = center - m;
        d *= 2.0;
        c.normalize();
        
        // Adding temporal Force
        fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5),3.0f);
    } 

    // Updating the obstacle texture if not using obstacles you have to call update()
    fluid.begin();
    if (bClear) ofClear(0);
    
    if (bLogo){
        ofSetColor(250);
        image.draw(ofGetWindowWidth()*0.5-image.getWidth()*0.5,ofGetWindowHeight()*0.5-image.getHeight()*0.5);
    }
    
    if (bObstacle){
        ofFill();
        ofColor(255,0,0);
        ofCircle(m.x, m.y, 5);
    } 
    
    if (bBounding){
        ofNoFill();
        ofSetColor(200);
        ofRect(2, 2, 636, 476);
    }
    fluid.end();
    
    fluid.update();
    
    // Storing the mouse position in order to get the velocity of it and pass it to the fluid
    oldM = m;
    
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
    if( key == 'l')
        bLogo = !bLogo;
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