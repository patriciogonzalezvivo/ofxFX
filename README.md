# [ofxFX](http://patriciogonzalezvivo.com/2011/ofxfx/)
[ ![ofxFlocking](http://patriciogonzalezvivo.com/2011/ofxfx/flock.png) ![ofxFluid](http://patriciogonzalezvivo.com/2011/ofxfx/fluid.png) ![ofxGlow](http://patriciogonzalezvivo.com/2011/ofxfx/oldTv.png) ![ofxGrayScott](http://patriciogonzalezvivo.com/2011/ofxfx/grayScott.png) ![ofxWater](http://patriciogonzalezvivo.com/2011/ofxfx/water.png)](http://patriciogonzalezvivo.com/2011/ofxfx/)

This addon begins while I was working on [Efecto Mariposa](http://patriciogonzalezvivo.com/butterfly.html "Efecto Mariposa") when I was to adapting different well know GLSL Shaders Algorithms for making fast simulations and organic effects. After that project I decide to make an addon that allow easy implementation of different effects that could be render super fast on the GPU.
All of this effect heritage from ofxFXObject. Making easy to combine them and create new ones.

## ofxFXObject ##
It´s the parent class of all the other effect. If you want to make a new filter you may want to start looking at the ofxFXObject.h .

The structure it´s easy.

1. Constructor: here it´s necessary to set three vital variables: 
    - ```int passes```: the number of passes or itineration of the main ping pong betweens FBO´s  
    - ```int internalFormat```: if it use GL_RGB, GL_RGBA, GL_RGB16f, GL_RGBA16f, GL_RGB32f, GL_RGBA32f, etc...
    - ```string fragShader```: it´s the code of the shader. Note that some changes have to be made in order to fill everything on a string
    
2. ```allocate(width,height,GL_RGBA)```: This usually it´s no need to be re-define. It´s basically allocate the FBO´s and loads the shader by using injectShader();

3. ```setCode(string fragContent)```: here is where the shaders are loaded. See the example below.

4. ```begin(int texN)``` and ```end(int texN)```: remember nTextures variable? you can passthrough information to it by using this end() and begin() and giving the argument the number of texture you want to open. This texture can be access from the shader by the ```uniform sample2DRect tex0``` or ```tex1``` or ```tex2``` and so on. 

    4.1. ```<<``` and ```setTexture(texture-like, num=0)``` if you have an FBO, a texture, an ofxFXObject, or something similar, you can use this function to write it to ```tex0```, ```tex1```, etc (as indicated by the num argument). ```<<<``` always writes to ```tex0```.

    4.2. If you're making a multi-pass shader, you can access to the previously rendered frame with ```uniform sample2DRect backbuffer``` in your shader. This will also contain the information from ```tex0``` on the first pass.

5. ```update()```: This is the core of the class, where the magic happens. If you check  for the ```ofxFXObject::update()``` you will see how the tex's, the backbuffer and other default uniforms variables (time, mouse, resolution) are loaded.

6. ```draw(x,y,w,h)```: After all you definitely want to look at it.

An example of an implementation of an ofxFXObject could be:

On setup:

```c++
ofxFXObject fxObject = ofxFXObject();

fxObject.allocate(800,600); // At this point it will load a default timer shader

//Let´s play a little injecting a new one
fxObject.setCode("#version 120\n\
                    \
                    uniform sampler2DRect backbuffer;\
                    uniform sampler2DRect tex0;\
                    uniform sampler2DRect tex1;\
                    uniform sampler2DRect tex2;\
                    \
                    uniform float time; \
                    uniform vec2 mouse;\
                    uniform vec2 resolution;\
                    \
                    void main( void ) {\
                        \
                        ... \
                        \
                        gl_FragColor = ... ;\
                    }"); 
```

Also look at the functions load, loadVersioned, and the different shader strings
to see how you can specify your shader code. You can also see how this is used
to easily load OpenGL2 and OpenGL3 shaders correctly if you look inside the
test-shader-load example. Note that if you don't specify any Vertex Shader for
OpenGL3, a simple pass-through shader will be added for you. :)

On update:

```c++
fxObject.begin();   // for tex0 you don´t need to pass the 0 as an argument 
    //What ever you want to render to tex0
fxObject.end();

fxObject.begin(1);
    //What ever you want to render to tex1
fxObject.end(1);

fxObject.setTexture(image.getTextureReference(),2); // For object that have ofTexture you could just pass the ofTexture reference 


fxObject.update();
```

On draw:

```c++
fxObject.draw();
``` 

### Copy, Inject & Share ###
If you are curious and want to learn the best way it´s to see and edit code. You could go to [Ricardo Caballero´s webGL Sandbox](http://mrdoob.com/projects/glsl_sandbox/) or [Inigo Quilez´s ShaderToy](http://www.iquilezles.org/apps/shadertoy/) to found some inspiration. Explore. Make changes. Inject. Compile.
May be it´s the case things not compile as you spect. Some times (most of them when you are dealing with other textures) you will need to make some changes. As far as I know openGL and GPU hardware it´s makes lot´s of changes and improves year after year. So lot´s of problems related to compatibility will happened. Also openFrameworks works fine with openGL 1.2 and it use by default the ARB Rect Textures. 

Having that in mind the process of adapting code it´s a matter of searching on google. Fortunately you can see the ones I already implement and make them work.

General tips:

* Casting: openGL it will not cast for you. So if you assign an int to a float it will not compile properly. 

```c++

float f = 1;    // FAIL 
float f = 1.0;  // GOOD

``` 

* Norm coordinates: as far as I know there are two types of textures sample2D and sample2DRect. The first ones have the same length in both sides while the second one not. When you are using texture2D( tex, pos) the position have to be normalized while on texture2DRect(tex, pos) it´s not normalized. 


### Mix them all ###

ofxFXObject have some handy operators that let you combine them really easily. 
At the Mix´s Example you will find how to do it. Basicaly it´s somethin like this

```c++

ofxFXObject objA;
ofxGrayScott objB;
ofxBlur     blur;

objA.allocate(width, height);
objB.allocate(width, height);
blur.allocate(width, height);

...

objA >> objB >> blur;

objA.update();
objB.update();
blur.update();

...

blur.draw();

``` 


## ofxFXObject subclasses ##

If you are thinking on making your own filter or generative shader. You may want to look at the ofxFXObject parent class. There you will find some handy function that could help you. 

* ```renderFrame(width,height)``` makes a frame where the textures could be draw;

* ```initFbo(ofxFbo, width, height, internalFormat )```: it takes the work of allocating and cleaning an FBO.

* ```ofxSwapBuffer.h```: this is actually a class for making easy dealing with ping-pongs.

In the src/ directory of the addon you will find lot´s of subClasses that inherit from ofxFXObject. They are decided in difference categories.

### Interactive

First case, the ones that in some point breaks the  structure of ofxFXObject with some extra tweaks. Like the way the pingPong works. The number of shaders need and how they pass the data to each other. Or if the implement vertex or Geometry shader as well. That´s the case of:

* ofxFlocking: a GPU flocking system that implement two different types of fragment shaders, plus one vertex and geometry shader [Look at the video example](http://patriciogonzalezvivo.com/2011/ofxfx/). If you are interested in this technique you can explore the openFrameworks core example that I made at: https://github.com/openframeworks/openFrameworks/tree/master/examples/gl/GPUparticleSystemExample

* ofxWater: water ripples waves effect. Take a look to the documentation [on this article on my blog](http://www.patriciogonzalezvivo.com/blog/?p=657)


### Filters

The second case, are the ones could be use as filters. That means, they could be use for many things. Giving lot of flexibility and freedom when you use it on your project. Like using the blur and the glow combined with mask and things like that.

* ofxBlur: convultion matrix

* ofxGaussianBlur

* ofxLUT

* ofxBokeh ( by [Tim Scaffidi](http://timothyscaffidi.com/)

* ofxGlow

* ofxBloom

* ofxOldTv ( from [ShaderToy postprocessing](http://www.iquilezles.org/apps/shadertoy/).

![filter0](http://patriciogonzalezvivo.com/2011/ofxfx/filter0.png) ![filter1](http://patriciogonzalezvivo.com/2011/ofxfx/filter1.png) ![filter2](http://patriciogonzalezvivo.com/2011/ofxfx/filter2.png) ![filter3](http://patriciogonzalezvivo.com/2011/ofxfx/filter3.png)


### Composition Shaders 

* ofxBlend: by [Akira Hayasaka Photoshop blend modes shader](https://github.com/Akira-Hayasaka/ofxPSBlend) with alpha mixing as well.  
	
* ofxClone: by [Arturo Castro](http://arturocastro.net/) and [Kyle McDonald](http://kylemcdonald.net/) for their brillant project call [FaceSubstitution](https://github.com/arturoc/FaceSubstitution). 

* ofxMask: based on ofxAlphaMaskShader made by [James George](http://www.jamesgeorge.org) in collaboration with [FlightPhase](http://www.flightphase.com)

* ofxMultiTexture: let you combine 3 textures into one using a RGB map of how to mix them.


### Generative

* ofxTint: generate a noise like pattern applied to a mask that´s it´s very similar to ink;
![tint](http://patriciogonzalezvivo.com/2011/ofxfx/tint.png)
 
* ofxGrayScott: based on ones [Cinder´s Reaction Diffusion example](http://libcinder.org/) that it´s based on [Gray-Scott model](http://mrob.com/pub/comp/xmorphia/). [VIDEO](http://www.patriciogonzalezvivo.com/blog/?p=488)


## Other Examples 
On this addon you will find examples of the classes I just describe. Some of them are combined together in order to show clearly how to use them. Other examples shows new ways of making new shaders by injecting code or extending classes as the following ones:

* sandbox: diferent eyecandy shaders from [Ricardo Caballero´s webGL Sandbox](http://mrdoob.com/projects/glsl_sandbox/) and [Inigo Quilez´s ShaderToy](http://www.iquilezles.org/apps/shadertoy/) together and then passed to post-processing GLSL shaders that acts as filters ( blur, glow, bloom, etc ) 

* conway: life game made by [Kalwalt](http://www.kalwaltart.it/)

## Tests

* test-buffer-copying is a regression test that shows three simple buffers printed side by side. They should all look the same. If they don't look the same, we are somehow changing textures as we are copying them.

* test-shader-load is a test to ensure that loading shaders from file and from string works properly. Should be tested both with and without ```#define USE_PROGRAMMABLE_RENDERER``` set in main.cpp.
