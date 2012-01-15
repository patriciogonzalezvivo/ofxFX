/*
 *  ofxMask.h
 *
 *  Created by Patricio González Vivo on 10/1/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *  Based on ofxAlphaMaskShader
 *  Created by James George, http://www.jamesgeorge.org
 *  in collaboration with FlightPhase http://www.flightphase.com
 *
 *  ******************************************************************
 *
 *  tex0 -> Mask
 *  tex1 -> masking Image
 *
 */
 
#ifndef OFXMASK
#define OFXMASK

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxMask : public ofxFXObject {	
public:
    ofxMask(){
        passes = 1;
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
};

#endif
