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
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform sampler2DRect tex1;
                                   
                                   void main(){
                                       vec4 color0 = texture2DRect(tex0, gl_TexCoord[0].st);
                                       vec4 color1 = texture2DRect(tex1, gl_TexCoord[0].st);
                                       vec3 diff =  abs(color0 - color1).rgb;
                                       gl_FragColor = vec4(diff,max(diff.r,max(diff.g,diff.b)));
                                   }
                                   );
    }
};