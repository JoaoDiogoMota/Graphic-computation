# Solar-System
Computer Graphics Engine Project

The goal of this assignment is to develop a mini scene graph based 3D engine and provide usage examples that show its potential. The assignment is split in four phases, each containing a report and code for the __engine__ and __generator__.

## Phase 1 - Graphical primitives

This phase requires two applications: the __generator__, which purpose is to generate the vertices for different models (plane, box, sphere and cone), and the __engine__ itself, which will read a configuration file, written in XML, and display the models: 

```xml
<scene>
  <model file=”sphere.3d” />
  <model file=”plane.3d” />
</scene>
```

[Phase 1 report](https://github.com/13caroline/Solar-System/blob/master/Fase1/relat%C3%B3rio.pdf)

##  Phase 2 - Geometric Transforms 

This phase is about creating __hierarchical scenes__ using geometric transforms. A scene is defined as a __tree__ where each node contains a set of geometric transforms (translate, rotate and scale) and optionally a set of models. Each node can also have children nodes.

```xml
<scene>
  <group> <!-- TERRA -->
    <translate Z="35"/>
    <scale X="0.73" Y="0.73" Z="0.73"/>
    <models>
      <model file="planeta.3d"/>
    </models>
    <group> <!-- LUA -->
      <translate X="1.5"/>
      <scale X="0.16" Y="0.16" Z="0.16"/>
      <models>
        <model file="planeta.3d"/>
      </models>
    </group>
  </group>
</scene>
```

In this phase, the models are now drawn with **VBOs**, as opposed to immediate mode used in the previous phase.

[Phase 2 report](https://github.com/13caroline/Solar-System/blob/master/Fase2/relat%C3%B3rio.pdf)

## Phase 3 - Curves, Cubic Surfaces and VBOs

In this phase, the __generator__ must be able to create a new type of model based on _Bezier patches_ applying Bezier rules for the defined **control points** and **tesselation level**. The generator will receive as parameters the name of a file where the Bezier control points are defined as well as the required tessellation level. The resulting file will contain a list of the triangles to draw the surface.

**Rotate** and **Translate** elements have been extended. The goal is to perform animations based on _Catmull-Rom cubic curves_, considering a set of points and the number of seconds to run the whole curve. 

[Phase 3 report](https://github.com/13caroline/Solar-System/blob/master/Fase3/relat%C3%B3rio.pdf)

## Phase 4 - Normals and Texture Coordinates

In the last phase, the generator should provide texture coordinates and normals for each vertex of each model, in order to add lighting and texture. 
The light sources in the _xml configuration file_ are demonstrated below.

```xml
<scene>
  <lights>
    <light type="POINT" posX="23" posY="15" posZ="23"/>
  </lights>
  ...
</scene>
```

[Phase 4 report](https://github.com/13caroline/Solar-System/blob/master/Fase4/relat%C3%B3rio.pdf)

## Collaborators

| Name            	|
|-----------------	|
| [Carolina Cunha](https://github.com/13caroline)  	|
| [Hugo Faria](https://github.com/KHiro13)      	|
| [João Diogo Mota](https://github.com/JoaoDiogoMota) 	|
| [Rodolfo Silva](https://github.com/Th0l)   	|

> <img src="https://seeklogo.com/images/U/Universidade_do_Minho-logo-CB2F98451C-seeklogo.com.png" align="left" height="48" width="48" > University of Minho, Software Engineering (3rd Year).
