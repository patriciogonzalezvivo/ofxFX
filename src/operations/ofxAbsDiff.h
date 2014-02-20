//
//  ofxAbsDiff.h
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxAbsDiff : public ofxFXObject {
public:
    ofxAbsDiff(){
        amount = 1.0;
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform sampler2DRect tex1;
                                   uniform float amount;
                                   
                                   void main(){
                                       vec4 color0 = texture2DRect(tex0, gl_TexCoord[0].st);
                                       vec4 color1 = texture2DRect(tex1, gl_TexCoord[0].st);
                                       vec3 diff =  abs(color0 - color1).rgb*amount;
                                       gl_FragColor = vec4(diff,max(diff.r,max(diff.g,diff.b)));
                                   }
                                   );
    }
    
    void update(){
        pingPong.dst->begin();
        ofClear(0,0);
        shader.begin();
        shader.setUniformTexture("tex0", textures[0], 0);
        shader.setUniformTexture("tex1", textures[1], 1);
        shader.setUniform1f("amount", amount );
        renderFrame();
        shader.end();
        pingPong.dst->end();
    };
    
    float amount;
};