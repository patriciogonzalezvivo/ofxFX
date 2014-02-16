//
//  ofxEdgeDetect.h
//
//  Created by Patricio Gonzalez Vivo on 2/16/14.
//
//

#ifndef OFXABSDIFF
#define OFXABSDIFF

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxEdgeDetect : public ofxFXObject {
public:
    ofxEdgeDetect(){
        passes = 1;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                   
                                   float getIntensity(vec2 u){
                                       vec3 a = texture2DRect(tex0,u).xyz;
                                       return (a.x+a.y+a.z)/3.0;
                                   }
                                   
                                   void main(void){
                                       vec2 uv = gl_FragCoord.xy;
                                       vec2 p = vec2(1.0);
                                       
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
                                       
                                       gl_FragColor = vec4(vec3(result),1.0);
                                   }
                                   );
    }
};
#endif