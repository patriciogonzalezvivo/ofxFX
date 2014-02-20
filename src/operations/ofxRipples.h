//
//  ofxRipples.h
//  example-waterRipples
//
//  Created by Patricio Gonzalez Vivo on 11/24/12.
//
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxRipples : public ofxFXObject {
public:
    ofxRipples(){
        passes = 1;
        internalFormat = GL_RGBA32F ;
        
        damping = 0.995;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect backbuffer;   // previus buffer
                                   uniform sampler2DRect tex0;         // actual buffer
                                   
                                   uniform float damping;
                                   
                                   vec2 offset[4];
                                   
                                   void main(){
                                       vec2 st = gl_TexCoord[0].st;
                                       
                                       offset[0] = vec2(-1.0, 0.0);
                                       offset[1] = vec2(1.0, 0.0);
                                       offset[2] = vec2(0.0, 1.0);
                                       offset[3] = vec2(0.0, -1.0);
                                       
                                       //  Grab the information arround the active pixel
                                       //
                                       //      [3]
                                       //
                                       //  [0]  st  [1]
                                       //
                                       //      [2]
                                       
                                       vec3 sum = vec3(0.0, 0.0, 0.0);
                                       
                                       for (int i = 0; i < 4 ; i++){
                                           sum += texture2DRect(tex0, st + offset[i]).rgb;
                                       }
                                       
                                       //  make an average and substract the center value
                                       //
                                       sum = (sum / 2.0) - texture2DRect(backbuffer, st).rgb;
                                       sum *= damping;
                                       
                                       gl_FragColor = vec4(sum, 1.0);
                                   } );
    }
    
    void begin() {
        ofPushStyle();
        ofPushMatrix();
        pingPong.src->begin();
    }
    
    void end() {
        pingPong.src->end();
        ofPopMatrix();
        ofPopStyle();
    }
    
    void update(){
        // Calculate the difference between buffers and spread the waving
        textures[0].begin();
        shader.begin();
        shader.setUniformTexture("backbuffer", pingPong.dst->getTextureReference(), 0);
        shader.setUniformTexture("tex0", pingPong.src->getTextureReference(), 1);
        shader.setUniform1f("damping", (float)damping );
        renderFrame();
        shader.end();
        textures[0].end();
        
        // TODO: improve this, it's almost non-sense
        pingPong.dst->begin();
        textures[0].draw(0, 0);
        pingPong.dst->end();
        
        pingPong.swap();
    }
    
    float   damping;
};
