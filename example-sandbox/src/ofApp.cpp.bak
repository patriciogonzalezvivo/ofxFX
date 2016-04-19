#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    beat = 0.0;
    nFrag = -1;
    selection = -1;
    
    width = 640;
    height = 480;
    
    ofSetWindowShape(width, height);
    
    // Electrocardiogram Effect
    // Imported from http://www.geeks3d.com/20111223/shader-library-electrocardiogram-effect-glsl/
    fragsTitles[0] = " ";
    frags[0] = STRINGIFY(
    uniform float time;
    uniform vec2 mouse;
    uniform vec2 resolution;
    uniform sampler2DRect tex0;
    
    float viewAngle = 1.0;
    float speed = 9.0;
    float rate = 15.0;
    float baseamp = 0.10;
     
    void main(void){
        vec2 p = -1.0 + 3.0 * ( gl_FragCoord.xy / resolution.xy );
        p.y = 1.0 - p.y;
        float x = speed * viewAngle * time + rate * p.x;
        float base = (1.0 + cos(x * 2.5 + time)) * (1.0 + sin(x * 3.5 + time));
        float z = fract(0.05 * x);
        z = max(z, 1.0 - z);
        z = pow(z, 2.0);
        float pulse = exp(-10.0 * z);
        vec4 ecg_color = vec4(0.3, 1.0, 0.4, 1.0);
        vec4 c = pow(clamp(1.0 - abs(p.y - (baseamp * base + pulse - 0.5)), 0.0, 1.0), 16.0) * ecg_color;
        gl_FragColor = c;
    } );
    
    // COLORFULL WAVES
    fragsTitles[1] = " ";
    frags[1] = STRINGIFY(
    uniform float time;
    uniform vec2 mouse;
    uniform vec2 resolution;
    uniform sampler2DRect tex0;
    uniform sampler2DRect tex1;
    
    float pi = 3.141592653589793238462643383279;
                         
    void main( void ) {
        float tscale = 2.5;
        
        vec2 position = gl_FragCoord.xy / resolution.xy;
        
        float lum   = abs(tan(position.y * pi)) - pi/5.0;
        
        float red   = sin(position.x * 5.0 + time*tscale*1.00) * 2.0 - 1.0;
        float green = sin(position.x * 8.0 + time*tscale*1.33) * 2.0 - 1.0;
        float blue  = sin(position.x * 2.0 + time*tscale*1.93) * 2.0 - 1.0;
        
        gl_FragColor = vec4( vec3( lum + red, lum + green, lum + blue ), 1.0 );\
    } );
        
    // HYPNOTIC COLOR SPIRAL 
    fragsTitles[2] = "";
    frags[2] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         
                         void main( void ){
                             vec2 position = (gl_FragCoord.xy - resolution * 0.5) / resolution.yy;
                             float longest = sqrt(float(resolution.x*resolution.x) + float(resolution.y*resolution.y))*0.5;
                             float dx = gl_FragCoord.x-resolution.x/2.0;
                             float dy = 0.2+gl_FragCoord.y-resolution.y/2.0;
                             float len = sqrt(dx*dx+dy*dy);
                             float ds = len/longest;
                             float md = time*2.0;
                             
                             float ang = 2.0*atan(dy,(len+dx));
                             ang += pow(len, 0.5)*5.0;
                             
                             float red = (128.0 - sin(ang + md*3.141592*2.0) * 127.0)*(1.0-ds);
                             float green = (128.0 - cos(ang + md*3.141592*2.0) * 127.0)*(1.0-ds);
                             float blue = (128.0 + sin(ang  + md*3.141592*2.0) * 127.0)*(1.0-ds);
                             
                             gl_FragColor = vec4( vec3( red/255.0, green/255.0, blue/255.0), 1.0 );\
                         } 
                         );
    
    
    // An attempt at Quilez's warping (domain distortions):
    // http://iquilezles.org/www/articles/warp/warp.htm
    // 
    // Not as good as his, but still interesting.
    // @SyntopiaDK, 20123
    fragsTitles[3] = "By @SyntopaDK";
    frags[3] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         
                         float rand(vec2 co){
                             return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
                         }
                         
                         float noise2f( in vec2 p ){
                             vec2 ip = vec2(floor(p));
                             vec2 u = fract(p);
                             u = u*u*(3.0-2.0*u);
                             
                             float res = mix(mix(rand(ip),  
                                                 rand(ip+vec2(1.0,0.0)),u.x), 
                                             mix(rand(ip+vec2(0.0,1.0)),
                                                 rand(ip+vec2(1.0,1.0)),u.x),
                                             u.y);
                             return res*res;
                         }
                         
                         float fbm(vec2 c) {
                             float f = 0.0;
                             float w = 1.0;
                             
                             for (int i = 0; i < 8; i++) {
                                 f += w*noise2f(c);
                                 c *=2.0;
                                 w*=0.5;
                             }
                             return f;
                         }
                         
                         vec2 cMul(vec2 a, vec2 b) {
                             return vec2( a.x*b.x -  a.y*b.y,a.x*b.y + a.y * b.x);
                         }
                         
                         float pattern(  vec2 p, out vec2 q, out vec2 r ){
                             q.x = fbm( p  +0.00*time);
                             q.y = fbm( p + vec2(1.0));
                             
                             r.x = fbm( p +1.0*q + vec2(1.7,9.2)+0.15*time );
                             r.y = fbm( p+ 1.0*q + vec2(8.3,2.8)+0.126*time);
                             return fbm(p +1.0*r + 0.0* time);
                         }
                         
                         const vec3 color1 = vec3(0.101961,0.619608,0.666667);
                         const vec3 color2 = vec3(0.666667,0.666667,0.498039);
                         const vec3 color3 = vec3(0,0,0.164706);
                         const vec3 color4 = vec3(0.666667,1,1);
                         
                         void main() {
                             vec2 q;
                             vec2 r;
                             vec2 c = 1000.0 * gl_FragCoord.xy / resolution.xy;
                             float f = pattern( c * 0.01, q, r);
                             vec3 col = mix( color1, color2, clamp( (f*f) * 4.0,0.0,1.0));
                             col = color2;
                             col = mix(col,color3,clamp(length(q),0.0,1.0));
                             col = mix(col,color4,clamp(length(r.x),0.0,1.0));
                             gl_FragColor =  vec4((0.2*f*f*f+0.6*f*f+0.5*f)*col,1.0);
                         } 
                         );
    
    
    //
    // Description : Array and textureless GLSL 2D simplex noise function.
    //      Author : Ian McEwan, Ashima Arts.
    //  Maintainer : ijm
    //     Lastmod : 20110822 (ijm)
    //     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
    //               Distributed under the MIT License. See LICENSE file.
    //               https://github.com/ashima/webgl-noise
    // 
    fragsTitles[4] = "By Ian McEwan & Ashima Arts";
    frags[4] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         uniform sampler2D backbuffer;
                         
                         vec3 mod289(vec3 x) {
                             return x - floor(x * (1.0 / 289.0)) * 289.0;
                         }
                         
                         vec2 mod289(vec2 x) {
                             return x - floor(x * (1.0 / 289.0)) * 289.0;\
                         }
                         
                         vec3 permute(vec3 x) {
                             return mod289(((x*34.0)+1.0)*x);
                         }
                         
                         float snoise(vec2 v){
                             const vec4 C = vec4(0.211324865405187, 0.366025403784439,  -0.577350269189626, 0.024390243902439);
                             vec2 i  = floor(v + dot(v, C.yy) );
                             vec2 x0 = v -   i + dot(i, C.xx);
                             
                             vec2 i1;
                             
                             i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
                             
                             vec4 x12 = x0.xyxy + C.xxzz;
                             x12.xy -= i1;
                             
                             i = mod289(i);
                             vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));
                             
                             vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
                             m = m*m ;
                             m = m*m ;
                             
                             vec3 x = 2.0 * fract(p * C.www) - 1.0;
                             vec3 h = abs(x) - 0.5;
                             vec3 ox = floor(x + 0.5);
                             vec3 a0 = x - ox;
                             
                             m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
                             
                             vec3 g;
                             g.x  = a0.x  * x0.x  + h.x  * x0.y;
                             g.yz = a0.yz * x12.xz + h.yz * x12.yw;
                             return 130.0 * dot(m, g);
                         }
                         
                         void main( void ) {
                             vec4 buf = texture2D(backbuffer, (gl_FragCoord.xy / resolution.xy));
                             vec2 position = ( gl_FragCoord.xy + time + 100.0);
                             float color = snoise(position /1000.0 + snoise(vec2(time / 100.0, time / 100.0 + 100.))) * 0.9 + snoise(position / 10.0) * 0.3 + snoise(position / 1.0) * 0.2;
                             
                             gl_FragColor = vec4( vec3( color ), 1.0 );
                         } 
                         );
    
    
    fragsTitles[5] = "";
    frags[5] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         
                         float getValue(vec2 p, float x2, float y2, float cmin, float cmax){
                             float x=p.x;
                             float y=p.y;
                             float theta=atan(y-y2, x+x2);
                             vec2 d=vec2(cos(theta * x2), sin(theta * cmin));
                             d *= abs(sin(time)) * (cmax-cmin) + cmin;
                             d += vec2(x2, y2) + length(p) * dot(y,x);
                             d *= vec2(y2, x2) * length(p * (time) * 0.1) * distance(y,x);
                             return length(d-p);
                         }
                         
                         void main( void ) {
                             vec2 position = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - vec2(1.0,resolution.y/resolution.x);
                             position = position * 2.0 ;
                             
                             float a = 0.0;
                             a += getValue(position, tan(time)*.77, sin(time)*.2, 5.3, 0.8);
                             a *= getValue(position, cos(time)*.1, cos(-1.*time)*.1, 8.3, 0.18);
                             a += getValue(position, sin(time)*.8, cos(time)*.2, 4.3, 1.18);
                             a *= getValue(position, tan(time)*.3, cos(time)*.8, -1.3, 5.18);
                             a = 1./a*1.75;
                             
                             float b = 0.0;
                             b += getValue(position, cos(time)*.81, sin(time)*.2, 5.3, 0.8);
                             b *= getValue(position, sin(time)*.1, tan(time)*.2, 1.3, 1.18);
                             b += getValue(position, tan(time)*.8, cos(time)*.01, 7.3, 1.18);
                             b *= getValue(position, cos(time)*.8, cos(time)*.2, -1.3, 5.18);
                             b = 1./b*1.75;
                             
                             float c = 0.0;
                             c += getValue(position, cos(time)*.41, cos(time)*.2, 5.3, 10.8);
                             c *= getValue(position, sin(time)*.1, sin(time)*.1, 1.3, 1.18);
                             c += getValue(position, tan(time)*.8, tan(time)*.2, 4.3, 0.18);
                             c *= getValue(position, cos(time)*1.8, sin(time)*.8, -10.3, 2.18);
                             c = 1./c*1.75;
                             
                             gl_FragColor = vec4( a, b, c, 1.0 );
                         }
                         );
    
    // blobbies - by @P_Malin
    fragsTitles[6] = "By @P_Malin";
    frags[6] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         
                         float k_PI = 3.141592654;
                         float k_MoveSpeed = 0.5;
                         float k_BlobSize = 0.5;
                         float k_OrbitRadius = 0.5;
                         
                         vec2 GetRandom2( in vec2 vInput ){
                             vec2 temp1 = vInput * vec2(17.1232, 33.32432);
                             vec2 temp2 = vInput * vec2(34.32432, 12.9742);
                             vec2 temp3 = vec2(sin(temp1.x) + sin(temp2.y), sin(temp1.y) + sin(temp2.x)) * 121.1231;
                             return fract(temp3);
                         }
                         
                         vec2 GetOffset2( in vec2 vInput ){
                             return vec2(sin(vInput.x * k_PI * 2.0), cos(vInput.y * k_PI * 2.0));
                         }
                         
                         float BlobTexture( in vec2 vPos ){
                             float fLen = length(vPos) / k_BlobSize;
                             return max(1.0 - fLen, 0.0);
                         }
                         
                         float GetBlobValue( in vec2 vPosition, in vec2 vTileOffset ){
                             vec2 vTilePos = floor(vPosition - vTileOffset);
                             
                             vec2 vRandom = GetRandom2(vTilePos);
                             vec2 vTime2d = vec2(time, time * 0.92312) * k_MoveSpeed;
                             
                             vec2 vBlobPos = GetOffset2(vRandom + vTime2d) * k_OrbitRadius;
                             
                             vec2 vSubTilePos = fract(vPosition) + vTileOffset;\
                             
                             return BlobTexture(vSubTilePos - vBlobPos);
                         }
                         
                         vec3 GetColour( in vec2 vPixelPosition ){
                             vec2 vPosition = vPixelPosition;
                             
                             float fValue = 0.0;
                             
                             fValue += GetBlobValue( vPosition, vec2( 0.0, 0.0 ) );
                             fValue += GetBlobValue( vPosition, vec2( -1.0, 0.0 ) );
                             fValue += GetBlobValue( vPosition, vec2( 0.0, -1.0 ) );
                             fValue += GetBlobValue( vPosition, vec2( -1.0, -1.0 ) );
                             
                             fValue = max(fValue, 0.0);
                             fValue = 1.0 - abs(0.7 - fValue);
                             fValue = max(fValue, 0.0);
                             
                             fValue = pow(fValue, 1.5);
                             
                             vec3 cColA = vec3(1.0, 0., 0.4);
                             vec3 cColB = vec3(0., 0.0, 0.0);
                             
                             return cColA * fValue + cColB * (1.0 - fValue);
                         }
                         
                         void main( void ) {
                             vec3 vOutputColour = GetColour(gl_FragCoord.xy * ( mouse.x / 8.0 ));
                             
                             gl_FragColor = vec4( vOutputColour, 1.0 );
                         }
                         );
    
    // By @paulofalcao
    // Blobs with nice gradient
    fragsTitles[7] = "By @paulofalcao";
    frags[7] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         
                         float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){
                             float xx=x+sin(t*fx)*sx;
                             float yy=y+cos(t*fy)*sy;
                             return 1.0/sqrt(xx*xx+yy*yy);
                         }
                         
                         vec3 gu(vec4 a,vec4 b,float f){
                             return mix(a.xyz,b.xyz,(f-a.w)*(1.0/(b.w-a.w)));
                         }
                         
                         vec3 grad(float f){
                             vec4 c01=vec4(0.0,0.0,0.0,0.00);
                             vec4 c02=vec4(0.5,0.0,0.0,0.50);
                             vec4 c03=vec4(1.0,0.0,0.0,0.55);
                             vec4 c04=vec4(1.0,1.0,0.0,0.80);
                             vec4 c05=vec4(1.0,1.0,1.0,1.00);
                             return (f<c02.w)?gu(c01,c02,f):
                             (f<c03.w)?gu(c02,c03,f):
                             (f<c04.w)?gu(c03,c04,f):
                             gu(c04,c05,f);
                         }
                         
                         void main( void ) {
                             vec2 p=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);
                             
                             p=p*2.0;
                             
                             float x=p.x;
                             float y=p.y;
                             
                             float a=
                             makePoint(x,y,3.3,2.9,0.3,0.3,time);
                             a=a+makePoint(x,y,1.9,2.0,0.4,0.4,time);
                             a=a+makePoint(x,y,0.8,0.7,0.4,0.5,time);
                             a=a+makePoint(x,y,2.3,0.1,0.6,0.3,time);
                             a=a+makePoint(x,y,0.8,1.7,0.5,0.4,time);
                             a=a+makePoint(x,y,0.3,1.0,0.4,0.4,time);
                             a=a+makePoint(x,y,1.4,1.7,0.4,0.5,time);
                             a=a+makePoint(x,y,1.3,2.1,0.6,0.3,time);
                             a=a+makePoint(x,y,1.8,1.7,0.5,0.4,time);
                             a=a+makePoint(x,y,1.2,1.9,0.3,0.3,time);
                             a=a+makePoint(x,y,0.7,2.7,0.4,0.4,time);
                             a=a+makePoint(x,y,1.4,0.6,0.4,0.5,time);
                             a=a+makePoint(x,y,2.6,0.4,0.6,0.3,time);
                             a=a+makePoint(x,y,0.7,1.4,0.5,0.4,time);
                             a=a+makePoint(x,y,0.7,1.7,0.4,0.4,time);
                             a=a+makePoint(x,y,0.8,0.5,0.4,0.5,time);
                             a=a+makePoint(x,y,1.4,0.9,0.6,0.3,time);
                             a=a+makePoint(x,y,0.7,1.3,0.5,0.4,time);
                             a=a+makePoint(x,y,3.7,0.3,0.3,0.3,time);
                             a=a+makePoint(x,y,1.9,1.3,0.4,0.4,time);
                             a=a+makePoint(x,y,0.8,0.9,0.4,0.5,time);
                             a=a+makePoint(x,y,1.2,1.7,0.6,0.3,time);
                             a=a+makePoint(x,y,0.3,0.6,0.5,0.4,time);
                             a=a+makePoint(x,y,0.3,0.3,0.4,0.4,time);
                             a=a+makePoint(x,y,1.4,0.8,0.4,0.5,time);
                             a=a+makePoint(x,y,0.2,0.6,0.6,0.3,time);
                             a=a+makePoint(x,y,1.3,0.5,0.5,0.4,time);
                             
                             vec3 a1=grad(a/128.0);
                             
                             gl_FragColor = vec4(a1.x,a1.y,a1.z,1.0);
                         }
                         );
    
    
    
    // Sea Noise
    // By @paulofalcao
    // Simple raymarching sandbox with camera
    
    // Raymarching Distance Fields
    // About http://www.iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm
    // Also known as Sphere Tracing
    // ******** Ashima Noise3D Start https://github.com/ashima/webgl-noise/blob/master/src/noise3D.glsl
    
    // Util Start
    fragsTitles[8] = "By @paolofalcao";
    frags[8] = STRINGIFY(
                         
                         uniform vec2 resolution;
                         uniform float time;
                         uniform vec2 mouse;
                         
                         float PI=3.14159265;
                         
                         vec3 mod289(vec3 x){
                             return x - floor(x * (1.0 / 289.0)) * 289.0;
                         }
                         
                         vec4 mod289(vec4 x){
                             return x - floor(x * (1.0 / 289.0)) * 289.0;
                         }
                         
                         vec4 permute(vec4 x) {
                             return mod289(((x*34.0)+1.0)*x);
                         }
                         
                         vec4 taylorInvSqrt(vec4 r){
                             return 1.79284291400159 - 0.85373472095314 * r;
                         }
                         
                         float snoise(vec3 v){
                             const vec2  C = vec2(1.0 / 6.0, 1.0 / 3.0);
                             const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
                             
                             vec3 i  = floor(v + dot(v, C.yyy) );
                             vec3 x0 =   v - i + dot(i, C.xxx);
                             
                             vec3 g = step(x0.yzx, x0.xyz);
                             vec3 l = 1.0 - g;
                             vec3 i1 = min( g.xyz, l.zxy );
                             vec3 i2 = max( g.xyz, l.zxy );
                             
                             vec3 x1 = x0 - i1 + C.xxx;
                             vec3 x2 = x0 - i2 + C.yyy;
                             vec3 x3 = x0 - D.yyy;
                             
                             i = mod289(i);
                             
                             vec4 p = permute( permute( permute( 
                                                                i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
                                                       + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
                                              + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
                             
                             float n_ = 0.142857142857;
                             vec3  ns = n_ * D.wyz - D.xzx;
                             
                             vec4 j = p - 49.0 * floor(p * ns.z * ns.z);
                             
                             vec4 x_ = floor(j * ns.z);
                             vec4 y_ = floor(j - 7.0 * x_ );
                             
                             vec4 x = x_ *ns.x + ns.yyyy;
                             vec4 y = y_ *ns.x + ns.yyyy;
                             vec4 h = 1.0 - abs(x) - abs(y);
                             
                             vec4 b0 = vec4( x.xy, y.xy );
                             vec4 b1 = vec4( x.zw, y.zw );
                             
                             vec4 s0 = floor(b0)*2.0 + 1.0;
                             vec4 s1 = floor(b1)*2.0 + 1.0;
                             vec4 sh = -step(h, vec4(0.0));
                             
                             vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy ;
                             vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww ;
                             
                             vec3 p0 = vec3(a0.xy , h.x);
                             vec3 p1 = vec3(a0.zw , h.y);
                             vec3 p2 = vec3(a1.xy , h.z);
                             vec3 p3 = vec3(a1.zw , h.w);
                             
                             vec4 norm = taylorInvSqrt( vec4( dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3) ) );
                             
                             p0 *= norm.x;
                             p1 *= norm.y;
                             p2 *= norm.z;
                             p3 *= norm.w;
                             
                             vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
                             m = m * m;
                             return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
                         }
                         
                         float obj(in vec3 p){
                             float d=p.y;
                             p.y=p.y+time;
                             if (d<0.2)
                                 d=d-snoise(vec3(p*2.0))*0.15+0.05;
                             return d;
                         }
                         
                         vec3 obj_c(in vec3 p){
                             p.y = p.y+time;
                             float c = snoise(vec3(p*2.0))*0.5+0.5;
                             c=c*c*(3.0-2.0*c);
                             vec3 c3=mix(vec3(0,0.5,1),vec3(1,0,0),c);
                             return c3;
                         }
                         
                         void main(void){
                             vec2 vPos = gl_FragCoord.xy / resolution.xy;
                             vPos.y = 1.0 - vPos.y;
                             vPos = -1.0 + 2.0 * vPos;
                             
                             vec3 vuv = vec3(0,1,sin(time*.23)*0.2);
                             vec3 prp = vec3(sin(time*.53)*4.0,2,cos(time*.39)*4.0);
                             vec3 vrp = prp - vec3(cos(time*.42)*0.5,1.0,sin(time*.34)*0.5);
                             
                             vec3 vpn = normalize(vrp-prp);
                             vec3 u = normalize(cross(vuv,vpn));
                             vec3 v = cross(vpn,u);
                             vec3 vcv = (prp+vpn);
                             vec3 scrCoord = vcv + vPos.x * u * resolution.x / resolution.y + vPos.y * v;
                             vec3 scp = normalize(scrCoord-prp);
                             
                             const vec3 e=vec3(0.1,0,0);
                             const float maxd=15.0;
                             float s = 0.1;
                             
                             vec3 c;
                             vec3 p; 
                             vec3 n;
                             
                             float f = -prp.y / scp.y;
                             
                             if (f>0.0) p = prp + scp * f;
                             else f = maxd;
                             
                             for(int i=0;i<4;i++){
                                 f += s;
                                 p=prp+scp*f;
                                 s=obj(p);
                             }
                             
                             if (f<maxd){
                                 c=obj_c(p);
                                 n=normalize( vec3(s-obj(p-e.xyy),s-obj(p-e.yxy),s-obj(p-e.yyx) ) );
                                 float b = dot(n,normalize(prp-p));
                                 gl_FragColor= vec4((b*c+pow(b,16.0))*(1.0-f*.06),1.0);
                             } else 
                                 gl_FragColor=vec4(0,0,0,1);
                         }
                         );
    
    
    /* lame-ass tunnel by kusma */
    fragsTitles[9] = "By kusma";
    frags[9] = STRINGIFY(
                         uniform float time;
                         uniform vec2 mouse;
                         uniform vec2 resolution;
                         
                         void main( void ) {
                             vec2 position = (gl_FragCoord.xy - resolution * 0.5) / resolution.yy;
                             float th = atan(position.y, position.x) / (2.0 * 3.1415926) + 0.5 + mouse.x;
                             float dd = length(position);
                             float d = 0.25 / dd + time + mouse.y;
                             
                             vec3 uv = vec3(th + d, th - d, th + sin(d) * 0.1);
                             float a = 0.5 + cos(uv.x * 3.1415926 * 2.0) * 0.5;
                             float b = 0.5 + cos(uv.y * 3.1415926 * 2.0) * 0.5;
                             float c = 0.5 + cos(uv.z * 3.1415926 * 6.0) * 0.5;
                             vec3 color = mix(vec3(1.0, 0.8, 0.9), vec3(0.1, 0.1, 0.2), pow(a, 0.2)) * 3.;
                             color += mix(vec3(0.8, 0.9, 1.0), vec3(0.1, 0.1, 0.2),  pow(b, 0.1)) * 0.75;
                             color += mix(vec3(0.9, 0.8, 1.0), vec3(0.1, 0.2, 0.2),  pow(c, 0.1)) * 0.75;
                             gl_FragColor = vec4(color * clamp(dd, 0.0, 1.0), 1.0);
                         }
                         );
    
    
    // "bumpmapping" @Flexi23
    // tweaks, anony_gt
    fragsTitles[10] = "By @Flexi23 & anony_gt";
    frags[10] = STRINGIFY(
                          uniform float time;
                          uniform vec2 mouse;
                          uniform vec2 resolution;
                          
                          uniform sampler2D backbuffer;
                          
                          float orbitDistance = 0.000025;
                          float waveLength = 300.00;
                          
                          vec3 sim(vec3 p,float s);
                          vec2 rot(vec2 p,float r);
                          vec2 rotsim(vec2 p,float s);
                          
                          vec2 makeSymmetry(vec2 p){
                              vec2 ret=p;
                              ret.y=abs(ret.y);
                              ret=rotsim(ret,sin(time*0.1)*2.0+9.0);
                              ret.x=abs(ret.x);
                              return ret;
                          }
                          const float PI=3.14159265;
                          
                          vec3 sim(vec3 p,float s){
                              vec3 ret=p;
                              ret=p+s/2.0;
                              ret=fract(ret/s)*s-s/2.0;
                              return ret;
                          }
                          
                          vec2 rot(vec2 p,float r){
                              vec2 ret;
                              ret.x=p.x*cos(r)-p.y*sin(r);
                              ret.y=p.x*sin(r)+p.y*cos(r);
                              return ret;
                          }
                          
                          vec2 rotsim(vec2 p,float s){
                              vec2 ret=p;
                              ret=rot(p,-PI/(s*2.0));
                              ret=rot(p,floor(atan(ret.x,ret.y)/PI*s)*(PI/s));
                              return ret;
                          }
                          
                          void main( void ) {
                              vec2 uv = (gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);
                              uv=uv*.2;
                              
                              uv=makeSymmetry(uv);
                              
                              vec2 p1 = (vec2(sin(time), cos(time))*orbitDistance)+.1;
                              vec2 p2 = (vec2(sin(time+3.142), cos(time+3.142))*orbitDistance)+.125;
                              
                              float d1 = .1-length(uv +p1);
                              float d2 = .1-length(uv -p2);
                              
                              float wave1 = sin(d1*waveLength+(time*5.))*0.5 + 0.5 * (((d1 - 0.5) * .5) - .5);
                              float wave2 = sin(d2*waveLength+(time*5.))*0.5 + 0.5 * (((d1 - 0.5) * 1.5) + 1.5);
                              float c = d1 > 0.99 || d2 > 0.995 ? 1. : 0.;
                              c + wave1*wave2;
                              gl_FragColor = vec4(c + wave1*wave2,c,c,1.);
                              
                              vec2 d = 1./resolution;
                              float dx = texture2D(backbuffer, uv + vec2(-1.,0.)*d).x - texture2D(backbuffer, uv + vec2(1.,0.)*d).x ;
                              float dy = texture2D(backbuffer, uv + vec2(0.,-1.)*d).x - texture2D(backbuffer, uv + vec2(0.,1.)*d).x ;
                              d = vec2(dx,dy)*resolution/resolution.x*2.;
                              gl_FragColor.z = pow(clamp(1.-1.5*length(uv  - vec2(1.15*cos(time*.2)+.5,1.*sin(time*.1)+.5) + d),0.,1.),4.0);
                              gl_FragColor.y = gl_FragColor.z*1.5 + gl_FragColor.x*0.3;
                              
                              gl_FragColor *=10.25;
                          } 
                          );
    
    
    
    fragsTitles[11] = "";
    frags[11] = STRINGIFY(
                          uniform float time;
                          uniform vec2 mouse;
                          uniform vec2 resolution;
                          
                          void main( void ) {
                              vec2 position = ( gl_FragCoord.xy / resolution.xy ) + mouse / 4.0;
                              
                              float color = 0.0;
                              color += sin( position.x * cos( time / 197.0 ) * 80.0 ) + cos( position.y * cos( time / 150.0 ) * 10.0 );
                              color += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );
                              color += sin( position.x * sin( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
                              color *= sin( time / 10.0 ) * 0.5;
                              
                              gl_FragColor = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );
                              
                          } 
                          );
    
    // By @paulofalcao
    //
    // Some blobs modifications with symmetries
    fragsTitles[12] = "By @paulofalcao";
    frags[12] = STRINGIFY(
                          uniform float time;
                          uniform vec2 mouse;
                          uniform vec2 resolution;
                          
                          vec3 sim(vec3 p,float s);
                          vec2 rot(vec2 p,float r);
                          vec2 rotsim(vec2 p,float s);
                          
                          vec2 makeSymmetry(vec2 p){
                              vec2 ret=p;
                              ret.y=abs(ret.y);
                              ret=rotsim(ret,sin(time*0.3)*2.0+3.0);
                              ret.x=abs(ret.x);
                              return ret;
                          }
                          
                          float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t){
                              float xx=x+tan(t*fx)*sx;
                              float yy=y-tan(t*fy)*sy;
                              return 0.5/sqrt(abs(x*xx+yy*yy));
                          }
                          
                          const float PI=3.14159265;
                          
                          vec3 sim(vec3 p,float s){
                              vec3 ret=p;
                              ret=p+s/2.0;
                              ret=fract(ret/s)*s-s/2.0;
                              return ret;
                          }
                          
                          vec2 rot(vec2 p,float r){
                              vec2 ret;
                              ret.x=p.x*cos(r)-p.y*sin(r);
                              ret.y=p.x*sin(r)+p.y*cos(r);
                              return ret;
                          }
                          
                          vec2 rotsim(vec2 p,float s){
                              vec2 ret=p;
                              ret=rot(p,-PI/(s*2.0));
                              ret=rot(p,floor(atan(ret.x,ret.y)/PI*s)*(PI/s));
                              return ret;
                          }
                          
                          void main( void ) {
                              vec2 p=(gl_FragCoord.xy/resolution.x)*2.0-vec2(1.0,resolution.y/resolution.x);
                              p=p*2.0;
                              p=makeSymmetry(p);
                              
                              float x=p.x;
                              float y=p.y;
                              
                              float t=time*0.5;
                              
                              float a=
                              makePoint(x,y,3.3,2.9,0.3,0.3,t);
                              a=a+makePoint(x,y,1.9,2.0,0.4,0.4,t);
                              a=a+makePoint(x,y,0.8,0.7,0.4,0.5,t);
                              a=a+makePoint(x,y,2.3,0.1,0.6,0.3,t);
                              a=a+makePoint(x,y,0.8,1.7,0.5,0.4,t);
                              a=a+makePoint(x,y,0.3,1.0,0.4,0.4,t);
                              a=a+makePoint(x,y,1.4,1.7,0.4,0.5,t);
                              a=a+makePoint(x,y,1.3,2.1,0.6,0.3,t);
                              a=a+makePoint(x,y,1.8,1.7,0.5,0.4,t);
                              
                              float b=
                              makePoint(x,y,1.2,1.9,0.3,0.3,t);
                              b=b+makePoint(x,y,0.7,2.7,0.4,0.4,t);
                              b=b+makePoint(x,y,1.4,0.6,0.4,0.5,t);
                              b=b+makePoint(x,y,2.6,0.4,0.6,0.3,t);
                              b=b+makePoint(x,y,0.7,1.4,0.5,0.4,t);
                              b=b+makePoint(x,y,0.7,1.7,0.4,0.4,t);
                              b=b+makePoint(x,y,0.8,0.5,0.4,0.5,t);
                              b=b+makePoint(x,y,1.4,0.9,0.6,0.3,t);
                              b=b+makePoint(x,y,0.7,1.3,0.5,0.4,t);
                              
                              float c=
                              makePoint(x,y,3.7,0.3,0.3,0.3,t);
                              c=c+makePoint(x,y,1.9,1.3,0.4,0.4,t);
                              c=c+makePoint(x,y,0.8,0.9,0.4,0.5,t);
                              c=c+makePoint(x,y,1.2,1.7,0.6,0.3,t);
                              c=c+makePoint(x,y,0.3,0.6,0.5,0.4,t);
                              c=c+makePoint(x,y,0.3,0.3,0.4,0.4,t);
                              c=c+makePoint(x,y,1.4,0.8,0.4,0.5,t);
                              c=c+makePoint(x,y,0.2,0.6,0.6,0.3,t);
                              c=c+makePoint(x,y,1.3,0.5,0.5,0.4,t);
                              
                              vec3 d=vec3(a,b,c)/32.0;
                              
                              gl_FragColor = vec4(d.x,d.y,d.z,1.0);
                          } 
                          );
    
    fragsTitles[13] = "";
    frags[13] = "uniform vec2 resolution;\
    uniform float time;\
    \
    void main(void){\
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;\
    float a = time*40.0;\
    float d,e,f,g=1.0/40.0,h,i,r,q;\
    e=400.0*(p.x*0.5+0.5);\
    f=400.0*(p.y*0.5+0.5);\
    i=200.0+sin(e*g+a/150.0)*20.0;\
    d=200.0+cos(f*g/2.0)*18.0+cos(e*g)*7.0;\
    r=sqrt(pow(i-e,2.0)+pow(d-f,2.0));\
    q=f/r;\
    e=(r*cos(q))-a/2.0;f=(r*sin(q))-a/2.0;\
    d=sin(e*g)*176.0+sin(e*g)*164.0+r;\
    h=((f+d)+a/2.0)*g;\
    i=cos(h+r*p.x/1.3)*(e+e+a)+cos(q*g*6.0)*(r+h/3.0);\
    h=sin(f*g)*144.0-sin(e*g)*212.0*p.x;\
    h=(h+(f-e)*q+sin(r-(a+h)/7.0)*10.0+i/4.0)*g;\
    i+=cos(h*2.3*sin(a/350.0-q))*184.0*sin(q-(r*4.3+a/12.0)*g)+tan(r*g+h)*184.0*cos(r*g+h);\
    i=mod(i/5.6,256.0)/64.0;\
    if(i<0.0) i+=4.0;\
    if(i>=2.0) i=4.0-i;\
    d=r/350.0;\
    d+=sin(d*d*8.0)*0.52;\
    f=(sin(a*g)+1.0)/2.0;\
    gl_FragColor=vec4(vec3(f*i/1.6,i/2.0+d/13.0,i)*d*p.x+vec3(i/1.3+d/8.0,i/2.0+d/18.0,i)*d*(1.0-p.x),1.0);\
    }";
    
    fragsTitles[14] = "";
    frags[14] = STRINGIFY(
                          uniform vec2 resolution;
                          uniform float time;
                          
                          void main(void){
                              float x = gl_FragCoord.x;
                              float y = gl_FragCoord.y;
                              float mov0 = x+y+cos(sin(time)*2.)*100.+sin(x/100.)*1000.;
                              float mov1 = y / resolution.y / 0.2 + time;
                              float mov2 = x / resolution.x / 0.2;
                              float c1 = abs(sin(mov1+time)/2.+mov2/2.-mov1-mov2+time);
                              float c2 = abs(sin(c1+sin(mov0/1000.+time)+sin(y/40.+time)+sin((x+y)/100.)*3.));
                              float c3 = abs(sin(c2+cos(mov1+mov2+c2)+cos(mov2)+sin(x/1000.)));
                              gl_FragColor = vec4( c1,c2,c3,1.0);
                          } 
                          );
    
    fragsTitles[15] = "";
    frags[15] = STRINGIFY(
                          uniform float time;
                          uniform vec2 mouse;
                          uniform vec2 resolution;
                          
                          vec3 mod289(vec3 x) {
                              return x - floor(x * (1.0 / 289.0)) * 289.0;
                          }
                          
                          vec2 mod289(vec2 x) {
                              return x - floor(x * (1.0 / 289.0)) * 289.0;
                          }
                          
                          vec3 permute(vec3 x) {
                              return mod289(((x*34.0)+1.0)*x);
                          }
                          
                          float snoise(vec2 v){
                              const vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
                              
                              vec2 i  = floor(v + dot(v, C.yy) );
                              vec2 x0 = v -   i + dot(i, C.xx);
                              
                              vec2 i1;
                              
                              i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
                              
                              vec4 x12 = x0.xyxy + C.xxzz;
                              x12.xy -= i1;
                              
                              i = mod289(i);
                              vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 )) + i.x + vec3(0.0, i1.x, 1.0 ));
                              
                              vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
                              m = m*m ;
                              m = m*m ;
                              
                              vec3 x = 2.0 * fract(p * C.www) - 1.0;
                              vec3 h = abs(x) - 0.5;
                              vec3 ox = floor(x + 0.5);
                              vec3 a0 = x - ox;
                              
                              m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
                              
                              vec3 g;
                              g.x  = a0.x  * x0.x  + h.x  * x0.y;
                              g.yz = a0.yz * x12.xz + h.yz * x12.yw;
                              return 130.0 * dot(m, g);
                          }
                          
                          void main( void ) {
                              vec2 p = ( gl_FragCoord.xy / resolution.xy );
                              p = (p * 2.0 - 1.0) * vec2(800, 600);
                              p += mouse * 8196.0;
                              float z = 84.0;
                              
                              float v = snoise(p * 0.002 + z + 0.05 * time);  p += 1017.0;
                              v += 0.5 * snoise(p * 0.004 - 0.2 * time);
                              v += 0.125 * snoise(p * 0.008 + 0.0001 * v * time);
                              v = 0.15 * v + 0.7;
                              
                              gl_FragColor = vec4(v);
                          } 
                          );
    
    fragsTitles[16] = "";
    frags[16] = STRINGIFY(
                          const float pi = 3.141592653589793238462643383279;
                          
                          uniform float time;
                          uniform vec2 mouse;
                          uniform vec2 resolution;
                          
                          // How fast it animates
                          float tscale = 1.5;
                          
                          float wave(vec2 position, float freq, float height, float speed) {
                              float result = sin(position.x*freq - time*tscale*speed);
                              result = result * 2.0 - 1.0;
                              result *= height;
                              return result;
                          }
                          
                          vec3 combo(vec2 position, float center, float size) {
                              
                              float offset = pi * (center - 0.5);
                              float lum   = abs(tan(position.y * pi + offset)) - pi/2.0;
                              lum *= size;
                              
                              float red   = wave(position, 5.0, 0.9*size, 1.008);
                              float green = wave(position, 3.5, 0.5*size, 2.23);
                              float blue  = wave(position, 1.5, 1.2*size, 1.42);
                              
                              return vec3( lum + red, lum + green, lum + blue );
                          }
                          
                          void main( void ) {
                              // normalize position
                              vec2 position = gl_FragCoord.xy / resolution.xy;
                              
                              vec3 result = vec3(0.0, 0.0, 0.0);
                              result += combo(position, 0.1, 0.05);
                              result += combo(position, 0.5, 0.25);
                              result += combo(position, 0.9, 0.05);
                              
                              gl_FragColor = vec4(result, 1.0);
                              
                          }
                          );
    
    
    //Simple Voronoi fracture map generator  
    //based on previous folk's work;
    
    // Modified to work at different resolutions (up to 4x)
    
    // Cellular noise ("Worley noise") in 2D in GLSL.
    // Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
    // This code is released under the conditions of the MIT license.
    // See LICENSE file for details, located in ZIP file here:
    // http://webstaff.itn.liu.se/~stegu/GLSL-cellular/
    
    // Permutation polynomial: (34x^2 + x) mod 289
    fragsTitles[17] = "By Stefan Gustavson";
    frags[17] = STRINGIFY(
                          const float FracMapScale = 32.0;
                          const float jitter = 0.76;
                          const float FracMapJitter = 0.76; //0.0 is a grid, >0.8 is bad edges
                          const float K = 0.142857142857; // 1/7
                          const float Ko = 0.428571428571; // 3/7
                          
                          uniform float time;
                          uniform vec2 mouse;
                          uniform vec2 resolution;
                          
                          vec3 permute(vec3 x) {
                              return mod((34.0 * x + 1.0) * x, 560.0);
                          }
                          
                          // Cellular noise, returning F1 and F2 in a vec2.
                          // Standard 3x3 search window for good F1 and F2 values
                          vec2 cellular(vec2 P) {
                              vec2 Pi = mod(floor(P), 289.0);
                              vec2 Pf = fract(P);
                              vec3 oi = vec3(-1.0, 0.0, 1.0);
                              vec3 of = vec3(-0.5, 0.5, 1.5);
                              vec3 px = permute(Pi.x + oi);
                              vec3 p = permute(px.x + Pi.y + oi); // p11, p12, p13
                              vec3 ox = fract(p*K) - Ko;
                              vec3 oy = mod(floor(p*K),7.0)*K - Ko;
                              vec3 dx = Pf.x + 0.5 + jitter*ox;
                              vec3 dy = Pf.y - of + jitter*oy;
                              vec3 d1 = dx * dx + dy * dy; // d11, d12 and d13, squared
                              p = permute(px.y + Pi.y + oi); // p21, p22, p23
                              ox = fract(p*K) - Ko;
                              oy = mod(floor(p*K),7.0)*K - Ko;
                              dx = Pf.x - 0.5 + jitter*ox;
                              dy = Pf.y - of + jitter*oy;
                              vec3 d2 = dx * dx + dy * dy; // d21, d22 and d23, squared
                              p = permute(px.z + Pi.y + oi); // p31, p32, p33
                              ox = fract(p*K) - Ko;
                              oy = mod(floor(p*K),7.0)*K - Ko;
                              dx = Pf.x - 1.5 + jitter*ox;
                              dy = Pf.y - of + jitter*oy;
                              vec3 d3 = dx * dx + dy * dy; // d31, d32 and d33, squared
                              // Sort out the two smallest distances (F1, F2)
                              vec3 d1a = min(d1, d2);
                              d2 = max(d1, d2); // Swap to keep candidates for F2
                              d2 = min(d2, d3); // neither F1 nor F2 are now in d3
                              d1 = min(d1a, d2); // F1 is now in d1
                              d2 = max(d1a, d2); // Swap to keep candidates for F2
                              d1.xy = (d1.x < d1.y) ? d1.xy : d1.yx; // Swap if smaller
                              d1.xz = (d1.x < d1.z) ? d1.xz : d1.zx; // F1 is in d1.x
                              d1.yz = min(d1.yz, d2.yz); // F2 is now not in d2.yz
                              d1.y = min(d1.y, d1.z); // nor in  d1.z
                              d1.y = min(d1.y, d2.x); // F2 is in d1.y, we're done.
                              return sqrt(d1.xy);
                          }
                          
                          void main( void ) {
                              float FracMapLineWidth = FracMapScale / resolution.x * 0.7;
                              float FracMapLineAA = FracMapScale / resolution.x * 1.5;
                              vec2 position = ((( gl_FragCoord.xy / resolution.x ) + mouse * 2.0) - 1.0) * FracMapScale;
                              vec2 F = cellular(position);
                              float lines = 1.0 - smoothstep(FracMapLineWidth, FracMapLineWidth + FracMapLineAA, abs(F.y-F.x));
                              gl_FragColor = vec4(lines * 0.7, lines * 0.5, lines * 0.2, 1.0);
                          }
                          );
    
    fragsTitles[18] = "";
    frags[18] = STRINGIFY(
                          const float pi2_inv = 0.159154943091895335768883763372;
                          
                          uniform float time;
                          uniform vec2 resolution;
                          uniform vec2 mouse;
                          
                          float border(vec2 uv, float thickness){
                              uv = fract(uv - vec2(0.5));
                              uv = min(uv, vec2(1.)-uv)*2.;
                              //	return 1.-length(uv-0.5)/thickness;
                              return clamp(max(uv.x,uv.y)-1.+thickness,0.,1.)/thickness;;
                          }
                          
                          vec2 div(vec2 numerator, vec2 denominator){
                              return vec2( numerator.x*denominator.x + numerator.y*denominator.y,
                                          numerator.y*denominator.x - numerator.x*denominator.y)/
                              vec2(denominator.x*denominator.x + denominator.y*denominator.y);
                          }
                          
                          vec2 spiralzoom(vec2 domain, vec2 center, float n, float spiral_factor, float zoom_factor, vec2 pos){
                              vec2 uv = domain - center;
                              float d = length(uv);
                              return vec2( atan(uv.y, mouse.x)*n*pi2_inv/sin(mouse.y) + log(d)*spiral_factor, -log(d)*zoom_factor) + pos;
                          }
                          
                          void main( void ) {
                              vec2 uv = gl_FragCoord.xy / resolution.xy;
                              uv = 0.5 + (uv - 0.5)*vec2(resolution.x/resolution.y,1.);
                              
                              vec2 p1 = vec2(0.2,0.5);
                              vec2 p2 = vec2(0.8, 0.5);
                              
                              vec2 moebius = div(uv-p1, uv-p2);
                              
                              uv = uv-0.5;
                              
                              vec2 spiral_uv = spiralzoom(moebius,vec2(0.),8.,-.5,1.8,vec2(0.5,0.5)*time*0.5);
                              vec2 spiral_uv2 = spiralzoom(moebius,vec2(0.),3.,.9,1.2,vec2(-0.5,0.5)*time*.8);
                              vec2 spiral_uv3 = spiralzoom(moebius,vec2(0.),5.,.75,4.0,-vec2(0.5,0.5)*time*.7);
                              
                              gl_FragColor = vec4(border(spiral_uv,0.9), border(spiral_uv2,0.9) ,border(spiral_uv3,0.9),1.);
                              
                              vec2 weed_uv = (uv);
                              weed_uv.y += 5.38;
                              float w = 0.05 * mouse.y;
                              float r = 0.33 * mouse.x + 0.1;
                              float o = atan(weed_uv.y, weed_uv.x);
                              r *= (1.+sin(o+(time*0.09)))*(1.+0.9 * cos(8.*o))*(1.+0.1*cos(24.*o))*(0.9+0.05*cos(200.*o));
                              float l = length(weed_uv);
                              
                              float d = clamp(1.-abs(l - r + w*2.)/w, 0., 1.);
                              
                              gl_FragColor *= 1.-d;
                          }
                          );
    
    sandbox.allocate(width,height,GL_RGB);
    
    bloom.allocate(width, height);
    blur.allocate(width, height);
    bokeh.allocate(width, height);
    glow.allocate(width, height);
    gaussianBlur.allocate(width, height);
    oldtv.allocate(width,height);
}

