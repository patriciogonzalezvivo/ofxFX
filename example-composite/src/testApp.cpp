#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    
    ofSetWindowShape(width, height);
	
#ifdef THERE_IS_CAM
    video.initGrabber(width , height);
#else
    image.loadImage("logo.jpg");
#endif

    blurEffect.allocate(width, height);
    blurEffect.setPasses(10);
    
    maskImage.allocate(width, height, OF_IMAGE_COLOR);
    maskImage.loadImage("mask.png");
    maskEffect.allocate(width, height);
    

    oldTvEffect.allocate(width, height);
    oldTvEffect.setBrightness(0.9);
    
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
        blurEffect.begin();
        video.draw(0,0); 
        blurEffect.end(false);
#else
        blurEffect.begin();
        ofClear(0);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        blurEffect.end(false);
#endif
        // Make the mask
        maskEffect.beginMask();
        ofClear(0);
        ofSetColor(255*sin(beat));
        //ofCircle(mouseX, mouseY, 100);
        maskImage.draw(0,0);
        //maskImage.draw(mouseX-maskImage.getWidth()*0.5, mouseY-maskImage.getHeight()*0.5);
        maskEffect.endMask();
        
        // Mask the original -> neat & focused
        maskEffect.begin();
        ofSetColor(255);
#ifdef THERE_IS_CAM
        video.draw(0,0);
#else
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
#endif
        maskEffect.end(false);
        
        // Composite and apply the old-tv effect
        oldTvEffect.begin();
        blurEffect.draw();
        maskEffect.draw();
        oldTvEffect.end(false);
        
#ifdef THERE_IS_CAM
    }
#endif
    
    beat += 0.01;
    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
    ofSetWindowTitle(ofToString(beat));
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