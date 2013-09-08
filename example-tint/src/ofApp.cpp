#include "ofApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    width = 1024;
    height = 400;
    
    ofSetWindowShape(width, height);
    
    background.loadImage("texture.jpg");
    blur.allocate( width , height );
    tint.allocate( width , width );
    tint.setZoom(60);
    
    font.loadFont("cardinal.ttf", 70);
    
    message = "~ One ring to rule them all ~";
}

//--------------------------------------------------------------
void testApp::update(){
    float fade = abs(sin( ofGetElapsedTimef()*0.099 ));
    
    ofPushView();
    ofPushStyle();
    
    blur.begin();
    ofPushMatrix();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0, 255);
    ofSetColor( ofMap( fade , 0.0f, 0.5f, 0.0f, 1.0f, true) * 255,255);
    font.drawString(message, 
                    width*0.5f - font.getStringBoundingBox(message,0,0).width*0.5f, 
                    height*0.5f);
    
    ofDisableBlendMode();
    ofPopMatrix();
    blur.end();
    
    blur.setRadius( 1.0f + ( 1.0f - fade ) * 5.0f );
    blur.setPasses( 1.0f + ( 1.0f - fade ) * 3.0f );
    blur.update();
    
    tint.setFade( 0.2 + (1.0-fade )*0.8 );
    
    if ( fade < 0.01 ){
        tint.clear();
    }
    
    tint << blur;
    tint.update();
    
    ofPopView();
    ofPopStyle();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofBackgroundGradient(ofColor(252,239,233,255), ofColor(58,30,0,255));
    
    ofSetColor(255, 50);
    background.draw(0,0);

    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(255, 255);
    tint.getTextureReference().draw(0,0);
    ofDisableBlendMode();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
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