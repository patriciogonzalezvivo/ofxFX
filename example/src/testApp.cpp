#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);
	image.loadImage("logo.jpg");
    
    fluid.allocate(640, 480, 0.5).setGravity(ofVec2f(0.0,0.0098)).setDensity(image.getTextureReference());
    //fluid.addConstantForce(ofVec2f(640*0.5,480*0.85), ofVec2f(0,-3), ofFloatColor(0.5,0.0,0.0), 6.f);
    //fluid.addConstantForce(ofVec2f(640*0.5,480*0.85), ofVec2f(0,-1), ofFloatColor(0.0,0.3,0.0), 3.f);
    
    grayScott.allocate(640, 480);
    
    bGrayScott = true;
    bPaint = true;
    bObstacle = false;
    bBounding = true;
    bClear = true;
    bLogo = false;
}

//--------------------------------------------------------------
void testApp::update(){
    ofVec2f m = ofVec2f(mouseX,mouseY);
    
    if (bGrayScott){
        grayScott.begin();
        ofClear(0, 0, 0, 255);
        ofSetColor(10);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        
        ofCircle(mouseX, mouseY, 5);
        grayScott.end();
    } else {
        
        if (bPaint){
            ofVec2f center = ofVec2f(640*0.5, 480*0.5);
            ofVec2f d = m - oldM;
            ofVec2f c = center - m;
            d *= 2.0;
            c.normalize();
        
            fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5),3.0f);
        } 
    
        fluid.obstaclesBegin();
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
        fluid.obstaclesEnd();
    
        oldM = m;
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    if (bGrayScott)
        grayScott.draw();
    else
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
    if ( key == 'g'){
        if (bGrayScott)
            fluid.allocate(640, 480, 0.5).setGravity(ofVec2f(0.0,0.0098)).setDensity( grayScott.getTextureReference() );
        
        bGrayScott = !bGrayScott;
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