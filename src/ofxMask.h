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
        nTextures = 2;
        internalFormat = GL_RGBA;
        
        fragmentShader = "#version 120\n \
    	#extension GL_ARB_texture_rectangle : enable\n \
    	\
        uniform sampler2DRect tex0;\
        uniform sampler2DRect tex1;\
        \
        void main (void){\
            vec2 st = gl_TexCoord[0].st;\
            vec4 mask = texture2DRect(tex0, st);\
            vec4 image = texture2DRect(tex1, st);\
            \
            gl_FragColor = vec4(image.rgb,mask.r);\
        }";
    }
    
    void setMaskTexture(ofTexture& tex){ 
        textures[0].begin(); 
        ofClear(0,255);
        ofSetColor(255);
        tex.draw(0,0); 
        textures[0].end();
    };
};

#endif
