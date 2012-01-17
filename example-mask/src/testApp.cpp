#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    image.loadImage("logo.jpg");
    
    width = 640;
    height = 480;
    
    ofSetWindowShape(width, height);
#ifdef I_HAVE_WEBCAM
    video.initGrabber(width , height);
#endif
    fbo.allocate(width,height);
    
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
    ofSetColor(255);
    
#ifdef I_HAVE_WEBCAM
    video.update();
    if (video.isFrameNew() ){
        
#endif
        fbo.begin();
        ofClear(0, 0, 0, 255);
#ifdef I_HAVE_WEBCAM
        video.draw(0,0);
#endif
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        fbo.end();
#ifdef I_HAVE_WEBCAM
    }
#endif
    // MAKS
    //  texture 0 it«s the mask  
    maskEffect.begin(0);
    ofClear(0,255);
    ofSetColor(255*sin(beat));
    maskImage.draw(0,0);
    maskEffect.end(0);
    //  texture 1 it«s the texture to be mask
    maskEffect.setTexture(fbo.getTextureReference(),1);
    //
    maskEffect.update();
        
        
    // BLUR
    blurEffect.setRadius(sin(beat*0.3)*10);
    blurEffect.setTexture(fbo.getTextureReference()); // Note: that when you are pass things 
    //                                                // to texture 0 you don«t need the 0
    blurEffect.update();
        
    // OLD-TV
    oldTvEffect.begin();
    blurEffect.draw();
    maskEffect.draw();
    oldTvEffect.end();
    //
    oldTvEffect.update();
    
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