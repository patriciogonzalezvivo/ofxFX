//
//  ofxBokeh.h
//
//
//  Created by Tim Scaffidi ( http://timothyscaffidi.com/ )
//
//  He share it on the openFrameworks forum here:
//  http://forum.openframeworks.cc/index.php/topic,8583.msg40032.html#msg40032
//

#pragma once

#define STRINGIFY(A) #A

#include "ofxFXObject.h"
#include "ofxBlur.h"

class ofxBokeh : public ofxBlur {
public:
    ofxBokeh(){
        passes = 3;
        internalFormat = GL_RGBA;
        
        // Fade constant
        fade = 1.0f;
        
        // In this example the tex0 it´s more like a backbuffer
        // The doble loop demands lot´s of resources to the GPU
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform float fade;
                                   
                                   void main(void) {
                                       vec4 finalColor = vec4(0.0,0.0,0.0,1.0);
                                       float weight = 0.;
                                       int radius = int(fade);
                                       
                                       for(int x = radius * -1 ; x < radius; x++) {
                                           for(int y = radius * -1; y < radius; y++){
                                               
                                               vec2 coord = gl_TexCoord[0].xy + vec2(x,y);
                                               
                                               if(distance(coord, gl_TexCoord[0].xy) < float(radius)){
                                                   vec4 texel = texture2DRect(tex0, coord);
                                                   float w = length(texel.rgb)+ 0.1;
                                                   weight += w;
                                                   finalColor += texel * w;
                                               }
                                           }
                                       }
                                       
                                       gl_FragColor = finalColor/weight;
                                   });
    };
    
	void setRadius(float _radius) { if (_radius >= 1) fade = _radius;};
};
