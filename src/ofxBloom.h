//
//  ofxBloom.h
//  emptyExample
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXBLOOM
#define OFXBLOOM


#include "ofMain.h"
#include "ofxFXObject.h"

class ofxBloom : ofxFXObject {	
public:
    ofxBloom(){
        passes = 1;
        nTextures = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = "#version 120\n \
        #extension GL_ARB_texture_rectangle : enable\n \
        \
        uniform sampler2DRect tex0;\
        \
        void main(){\
            vec4 sum = vec4(0);\
            vec2 st = gl_TexCoord[0].st;\
            int j;\
            int i;\
            \
            for( i= -4 ;i < 4; i++){\
                for (j = -3; j < 3; j++){\
                    sum += texture2DRect(backbuffer, st + vec2(j, i)*0.004) * 0.25;\
                }\
            }\
            \
            if (texture2DRect(backbuffer, st).r < 0.3){\
                gl_FragColor = sum * sum * 0.012 + texture2DRect(backbuffer, st);\
            } else {\
                if (texture2DRect(src_tex_unit0, st).r < 0.5){\
                    gl_FragColor = sum * sum * 0.009 + texture2DRect(backbuffer, st);\
                } else {\
                    gl_FragColor = sum * sum * 0.0075 + texture2DRect(backbuffer, st);\
                }\
            }\
        }";
    }
};
#endif
