//
//  ofxMask.h
//
//  Created by Patricio González Vivo on 26/12/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Based on ofxAlphaMaskShader
//  Created by James George, http://www.jamesgeorge.org
//  in collaboration with FlightPhase http://www.flightphase.com

#ifndef OFXMASK
#define OFXMASK

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxMask : public ofxFXObject {	
public:
    void allocate(int _width, int _height) {
        width = _width;
        height = _height;
        pingPong.allocate(width,height);
        
        maskFBO.allocate(width,height, GL_RGBA);
        
        maskBegin();
        ofClear(0,255);
        maskEnd();
        
        loadShaders();
	}
    
    ofTexture& getTextureReference() { return pingPong.dst->getTextureReference(); };
    
    void maskBegin(){
        maskFBO.begin();
    }
    
    void maskEnd(){
        maskFBO.end();
    }
    
	void begin() {
		ofPushStyle();
		ofPushMatrix();
        pingPong.src->begin();
	}
	
	void end(bool bDraw = true) {
        pingPong.src->end();
        
        ofPopStyle();
		ofPopMatrix();
        
        pingPong.dst->begin();
        ofClear(0, 0);
        maskShader.begin();
        maskShader.setUniformTexture("tex0",pingPong.src->getTextureReference(),0);
        maskShader.setUniformTexture("tex1",maskFBO.getTextureReference(),1);
        renderFrame();
        maskShader.end();
        pingPong.dst->end();
        
        if (bDraw)
            draw();
        
        pingPong.swap();
        
	}
	
	void draw(int x = 0, int y = 0){ pingPong.dst->draw(x, y);}
    
private:
    void loadShaders(){
        string fragmentMaskShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
        uniform sampler2DRect tex0;\
        uniform sampler2DRect tex1;\
        \
        void main (void){\
            vec2 st = gl_TexCoord[0].st;\
            vec4 image = texture2DRect(tex0, st);\
            vec4 mask = texture2DRect(tex1, st);\
            \
            gl_FragColor = vec4(image.rgb,mask.r);\
        }";
        maskShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentMaskShader);
    	maskShader.linkProgram();
    }
    
    swapBuffer pingPong;
    ofShader maskShader;
    ofFbo    maskFBO;
};

#endif
