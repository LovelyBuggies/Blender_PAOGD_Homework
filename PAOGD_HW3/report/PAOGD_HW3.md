# OpenGL 初步



---

### 开发环境
-  OS X MOjave 10.14.4
- OpenGL 3.3
- GLFW
- GLAD



---

### OpenGL 简介

OpenGL是用于渲染2D、3D矢量图形的跨语言、跨平台的应用程序编程接口。这个接口由近350个不同的函数调用组成，用来从简单的图形比特绘制复杂的三维景象。而另一种程序接口系统是仅用于Microsoft Windows上的Direct3D。OpenGL常用于CAD、虚拟实境、科学可视化程序和电子游戏开发。



---

### 测试步骤

运行步骤如下：

1. `brew install cmake assimp glm glfw`
3. `git clone https://github.com/JoeyDeVries/LearnOpenGL.git`
4. `cd ./PAOGD_HW3/src`
5. `mkdir build`
6. `cd build`
7. `cmake ..`
8. `make -j8`
8. `cd ./bin`
9. `./PAOGD_HW3`

运行可执行文件之后，我们可以看见如下画面：

![](https://i.loli.net/2019/05/01/5cc948e508298.png)

- 按`W`，`S`，`A`，`D`控制控制镜头的移动；
- 按`J`，模型随着时间绕着自身Z轴旋转；
- 按`K`，模型沿着水平方向往复移动；
- 按`L`， 模型在一定范围内不断放大缩小；
- 按`Q`， 模型恢复原始状态；
- 按`esc`，退出模型。



---

### CMake

模型是用C语言编写的，用CMake进行编译。src目录下CMakeLists.txt文件是用来进行CMake的文件，文件的主要构成如下。

1. 首先设置 build CMake 的路径，并查找需求包：

  ```
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
  link_directories(${CMAKE_SOURCE_DIR}/lib)

  find_package(GLM REQUIRED)
  message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
  find_package(GLFW3 REQUIRED)
  message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
  find_package(ASSIMP REQUIRED)
  message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
  ```

2. 查找相关的框架：

```
INCLUDE_DIRECTORIES(/System/Library/Frameworks)
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY})
set(LIBS ${LIBS} ${APPLE_LIBS})
```

3. 设置 DEMOs：

```
set(DEMOS
    PAOGD_HW3
)	
```

4. 为项目创建所需的静态库

```
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()
```

5. 为项目**创建可执行文件**，我们就可以在`./build/bin`中找到`PAOGD_HW3.exec`了。
   - 首先我们添加了源代码和库文件；
   - 然后我们复制着色器文件以构建目录；
   - 在每个着色器中，读取模型并为其创建链接。

```
foreach(DEMO ${DEMOS})
    file(GLOB SOURCE
        "src/${DEMO}/*.h"
        "src/${DEMO}/*.cpp"
        "src/${DEMO}/*.vs"
        "src/${DEMO}/*.fs"
        "src/${DEMO}/*.gs"
    )
    set(NAME "${DEMO}")
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})

    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin")

    file(GLOB SHADERS
             "src/${DEMO}/*.vs"
             "src/${DEMO}/*.fs"
             "src/${DEMO}/*.gs"
    )
    foreach(SHADER ${SHADERS})

        get_filename_component(SHADERNAME ${SHADER} NAME)
        makeLink(${SHADER} ${CMAKE_CURRENT_BINARY_DIR}/bin/${SHADERNAME} ${NAME})

    endforeach(SHADER)
    
endforeach(DEMO)
```

以上就是CMake生成可执行程序的过程。



---

### OBJ文件

OBJ文件是一套基于工作站的3D建模和动画软件开发的一种标准3D模型文件格式，很适合用于3D软件模型之间的互导。Blender支持OBJ文件的读写。**OBJ文件格式支持直线、多边形、表面和自由形态曲线。直线和多边形通过它们的点来描述，曲线和表面则根据它们的控制点和依附于曲线类型的额外信息来定义。**

- OBJ文件是一种**3D模型文件**，不包含动画、材质特性、贴图路径、动力学、粒子等信息。
- OBJ文件主要**支持多边形模型**，也支持曲线、表面、点组材质。
- OBJ文件支持**三个点以上的面**。
- OBJ文件支持**法线和贴图坐标**。



---

### OpenGL & Blender

OpenGL 是一种渲染编程语言接口，有良好的通用型和渲染表现。Blender 是一款知名的三维动画制作软件，其中很多三维模型的制作和导出需要渲染功能。可以说，Blender是基于OpenGL进行开发的，当然它也包含一些其他的渲染引擎，比如cycle、eevee等。



---

### OpenGL的坐标系统

OpenGL希望每次顶点着色后，我们的可见顶点都为标准化设备坐标。也就是说每个顶点的X、Y、Z都应该在−1 到1 之间，超出这个范围的顶点将是不可见的。通常情况下我们会自己设定一个坐标范围，之后再在顶点着色器中将这些坐标变换为表转化设备坐标。然后这些标化设备坐标传入光栅器，将它们变换为屏幕上的二维坐标和像素。

将坐标变换为标准化设备坐标，接着再转化为屏幕坐标的过程通常是分步进行的。物体的顶点在最终转化为屏幕坐标之前还会被变换到多个过渡坐标系统，使一些操作或运算更加方便和容易。一些常见的坐标系统包括：局部空间、世界空间、观察空间、裁剪空间、屏幕空间。

这就是一个顶点在最终被转化为片段之前需要经历的所有不同状态。为了将坐标从一个坐标系变换到另一个坐标系，我们**需要用到几个变换矩阵，最重要的几个分别是模型、观察、投影三个矩阵。**下面这张图阐释了流程以及各个变换的功能：

![](https://i.loli.net/2019/05/01/5cc95859d1c2a.png)



---

### 功能实现

首先设置基本变量：`J`、`K`、`L`布尔变量分别表示按键触发与否；浮点数`ft`、`fs`分别表示变化度量；`I`、`O`分别表示变化返回与否。

```c
bool j = false;
bool k = false;
bool l = false;
float ft = 0.0f;
float fs = 0.1f;
bool i = true;
bool o = true;
```


在`processInput`函数中，定义新的事件：

``` c
if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    j = true;
if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    k = true;
if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    l = true;
if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
    j = false;
    k = false;
    l = false;
}
```

在主函数增加一些新的选项（因为这次实验功能相对单一所以未设立新的函数）：

- 对于旋转选项：调用了`glm::rotate`函数，用矩阵变换，设置沿着Z轴旋，转`glm::vec3(0.0f, 1.0f, 0.0f)`；
- 对于平移选项：调用了`glm::translate`函数，用矩阵变换，设置x轴平移`ft`的位置，并用`i`变量控制往返移动的操作，`glm::vec3(ft, 0.0f, 0.0f)`；
- 对于缩放选项：调用了`glm::scale`函数，用矩阵变换，设置沿着各个方向放大`fs`倍，并用`i`变量控制往返移动的操作，`glm::vec3(fs, fs, fs)`；

``` c
// rotate option
if(j == true)
model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

// translation option
if(k == true){
    if(ft < 12.0f && i == true){
        model = glm::translate(model, glm::vec3(ft, 0.0f, 0.0f));
        ft += 0.005f;        
    } else{
        i = false;
        model = glm::translate(model, glm::vec3(ft, 0.0f, 0.0f));
        ft -= 0.005f;
        if(ft < -12.0f)
            i=true;        
    }
}

// scale option
if(l == true){
    if(fs < 3.0f && o == true){
        model = glm::scale(model,glm::vec3(fs, fs, fs));
        fs += 0.001f;       
    } else{   
        o = false;
        model = glm::scale(model, glm::vec3(fs, fs, fs));
        fs -= 0.001f;
        if(fs < 0.1f)
            o = true;        
    }
}
```

将着色器添加进入模型，新版本的模型就建立好了。

``` c
ourShader.setMat4("model", model);
ourModel.Draw(ourShader);
```



---

### 目录文件

![](https://i.loli.net/2019/05/01/5cc96208a837f.png)

生成可执行文件后的目录如图所示：

- **build** 中含有编译后的文件，其中的**bin**目录中包括可执行模型；
- **cmake** 目录中含有 CMake 时用到的需求包；
- **CMakeLists.txt** 中是 CMake 的文件，用来编译源代码；
- **configuration** 是模型配置目录，会在 CMake 时用到；
- **includes** 包含了源代码的头文件（.h），其中有很多已经处理好的库，包括矩阵处理等等；
- **resources** 包含了很多模型，我们的 `nanosuit.obj`模型就在其中；（*我这里只留了有用的 ./resources/objects/nanosuit/目录下的内容*）
- **src** 是模型的源代码。



---

### 效果展示

需求实现后的效果展示如下：

#### 旋转

![](https://i.loli.net/2019/05/01/5cc94840dbe7f.gif)

#### 平移

![](https://i.loli.net/2019/05/01/5cc94842b139d.gif)

#### 缩放

![](https://i.loli.net/2019/05/01/5cc9484636f28.gif)

#### 同时

![](https://i.loli.net/2019/05/01/5cc94848cac3f.gif)



---

### 参考

- [OpenGL中的矩阵变换参考](https://learnopengl.com/Getting-started/Transformations)
- [OpenGL坐标系统](https://www.cnblogs.com/tandier/p/8110977.html)
- [CMake](https://baike.baidu.com/item/cmake/7138032?fr=aladdin)
- [OpenGL](https://baike.baidu.com/item/OpenGL)
- [Blender](https://baike.baidu.com/item/Blender/222123#viewPageContent)
- [Learn OpenGL](https://github.com/JoeyDeVries/LearnOpenGL)
- 教程:https://learnopengl.com/](https://learnopengl.com/)
- [Demo 代码仓库](https://github.com/JoeyDeVries/LearnOpenGL.git)



---

