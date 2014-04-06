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

        if (ofIsGLProgrammableRenderer()) { // OpenGL 3.0
            string  vertexShader = "#version 150\n";
            vertexShader += STRINGIFY(
                    uniform mat4 modelViewProjectionMatrix;
                    uniform mat4 textureMatrix;

                    in vec4 position;
                    in vec2 texcoord;

                    out vec2 texCoordVarying;

                    void main(){
                        texCoordVarying = texcoord;
                        gl_Position = modelViewProjectionMatrix * position;
                    });
            
            shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);

            fragmentShader = "#version 150\n";
            fragmentShader += STRINGIFY(uniform sampler2DRect tex0;
                                        in vec2 texCoordVarying;
                                        out vec4 outputColor;

                                        void main(){
                                            vec2 st = gl_FragCoord.st;
                                            outputColor = texture(tex0, st);
                                        });

        } else { // OpenGL 2.0
            fragmentShader = ""; // For some reason "#version 120\n" makes this break.
            fragmentShader += STRINGIFY(uniform sampler2DRect tex0;

                                        void main(){
                                            vec2 st = gl_TexCoord[0].st;
                                            gl_FragColor = texture2DRect(tex0, st);
                                        });
        }
    }
};
