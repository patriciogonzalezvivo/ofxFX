/*
 *  ofxFXObject.h
 *
 *  Created by Patricio González Vivo on 03/07/11.
 *  Copyright 2011 http://www.PatricioGonzalezVivo.com All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************
 *
 *  Definition: Parent class of all other effects. It can be used in order to compile simple shaders such
 *  as the ones you could find at: Ricardo Caballero´s webGL Sandbox http://mrdoob.com/projects/glsl_sandbox/ 
 *  and ShaderToy by Inigo Quilez http://www.iquilezles.org/apps/shadertoy/
 *  
 */ 

#include "ofxFXObject.h"

ofxFXObject::ofxFXObject():nTextures(0),width(0),height(0){
    // Simple effects just need these three variables
    // For something more complex that require another structure, logic or more shaders working together
    // consider making a new stand-alone class like ofxBlur, ofxFluid, ofxGlow, etc ...
    // Collaborations are welcome
    
    passes = 1;                 // Number of iterations needed. Default is 1;
    
    internalFormat = GL_RGBA;   // Type of GL textures 
    
    // And the fragSahder itself. These are the default variables given in ofxFX:
    //
    // - time
    // - mouse position (normalized)
    // - resolution
    // - backbuffer texture
    // - tex0, tex1, tex2, ... : this are dynamicaly defined and allocated and can be
    //   filled with information by using .begin(0) and .end(0), or .begin(1) and .end(1), etc 

    fragmentShader = "";   
    vertexShader = "";
}

ofxFXObject::ofxFXObject(ofxFXObject& parent){
    passes = parent.getPasses();
    internalFormat = parent.getInternalFormat();
    fragmentShader = parent.getCode();
    vertexShader = parent.getVertexCode();
}

ofxFXObject::~ofxFXObject(){
    if (textures != NULL ){
        if (nTextures > 0) {
            delete [] textures;
        }
    }
}

ofxFXObject& ofxFXObject::operator =(ofxFXObject& parent){
    passes = parent.getPasses();
    internalFormat = parent.getInternalFormat();
    fragmentShader = parent.getCode();
    vertexShader = parent.getVertexCode();
    ofVec2f resolution = parent.getResolution();
    allocate(resolution.x, resolution.y);
    
    return * this;
}


void ofxFXObject::allocate(int _width, int _height, int _internalFormat){
    internalFormat = _internalFormat;
    allocate(_width, _height);
};

void ofxFXObject::allocate(int _width, int _height){
    width = _width;
    height = _height;
    
    pingPong.allocate(width, height, internalFormat);
    compileCode();
};

bool ofxFXObject::loadVersioned(string glESPath, string gl2Path, string gl3Path){
#ifdef TARGET_OPENGLES
    if (!glESPath.empty())
        return load(glESPath);
#else
    if (ofIsGLProgrammableRenderer()){
        if (!gl3Path.empty())
            return load(gl3Path);
    } else {
        if (!gl2Path.empty())
            return load(gl2Path);
    }
#endif
    return false;
}

bool ofxFXObject::load( string path ){
    ofShader code_loader;
    code_loader.load(path);

    string frag = code_loader.getShaderSource(GL_FRAGMENT_SHADER);
    string vert = code_loader.getShaderSource(GL_VERTEX_SHADER);

    if (frag.empty() && vert.empty()){
        ofLog(OF_LOG_ERROR, "Could not load shader from file " + path);
        return false;
    }

    bool loaded = setCode(frag, vert);
    if (loaded)
        shaderFilePath = path;
    return loaded;
}

bool ofxFXObject::reload(){
    if (!shaderFilePath.empty())
        return load(shaderFilePath);
    return false;
}

bool ofxFXObject::setCode(string frag, string vert){
    if (fragmentShader == frag && vertexShader == vert)
        return false;
        
    bool loaded = false;
    
    ofShader test;
    if (frag.empty() == false)
        test.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
    if (vert.empty() == false)
        test.setupShaderFromSource(GL_VERTEX_SHADER, vert);

    bFine = test.linkProgram();
    
    if( bFine ){
        if (frag.empty() == false)
            fragmentShader = frag;
        if (vert.empty() == false)
            vertexShader   = vert;
        loaded = compileCode();
    }
    
    return loaded;
}

