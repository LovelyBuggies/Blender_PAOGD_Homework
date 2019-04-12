# PAOGD_HW2

## Blender实现小人行走

---

### 目标实现

利用Blender 2.80 beta实现一个简单的行走循环动画。

![target](https://ws4.sinaimg.cn/large/006tNc79ly1g1zm621q06g30go09e1ky.gif)



---

### 效果展示

- 总体循环效果：

![whole effect](https://ws1.sinaimg.cn/large/006tNc79ly1g1zq5zyfycg30b605wk7j.gif)

- 局部效果：

![effect](https://ws4.sinaimg.cn/large/006tNc79ly1g1zqavv3r2g30mk0awgvf.gif)

- 走路动作的片段：

![walking snipshot1](https://ws1.sinaimg.cn/large/006tNc79ly1g1zq2v5a1kj30z80ist98.jpg)

![walking snipshot2](https://ws2.sinaimg.cn/large/006tNc79ly1g1zq4p3xmqj30w20iwt9a.jpg)

![walking snapshot3](https://ws3.sinaimg.cn/large/006tNc79ly1g1zq7trbe5j30wo0jwdgc.jpg)



#### 优点

- 小人拥有整体的骨架🦴
- stickman 有真实的骨肉影响权重（骨动肉动🍖）
- stickman 骨架符合反向动力学IK构造
- 基本实现小人循环行走🚶的姿态
- 相机跟踪渲染小人📷

#### 缺点

- 行走姿态不够自然，右腿比左腿略长一点
- 渲染场景还不够多样化
- 还未尝试用表皮修改器为小人面部进行描摹



---

### 实现步骤

实现小人行走看似容易，却包含了繁杂的步骤，我按照实现顺序进行描述。



#### 建立骨架

为了实现复杂的行为动画，首先需要导入小人模型并键入骨架。

![bone](https://ws3.sinaimg.cn/large/006tNc79ly1g1zn39jtfkj31d80u0tq8.jpg)

调整骨架并选中，`tab`编辑模式下`E`键挤出骨头，由此构建统一的骨架。

![armature](https://ws2.sinaimg.cn/large/006tNc79ly1g1zn72aygwj31d80u04m6.jpg)

为了让骨头带动肉动，**为skin设置父集armature，**我们要建立父集关系：在物体模式下，选中armature，然后`shift`同时选中stickman，`control+P`选择`set parents to object`。

![bones connect skin](https://ws3.sinaimg.cn/large/006tNc79ly1g1zo9ajg0qj31d80u0ttk.jpg)

这时候，小人的“皮”就能跟着肉动了。有时候骨头是分离的，但是我们需要让它们有关联，为了**让父骨带动子骨动**，我们也要建立父集关系：首先`option+P`选择`clear parent`，清除父集关系；然后`control+P`选择`connected`。

![separate parents](https://ws1.sinaimg.cn/large/006tNc79ly1g1zngy6yxvj31d80u0nke.jpg)

![connect parents](https://ws2.sinaimg.cn/large/006tNc79ly1g1znhizqvgj31d80u01fs.jpg)

小人本身就是一个obj，所以无需再进行整体连接。为了简单起见，设置为自动权重。`control+tab`姿态模式下，`control+P`进行骨架形变，然后从弹出的小框内选择自动权重。

![deform](https://ws4.sinaimg.cn/large/006tNc79ly1g1znwedzvmj31d80u0qml.jpg)

![automatic weights](https://ws3.sinaimg.cn/large/006tNc79ly1g1znxzk7xzj31d80u0tsz.jpg)

当然也可以手动调整权重：物体模式下，选中骨架，再选中stickman，打开权值绘制模式，我们就可以绘制权重了。调整笔刷大小和颜色，蓝色表示无影响，红色表示影响力为1。

![weight paint](https://ws2.sinaimg.cn/large/006tNc79ly1g1zta2xv0jj31d80u0ke4.jpg)

为了获得更好的效果，让每根骨头“牵一发而动全身”，IK骨应用反向动力学原理设计更优雅的行走动画。挤出一根骨头，姿态模式下选中IK目标骨，同时`shift`选中依附骨，`shift+I`添加IK骨。这时候我们发现，移动IK骨，依附骨也会跟着动。

![Add IK](https://ws1.sinaimg.cn/large/006tNc79ly1g1zo36ejegj31d80u01ca.jpg)

这样，小人的骨架就基本制作好了。



#### 动画制作

首先我们为了让动画循环并让小人沿曲线行走，我们首先要设置一个行走路径。Add一个圆，用同样的方法，为骨架设置父集circle。这样小人就能按着圆的轨迹平移了。

![circle parent](https://ws2.sinaimg.cn/large/006tNc79ly1g1zoh1btk3j31d80u0tuz.jpg)

在动作编辑器模式创建行走动作。首先要新建一个walk轨道。

![walk track](https://ws3.sinaimg.cn/large/006tNc79ly1g1zonlw4quj31d80u0ke2.jpg)

在这个通道中，插入关键帧。右键，在`insert key frame`中采用`insert whole frame`。

![insert keyframe](https://ws3.sinaimg.cn/large/006tNc79ly1g1zovf8p7qj31e10u0kec.jpg)

![whole character as frame](https://ws4.sinaimg.cn/large/006tNc79ly1g1zown36ldj31d80u0h93.jpg)

以此类推，插入多个关键帧在walk通道。新建一个NLA（NonLinear Animation）轨道——`NLA track`。这之后，将walk通道`push down`到NLA轨道中，我们这时候就看到骨架对象下，出现了包含walk通道的NLA轨道。

![push down channel](https://ws2.sinaimg.cn/large/006tNc79ly1g1zpcrsco9j31d80u01br.jpg)

为了让小人行走动作循环，选中walk通道，并进行`Action Clip`进行操作，我这里调整了frame长度为28和重复30次数，来应对实验要求。

![action clip](https://ws2.sinaimg.cn/large/006tNc79ly1g1zpfkveaxj31d80u0k5m.jpg)

制作动画的最后一个环节就是为了让动画能够渲染，我们必须要合理地摆放控制镜头。毕竟小人是这个动画的主角，为了让相机时时刻刻对准小人，需要相机跟踪功能。首先新建一个相机。

![new camera](https://ws2.sinaimg.cn/large/006tNc79ly1g1zsp5it5tj31d80u0x17.jpg)

打开相机约束，设置相机跟踪的相关功能，设置成功之后镜头对准目标，有一条虚线，镜头对准target——我设置的骨架。当然相机也可以设置轨道——建立和circle的依附关系，原理和上面一样，不再赘述。

![target projection](https://ws4.sinaimg.cn/large/006tNc79ly1g1zstakbe3j31e10u07ql.jpg)

![protagonist](https://ws4.sinaimg.cn/large/006tNc79ly1g1zsvw2dvij31d80u0avo.jpg)

这样小人行走动画就制作好了！大功告成，简单渲染了一下。虽然走地有点丑，但是总体效果还是差强人意的😄！

![effect](https://ws4.sinaimg.cn/large/006tNc79ly1g1zqavv3r2g30mk0awgvf.gif)



---

### 实验心得

- 编辑模式下挤出骨头的时候，如果要单边挤出，不能选中整根骨头，不然会从两边挤出。
- 这个实验中，`stickman.obj`文件中的小人一开始好像就是不对称的，所以建立骨架的时候不能用X镜像，不然会不对称（~~然而自己手动调整好像更丑~~）。
- 实际上，微调的贡献远远大于采用权重调整、反向动力学等操作。因为走路过程手上动作很少，所以不给手建立IK骨看起来效果反而更好。
- 这个题目没有体现权重的重要性，*因为是一个裸体小人，所以每一根骨头都会直接影响肉身，所以这个题目设置blender自带的自动调整权重就很好，但在某些带饰品的动画人物身上需要做微调*。
- 为了让父骨带动子骨动，两个不连接的骨头也会有藕断丝连的牵连关系，*个人理解模拟了人体韧带的功能*。
- 个人感觉这个实验中，IK骨的功能完全可以用为游离骨添加父集IK骨功能来实现，觉得IK骨没什么特殊的功能。
- 添加IK骨的时候，应该选中两个目标骨，一开始我以为只用给IK骨add IK就OK了，导致卡了很久。
- 动画制作的原理是让小人围着一个圈原地踏步，但是这个好像不符合走路运动的常识。
- 一开始我想采用形态键来设置关键帧，但是后来证明这是个naïve的想法。
- 调整任务动作十分的耗费精力，因为不对称，我全程手动调整，所以有些动作不是对称的，这点严重影响了我的工作效率和效果。而且丑陋的姿态严重影响了我的成就感。
- 编辑动画的过程中有很多概念，按部就班就不会出问题，顺序对实验结果的影响很大。动画制作过程中经常会混淆track和channel的概念，加上我命名没有区分，导致后期逻辑混乱。
- Action clip操作弥足重要，因为如果没有好的clip frame length，会导致小人动作不够协调——当然这个问题也可以从制作关键帧的那步做文章。
- 设置相机轨道的顺序会对相机跟踪的参数产生影响。
- **To Do**：*未来打算尝试多场景渲染和利用表皮修改器进行面部描摹。*



---

### 参考资料

- [零基础学Blender](https://www.bilibili.com/video/av24292767/?p=21)
- [Blender Tutorial](https://www.blender.org/support/tutorials/)
- [blender零基础入门教程](https://www.bilibili.com/video/av24292767/?p=34)



---



