//
//  ofxGrayscale.h
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Based on http://www.iquilezles.org/apps/shadertoy/ Postprocessing shader
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxGrayscale : public ofxFXObject {
public:
    ofxGrayscale(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   
                                   void main(void){
                                       vec4 textureColor = texture2DRect(tex0, gl_TexCoord[0].st);
                                       float luminance = dot(vec3 (0.2125, 0.7154, 0.0721), vec3(textureColor));
                                       
                                       gl_FragColor = vec4(luminance, luminance, luminance, textureColor.a);
                                   }
                                   );
    }
};