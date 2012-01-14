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
    ofxMask(){
        passes = 1;
        
        fragmentShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
        uniform sampler2DRect backbuffer;\
        uniform sampler2DRect tex;\
        \
        void main (void){\
            vec2 st = gl_TexCoord[0].st;\
            vec4 image = texture2DRect(backbuffer, st);\
            vec4 mask = texture2DRect(tex, st);\
            \
            gl_FragColor = vec4(image.rgb,mask.r);\
        }";
    }
    
    void allocate(int _width, int _height) {
        width = _width;
        height = _height;
        pingPong.allocate(width,height);
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    	shader.linkProgram();
        
        initFbo(maskFBO, width, height,GL_RGBA);
	}
    
    void setMaskTexture(ofTexture& tex){ 
        maskFBO.begin(); 
        ofClear(0,255); 
        tex.draw(0,0); 
        maskFBO.end();
    };
    
    void beginMask(){ maskFBO.begin(); };
    void endMask(){ maskFBO.end(); };
    
    void update(){
        ofPushStyle();
        ofEnableAlphaBlending();
        
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            ofClear(0, 0);
            shader.begin();
            shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0 );
            shader.setUniformTexture("tex",maskFBO.getTextureReference(),1);
            renderFrame();
            shader.end();
            pingPong.dst->end();
            pingPong.swap();
        }
        
        ofPopStyle();
        
        pingPong.swap();
	}
	
private:
    ofFbo      maskFBO;
};

#endif
