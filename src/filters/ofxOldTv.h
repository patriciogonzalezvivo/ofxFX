//
//  ofxOldTv.h
//
//  Created by Patricio González Vivo on 25/11/11.
//  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
//
//  Based on http://www.iquilezles.org/apps/shadertoy/ Postprocessing shader
//

#ifndef OFXOLDTV
#define OFXOLDTV

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxOldTv : public ofxFXObject {	
public:
    ofxOldTv(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = "#version 120\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
uniform float time;\n\
uniform sampler2DRect tex0;\n\
\n\
float brightness = 0.2;\n\
float rows = 5.0;\n\
\n\
void main(void){\n\
    vec2 st = gl_TexCoord[0].st;\n\
    vec2 uv = 0.5 + (1.0/st-0.5)*( (1.0-brightness) + brightness*sin(0.2*time) );\n\
    \n\
    vec3 col;\n\
    \n\
    col.r = texture2DRect(tex0, vec2(st.x+0.003,st.y)).r;\n\
    col.g = texture2DRect(tex0, vec2(st.x+0.000,st.y)).g;\n\
    col.b = texture2DRect(tex0, vec2(st.x-0.003,st.y)).b;\n\
    \n\
    col = clamp(col * 0.5 + 0.5 * col * col * 1.2,0.0,1.0);\n\
    \n\
    col *= 0.5 + 0.5 * 16.0 * uv.x * uv.y * (1.0-uv.x) * (1.0-uv.y);\n\
    \n\
    col *= vec3(0.8,1.0,0.7);\n\
    \n\
    col *= 0.9 + 0.1 * sin(10.0 * time + st.y * rows);\n\
    \n\
    col *= 0.97 + 0.03 * sin(110.0 * time);\n\
    \n\
    gl_FragColor = vec4(col,1.0);\n\
}\n\
\n";
    }
};
#endif