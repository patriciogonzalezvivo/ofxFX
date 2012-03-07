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
 *  tex0 -> multi texture map
 *  tex1 -> RED CHANNEL
 *  tex2 -> GREEN CHANNEL
 *  tex3 -> BLUE CHANNEL
 *
 */

#ifndef OFXMULTITEXTURE
#define OFXMULTITEXTURE

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxMultiTexture : public ofxFXObject {	
public:
    ofxMultiTexture(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(
                                   uniform sampler2DRect tex0;
                                   uniform sampler2DRect tex1;
                                   uniform sampler2DRect tex2;
                                   uniform sampler2DRect tex3;
                                   
                                   void main (void){
                                       vec2 pos = gl_TexCoord[0].st;
                                       
                                       vec4 mask = texture2DRect(tex0, pos);
                                       
                                       vec4 rTxt = texture2DRect(tex1, pos);
                                       vec4 gTxt = texture2DRect(tex2, pos);
                                       vec4 bTxt = texture2DRect(tex3, pos);
                                       
                                       vec4 color = vec4(0.0,0.0,0.0,1.0);
                                       color = mix(color, rTxt, mask.r );
                                       color = mix(color, gTxt, mask.g );
                                       color = mix(color, bTxt, mask.b );
                                       
                                       gl_FragColor = color;
                                   }
                                   );
    }
};

#endif