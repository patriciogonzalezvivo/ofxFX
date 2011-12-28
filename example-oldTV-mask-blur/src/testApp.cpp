#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    
    ofSetWindowShape(width, height);
	
    video.initGrabber(width , height);

    blurEffect.allocate(width, height);
    blurEffect.setPasses(10);
    
    maskImage.allocate(width, height, OF_IMAGE_COLOR);
    maskImage.loadImage("mask.png");
    maskEffect.allocate(width, height);
    

    oldTvEffect.allocate(width, height);
    oldTvEffect.setBrightness(0.9);
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Get Video input image
    video.update();
    if (video.isFrameNew() ){
        ofSetColor(255);
        
        // Blur video -> out of focus
        blurEffect.begin();
        video.draw(0,0); 
        blurEffect.end(false);
        
        // Make the mask
        maskEffect.beginMask();
        ofClear(0);
        ofSetColor(255);
        //ofCircle(mouseX, mouseY, 100);
        maskImage.draw(mouseX-maskImage.getWidth()*0.5, mouseY-maskImage.getHeight()*0.5);
        maskEffect.endMask();
        
        // Mask the original -> neat & focused
        maskEffect.begin();
        video.draw(0,0);
        maskEffect.end(false);
        
        // Composite and apply the old-tv effect
        oldTvEffect.begin();
        blurEffect.draw();
        maskEffect.draw();
        oldTvEffect.end(false);
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    // Black baground
    ofBackground(0);
    
    oldTvEffect.draw();
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