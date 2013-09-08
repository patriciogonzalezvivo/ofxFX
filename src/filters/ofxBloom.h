//
//  ofxBloom.h
//  emptyExample
//
//  Created by Patricio Gonzalez Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXBLOOM
#define OFXBLOOM

#include "ofMain.h"
#include "ofxFXObject.h"

#define STRINGIFY(A) #A

class ofxBloom : public ofxFXObject {	
public:
    ofxBloom(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = "uniform sampler2DRect tex0; \n \
                        \n \
                                   void main(){\n \
                                       vec4 sum = vec4(0);\n \
                                       vec2 st = gl_TexCoord[0].st;\n \
                                       int j,i;\n \
                                       for( i= -4 ;i < 4; i++){\n \
                                           for (j = -3; j < 3; j++){\n \
                                               sum += texture2DRect(tex0, st + vec2(j, i)*0.004) * 0.25;\n \
                                           }\n \
                                       }\n \
                                       \n \
                                       if (texture2DRect(tex0, st).r < 0.3){\n \
                                           gl_FragColor = sum * sum * 0.012 + texture2DRect(tex0, st);\n \
                                       } else {\n \
                                           if (texture2DRect(tex0, st).r < 0.5){\n \
                                               gl_FragColor = sum * sum * 0.009 + texture2DRect(tex0, st);\n \
                                           } else {\n \
                                               gl_FragColor = sum * sum * 0.0075 + texture2DRect(tex0, st);\n \
                                           }\n \
                                       }\n \
                                   }";
    }
};
#endif
