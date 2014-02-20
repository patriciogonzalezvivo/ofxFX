//
//  ofxInverse.h
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxInverse : public ofxFXObject {
public:
    ofxInverse(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   
                                   void main(){
                                       vec4 color = texture2DRect(tex0,gl_TexCoord[0].st);
                                       gl_FragColor = vec4( vec3(1.0,1.0,1.0) - color.rgb, 1.0);
                                   }
                                   );
    }
};