#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetWindowShape(640*2, 480);
    
    rip.allocate(640,480);
    bounce.allocate(640,480);
    
    ofImage background;
    background.loadImage("fondo.jpg");
    
    bounce.setTexture(background.getTextureReference(), 1);
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    rip.begin();
    ofFill();
    ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
    ofEllipse(mouseX,mouseY, 10,10);
    rip.end();
    rip.update();
    
    bounce << rip;
    bounce.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    ofSetColor(255,255);
    
    rip.draw(0,0);
    ofDrawBitmapString("ofxRipples ( damping = " + ofToString(rip.damping) + " )", 15,15);
    
    bounce.draw(640,0);
    ofDrawBitmapString("ofxBounce", 640+15,15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == OF_KEY_UP){
        rip.damping += 0.01;
    } else if ( key == OF_KEY_DOWN){
        rip.damping -= 0.01;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    rip.damping = ofMap(y, 0, ofGetHeight(), 0.9, 1.0, true);
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