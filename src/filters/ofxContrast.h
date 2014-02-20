//
//  ofxContrast.h
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxContrast : public ofxFXObject {
public:
    ofxContrast(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform float contrast;
                                   uniform float brightness;
                                   
                                   void main(){
                                       vec4 color = texture2DRect(tex0,gl_TexCoord[0].st);
            
                                       float p = 0.3 *color.g + 0.59*color.r + 0.11*color.b;
                                       p = p * brightness;
                                       color = vec4(p,p,p,1.0);
                                       color = mix( vec4(1.0,1.0,1.0,1.0),color,contrast);
            
                                       gl_FragColor =  vec4(color.r , color.g, color.b, 1.0);
                                   }
                                   );
    }
    
    void update(){
        pingPong.dst->begin();
            
        ofClear(0);
        shader.begin();
        
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniform1f("contrast", contrast );
        shader.setUniform1f("brightness", brightness);
            
        renderFrame();
            
        shader.end();
            
        pingPong.dst->end();
    };
    
    float contrast;
    float brightness;
};

