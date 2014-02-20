/*
 *  ofxMask.h
 *
 *  Created by Patricio Gonzalez Vivo on 10/1/11.
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

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxMask : public ofxFXObject {	
public:
    ofxMask(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform sampler2DRect tex1;

                                   void main (void){
                                       vec2 st = gl_TexCoord[0].st;
                                       vec4 mask = texture2DRect(tex0, st);
                                       vec4 image = texture2DRect(tex1, st);
                                       
                                       gl_FragColor = vec4(image.rgb,max(mask.r,max(mask.g,mask.b))*image.a );\n\
                                   });
    }
    
    void begin(int _texNum = 0 ) {
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            ofPushStyle();
            ofPushMatrix();
            textures[_texNum].begin();
            //ofClear(0,0);
        }
    }
    
    void end(int _texNum = 0) {
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            textures[_texNum].end();
            ofPopMatrix();
            ofPopStyle();
        }
    }
    
    void setTexture(ofTexture& tex, int _texNum){ 
        if ((_texNum < nTextures) && ( _texNum >= 0)){
            textures[_texNum].begin(); 
            ofClear(0,0);
            ofSetColor(255);
            tex.draw(0,0); 
            textures[_texNum].end();
        }
    };
};
