![REPO SIZE](https://img.shields.io/github/repo-size/DBWGLX/-OpenGL-3D-Lighting-and-Shadow-Modeling.svg)
![CODE SIZE](https://img.shields.io/github/languages/code-size/DBWGLX/-OpenGL-3D-Lighting-and-Shadow-Modeling.svg)
![License](https://img.shields.io/github/license/DBWGLX/-OpenGL-3D-Lighting-and-Shadow-Modeling.svg)
![](https://img.shields.io/badge/%20SZU%20-8C0A41)

# Final project of SZU Computer Graphics 

## Introduction

The project features lighting, Phong shading model, planar shadow rendering, and support for loading and displaying models in .obj and .off formats. The camera perspective is first-person, allowing you to freely move through the scene, similar to playing an FPS game.

In terms of implementation, the fragment shader supports both color and texture rendering, and it is compatible with reading quadrilateral faces from .obj files, with smooth groups handled. However, it does not yet support loading .obj files with multi-textures (.mtl).

![GIF 2024-12-18 4](https://github.com/user-attachments/assets/fbee1020-85b0-4f7f-95ce-8002cbc5d878)

You can easily load models with this code and freely admire them!

Of course, you can also expand and implement other features based on this foundation.

## Implementation effect

1.	场景设计和显示<br>
学生可以通过层级建模（ 实验补充1和2）的方式建立多个虚拟物体，由多个虚拟物体组成一个虚拟场景，要求在程序中显示该虚拟场景，场景可以是室内或者室外场景；场景应包含地面。层级建模的最深层次需要达到至少四层。<br>
总场景：<br>
 ![image](https://github.com/user-attachments/assets/8d125791-775d-46ca-a152-41687589ec55)<br>
层级建模：<br>
机械臂。四层：底座-上臂-下臂-手掌<br>
 ![image](https://github.com/user-attachments/assets/ce1ea30a-f567-4d88-8398-6d2a17e376ad)<br>
2.	添加纹理<br>
参考实验4.1，为场景中至少两个主要物体添加纹理贴图 。<br>
 ![image](https://github.com/user-attachments/assets/c438ffc5-4a28-4cbd-b107-71b0792e8ffc)<br>
图中四个物体，地板和后面的背景图片都是贴的纹理<br>
3.	添加光照、材质、阴影效果<br>
参考实验3.2，实验3.3和实验3.4，实现光照效果、材质、阴影等。<br>
 ![image](https://github.com/user-attachments/assets/85631fda-442f-4b15-9711-823e3b781bdb)<br>
可以看到phong光照模型，有阴影。材质由光的效果和网格面片的法向共同实现。<br>
4.	用户交互实现视角切换完成对场景的任意角度浏览<br>
参考实验3.1，完成相机变换。<br>
以上图片已经体现出自由的相机视角<br>
5.	通过交互控制物体<br>
参考实验2.3，实现物体的变换，允许用户通过键盘或者鼠标实现场景中至少两个物体的控制（移动，旋转，缩放等等）。<br>
 ![image](https://github.com/user-attachments/assets/0aac0b1e-1f03-48cd-81d6-1d64cec1bbfa)<br>
可以对除层级建模的机械臂外所有网格旋转，移动，放缩<br>
 ![image](https://github.com/user-attachments/assets/a8a426a9-5c9a-41cc-aa6d-51ecfad11042)<br>


## Code Overview

主要是main，然后是三个类的实现：1.相机(Camera.h/Camera.cpp)，2.三角网格(TriMesh.h/TriMesh.cpp)，3.网格绘制器(MeshPainter.h/MeshPainter.cpp)

![image](https://github.com/user-attachments/assets/22a8d7ca-6e3f-4e8d-b5d7-d0ad8d4357b8)<br>

1.	Camera.cpp：管理摄像机的属性，投影的计算。（Camera.h的实现）<br>
2.	glad.c：加载了 c头文件和OpenGL 函数的实现，以及平台特定的处理。确保程序能够在不同的平台上正确调用 OpenGL API。（GLAD 是一个 OpenGL 加载库，它帮助加载 OpenGL 扩展。）<br>
3.	InitShader.cpp：GLSL着色器初始化工具。用于读取和编译 GLSL 着色器（顶点和片段着色器），然后将它们链接成一个 OpenGL 程序对象。<br>
4.	main.cpp：程序的入口点，包含 main 函数。设置 OpenGL 上下文、窗口，定义键鼠交互逻辑，并调用模型加载，渲染循环等。<br>
5.	TriMesh.cpp：定义了一个基于 OpenGL 的三角形网格类 TriMesh 和一个继承自它的光源类 Light，为 3D 图形渲染和光影处理提供支持。（TriMesh.h的实现）<br>
6.	MeshPainter.cpp：负责使用 OpenGL 绘制 3D 网格对象。包含了将数据绑定到 OpenGL 缓冲区、设置着色器、纹理、光照与材质，以及绘制物体和阴影等功能的实现。（MeshPainter.h的实现）<br>

/shaders 存放着色器

/assets 存放.off ,.obj ,textures

## Environment Configuration

见 [https://github.com/DBWGLX/SZU_Tetris/tree/environment](https://github.com/DBWGLX/SZU_Tetris/tree/environment)
