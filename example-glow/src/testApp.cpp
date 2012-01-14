#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);
	image.loadImage("logo.jpg");
    
    glow.allocate(640, 480);
    glow.setPasses(6);
    
    beat = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    
    glow.setRadius(sin(beat)*15);
    
    glow.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        
    ofCircle(mouseX, mouseY, 5);
    glow.end();
        
    glow.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    beat += 0.01;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    glow.draw();
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