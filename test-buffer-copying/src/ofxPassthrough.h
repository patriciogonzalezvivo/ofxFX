//
//  ofxBloom.h
//  emptyExample
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com. All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxPassthrough : public ofxFXObject {	
public:
    ofxPassthrough(){
        passes = 1;
        internalFormat = GL_RGBA;

        gl3FragmentShader = "#version 150\n";
        gl3FragmentShader += STRINGIFY(uniform sampler2DRect tex0;
                                    in vec2 texCoordVarying;
                                    out vec4 outputColor;

                                    void main(){
                                        vec2 st = gl_FragCoord.st;
                                        outputColor = texture(tex0, st);
                                    });

        gl2FragmentShader = ""; // For some reason "#version 120\n" makes this break.
        gl2FragmentShader += STRINGIFY(uniform sampler2DRect tex0;

                                    void main(){
                                        vec2 st = gl_TexCoord[0].st;
                                        gl_FragColor = texture2DRect(tex0, st);
                                    });
    }
};