void ofxFXObject::selectShaderSource(){
    if (fragmentShader.empty()){
#ifdef TARGET_OPENGLES
        fragmentShader = glESFragmentShader;
#else
        if (ofIsGLProgrammableRenderer()){
            fragmentShader = gl3FragmentShader;
        } else {
            fragmentShader = gl2FragmentShader;
        }
    }
#endif

    if (vertexShader.empty()){
#ifdef TARGET_OPENGLES
        vertexShader = glESVertexShader;
#else
        if (ofIsGLProgrammableRenderer()){
            vertexShader = gl3VertexShader;
            // If the vertex shader for GL3 isn't specified, we fill
            // in a simple pass-through shader. This way, users can
            // give only fragment shaders, just like for GL2/ES. This
            // is necessary because having a vertex shader is mandatory
            // in GL3.
            if (vertexShader.empty()){
                vertexShader = "#version 150\n";
                vertexShader += STRINGIFY(
                        uniform mat4 modelViewProjectionMatrix;
                        in vec4 position;
                        void main(){
                            gl_Position = modelViewProjectionMatrix * position;
                        });
            }
        } else {
            vertexShader = gl2VertexShader;
        }
#endif
    }
}

bool ofxFXObject::needsFboResize(){
    if (nTextures > 0){
        // Assume all textures have the same size.
        if (textures[0].getWidth() != width)
            return true;
        if (textures[0].getHeight() != height)
            return true;
    }
    return false;
}

bool ofxFXObject::compileCode(){
    // Load the correct shader sources into fragmentShader and vertexShader.
    selectShaderSource();
    
    // Looks how many textures are declared in the injected fragment shader
    int num = 0;

    for (int i = 0; i < 10; i++){
        string searchFor = "tex" + ofToString(i);
        if (fragmentShader.find(searchFor)!= -1)
            num++;
        else 
            break;
    }
    
    // If we have several passes, the shader might only have backbuffer
    // declared, but we still want to use tex0 for input.
    if (num == 0 && fragmentShader.find("backbuffer") != -1){
        num++;
    }

    // We need to re-allocate frame buffers if the number of textures is
    // different, or if the width or height have changed.
    if (num != nTextures || needsFboResize()){
        if (textures != NULL){
            if (nTextures > 0) {
                delete [] textures;
            }
        }
        // Initialate the right amount of textures with correct size.
        nTextures = num;
        if (nTextures > 0){
            textures = new ofFbo[nTextures];
        } else if (nTextures == 0){
            textures = NULL;
        }
        
        // In any case it will allocate the total amount of textures with the internalFormat needed
        for( int i = 0; i < nTextures; i++){
            initFbo(textures[i], width, height, internalFormat);
        }
    }
    
    // Compile the shader and load it to the GPU
    shader.unload();
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    if (vertexShader.empty() == false)
        shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    bFine = shader.linkProgram();
    
    return bFine;
}

// A simplified way of filling the textures
void ofxFXObject::setTexture(ofBaseDraws& tex, int _texNum){
    if ( tex.getWidth() > width || tex.getHeight() > height ) {
        allocate(tex.getWidth(), tex.getHeight());
    }
    
    if ((_texNum < nTextures) && ( _texNum >= 0)){
        textures[_texNum].begin(); 
        ofPushStyle();
        {
            ofClear(0,0);
            ofSetColor(255);
            ofDisableAlphaBlending(); // Defer alpha blending until .draw() to keep transparencies clean.
            tex.draw(0,0); 
        }
        ofPopStyle();
        textures[_texNum].end();
    }
};

// With begin(int) and end(int) the textures allocated can be filled with data
void ofxFXObject::begin(int _texNum ) {
    if ((_texNum < nTextures) && ( _texNum >= 0)){
        ofPushStyle();
        ofPushMatrix();
        textures[_texNum].begin();
        ofClear(0,0);
        
        bUpdate = true;
    }
}

void ofxFXObject::end(int _texNum) {
    if ((_texNum < nTextures) && ( _texNum >= 0)){
        textures[_texNum].end();
        ofPopMatrix();
        ofPopStyle();
    }
}

