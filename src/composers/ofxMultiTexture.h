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
 *  tex0 -> multi texture map
 *  tex1 -> RED CHANNEL
 *  tex2 -> GREEN CHANNEL
 *  tex3 -> BLUE CHANNEL
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxMultiTexture : public ofxFXObject {	
public:
    ofxMultiTexture(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = "#version 120\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
uniform sampler2DRect tex0;\n\
uniform sampler2DRect tex1;\n\
uniform sampler2DRect tex2;\n\
uniform sampler2DRect tex3;\n\
\n\
void main (void){\n\
    vec2 pos = gl_TexCoord[0].st;\n\
    \n\
    vec4 mask = texture2DRect(tex0, pos);\n\
    \n\
    vec4 rTxt = texture2DRect(tex1, pos);\n\
    vec4 gTxt = texture2DRect(tex2, pos);\n\
    vec4 bTxt = texture2DRect(tex3, pos);\n\
    \n\
    vec4 color = vec4(0.0,0.0,0.0,1.0);\n\
    color = mix(color, rTxt, mask.r );\n\
    color = mix(color, gTxt, mask.g );\n\
    color = mix(color, bTxt, mask.b );\n\
    \n\
    gl_FragColor = color;\n\
}\n\
\n";
    }
};