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

class ofxBloom : public ofxFXObject {	
public:
    ofxBloom(){
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
                                            vec4 sum = vec4(0);
                                            vec2 st = gl_FragCoord.st;
                                            int j;
                                            int i;
                                            for(i=-4; i<4; i++){
                                                for (j=-3; j<3; j++){
                                                    sum += texture(tex0, st + vec2(j, i)*0.004) * 0.25;
                                                }
                                            }
                                            
                                            if (texture(tex0, st).r < 0.3){
                                                outputColor = sum * sum * 0.012 + texture(tex0, st);
                                            } else {
                                                if (texture(tex0, st).r < 0.5){
                                                    outputColor = sum * sum * 0.009 + texture(tex0, st);
                                                } else {
                                                    outputColor = sum * sum * 0.0075 + texture(tex0, st);
                                                }
                                            }
                                            outputColor.a = texture(tex0, st).a;
                                        });

        } else { // OpenGL 2.0
            fragmentShader = ""; // For some reason "#version 120\n" makes this break.
            fragmentShader += STRINGIFY(uniform sampler2DRect tex0;

                                        void main(){
                                            vec4 sum = vec4(0);
                                            vec2 st = gl_TexCoord[0].st;
                                            int j;
                                            int i;
                                            for(i=-4; i<4; i++){
                                                for (j=-3; j<3; j++){
                                                    sum += texture2DRect(tex0, st + vec2(j, i)*0.004) * 0.25;
                                                }
                                            }
                                            
                                            if (texture2DRect(tex0, st).r < 0.3){
                                                gl_FragColor = sum * sum * 0.012 + texture2DRect(tex0, st);
                                            } else {
                                                if (texture2DRect(tex0, st).r < 0.5){
                                                    gl_FragColor = sum * sum * 0.009 + texture2DRect(tex0, st);
                                                } else {
                                                    gl_FragColor = sum * sum * 0.0075 + texture2DRect(tex0, st);
                                                }
                                            }
                                            gl_FragColor.a = texture2DRect(tex0, st).a;
                                        });
        }
    }
};
