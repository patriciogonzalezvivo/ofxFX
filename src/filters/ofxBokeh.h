//
//  ofxBokeh.h
//
//
//  Created by Tim Scaffidi ( http://timothyscaffidi.com/ )
//
//  He share it on the openFrameworks forum here:
//  http://forum.openframeworks.cc/index.php/topic,8583.msg40032.html#msg40032
//

#ifndef OFXBOKEH 
#define OFXBOKEH

#include "ofxFXObject.h"
#include "ofxUnsharp.h"

class ofxBokeh : public ofxUnsharp {	
public:
    ofxBokeh(){
        passes = 3;
        internalFormat = GL_RGBA;
        
        // Fade constant
        value0 = 1.f;   
        
        // In this example the tex0 it´s more like a backbuffer
        // The doble loop demands lot´s of resources to the GPU
        fragmentShader = "#version 120\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
uniform sampler2DRect tex0;\n\
uniform float value0;\n\
\n\
void main(void) {\n\
    vec4 finalColor = vec4(0.0,0.0,0.0,1.0);\n\
    float weight = 0.;\n\
    int radius = int(value0);\n\
    \n\
    for(int x = radius * -1 ; x < radius; x++) {\n\
        for(int y = radius * -1; y < radius; y++){\n\
            \n\
            vec2 coord = gl_TexCoord[0].xy + vec2(x,y);\n\
            \n\
            if(distance(coord, gl_TexCoord[0].xy) < float(radius)){\n\
                vec4 texel = texture2DRect(tex0, coord);\n\
                float w = length(texel.rgb)+ 0.1;\n\
                weight += w;\n\
                finalColor += texel * w;\n\
            }\n\
        }\n\
    }\n\
    \n\
    gl_FragColor = finalColor/weight;\n\
}\n\
\n";  
    };
    
	void setRadius(float _radius) { if (_radius >= 1) value0 = _radius;};
};

#endif
