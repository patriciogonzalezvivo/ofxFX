#include "ofApp.h"
#include "ofFbo.h"

void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableAlphaBlending();

    render_buffer.allocate(width, height, GL_RGBA);

    fx_string.setFade(1.0);
    fx_string.allocate(width, height);

    fx_file.setFade(1.0);
    fx_file.allocate(width, height);
}

void ofApp::draw_rect_to_fbo_and_update_effect() {
    render_buffer.begin();
    {
        ofPushStyle();
        ofPushMatrix();
        {
            // Draw a dark-teal, half transparent background
            // but don't cover everything
            ofSetColor(0, 75, 75, 128);
            ofRect(5, 5, width-10, height-10);

            // Draw an opaque inner teal rectangle
            float xr = width / 4.0;
            float yr = height / 4.0;
            float wr = width / 2.0;
            float hr = height / 2.0;
            ofSetColor(0, 128, 128);
            ofRect(xr, yr, wr, hr);

            // And finally a half-transparent white dot. This will
            // "cut a hole" through our rectangles, allowing us to
            // see whatever is behind. This is an odd behaviour,
            // unrelated to ofxFX and our fbo business.
            ofSetColor(255, 255, 255, 128);
            ofCircle(width/2, height/2, 30);
            
        }
        ofPopMatrix();
        ofPopStyle();
    }
    render_buffer.end();

    fx_string << render_buffer;
    fx_file << render_buffer;
}


void ofApp::update(){
 } 

//--------------------------------------------------------------
void ofApp::draw(){

    draw_rect_to_fbo_and_update_effect();

    ofBackgroundGradient(ofColor(0), ofColor(255), OF_GRADIENT_LINEAR);
    ofSetColor(0);
    ofRect(0,height/2,width*3,height/2);

    ofSetColor(255);

    // Left, directly from fbo. This should render clearly.
    render_buffer.draw(0, 0, width, height);

    // Middle, with fx_file's shader applied. Should blur.
    fx_file.draw(width, 0, width, height);

    // Right, with fx_string's shader applied. Should be equal to fx_file's output.
    fx_string.draw(width*2, 0, width, height);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
