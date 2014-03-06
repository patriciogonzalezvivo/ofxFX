//
//  ofxNoise.h
//
//  Created by Patricio Gonzalez Vivo on 4/12/12.
//  Copyright (c) 2012 http://www.PatricioGonzalezVivo.com. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

#define STRINGIFY(A) #A

class ofxNoise : public ofxFXObject {
public:
    
    ofxNoise(){
        zoom = 0.5;
        speed = 0.5;
        internalFormat = GL_RGBA;
        
        fragmentShader = STRINGIFY(
                                   float permute(float x0,vec3 p) {
                                       float x1 = mod(x0 * p.y, p.x);
                                       return floor(  mod( (x1 + p.z) *x0, p.x ));
                                   }
                                   
                                   vec2 permute(vec2 x0,vec3 p){
                                       vec2 x1 = mod(x0 * p.y, p.x);
                                       return floor(  mod( (x1 + p.z) *x0, p.x ));
                                   }
                                   vec3 permute(vec3 x0,vec3 p){
                                       vec3 x1 = mod(x0 * p.y, p.x);
                                       return floor(  mod( (x1 + p.z) *x0, p.x ));
                                   }
                                   vec4 permute(vec4 x0,vec3 p){
                                       vec4 x1 = mod(x0 * p.y, p.x);
                                       return floor(  mod( (x1 + p.z) *x0, p.x ));
                                   }
                                   
                                   const vec4 pParam = vec4( 17.0*17.0, 34.0, 1.0, 7.0);
                                   
                                   float taylorInvSqrt(float r){
                                       return ( 0.83666002653408 + 0.7*0.85373472095314 - 0.85373472095314 * r );
                                   }
                                   
                                   float simplexNoise2(vec2 v){
                                       const vec2 C = vec2(0.211324865405187134,
                                                           0.366025403784438597);
                                       const vec3 D = vec3( 0., 0.5, 2.0) * 3.14159265358979312;
                                       
                                       vec2 i  = floor(v + dot(v, C.yy) );
                                       vec2 x0 = v -   i + dot(i, C.xx);
                                       
                                       vec2 i1  =  (x0.x > x0.y) ? vec2(1.,0.) : vec2(0.,1.) ;
                                       
                                       vec2 x1 = x0 - i1 + 1. * C.xx ;
                                       vec2 x2 = x0 - 1. + 2. * C.xx ;
                                       
                                       i = mod(i, pParam.x);
                                       vec3 p = permute( permute(
                                                                 i.y + vec3(0., i1.y, 1. ), pParam.xyz)
                                                        + i.x + vec3(0., i1.x, 1. ), pParam.xyz);
                                       
                                       vec3 x = fract(p / pParam.w) ;
                                       vec3 h = 0.5 - abs(x) ;
                                       
                                       vec3 sx = vec3(lessThan(x,D.xxx)) *2. -1.;
                                       vec3 sh = vec3(lessThan(h,D.xxx));
                                       
                                       vec3 a0 = x + sx*sh;
                                       vec2 p0 = vec2(a0.x,h.x);
                                       vec2 p1 = vec2(a0.y,h.y);
                                       vec2 p2 = vec2(a0.z,h.z);
                                       
                                       
                                       p0 *= taylorInvSqrt(dot(p0,p0));
                                       p1 *= taylorInvSqrt(dot(p1,p1));
                                       p2 *= taylorInvSqrt(dot(p2,p2));
                                       
                                       
                                       vec3 g = 2.0 * vec3( dot(p0, x0), dot(p1, x1), dot(p2, x2) );
                                       
                                       vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.);
                                       m = m*m ;
                                       return 1.66666* 70.*dot(m*m, g);
                                   }
                                   
                                   float simplexNoise3(vec3 v){
                                       const vec2  C = vec2(1./6. , 1./3. ) ;
                                       const vec4  D = vec4(0., 0.5, 1.0, 2.0);
                                       
                                       vec3 i  = floor(v + dot(v, C.yyy) );
                                       vec3 x0 =   v - i + dot(i, C.xxx) ;
                                       
                                       vec3 g = vec3( greaterThan(   x0.xyz, x0.yzx) );
                                       vec3 l = vec3( lessThanEqual( x0.xyz, x0.yzx) );
                                       
                                       vec3 i1 = g.xyz  * l.zxy;
                                       vec3 i2 = max( g.xyz, l.zxy);
                                       
                                       vec3 x1 = x0 - i1 + 1. * C.xxx;
                                       vec3 x2 = x0 - i2 + 2. * C.xxx;
                                       vec3 x3 = x0 - 1. + 3. * C.xxx;
                                       
                                       i = mod(i, pParam.x );
                                       vec4 p = permute( permute( permute(
                                                                          i.z + vec4(0., i1.z, i2.z, 1. ), pParam.xyz)
                                                                 + i.y + vec4(0., i1.y, i2.y, 1. ), pParam.xyz)
                                                        + i.x + vec4(0., i1.x, i2.x, 1. ), pParam.xyz);
                                       
                                       float n_ = 1.0/pParam.w ;
                                       vec3  ns = n_ * D.wyz - D.xzx ;
                                       
                                       vec4 j = p - pParam.w*pParam.w*floor(p * ns.z *ns.z);
                                       
                                       vec4 x_ = floor(j * ns.z)  ;
                                       vec4 y_ = floor(j - pParam.w * x_ );
                                       
                                       vec4 x = x_ *ns.x + ns.yyyy;
                                       vec4 y = y_ *ns.x + ns.yyyy;
                                       vec4 h = 1. - abs(x) - abs(y);
                                       
                                       vec4 b0 = vec4( x.xy, y.xy );
                                       vec4 b1 = vec4( x.zw, y.zw );
                                       
                                       vec4 s0 = vec4(lessThan(b0,D.xxxx)) *2. -1.;
                                       vec4 s1 = vec4(lessThan(b1,D.xxxx)) *2. -1.;
                                       vec4 sh = vec4(lessThan(h, D.xxxx));
                                       
                                       vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
                                       vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
                                       
                                       vec3 p0 = vec3(a0.xy,h.x);
                                       vec3 p1 = vec3(a0.zw,h.y);
                                       vec3 p2 = vec3(a1.xy,h.z);
                                       vec3 p3 = vec3(a1.zw,h.w);
                                       
                                       p0 *= taylorInvSqrt(dot(p0,p0));
                                       p1 *= taylorInvSqrt(dot(p1,p1));
                                       p2 *= taylorInvSqrt(dot(p2,p2));
                                       p3 *= taylorInvSqrt(dot(p3,p3));
                                       
                                       // Mix
                                       vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.);
                                       m = m * m;
                                       //used to be 64.
                                       return 48.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                                                    dot(p2,x2), dot(p3,x3) ) );
                                   }
                                   
                                   vec4 grad4(float j, vec4 ip){
                                       const vec4 ones = vec4(1.,1.,1.,-1.);
                                       vec4 p,s;
                                       
                                       p.xyz = floor( fract (vec3(j) * ip.xyz) *pParam.w) * ip.z -1.0;
                                       p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
                                       s = vec4(lessThan(p,vec4(0.)));
                                       p.xyz = p.xyz + (s.xyz*2.-1.) * s.www;
                                       
                                       return p;
                                   }
                                   
                                   float simplexNoise4(vec4 v){
                                       const vec2  C = vec2( 0.138196601125010504,
                                                            0.309016994374947451);
                                       
                                       vec4 i  = floor(v + dot(v, C.yyyy) );
                                       vec4 x0 = v -   i + dot(i, C.xxxx);
                                       
                                       vec4 q0 = floor(x0 * 1024.0) + vec4( 0., 1./4., 2./4. , 3./4.);
                                       vec4 q1;
                                       q1.xy = max(q0.xy,q0.zw);
                                       q1.zw = min(q0.xy,q0.zw);
                                       
                                       vec4 q2;
                                       q2.xz = max(q1.xz,q1.yw);
                                       q2.yw = min(q1.xz,q1.yw);
                                       
                                       vec4 q3;
                                       q3.y = max(q2.y,q2.z);
                                       q3.z = min(q2.y,q2.z);
                                       q3.xw = q2.xw;
                                       
                                       vec4 i1 = vec4(lessThanEqual(q3.xxxx, q0));
                                       vec4 i2 = vec4(lessThanEqual(q3.yyyy, q0));
                                       vec4 i3 = vec4(lessThanEqual(q3.zzzz, q0));
                                       
                                       vec4 x1 = x0 - i1 + 1. * C.xxxx;
                                       vec4 x2 = x0 - i2 + 2. * C.xxxx;
                                       vec4 x3 = x0 - i3 + 3. * C.xxxx;
                                       vec4 x4 = x0 - 1. + 4. * C.xxxx;
                                       
                                       i = mod(i, pParam.x );
                                       float j0 = permute( permute( permute( permute (
                                                                                      i.w, pParam.xyz) + i.z, pParam.xyz)
                                                                   + i.y, pParam.xyz) + i.x, pParam.xyz);
                                       vec4 j1 = permute( permute( permute( permute (
                                                                                     i.w + vec4(i1.w, i2.w, i3.w, 1. ), pParam.xyz)
                                                                           + i.z + vec4(i1.z, i2.z, i3.z, 1. ), pParam.xyz)
                                                                  + i.y + vec4(i1.y, i2.y, i3.y, 1. ), pParam.xyz)
                                                         + i.x + vec4(i1.x, i2.x, i3.x, 1. ), pParam.xyz);

                                       vec4 ip = pParam ;
                                       ip.xy *= pParam.w ;
                                       ip.x  *= pParam.w ;
                                       ip = vec4(1.,1.,1.,2.) / ip ;
                                       
                                       vec4 p0 = grad4(j0,   ip);
                                       vec4 p1 = grad4(j1.x, ip);
                                       vec4 p2 = grad4(j1.y, ip);
                                       vec4 p3 = grad4(j1.z, ip);
                                       vec4 p4 = grad4(j1.w, ip);
                                       
                                       p0 *= taylorInvSqrt(dot(p0,p0));
                                       p1 *= taylorInvSqrt(dot(p1,p1));
                                       p2 *= taylorInvSqrt(dot(p2,p2));
                                       p3 *= taylorInvSqrt(dot(p3,p3));
                                       p4 *= taylorInvSqrt(dot(p4,p4));
                                       
                                       vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.);
                                       vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.);
                                       m0 = m0 * m0;
                                       m1 = m1 * m1;
                                       return 32. * (dot(m0*m0, vec3(dot(p0, x0), dot(p1, x1), dot(p2, x2)))
                                                     + dot(m1*m1, vec2(dot(p3, x3), dot(p4, x4)))) ;
                                       
                                   }
                                   
                                   uniform float time;
                                   uniform float speed;
                                   
                                   vec3 noiseXY(vec2 _st; float _zoom, float _speed){
                                       vec2 v1 = _st;
                                       vec2 v2 = _st;
                                       vec2 v3 = _st;
                                       float expon = pow(10.0, _zoom*2.0);
                                       v1 /= 1.0*expon;
                                       v2 /= 0.62*expon;
                                       v3 /= 0.83*expon;
                                       float n = time*speed;
                                       float nr = (simplexNoise3(vec3(v1, n)) + simplexNoise3(vec3(v2, n)) + simplexNoise3(vec3(v3, n))) / 6.0 + 0.5;
                                       n = time * _speed + 1000.0;
                                       float ng = (simplexNoise3(vec3(v1, n)) + simplexNoise3(vec3(v2, n)) + simplexNoise3(vec3(v3, n))) / 6.0 + 0.5;
                                       n = time * _speed + 1000.0;
                                       float nb = (simplexNoise3(vec3(v1, n)) + simplexNoise3(vec3(v2, n)) + simplexNoise3(vec3(v3, n))) / 6.0 + 0.5;
                                       return vec3(nr,ng,nb);
                                   }
                                   
                                   void main (void){
                                       vec2 st = gl_TexCoord[0].st;
                                       gl_FragColor = noiseXY(st,zoom,speed);
                                   }
                                   );
    };
    
    void setZoom(float _zoom){zoom = _zoom;};
    
    void update(){
        
        pingPong.dst->begin();
        ofClear(0);
        shader.begin();
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("zoom", zoom);
        shader.setUniform1f("speed", speed);
        shader.setUniform2f("resolution", (float)width, (float)height);
        renderFrame();
        shader.end();
        pingPong.dst->end();
    };
    
    float   zoom, speed;
};