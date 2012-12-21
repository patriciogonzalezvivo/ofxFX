//
//  ofxSwapBuffer.h
//  emptyExample
//
//  Created by Patricio González Vivo on 13/01/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXSWAPBUFFER
#define OFXSWAPBUFFER

class ofxSwapBuffer {
public:
    virtual void allocate( int _width, int _height, int _internalformat = GL_RGBA){

        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
        }
        
        
        clear();
        
        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }
    
    virtual void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    virtual void clear(int _alpha = 255){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,_alpha);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
    
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s  
    int     flag;       // Integer for making a quick swap
};

#endif