#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(640, 480);
	image.loadImage("logo.jpg");
    oldTv.allocate(640, 480);
    
    video.initGrabber(640, 480);
    
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if (video.isFrameNew() ){
        oldTv.begin();
        ofClear(0, 0, 0, 255);
        ofSetColor(255);
        video.draw(0,0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        ofDisableBlendMode();
        oldTv.end();
    }
        
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    oldTv.draw();
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