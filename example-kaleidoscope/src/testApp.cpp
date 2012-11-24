#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetWindowShape(640, 480);
    video.initGrabber(640, 480);
    kal.allocate(640, 480);
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    kal.setTexture(video.getTextureReference());
    kal.setRotation(ofGetElapsedTimef()*0.1);
    kal.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor(abs(sin(ofGetElapsedTimef()*0.01)*255),
                                 abs(sin(ofGetElapsedTimef()*0.03)*255),
                                 abs(sin(ofGetElapsedTimef()*0.09)*255)),
                         ofColor(0));
    ofSetColor(255, 200);
    
    
    kal.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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