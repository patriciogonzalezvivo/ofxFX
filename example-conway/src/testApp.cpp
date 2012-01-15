//  Conway Life GLSL Game
//
//  Created by kalwalt alias Walter Perdan on 24/12/11
//  Copyright 2011 http://www.kalwaltart.it/ All rights reserved.
//

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);

	image.loadImage("mem.gif");

    conway.allocate(640, 480);
    conway.setPasses(10);
    //
    // Created by kalwalt alias Walter Perdan on 24/12/11
    // Copyright 2011 http://www.kalwaltart.it/ All rights reserved.
    conway.injectShader("#version 120\n \
                        #extension GL_ARB_texture_rectangle : enable\n \
                        \
                        uniform sampler2DRect tex0; \
                        \
                        vec4 dead = vec4(1.0,1.0,1.0,1.0); \
                        vec4 alive = vec4(0.0,0.0,0.0,1.0); \
                        \
                        void main(void) { \
                            vec2  st = gl_TexCoord[0].st;\
                            int sum = 0; \
                            vec4 y = texture2DRect(tex0, st); \
                            \
                            if (texture2DRect(tex0, st + vec2(-1.0, -1.0)) == alive) ++sum; \
                            if (texture2DRect(tex0, st + vec2(0.0, -1.0)) == alive) ++sum; \
                            if (texture2DRect(tex0, st + vec2(1.0, -1.0)) == alive) ++sum; \
                            \
                            if (texture2DRect(tex0, st + vec2(-1.0, 0.0)) == alive) ++sum; \
                            if (texture2DRect(tex0, st + vec2(1.0, 0.0)) == alive) ++sum; \
                            \
                            if (texture2DRect(tex0, st + vec2(-1.0, 1.0)) == alive) ++sum; \
                            if (texture2DRect(tex0, st + vec2(0.0, 1.0)) == alive) ++sum; \
                            if (texture2DRect(tex0, st + vec2(1.0, 1.0)) == alive) ++sum; \
                            \
                            if (sum < 2) gl_FragColor = dead; \
                            else if (sum > 3) gl_FragColor = dead; \
                            else if (sum == 3) gl_FragColor = alive; \
                            else gl_FragColor = y; \
                        }");
    
    conway.setTexture(image.getTextureReference());
}
//--------------------------------------------------------------
void testApp::update(){

    conway.begin();
    ofClear(255, 255);
    conway.draw();
    ofSetColor(0,255);
    ofCircle(mouseX, mouseY, 5);
    conway.end();
    
    conway.update();

    ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

//--------------------------------------------------------------
void testApp::draw(){

    conway.draw();

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
