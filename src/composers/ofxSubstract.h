//
//  ofxSubstract.h
//  week5_GLSL_vectorFields
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef OFXSUBSTRACT
#define OFXSUBSTRACT

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxSubstract : public ofxFXObject {
public:
    ofxSubstract(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform sampler2DRect tex1;
                                   
                                   void main(){
                                       vec4 color0 = texture2DRect(tex0,gl_TexCoord[0].st);
                                       vec4 color1 = texture2DRect(tex1,gl_TexCoord[0].st);
                                       gl_FragColor = color0 - color1;
                                   }
                                   );
    }
};
#endif
