#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetWindowShape(640, 480);
	image.loadImage("logo.jpg");
    
    
    electro = "#version 120\n\
    #extension GL_ARB_texture_rectangle : enable\n \
    uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
     \
    float viewAngle = 1.0;\
    float speed = 3.0;\
    float rate = 15.0;\
    float baseamp = 0.10;\
     \
    void main(void){\
        vec2 p = -1.0 + 3.0 * ( gl_FragCoord.xy / resolution.xy );\
        float x = speed * viewAngle * time + rate * p.x;\
        float base = (1.0 + cos(x * 2.5 + time)) * (1.0 + sin(x * 3.5 + time));\
        float z = fract(0.05 * x);\
        z = max(z, 1.0 - z);\
        z = pow(z, 2.0);\
        float pulse = exp(-10.0 * z);\
        vec4 ecg_color = vec4(0.3, 1.0, 0.4, 1.0);\
        vec4 c = pow(clamp(1.0 - abs(p.y - (baseamp * base + pulse - 0.5)), 0.0, 1.0), 16.0) * ecg_color;\
        gl_FragColor = c;\
    }";
    
    coloredWaves = "#version 120\n\
                    #extension GL_ARB_texture_rectangle : enable\n \
                    uniform float time; \
                    uniform vec2 mouse;\
                    uniform vec2 resolution;\
                    \
                    float pi = 3.141592653589793238462643383279;\
                    \
                    void main( void ) {\
                        float tscale = 2.5;\
                        \
                        vec2 position = gl_FragCoord.xy / resolution.xy;\
                        \
                        float lum   = abs(tan(position.y * pi)) - pi/5.0;\
                        \
                        float red   = sin(position.x * 5.0 + time*tscale*1.00) * 2.0 - 1.0;\
                        float green = sin(position.x * 8.0 + time*tscale*1.33) * 2.0 - 1.0;\
                        float blue  = sin(position.x * 2.0 + time*tscale*1.93) * 2.0 - 1.0;\
                        \
                        gl_FragColor = vec4( vec3( lum + red, lum + green, lum + blue ), 1.0 );\
                    }";
    
    colorSpiral =  "#version 120\n\
                    #extension GL_ARB_texture_rectangle : enable\n \
                    uniform float time;\
                    uniform vec2 mouse;\
                    uniform vec2 resolution;\
                    \
                    void main( void ){ \
                        vec2 position = (gl_FragCoord.xy - resolution * 0.5) / resolution.yy;\
                        \
                        float longest = sqrt(float(resolution.x*resolution.x) + float(resolution.y*resolution.y))*0.5;\
                        float dx = gl_FragCoord.x-resolution.x/2.0;\
                        float dy = 0.2+gl_FragCoord.y-resolution.y/2.0;\
                        float len = sqrt(dx*dx+dy*dy);\
                        float ds = len/longest;\
                        float md = time*2.0;\
                        \
                        float ang = 2.0*atan(dy,(len+dx));\
                        ang += pow(len, 0.5)*5.0;\
                        \
                        float red = (128.0 - sin(ang + md*3.141592*2.0) * 127.0)*(1.0-ds);\
                        float green = (128.0 - cos(ang + md*3.141592*2.0) * 127.0)*(1.0-ds);\
                        float blue = (128.0 + sin(ang  + md*3.141592*2.0) * 127.0)*(1.0-ds);\
                        \
                        gl_FragColor = vec4( vec3( red/255.0, green/255.0, blue/255.0), 1.0 );\
                    }";
    
    noise1 =   "#version 120\n\
                #extension GL_ARB_texture_rectangle : enable\n \
                \
                uniform float time;\
                uniform vec2 mouse;\
                uniform vec2 resolution;\
                \
                float rand(vec2 co){\
                    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\
                }\
                \
                float noise2f( in vec2 p ){ \
                    vec2 ip = vec2(floor(p));\
                    vec2 u = fract(p);\
                    u = u*u*(3.0-2.0*u);\
                    \
                    float res = mix(mix(rand(ip),  rand(ip+vec2(1.0,0.0)),u.x),\
                                    mix(rand(ip+vec2(0.0,1.0)),   rand(ip+vec2(1.0,1.0)),u.x),\
                                    u.y);\
                    return res*res;\
                }\
                \
                float fbm(vec2 c) {\
                    float f = 0.0;\
                    float w = 1.0;\
                    for (int i = 0; i < 8; i++) {\
                        f += w*noise2f(c);\
                        c *=2.0;\
                        w*=0.5;\
                    }\
                    return f;\
                }\
                \
                vec2 cMul(vec2 a, vec2 b) {\
                    return vec2( a.x*b.x -  a.y*b.y,a.x*b.y + a.y * b.x);\
                }\
                \
                float pattern(  vec2 p, out vec2 q, out vec2 r ){\
                    q.x = fbm( p  +0.00*time);\
                    q.y = fbm( p + vec2(1.0));\
                    \
                    r.x = fbm( p +1.0*q + vec2(1.7,9.2)+0.15*time );\
                    r.y = fbm( p+ 1.0*q + vec2(8.3,2.8)+0.126*time);\
                    return fbm(p +1.0*r + 0.0* time);\
                }\
                \
                const vec3 color1 = vec3(0.101961,0.619608,0.666667);\
                const vec3 color2 = vec3(0.666667,0.666667,0.498039);\
                const vec3 color3 = vec3(0,0,0.164706);\
                const vec3 color4 = vec3(0.666667,1,1);\
                \
                void main() {\
                    vec2 q;\
                    vec2 r;\
                    vec2 c = 1000.0 * gl_FragCoord.xy / resolution.xy;\
                    float f = pattern( c * 0.01, q, r);\
                    vec3 col = mix( color1, color2, clamp( (f*f) * 4.0,0.0,1.0));\
                    col = color2;\
                    col = mix(col,color3,clamp(length(q),0.0,1.0));\
                    col = mix(col,color4,clamp(length(r.x),0.0,1.0));\
                    gl_FragColor =  vec4((0.2*f*f*f+0.6*f*f+0.5*f)*col,1.0);\
                }";
    
    //
    // Description : Array and textureless GLSL 2D simplex noise function.
    //      Author : Ian McEwan, Ashima Arts.
    //  Maintainer : ijm
    //     Lastmod : 20110822 (ijm)
    //     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
    //               Distributed under the MIT License. See LICENSE file.
    //               https://github.com/ashima/webgl-noise
    // 
    
    noise2 =   "#version 120\n\
                #extension GL_ARB_texture_rectangle : enable\n \
                \
                uniform float time;\
                uniform vec2 mouse;\
                uniform vec2 resolution;\
                uniform sampler2D backbuffer;\
                \
                vec3 mod289(vec3 x) {\
                    return x - floor(x * (1.0 / 289.0)) * 289.0;\
                }\
                \
                vec2 mod289(vec2 x) {\
                    return x - floor(x * (1.0 / 289.0)) * 289.0;\
                }\
                \
                vec3 permute(vec3 x) {\
                    return mod289(((x*34.0)+1.0)*x);\
                }\
                \
                float snoise(vec2 v){\
                    const vec4 C = vec4(0.211324865405187, 0.366025403784439,  -0.577350269189626, 0.024390243902439);\
                    vec2 i  = floor(v + dot(v, C.yy) );\
                    vec2 x0 = v -   i + dot(i, C.xx);\
                    \
                    vec2 i1;\
                    \
                    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\
                    \
                    vec4 x12 = x0.xyxy + C.xxzz;\
                    x12.xy -= i1;\
                    \
                    i = mod289(i);\
                    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));\
                    \
                    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\
                    m = m*m ;\
                    m = m*m ;\
                    \
                    vec3 x = 2.0 * fract(p * C.www) - 1.0;\
                    vec3 h = abs(x) - 0.5;\
                    vec3 ox = floor(x + 0.5);\
                    vec3 a0 = x - ox;\
                    \
                    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\
                    \
                    vec3 g;\
                    g.x  = a0.x  * x0.x  + h.x  * x0.y;\
                    g.yz = a0.yz * x12.xz + h.yz * x12.yw;\
                    return 130.0 * dot(m, g);\
                }\
                \
                void main( void ) {\
                    vec4 buf = texture2D(backbuffer, (gl_FragCoord.xy / resolution.xy));\
                    vec2 position = ( gl_FragCoord.xy + time + 100.0);\
                    float color = snoise(position /1000.0 + snoise(vec2(time / 100.0, time / 100.0 + 100.))) * 0.9 + snoise(position / 10.0) * 0.3 + snoise(position / 1.0) * 0.2;\
                    \
                    gl_FragColor = vec4( vec3( color ), 1.0 );\
                }";
    
    waves = "#version 120\n\
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    uniform sampler2DRect backbuffer;\
    \
    float getSpring(float r, vec2 stPos, float power){\
        return ( texture2DRect(backbuffer, stPos).r - r) * power;\
    }\
    \
    void main(){\
        vec2 st = gl_TexCoord[0].st;\
        float aspect = resolution.x / resolution.y;\
        \
        vec4 texel_prev = texture2DRect(backbuffer, st);\
        \
        float r_prev = texel_prev.r;\
        float power = .5;\
        \
        float vel = texel_prev.a - 0.5;\
        vel += getSpring(r_prev, st + vec2(2, 3), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(1, 3), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(0, 3), 0.005681818181818182 * power);\
        vel += getSpring(r_prev, st + vec2(-1, 3), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(-2, 3), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(3, 2), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(2, 2), 0.0066566640639421 * power);\
        vel += getSpring(r_prev, st + vec2(1, 2), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(0, 2), 0.011363636363636364 * power);\
        vel += getSpring(r_prev, st + vec2(-1, 2), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(-2, 2), 0.0066566640639421 * power);\
        vel += getSpring(r_prev, st + vec2(-3, 2), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(3, 1), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(2, 1), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(1, 1), 0.014691968395607415 * power);\
        vel += getSpring(r_prev, st + vec2(0, 1), 0.017045454545454544 * power);\
        vel += getSpring(r_prev, st + vec2(-1, 1), 0.014691968395607415 * power);\
        vel += getSpring(r_prev, st + vec2(-2, 1), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(-3, 1), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(3, 0), 0.005681818181818182 * power);\
        vel += getSpring(r_prev, st + vec2(2, 0), 0.011363636363636364 * power);\
        vel += getSpring(r_prev, st + vec2(1, 0), 0.017045454545454544 * power);\
        vel += getSpring(r_prev, st + vec2(-1, 0), 0.017045454545454544 * power);\
        vel += getSpring(r_prev, st + vec2(-2, 0), 0.011363636363636364 * power);\
        vel += getSpring(r_prev, st + vec2(-3, 0), 0.005681818181818182 * power);\
        vel += getSpring(r_prev, st + vec2(3, -1), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(2, -1), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(1, -1), 0.014691968395607415 * power);\
        vel += getSpring(r_prev, st + vec2(0, -1), 0.017045454545454544 * power);\
        vel += getSpring(r_prev, st + vec2(-1, -1), 0.014691968395607415 * power);\
        vel += getSpring(r_prev, st + vec2(-2, -1), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(-3, -1), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(3, -2), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(2, -2), 0.0066566640639421 * power);\
        vel += getSpring(r_prev, st + vec2(1, -2), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(0, -2), 0.011363636363636364 * power);\
        vel += getSpring(r_prev, st + vec2(-1, -2), 0.010022341036933013 * power);\
        vel += getSpring(r_prev, st + vec2(-2, -2), 0.0066566640639421 * power);\
        vel += getSpring(r_prev, st + vec2(-3, -2), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(2, -3), 0.0022411859348636983 * power);\
        vel += getSpring(r_prev, st + vec2(1, -3), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(0, -3), 0.005681818181818182 * power);\
        vel += getSpring(r_prev, st + vec2(-1, -3), 0.004759786021770571 * power);\
        vel += getSpring(r_prev, st + vec2(-2, -3), 0.0022411859348636983 * power);\
        \
        vel += (.25 - r_prev) * .025 * power;\
        \
        vec2 m = mouse / resolution;\
        vel += max(0., .1 * (1. - (length((st - m) * vec2(aspect, 1.)) * 15.)));\
        \
        gl_FragColor = vec4(texel_prev.rgb + vel, vel * .98 + .5);\
    }";
    
    destello = "#version 120\n\
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform float time;\
    uniform vec2 mouse;\
    uniform vec2 resolution;\
    \
    float getValue(vec2 p, float x2, float y2, float cmin, float cmax){\
        float x=p.x;\
        float y=p.y;\
        float theta=atan(y-y2, x+x2);\
        vec2 d=vec2(cos(theta * x2), sin(theta * cmin));\
        d *= abs(sin(time)) * (cmax-cmin) + cmin;\
        d += vec2(x2, y2) + length(p) * dot(y,x);\
        d *= vec2(y2, x2) * length(p * (time) * 0.1) * distance(y,x);\
        return length(d-p);\
    }\
    \
    void main( void ) {\
        vec2 position = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - vec2(1.0,resolution.y/resolution.x);\
        position = position * 2.0 ;\
        \
        float a = 0.0;\
        a += getValue(position, tan(time)*.77, sin(time)*.2, 5.3, 0.8);\
    	a *= getValue(position, cos(time)*.1, cos(-1.*time)*.1, 8.3, 0.18);\
        a += getValue(position, sin(time)*.8, cos(time)*.2, 4.3, 1.18);\
        a *= getValue(position, tan(time)*.3, cos(time)*.8, -1.3, 5.18);\
        a = 1./a*1.75;\
        \
        float b = 0.0;\
        b += getValue(position, cos(time)*.81, sin(time)*.2, 5.3, 0.8);\
    	b *= getValue(position, sin(time)*.1, tan(time)*.2, 1.3, 1.18);\
        b += getValue(position, tan(time)*.8, cos(time)*.01, 7.3, 1.18);\
        b *= getValue(position, cos(time)*.8, cos(time)*.2, -1.3, 5.18);\
        b = 1./b*1.75;\
        \
        float c = 0.0;\
        c += getValue(position, cos(time)*.41, cos(time)*.2, 5.3, 10.8);\
    	c *= getValue(position, sin(time)*.1, sin(time)*.1, 1.3, 1.18);\
        c += getValue(position, tan(time)*.8, tan(time)*.2, 4.3, 0.18);\
        c *= getValue(position, cos(time)*1.8, sin(time)*.8, -10.3, 2.18);\
        c = 1./c*1.75;\
        \
        gl_FragColor = vec4( a, b, c, 1.0 );\
    }";
    
    shader.allocate(640, 480);
}

//--------------------------------------------------------------
void testApp::update(){
    shader.update();
        
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    shader.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case '1':
            shader.injectShader(noise1);
            break;
        case '2':
            shader.injectShader(colorSpiral);
            break;
        case '3':
            shader.injectShader(coloredWaves);
            break;
        case '4':
            shader.injectShader(electro);
            break;
        case '5':
            shader.injectShader(noise2);
            break;
        case '6':
            shader.injectShader(waves);
            break;
        case '7':
            shader.injectShader(destello);
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}