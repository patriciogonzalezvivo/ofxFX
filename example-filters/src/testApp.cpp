#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    beat = 0;
    width = 640;
    height = 480;
    selection = -1;
    
    ofSetWindowShape(width, height);

    video.initGrabber(width , height);

    fbo.allocate(width, height);
    
    bloom.allocate(width, height);
    blur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    unsharp.allocate(width, height);
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    if (video.isFrameNew() ){
        ofSetColor(255);

        fbo.begin();
        ofClear(0, 0, 0, 255);
        ofSetColor(255);
        video.draw(0,0);
        fbo.end();
    }
    
    if ( selection == 0 ){
        bloom.setTexture(fbo.getTextureReference());
        bloom.update();
    } else if ( selection == 1 ){
        blur.setRadius(sin(beat)*10);
        blur.setTexture(fbo.getTextureReference());
        blur.update();
    } else if ( selection == 2 ){
        bokeh.setRadius(abs(sin(beat)*10));
        bokeh.setTexture(fbo.getTextureReference());
        bokeh.update();
    } else if ( selection == 3 ){
        glow.setRadius(sin(beat)*15);
        glow.setTexture(fbo.getTextureReference());
        glow.update();
    } else if ( selection == 4 ){
        unsharp.setFade(sin(beat));
        unsharp.setTexture(fbo.getTextureReference());
        unsharp.update();
    } 
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    beat += 1.0/ofGetFrameRate();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    if ( selection == 0 ){
        bloom.draw();
        ofDrawBitmapString("Bloom" , 15, 15);
    } else if ( selection == 1 ){
        blur.draw();
        ofDrawBitmapString("Blur at: " + ofToString(sin(beat)*10), 15, 15);
    } else if ( selection == 2 ){
        bokeh.draw();
        ofDrawBitmapString("Bokeh at: "+ ofToString( abs(sin(beat)*10) ), 15, 15);
    } else if ( selection == 3 ){
        glow.draw();
        ofDrawBitmapString("Glow at: "+ ofToString(sin(beat)*15), 15, 15);
    } else if ( selection == 4 ){
        unsharp.draw();
        ofDrawBitmapString("Unsharp at: "+ ofToString(sin(beat)), 15, 15);
    } else {
        fbo.draw(0, 0);
        ofDrawBitmapString("Original", 15, 15);
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            selection++;
            break;
        case OF_KEY_DOWN:
            selection--;
            break;
    }
    
    if (selection >= 5)
        selection = -1;
    else if ( selection < -1)
        selection = 4;
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