//--------------------------------------------------------------
void testApp::update(){
    sandbox.update();
    
    if ( selection == 0 ){
        bloom << sandbox;
        bloom.update();
    } else if ( selection == 1 ){
        gaussianBlur.setRadius(sin(beat)*10);
        gaussianBlur << sandbox;
        gaussianBlur.update();
    } else if ( selection == 2 ){
        blur.setFade(sin(beat));
        blur << sandbox;
        blur.update();
    } else if ( selection == 3 ){
        bokeh.setRadius(abs(sin(beat)*5));
        bokeh << sandbox;
        bokeh.update();
    } else if ( selection == 4 ){
        glow.setRadius(sin(beat)*15);
        glow << sandbox;
        glow.update();
    } else if ( selection == 5 ){
        oldtv << sandbox;
        oldtv.update();
    } 
    
    beat += (1.0/ofGetFrameRate())*2;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA );
    
    ofSetColor(255);
    //sandbox.draw();
    
    string title = "";
    if (nFrag != -1)
        title += fragsTitles[ nFrag ];
    else
        title += " Default ofxFXObject timer shader ";
    
    if ( selection == 0 ){
        bloom.draw();
        title += " with bloom filter";
        ofDrawBitmapString("ofxBloom" , 15, 15);
    } else if ( selection == 1 ){
        gaussianBlur.draw();
        title += " width Gaussian blur filter (" + ofToString(sin(beat)*10,1) + " )";
        ofDrawBitmapString("ofxGaussinaBlur.setRadius( " + ofToString(sin(beat)*10,1) + " )", 15, 15);
    } else if ( selection == 2 ){
        blur.draw();
        title += " width blur filter (" + ofToString(sin(beat),1) + " )";
        ofDrawBitmapString("ofxBlur.setFade( " + ofToString(sin(beat),1) + " )", 15, 15);
    } else if ( selection == 3 ){
        bokeh.draw();
        title += " width bokeh filter ( "+ ofToString( int(abs(sin(beat)*5))) + " )";
        ofDrawBitmapString("ofxBokeh.setRadius( "+ ofToString( int(abs(sin(beat)*5))) + " )", 15, 15);
    } else if ( selection == 4 ){
        glow.draw();
        title +=  " width glow filter ( "+ ofToString(sin(beat)*15,1)+ " )";
        ofDrawBitmapString("ofxGlow.setRadius( "+ ofToString(sin(beat)*15,1) + " )", 15, 15);
    } else if ( selection == 5 ){
        oldtv.draw();
        title += " width oldtv filter";
        ofDrawBitmapString("ofxOld-tv", 15, 15);
    } else {
        sandbox.draw();
    }
     
    ofPopStyle();
    
    title += " runing at " + ofToString(ofGetFrameRate(),1) + " fps";
    ofSetWindowTitle(title);
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case OF_KEY_UP:
            nFrag++;
            break;
        case OF_KEY_DOWN:
            nFrag--;
            break;
        case OF_KEY_RIGHT:
            selection++;
            break;
        case OF_KEY_LEFT:
            selection--;
            break;
    }
    
    if (nFrag >= nTotalFrag)
        nFrag = 0;
    else if ( nFrag < 0)
        nFrag = nTotalFrag - 1;
    
    sandbox.setCode( frags[nFrag] );
    
    if (selection >= 6)
        selection = -1;
    else if ( selection < -1)
        selection = 5;
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
