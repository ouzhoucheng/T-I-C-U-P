# TI-Cup-2021-1st
Task F-Intelligent drug delivery trolley, we got 1st prize in national finals 

[【电赛】送药小车 国赛一等奖1st TI Cup National Undergraduate Electronics Design Contest 2021](https://www.bilibili.com/video/BV13L411K7ex/)

[为了装这个小车，拆了一辆国一智能车！2021全国电赛F题（天津大学）](https://www.bilibili.com/video/BV1Qr4y1Y7YR)

# convert RGB color gamut image to Lab color gamut image using C language
# RGB颜色空间
RGB颜色是一种便于计算机识别的色彩模式,采用加色法,R为红色,G为绿色,B为蓝色,三种颜色从0(关闭)到255(亮度最大).
![在这里插入图片描述](https://img-blog.csdnimg.cn/b5db6cb6a3784168939aca51a79e884c.png)

一般有24位的RGB888,三个分量都为8位,和16位的RGB565,三个分量分别为5位6位5位.

# 设备相关
因为颜色的视觉效果，是基于设备和材料的。当设备和材料改变时，对应的颜色效果就会发生变化，我们称这样的颜色空间为设备相关颜色空间，如RGB和CMYK颜色空间。当不同呈色模式的设备来表现同一颜色时，对应的颜色数值是不同的。

而如在HSB的基础上建立起来的CIE L*a*b*颜色空间，直接用一组数值来模拟人类的颜色视觉，而不是使用一组所需要的数值去驱动一个特定设备来生成颜色，其色彩数值与设备无关，称为设备无关颜色空间。

# XYZ颜色空间
RGB是一种设备相关的颜色空间，每一种设备使用RGB模型时，都有不太相同的定义，如在显示器和打印机上就不太一样，因此在使用RGB模型时，这些设备之间关于RGB的定义，并不能通用。我们在比较两种颜色时，经常会忽略光照的因素，如果比较两种颜色所处的光照环境不一致，那将毫无意义。

RGB匹配光谱时可能会出现负值，并且没有考虑亮度，而设备无关的颜色空间需要考虑亮度，对此CIE制定了两条标准
- 定义了XYZ颜色空间，一种与RGB相关的想象基色系统，更适于颜色计算
- 定义了标准光源

XYZ是对RGB的一种线性变换，变换后颜色空间使用XYZ为三基色，三个值都用正数匹配所有颜色。

![在这里插入图片描述](https://img-blog.csdnimg.cn/cd593a1b708344e98800aaca3701c4db.png)

为什么变换一下就能设备无关呢，因为绿波是中波，横跨了光谱坐标，蓝色是短波，红色是长波，而比短波短或比长波长的电磁波我们就看不见了，人眼对绿色最为敏感，故Y定义为为亮度，又包含了绿色信息。

XYZ不是RGB三者的组合，X值近一半由R组成，Y值80%由G组成，Z值99%都是B。





# Lab颜色空间
Lab色域是CIE(国际照明委员会)制定的一种色彩模式,由一个亮度通道和两个颜色通道组成
- L: 亮度
- a: -128绿色 -> +127红色分量,
- b: -128蓝色 -> +127黄色分量

![在这里插入图片描述](https://img-blog.csdnimg.cn/ba73a96b541b42489da4e8a62ef1d651.png)

Lab色域是基于人对颜色的感觉设计的,与人视觉工作方式接近,概括人眼能看到的所有颜色,如果Lab三通道变化幅度一样,其给人在视觉上变化幅度也相近,要比RGB,CMYK等颜色空间有更广的色域等

![请添加图片描述](https://img-blog.csdnimg.cn/ca3fcf49703d43c88b1ba541713cd9c7.png)
隐藏a通道
![在这里插入图片描述](https://img-blog.csdnimg.cn/5f2f7a66e32a49fd8cf9934bfed7d7be.png)
隐藏b通道
![在这里插入图片描述](https://img-blog.csdnimg.cn/f063d0eb84e34576be3f273348d5b059.png)

# RGB565转Lab

## gamma校正
上面的色彩空间都是线性增长的，但人眼对亮度感知能力却不是线性关系的，相对纯白(亮度100%)和纯黑(亮度0%),人眼对中等亮度的感知并非中灰,当人眼接受20%左右亮度光源,才会觉得这是中等亮度.

## RGB转XYZ
需要归一化后进行gamma校正

$
\left\{\begin{matrix}
R=\rm gamma (\frac{r}{255} ) \\
G=\rm gamma (\frac{g}{255} )  \\
B=\rm gamma (\frac{b}{255} )
\end{matrix}\right.
$

$
\rm gamma(x)=\left\{\begin{matrix}
\left ( \frac{x+0.055}{1.055}  \right ) ^{2.4}& x>0.04045 \\
\frac{x}{12.92} 
\end{matrix}\right.
$

$
\begin{bmatrix}X \\Y \\Z
\end{bmatrix}=
M*\begin{bmatrix}R \\G \\B
\end{bmatrix}
$

$
M=\begin{bmatrix}
 0.4124 & 0.3576 & 0.1805 \\
 0.2126 & 0.7152 & 0.0722\\
 0.0193 & 0.1192 & 0.9505
\end{bmatrix}
$

## XYZ色域转Lab色域
$
f(t)=\left \{\begin{matrix}
t^{1/3} & t>(\frac{6}{29} )^3\\
\frac{1}{3}(\frac{29}{6} )^2t+\frac{4}{29}  
\end{matrix}\right .
$

$
\left\{\begin{matrix}
L=116*f(Y_1)-16 \\
a=500*(f(X/X_n)-f(Y/Y_n)) \\
b=200*(f(Y_1)-f(Z_1)) 
\end{matrix}\right.
$

## C代码实现
```C
inline float gamma(float x)
{return x>0.04045?pow((x+0.055f)/1.055f,2.4f):x/12.92;};

void RGBToLab(unsigned char*rgbImg,float*labImg)
{
    float B=gamma(rgbImg[0]/255.0f);
    float G=gamma(rgbImg[1]/255.0f);
    float R=gamma(rgbImg[2]/255.0f);
    float X=0.412453*R+0.357580*G+0.180423*B;
    float Y=0.212671*R+0.715160*G+0.072169*B;
    float Z=0.019334*R+0.119193*G+0.950227*B;

　　float X/=0.95047;
　　float Y/=1.0;
　　float Z/=1.08883;

    float FX = X > 0.008856f ? pow(X,1.0f/3.0f) : (7.787f * X +0.137931f);
    float FY = Y > 0.008856f ? pow(Y,1.0f/3.0f) : (7.787f * Y +0.137931f);
    float FZ = Z > 0.008856f ? pow(Z,1.0f/3.0f) : (7.787f * Z +0.137931f);
    labImg[0] = Y > 0.008856f ? (116.0f * FY - 16.0f) : (903.3f * Y);
    labImg[1] = 500.f * (FX - FY);
    labImg[2] = 200.f * (FY - FZ);
}
```
这是未经过优化的代码,对每个像素三通道都进行原公式运算,运算量非常大,不适于在性能有限的单片机上跑.

而gamma和f(t)都是比较固定的运算,可以转换为查表运算,将浮点转换为整型,将乘除转换为移位.
```C
static int LabTable[1024];
static int GamaTable1[32];
static int GamaTable2[64];//B有6位，特殊对待
void CreateTable() // 初始化建表
{
  for (int I = 0; I < 1024; I++)
  {
    if (I > 9)
      LabTable[I] = (int)(pow((float)I / 1024, 1.0F / 3.0F) * 1024 );
    else
      LabTable[I] = (int)(7.787F * I + 141.2 );
  }
  for (int J = 0; J < 32; J++)
  {
    float x = J/32.0F;
    x = x>0.04045?pow((x+0.055f)/1.055f,2.4f):x/12.92;
    GamaTable1[J] = (int)(x*1024);
  }
  for (int K = 0; K < 64; K++)
  {
    float y = K/64.0F;
    y = y>0.04045?pow((y+0.055f)/1.055f,2.4f):y/12.92;
    GamaTable2[K] = (int)(y*1024);
  }
}
```

在2021年电赛题中的应用,图中屏幕上的白色区域就是农夫山泉包装,摄像头为凌瞳彩色摄像头.
![在这里插入图片描述](https://img-blog.csdnimg.cn/b9472936f7894eecb2db6781945db68b.png)


这样对每一帧图像,遍历其像素,调用以下代码即可得到Lab三通道值.
```C
x=(455026*GamaTable1[r]+394489*GamaTable2[g]+199046*GamaTable1[b])>>20;
y=(223002*GamaTable1[r]+749900*GamaTable2[g]+75675*GamaTable1[b])>>20;
z=(18619*GamaTable1[r]+114786*GamaTable2[g]+915097*GamaTable1[b])>>20;
L = y > 9 ? (116 * LabTable[y] - 16384)>> 10: (903 * LabTable[y])>> 10;
A = (500 * (LabTable[x] - LabTable[y]))>> 10;
B = (200 * (LabTable[y] - LabTable[z]))>> 10;
```


> 参考
> - [图片](http://k.sina.com.cn/article_5194219036_135997e1c00100649v.html?display=0&retcode=0)
> - [C语言实现RGB565转LAB色彩空间（RT1064 + 凌瞳）](https://blog.csdn.net/weixin_44349732/article/details/104062757)
> - [RGB转LAB色彩空间](https://www.cnblogs.com/hrlnw/p/4126017.html)
> - [如何用PS中Lab颜色模式进行简单调色](https://zhuanlan.zhihu.com/p/87324864)
> - [LAB、HSB、RGB和CMYK色彩模式简介](https://blog.csdn.net/qq_26654881/article/details/52442887)
> - [零基础入门音视频开发](https://blog.csdn.net/u011399342/category_7463440.html)
> - [计算机色彩知识调研（二）](https://blog.csdn.net/Jacky_chenjp/article/details/83116976)
> - [为了装这个小车，拆了一辆国一智能车！2021全国电赛F题（天津大学）](https://www.bilibili.com/video/BV1Qr4y1Y7YR?spm_id_from=444.41.0.0)
