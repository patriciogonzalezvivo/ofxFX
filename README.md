# ofxFX #
![ofxFlocking](http://patriciogonzalezvivo.com/images/flock.png) ![ofxFluid](http://patriciogonzalezvivo.com/images/fluid.png) ![ofxGlow](http://patriciogonzalezvivo.com/images/oldTv.png) ![ofxGrayScott](http://patriciogonzalezvivo.com/images/grayScott.png) ![ofxWater](http://patriciogonzalezvivo.com/images/water.png)
This addons begins while I was working on [Efecto Mariposa](http://patriciogonzalezvivo.com/butterfly.html "Efecto Mariposa") and I need some extra GPU processing power in order to make fast simulation of complex ecosystems. 
On does days I found GLSL Shaders a hard and cryptic topic. That´s why the main goal of this addon it´s to make super easy to use and edit shaders on your projects. That´s why it becomes a sort of common used shader library.
Recently I start playing with them new GLSL editors like [Ricardo Caballero´s WebGL SandBox](http://mrdoob.com/projects/glsl_sandbox/), [Inigo Quilez´s ShaderToy](http://www.iquilezles.org/apps/shadertoy/) or [Kode80´s GLSL Studio](http://glslstudio.com/) that let you edit the shader on-the-fly. So right now I´m working on making a standard ofxFXObject that could receive fragment shader and know how to deal with them while the program it´s running. 

## ofxFXObject ##
It´s the parent class of all the other effect. If you want to make a new filter you may want to start looking at the ofxFXObject.h .

The structure it´s easy.

1. Constructor: here it´s necessary to set three vital variables: 
    - passes: the number of passes or itineration of the main ping pong betweens FBO´s  
    - internalFormat: if it use GL_RGB, GL_RGBA, GL_RGB16f, GL_RGBA16f, GL_RGB32f, GL_RGBA32f, etc...
    - fragShader: it´s the code of the shader. Note that some changes have to be made in order to fill everything on a string
    
2. ```allocate(width,height,GL_RGBA)```: This usually it´s no need to bee re-define. It´s basically allocate the FBO´s and loads the shader by using injectShader();

3. ```injectShader(string fragContent)```: here is where the shaders are loaded. See the example bellow.

4. ```begin(int texN)``` and ```end(int texN)```: remember nTextures variable? you can passthrough information to it by using this end() and begin() and giving the argument the number of texture you want to open. This texture can be access from the shader by the ```uniform sample2DRect tex0``` or ```tex1``` or ```tex2``` and so on. Also you can access to the previous FBO of the ping pong by calling ```uniform sample2DRect backbuffer```. 

5. ```update()```: this is the core of the class, where the magic happens. If you check  for the ```ofxFXObject::update()``` you will see how the tex´s, the backbuffer and other default uniforms variables (time, mouse, resolution) are loaded.

6. ```draw(x,y,w,h)```: after all you definitely want to look at it.

An example of an implementation of an ofxFXObject could be:

On setup:

```c++
ofxFXObject fxObject = ofxFXObject();

fxObject.allocate(800,600); // At this point it will load a default timer shader

//Let´s play a little injecting a new one
fxObject.injectShader("#version 120\n\
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

### In-line Code ###
You may notice that I´m not dealing with .frag of .vert files. This have to main reasons. First, make your binnary excecutables more compact. And Secondly, to work in the on-the-fly edition mode.
Thanks for Zach Lieberman tip you have to options one it´s to use normal " " with \ on the end of each line. Witch it´s very hard to read and your IDE couldn´t help you. Or by adding ```#define STRINGIFY(A)  #A``` at the .h and using it like this:

```c++
#define STRINGIFY(A)  #A

const char shader[] = 
STRINGIFY(

//multiline code goes here.  no newline markers needed

);
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

In the src/ directory of the addon you will find lot´s of subClasses that inherit from ofxFXObject. Most of them are there for two reasons. 

First case, the ones that in some point breaks the  structure of ofxFXObject with some extra tweaks. Like the way the pingPong works. The number of shaders need and how they pass the data to each other. Or if the implement vertex or Geometry shader as well. That´s the case of:

* ofxFlocking: a GPU flocking system that implement two different types of fragment shaders, plus one vertex and geometry shader [VIDEO](http://www.patriciogonzalezvivo.com/blog/?p=488) 

* ofxFluid: fluid simulation based on [this article of Mark Harris](http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html). [Look at the video example](http://www.patriciogonzalezvivo.com/blog/?p=488) that use a lot of shader on a very complex and crazy way

* ofxGrayScott: based on ones [Cinder´s Reaction Diffusion example](http://libcinder.org/) that it´s based on [Gray-Scott model](http://mrob.com/pub/comp/xmorphia/). [VIDEO](http://www.patriciogonzalezvivo.com/blog/?p=488)

* ofxWater: a regular water waves effect based on [Hugo Elias´s Tutorial](http://freespace.virgin.net/hugo.elias/graphics/x_water.htm). [VIDEO](http://www.patriciogonzalezvivo.com/blog/?p=488)


The second case, are the ones could be use as filters. That means, they could be use for many things. Giving lot of flexibility and freedom when you use it on your project. Like using the blur and the glow combined with mask and things like that.

* Filters: ofxBloom, ofxBlur, ofxBokeh ( by [Tim Scaffidi](http://timothyscaffidi.com/), ofxGlow, ofxUnsharp and ofxOldTv ( from [ShaderToy postprocessing](http://www.iquilezles.org/apps/shadertoy/). [VIDEO](http://www.patriciogonzalezvivo.com/blog/?p=488))

* Composers: ofxClone: maden by [Arturo Castro](http://arturocastro.net/) and [Kyle McDonald](http://kylemcdonald.net/) for their brillant project call [FaceSubstitution](https://github.com/arturoc/FaceSubstitution). And ofxMask: based on ofxAlphaMaskShader made by [James George](http://www.jamesgeorge.org) in collaboration with [FlightPhase](http://www.flightphase.com)


# Examples #
On this addon you will find examples of the classes I just describe. Some of them are combined together in order to show clearly how to use them in the following examples:

*   Mix: diferent eyecandy shaders from [Ricardo Caballero´s webGL Sandbox](http://mrdoob.com/projects/glsl_sandbox/) and [Inigo Quilez´s ShaderToy](http://www.iquilezles.org/apps/shadertoy/) together and then passed to post-processing GLSL shaders that acts as filters ( blur, glow, bloom, etc ) 

*   Mask: an example of how to mask the effects between them.

*   Conway: life game made by [Kalwalt](http://www.kalwaltart.it/)