#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    gray.allocate(640, 480);
    
    ofSetWindowShape(640, 480);
}

//--------------------------------------------------------------
void testApp::update(){
    gray.update();
    
    gray.setK( ofMap(mouseX,0,ofGetWidth(),0.005,0.095) );
    gray.setF( ofMap(mouseY,0,ofGetHeight(),0.01,0.028) );
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    ofSetColor(255);
    
    gray.draw();
    
    ofDrawBitmapString("K ( mouseX ): " + ofToString(gray.getK()) , 15,15);
    ofDrawBitmapString("F ( mouseY ): " + ofToString(gray.getF()) , 15,35);
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
    gray.begin();
    ofSetColor(ofNoise( ofGetElapsedTimef() )*255);
    ofCircle(x, y, 3);
    gray.end();
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