#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    width = 640;
    height = 480;
    ofSetWindowShape(width, height);
    video.initGrabber(width, height);
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    blur.setRadius(10);
    blur.setPasses(10);
    normals.allocate(width, height);
    disp.allocate(width,height);
    
    bDrawVideo = false;
    bDrawField = false;
    bDrawNormals = false;
    
    bCapture = true;
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if ( video.isFrameNew() ){
        grayscale.setTexture( video.getTextureReference() );
        grayscale.update();
        blur << grayscale;
        blur.update();
        normals << blur;
        normals.update();
    }

    if (bCapture){
        disp.getBackBuffer()->begin();
        ofSetColor(255);
        video.draw(0, 0);
        disp.getBackBuffer()->end();
        
        bCapture = false;
    }
    
    disp.setTexture(normals.getTextureReference());
    disp.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    if ( bDrawVideo ){
        video.draw(0,0,ofGetWidth(),ofGetHeight());
        ofSetColor(255,200);
    }
    
    disp.draw(0,0,ofGetWidth(),ofGetHeight());
    
    if ( bDrawNormals ){
        ofSetColor(255);
        normals.draw(0,0,ofGetWidth(),ofGetHeight());
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == 'v'){
        bDrawVideo = !bDrawVideo;
    } else if (key == 'n'){
        bDrawNormals = !bDrawNormals;
    } else {
        bCapture = true;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    blur.setRadius(ofMap(x, 0 , ofGetWidth(), 0, 10, true));
    blur.setPasses(ofMap(y, 0, ofGetHeight(),0, 10, true));
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