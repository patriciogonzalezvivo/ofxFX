#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofSetWindowShape(640, 480);
    
    water.loadBackground("fondo.jpg");
    logo.loadImage("logo.png");
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    water.begin();
    ofFill();
    ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
    ofEllipse(mouseX,mouseY, 10,10);
    
    ofPushMatrix();
    //ofSetColor(255);
    
    ofEnableAlphaBlending();
    if((int)(ofRandom(100)) == 50){
        ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
        logo.draw(640*0.5-logo.getWidth()*.5, 480*0.5-logo.getHeight()*0.5);
    }
    ofPopMatrix();
    
    water.end();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    water.draw();
    
    ofDisableBlendMode();
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD );
    ofSetColor(255,255);
    logo.draw(640*0.5-logo.getWidth()*.5, 480*0.5-logo.getHeight()*0.5);
    ofDisableBlendMode();
    ofDisableAlphaBlending();
    
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