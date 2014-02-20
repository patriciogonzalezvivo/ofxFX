//
//  ofxNormals.h
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxNormals : public ofxFXObject {
public:
    ofxNormals(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   float xOffset = 1.0;
                                   float yOffset = 1.0;
                                   
                                   void main(){
                                       float sCoord		= gl_TexCoord[0].s;
                                       float tCoord		= gl_TexCoord[0].t;
                                       
                                       float center		= texture2DRect(tex0, vec2( sCoord, tCoord ) ).r;
                                       float topLeft	= texture2DRect(tex0, vec2(sCoord - xOffset	, tCoord - yOffset	) ).r;
                                       float left		= texture2DRect(tex0, vec2(sCoord - xOffset	, tCoord			) ).r;
                                       float bottomLeft	= texture2DRect(tex0, vec2(sCoord - xOffset	, tCoord + yOffset	) ).r;
                                       float top		= texture2DRect(tex0, vec2(sCoord			, tCoord - yOffset	) ).r;
                                       float bottom		= texture2DRect(tex0, vec2(sCoord			, tCoord + yOffset	) ).r;
                                       float topRight	= texture2DRect(tex0, vec2(sCoord + xOffset	, tCoord - yOffset	) ).r;
                                       float right		= texture2DRect(tex0, vec2(sCoord + xOffset	, tCoord			) ).r;
                                       float bottomRight= texture2DRect(tex0, vec2(sCoord + xOffset	, tCoord + yOffset	) ).r;
                                       
                                       float dX = topRight + 2.0 * right + bottomRight - topLeft - 2.0 * left - bottomLeft;
                                       float dY = bottomLeft + 2.0 * bottom + bottomRight - topLeft - 2.0 * top - topRight;
                                       
                                       vec3 N = normalize( vec3( dX, dY, 0.01) );
                                       
                                       N *= 0.5;
                                       N += 0.5;
                                       
                                       gl_FragColor.rgb = N;
                                       gl_FragColor.a = 1.0;
                                   }
        );
    }
};