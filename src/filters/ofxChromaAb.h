//
//  ofxChromaAb.h
//
//  Created by Patricio Gonzalez Vivo on 3/18/13.
//
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxChromaAb : public ofxFXObject {
public:
    ofxChromaAb(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform vec2 windowSize;
                                   uniform float offsetALL;
                                   
                                   void main(){
                                       vec2 st          = gl_TexCoord[0].st;
                                       vec2 offset      = ( gl_FragCoord.xy/windowSize ) * offsetALL;
                                       
                                       float rChannel   = texture2DRect( tex0, st - offset ).r;
                                       float gChannel   = texture2DRect( tex0, st ).g;
                                       float bChannel   = texture2DRect( tex0, st + offset ).b;
                                       
                                       vec3 finalCol	= vec3( rChannel, gChannel, bChannel );
                                       
                                       gl_FragColor.rgb = finalCol;
                                       gl_FragColor.a   = 1.0;
                                   }
                                   );
    }
    
    void update(){
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniform2f("windowSize", width, height);
        shader.setUniform1f("offsetALL", offset );
        
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
    };
    
    float offset;
};

