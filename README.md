# ofxFX #
![ofxFlocking](http://patriciogonzalezvivo.com/images/flock.png) ![ofxFluid](http://patriciogonzalezvivo.com/images/fluid.png) ![ofxGlow](http://patriciogonzalezvivo.com/images/oldTv.png) ![ofxGrayScott](http://patriciogonzalezvivo.com/images/grayScott.png) ![ofxWater](http://patriciogonzalezvivo.com/images/water.png)

This addons originally born while I was working on [Efecto Mariposa](http://patriciogonzalezvivo.com/butterfly.html "Efecto Mariposa") and I need to do fast GPU image processing thought GLSL Shaders.

The main goal of this addon it´s to make super easy to use and edit shaders on-the-fly for adding them on your projects.

## ofxFXObject ##
It´s the parent class of all the other effect. If you want to make a new filter it´s better if you start looking at the ```ofxFXObject.h``` in order to see how it works. 

It have a simple structure:
1. Constructor: here it´s necessary to set three vital variables: 
    - passes: the number of passes or itineration of the main ping pong betweens FBO´s  
    - nTextures: number of textures used;
    - internalFormat: if it use GL_RGB, GL_RGBA, GL_RGB16f, GL_RGBA16f, GL_RGB32f, GL_RGBA32f, etc...
    
2. ```allocate(width,height)```: This usualy it´s no need to bee re-define. It´s basically allocate the FBO´s and loads the shader by using injectShader();

3. ```injectShader(string fragContent)```: here is where the shaders are loaded. See the example bellow.

4. ```begin(int texN)``` and ```end(int texN)```: remember nTextures variable? you can passthrough information to it by using this end() and begin() and giving the argument the number of texture you want to open. This texture can be access from the shader by the ```uniform sample2DRect tex0``` or ```tex1``` or ```tex2``` and so on. Also you can access to the previous FBO of the ping pong by calling ```uniform sample2DRect backbuffer```. 

5. ```update()```: this is the core of the class, where the magic happens. If you check  for the ```ofxFXObject::update()``` you will see how the tex´s, the backbuffer and other default uniforms variables (time, mouse, resolution) are loaded.

6. ```draw()```: after all you definitely want to look at it.


On setup:

```c++
ofxFXObject fxObject = ofxFXObject();

fxObject.allocate(800,600); // At this point it will load a default timer shader

//Let´s play a little injecting a new one
fxObject.injectShader("#version 120\n\
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
                    }"); 
```


On update:

```c++
fxObject.begin();
    //Waht ever you want to render to tex0
fxObject.end();

fxObject.begin(1);
    //Waht ever you want to render to tex1
fxObject.end(1);


fxObject.update();
```

On draw:

```c++
fxObject.draw();
```


Also in this class you can found some handy funtion as:

* ```renderFrame(width,height)``` makes a frame where the textures could be draw;
* ```initFbo(ofxFbo, width, height, internalFormat )```: it takes the work of allocating and cleaning an FBO.
* ```ofxSwapBuffer.h```: this is actually a class for making easy dealing with ping-pongs.

Most of the subClass that inherit from ofxFXObject that are part of this add-on are ones that in some point breaks this logic and structure and use just some elements and re-define new ones.

### Copy, Inject & Share ###
The fun part start here. Go to [Ricardo Caballero´s webGL Sandbox](http://mrdoob.com/projects/glsl_sandbox/) or [ShaderToy by Inigo Quilez](http://www.iquilezles.org/apps/shadertoy/) to found some inspiration. Explore by making changes. Inject it and compile. 
Don´t forget to share and preserve the authors credits.

Take this opportunity to thank the developers of both communities that their examples have been a wonderful motivation to learn about GLSL Shaders.

Note: May be you will need to make some changes in order to make it work with openGL version of openFrameworks

## ofxFXObject extensions ##
This are the subclases of ofxFXObject that share part of the structure and need more variables or shaders. This sometimes are complex or just it´s more handy to have them separated.

* ofxBloom
* ofxBlur
* ofxBLurFast
* ofxConway: life game made by [Kalwalt](http://www.kalwaltart.it/). It still needs some work
* ofxFLuid: fluid simulation based on [Mark Harris article from GPU Gems 1](http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html)
* ofxGlow
* ofxGrayScott: based on Reaction Diffusion by Gray-Scott Model from [here](http://mrob.com/pub/comp/xmorphia/)
* ofxMask: Based on ofxAlphaMaskShader reated by [James George](http://www.jamesgeorge.org) in collaboration with [FlightPhase](http://www.flightphase.com)
* ofxOldTv: on Postprocessing from [ShaderToy](http://www.iquilezles.org/apps/shadertoy/)
* ofxWater: water ripplies effect based on [Hugo Elias Tutorial](http://freespace.virgin.net/hugo.elias/graphics/x_water.htm)

# Examples #
Watch some of this classes in action [HERE](http://www.patriciogonzalezvivo.com/blog/?p=488)