// UPDATE
// -------------------------------------
//

// As most objects in openFrameworks, ofxFXObject have to be updated() in order to process the information on the GPU
void ofxFXObject::update(){
    ofPushStyle();
    ofSetColor(255,255);

    // This process is going to be repeated as many times passes variable specifies
    for(int pass = 0; pass < passes; pass++) {
        
        // All the processing is done on the pingPong ofxSwapBuffer (basicaly two ofFbo that have a swap() funtion)
        pingPong.dst->begin();
        
        ofClear(0,0);
        ofDisableAlphaBlending(); // Defer alpha blending until .draw() to keep transparencies clean.
        shader.begin();
        
        // The other ofFbo of the ofxSwapBuffer can be accessed by calling the unicode "backbuffer"
        // This is usually used to access "the previous pass", or the original frame for the first pass.
        if (pass == 0 && nTextures >= 1){
            shader.setUniformTexture("backbuffer", textures[0].getTextureReference(), 0);
        } else {
            shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0);
        }
        
        // All the necessary textures are provided to the shader in this loop
        for( int i = 0; i < nTextures; i++){
            string texName = "tex" + ofToString(i); 
            shader.setUniformTexture(texName.c_str(), textures[i].getTextureReference(), i+1 );
            string texRes = "size" + ofToString(i); 
            shader.setUniform2f(texRes.c_str() , (float)textures[i].getWidth(), (float)textures[i].getHeight());
        }
        
        // There are some standard variables that are passes to the shaders. These follow the names used by
        // Ricardo Caballero's webGL Sandbox http://mrdoob.com/projects/glsl_sandbox/ and
        // ShaderToy by Inigo Quilez http://www.iquilezles.org/apps/shadertoy/ webGL interactive GLSL editors.
        shader.setUniform1f("time", ofGetElapsedTimef() );
        shader.setUniform2f("size", (float)width, (float)height);
        shader.setUniform2f("resolution", (float)width, (float)height);
        shader.setUniform2f("mouse", (float)(ofGetMouseX()/width), (float)(ofGetMouseY()/height));

        injectUniforms();
        
        // renderFrame() is a built-in funtion of ofxFXObject that only draws a white box that
        // functions as a frame where the textures can rest.
        // If you want to distort the points of a texture, you probably want to re-define the renderFrame funtion.
        renderFrame();
        
        shader.end();
        
        pingPong.dst->end();
        
        // Swap the ofFbos. Now dst is src and src is dst. Each iteration writes to
        // dst and uses src as a backbuffer, where the previous frame is kept.
        pingPong.swap();
    }
    
    pingPong.swap(); // After the loop the finished render will be at the src ofFbo of the ofxSwapBuffer
                     // this extra swap() call will put it on the dst one. Which sounds more reasonable...
    
    ofPopStyle();
    
    bUpdate = false;
};

// DRAW
// --------------------------------------

// Finaly the drawing funtion. It can be used with or without arguments in order to make it more flexible
void ofxFXObject::draw(int _x, int _y, float _width, float _height){
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    pingPong.dst->draw(_x, _y, _width, _height);
    ofPopStyle();
}

// BUILD-IN functions & variables
// -------------------------------------
//

// Allocates and cleans an ofFbo
void ofxFXObject::initFbo(ofFbo & _fbo, int _width, int _height, int _internalformat) {
    _fbo.allocate(_width, _height, _internalformat);
    _fbo.begin();
    ofClear(0,0);
    _fbo.end();
}

// Draw a white box in order to render the final texture
// It acts as a frame where the dst textures can rest.
// If you want to distort the points of a texture, you probably want to re-define something here.
void ofxFXObject::renderFrame(float _width, float _height){
    if (_width == -1) _width = width;
    if (_height == -1) _height = height;
    
    // If the shader is not well compiled it will show an image little more gray.
    //
    if (bFine)
        ofSetColor(255,255);  
    else
        ofSetColor(150,255);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(_width, 0); glVertex3f(_width, 0, 0);
    glTexCoord2f(_width, _height); glVertex3f(_width, _height, 0);
    glTexCoord2f(0,_height);  glVertex3f(0,_height, 0);
    glEnd();
}
