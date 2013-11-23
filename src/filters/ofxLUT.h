/*
 *  ofxMask.h
 *
 *  Created by Patricio Gonzalez Vivo on 10/1/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *  ******************************************************************
 *
 *  tex0 -> Image
 *  tex1 -> LUT
 *
 */

#ifndef OFXLUT
#define OFXLUT

#define STRINGIFY(A) #A

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxLUT : public ofxFXObject {	
public:
    ofxLUT(){
        passes = 1;
        internalFormat = GL_RGB32F;
        
#ifdef TARGET_OPENGLES
        //  OpenGL ES
        fragmentShader = STRINGIFY(precision highp float;
                                   
                                   uniform sampler2D tex0;
                                   uniform sampler2D tex1;
                                   
                                   float size = 32.0;
                                   
                                   void main(void){
                                       vec2 st = gl_TexCoord[0].st;
                                       vec4 srcColor = texture2D(tex0, st);
                                       
                                       float x = (srcColor.r/size)/size;
                                       float y = srcColor.g/size;
                                       float z = srcColor.b;
                                       
                                       vec3 color = texture2D(tex1, vec2(x+y,z) ).rgb;
                                       
                                       gl_FragColor = vec4( color , 1.0);
                                   });
#else
        if( ofIsGLProgrammableRenderer() ){
            
            //  TODO:
            //          - Implement OpenGL 3.0 shader
            //
            //  OpenGL 3.0
            fragmentShader = STRINGIFY( );
            
        } else {
            //  OpenGL 2.0
            fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
                                       uniform sampler2DRect tex1;
                                       
                                       float size = 32.0;
                                       
                                       void main(void){
                                           vec2 st = gl_TexCoord[0].st;
                                           vec4 srcColor = texture2DRect(tex0, st);
                                           
                                           float x = srcColor.r * (size-1.0);
                                           float y = srcColor.g * (size-1.0);
                                           float z = srcColor.b * (size-1.0);
                                           
                                           vec3 color = texture2DRect(tex1, vec2(floor(x)+0.5 +(floor(y)*size+0.5),
                                                                                 floor(z)+0.5) ).rgb;
                                           
                                           gl_FragColor = vec4( color , 1.0);
                                       });
        }
#endif
    }
    
    bool loadLUT(ofBuffer &buffer){
        buffer.resetLineReader();
        
        int mapSize = 32;
        
        float * pixels = new float[mapSize * mapSize * mapSize * 3];
        
        textures[1].getTextureReference().allocate(mapSize * mapSize, mapSize,GL_RGB32F);
        
        ofDirectory dir;
        
        for(int z=0; z<mapSize ; z++){
            for(int y=0; y<mapSize; y++){
                for(int x=0; x<mapSize; x++){
                    string content = buffer.getNextLine();
                    
                    int pos = x + y*mapSize + z*mapSize*mapSize;
                    
                    vector <string> splitString = ofSplitString(content, " ", true, true);
                    
                    if (splitString.size() >=3) {
                        pixels[pos*3 + 0] = ofToFloat(splitString[0]);
                        pixels[pos*3 + 1] = ofToFloat(splitString[1]);
                        pixels[pos*3 + 2] = ofToFloat(splitString[2]);
                    }
                }
            }
        }
        
        textures[1].getTextureReference().loadData( pixels, mapSize * mapSize, mapSize, GL_RGB);
        
        return true;
    }
    
    bool loadLUT(string lutFile){
        ofBuffer buffer = ofBufferFromFile(lutFile);
        
        return loadLUT(buffer);
    }
    
};
#endif
