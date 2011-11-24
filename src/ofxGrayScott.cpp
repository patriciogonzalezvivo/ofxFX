//
//  ofxGrayScott.cpp
//  grayScott
//
//  Created by Patricio González Vivo on 10/1/11.
//  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ofxGrayScott.h"

ofxGrayScott::ofxGrayScott()
{
    string fragmentShader = "#version 120 \n \
    #extension GL_ARB_texture_rectangle : enable \n \
    #define KERNEL_SIZE 9 \n \
    \
    float kernel[KERNEL_SIZE];\
    vec2 offset[KERNEL_SIZE];\
    \
    uniform sampler2DRect texture;\
    uniform sampler2DRect buffer;\
    uniform float diffU;\
    uniform float diffV;\
    uniform float f;\
    uniform float k;\
    \
    void main(void){\
        vec2 texCoord   = gl_TexCoord[0].st;\
        \
        kernel[0] = 0.707106781;\
        kernel[1] = 1.0;\
        kernel[2] = 0.707106781;\
        kernel[3] = 1.0;\
        kernel[4] = -6.82842712;\
        kernel[5] = 1.0;\
        kernel[6] = 0.707106781;\
        kernel[7] = 1.0;\
        kernel[8] = 0.707106781;\
        \
        offset[0] = vec2( -1.0, -1.0);\
        offset[1] = vec2(  0.0, -1.0);\
        offset[2] = vec2(  1.0, -1.0);\
        \
        offset[3] = vec2( -1.0, 0.0);\
        offset[4] = vec2(  0.0, 0.0);\
        offset[5] = vec2(  1.0, 0.0);\
        \
        offset[6] = vec2( -1.0, 1.0);\
        offset[7] = vec2(  0.0, 1.0);\
        offset[8] = vec2(  1.0, 1.0);\
        \
        vec2 texColor		= texture2DRect( buffer, texCoord ).rb;\
        float srcTexColor   = texture2DRect( texture, texCoord ).r;\
        \
        vec2 lap            = vec2( 0.0, 0.0 );\
        \
        for( int i=0; i < KERNEL_SIZE; i++ ){\
            vec2 tmp    = texture2DRect( buffer, texCoord + offset[i] ).rb;\
            lap         += tmp * kernel[i];\
        }\
        \
        float F     = f + srcTexColor * 0.025 - 0.0005;\
        float K     = k + srcTexColor * 0.025 - 0.0005;\
        \
        float u		= texColor.r;\
        float v		= texColor.g + srcTexColor * 0.5;\
        \
        float uvv   = u * v * v;\
        \
        float du    = diffU * lap.r - uvv + F * (1.0 - u);\
        float dv    = diffV * lap.g + uvv - (F + K) * v;\
        \
        u += du * 0.6;\
        v += dv * 0.6;\
        \
        gl_FragColor.rgba = vec4(clamp( u, 0.0, 1.0 ), 1.0 - u/v ,clamp( v, 0.0, 1.0 ), 1.0);\
    }";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.linkProgram();
    
    frame = 0;
    iterations = 10;
    
    diffU = 0.25f;
	diffV = 0.04f;//0.04f;
	k = 0.047f;//0.037f;
	f = 0.1f;//0.2f;
}

ofxGrayScott& ofxGrayScott::allocate(int _width, int _height)
{ 
    width = _width; 
    height = _height; 
    
    for(int i = 0; i < 2; i++){
        fbo[i].allocate(width,height,GL_RGB);
        fbo[i].begin();
        ofClear(0,0,0,255);
        fbo[i].end();
    }
    
    texture.allocate(width, height, GL_RGB);
    texture.begin();
    ofClear(0,0,0,255);
    texture.end();
    
    return * this;
}

void ofxGrayScott::begin()
{
    texture.begin();
}

void ofxGrayScott::end( bool drawIt )
{
    texture.end();
    
    update();
    
    if (drawIt)
        draw();
}

void ofxGrayScott::update()
{    
    int actual,prev;
    
    for( int i = 0; i < iterations ; i++ ){
		actual = ( frame ) % 2;
		prev   = ( frame + 1 ) % 2;

		fbo[actual].begin();
		ofClear(0,0,0,255);
		
		shader.begin();
		shader.setUniformTexture("buffer", fbo[prev].getTextureReference(), 0 );
		shader.setUniformTexture("texture", texture.getTextureReference(), 1 );
		shader.setUniform1f( "diffU", (float)diffU);
		shader.setUniform1f( "diffV", (float)diffV );
		shader.setUniform1f( "f", (float)f );
		shader.setUniform1f( "k", (float)k );
		
		renderFrame();
		shader.end();
        fbo[actual].end();
		
        fbo[prev].begin();
        ofClear(0, 0, 0, 0);
        fbo[actual].draw(0,0);
        fbo[prev].end();
        
        // Switch buffers
        frame++;
	}
}

void ofxGrayScott::renderFrame(){
    // Rendering canvas frame in order to make it cleaner to read.
    ofSetColor(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
	glTexCoord2f(width, height); glVertex3f(width, height, 0);
	glTexCoord2f(0,height);  glVertex3f(0,height, 0);
	glEnd();
}

void ofxGrayScott::draw(){
    int actual = (frame+1)%2;   // Actual buffer
    fbo[actual].draw(0, 0);
}