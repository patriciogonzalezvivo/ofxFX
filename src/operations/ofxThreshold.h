//
//  ofxThreshold.h
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxThreshold : public ofxFXObject {
public:
    ofxThreshold(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   float threshold;
                                   
                                   void main(){
                                       vec4 color = texture2DRect(tex0,gl_TexCoord[0].st);
                                       float c = color.g;
                                       c = c - threshold;
                                       c = max(c,0.0);
                                       c = c/c;
                                       gl_FragColor = vec4(c,c,c,1.0);
                                   }
                                   );
    }
    
    void update(){
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniform1f("threshold", threshold );
        
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
    };
    
    float threshold;
    
};