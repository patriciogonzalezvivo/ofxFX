#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);
	image.loadImage("logo.jpg");
    
    grayScott.allocate(640, 480);
}

//--------------------------------------------------------------
void testApp::update(){
    
    grayScott.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(10);
    image.draw(640*0.5 - image.getWidth()*0.5, 480*0.5 - image.getHeight()*0.5);
    ofCircle(mouseX, mouseY, 5);
    grayScott.end();
        
    grayScott.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    grayScott.draw();
    
    ofDrawBitmapString("Diff U: " + ofToString(grayScott.getDiffU()), 15,15);
    ofDrawBitmapString("Diff V: " + ofToString(grayScott.getDiffV()), 15,30);
    ofDrawBitmapString("K: " + ofToString(grayScott.getK() ), 15,45);
    ofDrawBitmapString("F: " + ofToString(grayScott.getF() ), 15,60);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'q':
            grayScott.setDiffU( grayScott.getDiffU() + 0.001 );
            break;
        case 'a':
            grayScott.setDiffU( grayScott.getDiffU() - 0.001 );
            break;
        case 'w':
            grayScott.setDiffV( grayScott.getDiffV() + 0.001 );
            break;
        case 's':
            grayScott.setDiffV( grayScott.getDiffV() - 0.001 );
            break;
        case 'e':
            grayScott.setK( grayScott.getK() + 0.001 );
            break;
        case 'd':
            grayScott.setK( grayScott.getK() - 0.001 );
            break;
        case 'r':
            grayScott.setF( grayScott.getF() + 0.001 );
            break;
        case 'f':
            grayScott.setF( grayScott.getF() - 0.001 );
            break;
        case ' ':
            grayScott.clear();
        default:
            break;
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