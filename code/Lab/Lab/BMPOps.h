#ifndef _BMP_H_
#define _BMP_H_
#include<cstdio>
#include<cmath>

//(c)2020,Gordon Freeman
//BMPOps.h    Ver.1.02

//定义常量
#define pi 3.1415926

//声明别名
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//结构体声明
class BITMAPFILEHEADER {
public:
	WORD bfType = 0x4D42;		//文件类型（对于BMP文件，该值恒为"BM",十六进制值为4D42）
	DWORD bfSize;				//文件大小（字节）。不使用调色板时，其值为数据区大小 + 54.
	WORD bfReserved1 = 0;		//保留字，忽略
	WORD bfReserved2 = 0;		//保留字，忽略
	DWORD bfOffBits;			//数据区相比文件起始位置的偏移量。不使用调色板时，其值为54.
	//默认构造函数：除有固定值的成员外，所有成员变量初始化为0
	BITMAPFILEHEADER() {
		bfSize = 0;
		bfOffBits = 0;
	}
};
class BITMAPINFOHEADER {
public:
	DWORD biSize = 40;			//本结构大小。在Windows下总为40（字节）。
	LONG biWidth;				//图像宽度（像素）
	LONG biHeight;				//图像高度（像素）
	WORD biPlanes = 1;			//位图平面数，始终为1.
	WORD biBitCount;			//颜色位数
	DWORD biCompression;		//图像数据的压缩类型，0表示不压缩。
	DWORD biSizeImage;			//数据区大小（字节）。有时会比实际数据区的大小大2.
	LONG biXPelsPerMeter;		//横向像素密度，单位为像素/米。
								//“画图”产生的图像，此项通常为3780；PS产生的图像，此项通常为2834.
	LONG biYPelsPerMeter;		//纵向像素密度，单位为像素/米。
	DWORD biClrUsed;			//BMP图像使用的调色板中的颜色数目，0表示使用全部颜色。
	DWORD biClrImportant;		//重要的颜色数目，0表示所有颜色都重要。
	//默认构造函数：除有固定值的成员外，所有成员变量初始化为0
	BITMAPINFOHEADER() {
		biWidth = 0;
		biHeight = 0;
		biBitCount = 0;
		biCompression = 0;
		biSizeImage = 0;
		biXPelsPerMeter = 0;
		biYPelsPerMeter = 0;
		biClrUsed = 0;
		biClrImportant = 0;
	}
};
class RGBQUAD {
public:
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE reserved;
	//默认构造函数：所有成员变量初始化为0
	RGBQUAD() {
		R = G = B = reserved = 0;
	}
};
class RGBInfo {
public:
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	BYTE* data;
	int width_byte;
	//默认构造函数：所有成员变量初始化为0
	RGBInfo() {
		data = NULL;
		width_byte = 0;
	}
	//计算每行的字节数
	void calcByteWidth() {
		width_byte = (InfoHeader.biWidth * InfoHeader.biBitCount + 31) / 32 * 4;
	}
};
class GSInfo {
public:
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	RGBQUAD* palette;
	BYTE* data;
	int width_byte;
	//默认构造函数：所有成员变量初始化为0
	GSInfo() {
		palette = NULL;
		data = NULL;
		width_byte = 0;
	}
	//计算每行的字节数
	void calcByteWidth() {
		width_byte = (InfoHeader.biWidth * InfoHeader.biBitCount + 31) / 32 * 4;
	}
};

//函数原型声明
RGBInfo imread_RGB(const char name[]);
GSInfo imread_GS(const char name[]);
void imwrite(RGBInfo info, const char name[]);
void imwrite(GSInfo info, const char name[]);

//取像素函数
//参数：接收RGB图像信息（图像信息应为对象，下同）引用，横、纵坐标以及层数作为参数
//返回值：返回所取像素指定层数据的引用
//注意：RGB值在外存中以B,G,R的顺序存储
//     函数认为数据区中第一个像素的坐标为(1,1)
BYTE& GetPixel(RGBInfo& info, int x, int y, int layer);
//重载取像素函数，适用于只接受RGB数据区的情况
//参数：接受数据区指针，宽度（以字节计），横/纵坐标，层数作为参数
//返回值：返回所取像素指定层数据的BYTE型引用
BYTE& GetPixel(BYTE* data, int w_byte, int x, int y, int layer);
//重载取像素函数，适用于数据区为整型的情况
//参数：接受数据区指针，宽度（以像素计），横/纵坐标，层数作为参数
//返回值：返回所取像素指定层数据的int型引用
int& GetPixel(int* data, int w_pixel, int x, int y, int layer);
//重载取像素函数，适用于数据区为浮点型的情况
//参数：接受数据区指针，宽度（以像素计），横/纵坐标，层数作为参数
//返回值：返回所取像素指定层数据的double型引用
double& GetPixel(double* data, int w_pixel, int x, int y, int layer);

//RGB转L*a*b*函数
//参数：接受RGBInfo对象的引用作为参数
//返回值：返回指向L*a*b*的int型指针
int* RGB2Lab(RGBInfo& info);
//RGB转L*a*b*过程中需要的函数
double f_Lab(double x);
//L*a*b*转RGB函数
//参数：接受指向L*a*b*数据区的指针，RGB数据区的像素宽度，字节宽度以及像素高度作为参数
//返回值：返回指向RGB数据区的BYTE型指针
//注意：函数不会释放L*a*b*数据区
BYTE* Lab2RGB(int* Lab, int width_pixel, int width_byte, int height);
//L*a*b*转RGB过程中需要的函数
double if_Lab(double x);

//双边滤波函数（L*a*b*空间）
//参数：接受RGBInfo对象的引用，空间域标准差，强度域标准差作为参数
//返回值：返回经过双边滤波的图像的RGBInfo对象
RGBInfo bilateral_Lab(RGBInfo& image, double spa_std_devia, double int_std_devia);
//RGB空间双边滤波函数
RGBInfo bilateral(RGBInfo& image, double spa_std_devia, double int_std_devia);
//量化双边滤波函数，为加快速度而设计（滤波过程在L*a*b*空间进行），比bilateral_Lab快15%左右
RGBInfo bilateral_quan(RGBInfo& image, double spa_std_devia, double int_std_devia);
//一元正态分布密度函数
double norm_dis(double x, double expect, double var);

//交换RGB图像数据区B和R分量的位置，使之在内存中按R,G,B的顺序存储
//参数：接受RGBInfo对象的引用作为参数
//返回值：返回B和R分量位置经过交换的数据区指针
BYTE* swapBR(RGBInfo& image);

#endif