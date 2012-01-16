#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    
    ofSetWindowShape(width, height);
	
    video.initGrabber(width , height);
    image.loadImage("logo.jpg");

    maskImage.allocate(width, height, OF_IMAGE_COLOR);
    maskImage.loadImage("mask.png");
    maskEffect.allocate(width, height);
    
    blurEffect.allocate(width, height);
    blurEffect.setPasses(10);
    
    oldTvEffect.allocate(width, height);
    
    beat = 13.0;
}

//--------------------------------------------------------------
void testApp::update(){
    // Get Video input image
    video.update();
    if (video.isFrameNew() ){
        ofSetColor(255);
        
        // MAKS
        //  texture 0 it«s the mask  
        maskEffect.begin(0);
        ofClear(0,255);
        ofSetColor(255*sin(beat));
        maskImage.draw(0,0);
        maskEffect.end(0);
        //  texture 1 it«s the texture to be mask
        maskEffect.setTexture(video.getTextureReference(),1);
        
        maskEffect.update();
        
        
        // BLUR
        blurEffect.setRadius(sin(beat*0.3)*10);
        blurEffect.setTexture(video.getTextureReference()); // Note: that when you are pass things 
                                                            // to texture 0 you don«t need the 0
        blurEffect.update();
        

        // OLD-TV
        oldTvEffect.begin();
        blurEffect.draw();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        maskEffect.draw();
        oldTvEffect.end();
        
        oldTvEffect.update();
    }
    
    beat += 1.0/ofGetFrameRate();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
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