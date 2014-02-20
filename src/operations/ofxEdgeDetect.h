//
//  ofxEdgeDetect.h
//
//  Created by Patricio Gonzalez Vivo on 2/16/14.
//
//

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxEdgeDetect : public ofxFXObject {
public:
    ofxEdgeDetect(){
        passes = 1;
        radius = 0.25;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   uniform float radius;
                                   
                                   float getIntensity(vec2 u){
                                       vec3 a = texture2DRect(tex0,u).xyz;
                                       return (a.x+a.y+a.z)/3.0;
                                   }
                                   
                                   void main(void){
                                       vec2 uv = gl_FragCoord.xy;
                                       vec2 p = vec2(0.5+radius*2.0);
                                       
                                       float avg = 0.0;
                                       avg += getIntensity(uv+vec2(p.x,0.0));
                                       avg += getIntensity(uv+vec2(-p.x,0.0));
                                       avg += getIntensity(uv+vec2(0.0,p.y));
                                       avg += getIntensity(uv+vec2(0.0,-p.y));
                                       avg += getIntensity(uv+vec2(p.x,p.y));
                                       avg += getIntensity(uv+vec2(-p.x,-p.y));
                                       avg += getIntensity(uv+vec2(p.x,-p.y));
                                       avg += getIntensity(uv+vec2(-p.x,p.y));
                                       avg /= 8.0;
                                       
                                       float result = (1.0-getIntensity(uv)) + avg;
                                       result = (1.0 - result) * 10.0;
                                       
                                       gl_FragColor = vec4(result);
                                   }
                                   );
    }
    
    void update(){
        pingPong.dst->begin();
        
        ofClear(0);
        shader.begin();
        
        shader.setUniformTexture( "tex0" , textures[0].getTextureReference(), 0 );
        shader.setUniform1f("radius", radius );
        
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
    };
    
    float radius;
};