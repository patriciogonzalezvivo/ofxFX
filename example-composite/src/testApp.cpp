#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    
    ofSetWindowShape(width, height);
	
    video.initGrabber(width , height);
    image.loadImage("logo.jpg");

    blurEffect.allocate(width, height);
    blurEffect.setPasses(10);
    
    maskImage.allocate(width, height, OF_IMAGE_COLOR);
    maskImage.loadImage("mask.png");
    maskEffect.allocate(width, height);
    

    oldTvEffect.allocate(width, height);
    
    beat = 13.0;
}

//--------------------------------------------------------------
void testApp::update(){
    
    blurEffect.setRadius(sin(beat*0.3)*10);
    
#ifdef THERE_IS_CAM
    // Get Video input image
    video.update();
    if (video.isFrameNew() ){
        ofSetColor(255);
        
        // Blur video -> out of focus
        blurEffect.setTexture(video.getTextureReference());
#else
        blurEffect.begin();
        ofClear(0);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        blurEffect.end();
#endif
        blurEffect.update();
        
        // Make the mask
        maskEffect.begin();
        ofClear(0,255);
        ofSetColor(255*sin(beat));
        maskImage.draw(0,0);
        maskEffect.end();
        
        // Mask the original -> neat & focused
#ifdef THERE_IS_CAM
        maskEffect.setTexture(video.getTextureReference(),1);
#else   
        maskEffect.begin(1);
        ofSetColor(255);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        maskEffect.end(1);
#endif
        maskEffect.update();
        
        // Composite and apply the old-tv effect
        oldTvEffect.begin();
        blurEffect.draw();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        maskEffect.draw();
        oldTvEffect.end();
        
        oldTvEffect.update();
#ifdef THERE_IS_CAM
    }
#endif
    
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