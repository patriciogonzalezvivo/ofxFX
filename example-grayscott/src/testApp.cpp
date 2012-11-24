#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    gray.allocate(640, 480);
    normals.allocate(640, 480);
    
    ofSetWindowShape(640*2, 480);
}

//--------------------------------------------------------------
void testApp::update(){
    gray.update();

    normals << gray;
    normals.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    ofSetColor(255);
    
    gray.draw();
    ofDrawBitmapString("GrayScott Reaction Diffusion", 15,15);
    ofDrawBitmapString("K ( mouseX ): " + ofToString(gray.getK()) , 15,35);
    ofDrawBitmapString("F ( mouseY ): " + ofToString(gray.getF()) , 15,55);
    
    normals.draw(640,0);
    ofDrawBitmapString("NormalMap of the GrayScott", 640+15,15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    gray.setK( ofMap(mouseX,0,640,0.005,0.095,true) );
    gray.setF( ofMap(mouseY,0,480,0.01,0.028,true) );
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