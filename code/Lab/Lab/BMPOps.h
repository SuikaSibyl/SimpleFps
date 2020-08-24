#ifndef _BMP_H_
#define _BMP_H_
#include<cstdio>
#include<cmath>

//(c)2020,Gordon Freeman
//BMPOps.h    Ver.1.02

//���峣��
#define pi 3.1415926

//��������
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

//�ṹ������
class BITMAPFILEHEADER {
public:
	WORD bfType = 0x4D42;		//�ļ����ͣ�����BMP�ļ�����ֵ��Ϊ"BM",ʮ������ֵΪ4D42��
	DWORD bfSize;				//�ļ���С���ֽڣ�����ʹ�õ�ɫ��ʱ����ֵΪ��������С + 54.
	WORD bfReserved1 = 0;		//�����֣�����
	WORD bfReserved2 = 0;		//�����֣�����
	DWORD bfOffBits;			//����������ļ���ʼλ�õ�ƫ��������ʹ�õ�ɫ��ʱ����ֵΪ54.
	//Ĭ�Ϲ��캯�������й̶�ֵ�ĳ�Ա�⣬���г�Ա������ʼ��Ϊ0
	BITMAPFILEHEADER() {
		bfSize = 0;
		bfOffBits = 0;
	}
};
class BITMAPINFOHEADER {
public:
	DWORD biSize = 40;			//���ṹ��С����Windows����Ϊ40���ֽڣ���
	LONG biWidth;				//ͼ���ȣ����أ�
	LONG biHeight;				//ͼ��߶ȣ����أ�
	WORD biPlanes = 1;			//λͼƽ������ʼ��Ϊ1.
	WORD biBitCount;			//��ɫλ��
	DWORD biCompression;		//ͼ�����ݵ�ѹ�����ͣ�0��ʾ��ѹ����
	DWORD biSizeImage;			//��������С���ֽڣ�����ʱ���ʵ���������Ĵ�С��2.
	LONG biXPelsPerMeter;		//���������ܶȣ���λΪ����/�ס�
								//����ͼ��������ͼ�񣬴���ͨ��Ϊ3780��PS������ͼ�񣬴���ͨ��Ϊ2834.
	LONG biYPelsPerMeter;		//���������ܶȣ���λΪ����/�ס�
	DWORD biClrUsed;			//BMPͼ��ʹ�õĵ�ɫ���е���ɫ��Ŀ��0��ʾʹ��ȫ����ɫ��
	DWORD biClrImportant;		//��Ҫ����ɫ��Ŀ��0��ʾ������ɫ����Ҫ��
	//Ĭ�Ϲ��캯�������й̶�ֵ�ĳ�Ա�⣬���г�Ա������ʼ��Ϊ0
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
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
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
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	RGBInfo() {
		data = NULL;
		width_byte = 0;
	}
	//����ÿ�е��ֽ���
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
	//Ĭ�Ϲ��캯�������г�Ա������ʼ��Ϊ0
	GSInfo() {
		palette = NULL;
		data = NULL;
		width_byte = 0;
	}
	//����ÿ�е��ֽ���
	void calcByteWidth() {
		width_byte = (InfoHeader.biWidth * InfoHeader.biBitCount + 31) / 32 * 4;
	}
};

//����ԭ������
RGBInfo imread_RGB(const char name[]);
GSInfo imread_GS(const char name[]);
void imwrite(RGBInfo info, const char name[]);
void imwrite(GSInfo info, const char name[]);

//ȡ���غ���
//����������RGBͼ����Ϣ��ͼ����ϢӦΪ������ͬ�����ã��ᡢ�������Լ�������Ϊ����
//����ֵ��������ȡ����ָ�������ݵ�����
//ע�⣺RGBֵ���������B,G,R��˳��洢
//     ������Ϊ�������е�һ�����ص�����Ϊ(1,1)
BYTE& GetPixel(RGBInfo& info, int x, int y, int layer);
//����ȡ���غ�����������ֻ����RGB�����������
//����������������ָ�룬��ȣ����ֽڼƣ�����/�����꣬������Ϊ����
//����ֵ��������ȡ����ָ�������ݵ�BYTE������
BYTE& GetPixel(BYTE* data, int w_byte, int x, int y, int layer);
//����ȡ���غ�����������������Ϊ���͵����
//����������������ָ�룬��ȣ������ؼƣ�����/�����꣬������Ϊ����
//����ֵ��������ȡ����ָ�������ݵ�int������
int& GetPixel(int* data, int w_pixel, int x, int y, int layer);
//����ȡ���غ�����������������Ϊ�����͵����
//����������������ָ�룬��ȣ������ؼƣ�����/�����꣬������Ϊ����
//����ֵ��������ȡ����ָ�������ݵ�double������
double& GetPixel(double* data, int w_pixel, int x, int y, int layer);

//RGBתL*a*b*����
//����������RGBInfo�����������Ϊ����
//����ֵ������ָ��L*a*b*��int��ָ��
int* RGB2Lab(RGBInfo& info);
//RGBתL*a*b*��������Ҫ�ĺ���
double f_Lab(double x);
//L*a*b*תRGB����
//����������ָ��L*a*b*��������ָ�룬RGB�����������ؿ�ȣ��ֽڿ���Լ����ظ߶���Ϊ����
//����ֵ������ָ��RGB��������BYTE��ָ��
//ע�⣺���������ͷ�L*a*b*������
BYTE* Lab2RGB(int* Lab, int width_pixel, int width_byte, int height);
//L*a*b*תRGB��������Ҫ�ĺ���
double if_Lab(double x);

//˫���˲�������L*a*b*�ռ䣩
//����������RGBInfo��������ã��ռ����׼�ǿ�����׼����Ϊ����
//����ֵ�����ؾ���˫���˲���ͼ���RGBInfo����
RGBInfo bilateral_Lab(RGBInfo& image, double spa_std_devia, double int_std_devia);
//RGB�ռ�˫���˲�����
RGBInfo bilateral(RGBInfo& image, double spa_std_devia, double int_std_devia);
//����˫���˲�������Ϊ�ӿ��ٶȶ���ƣ��˲�������L*a*b*�ռ���У�����bilateral_Lab��15%����
RGBInfo bilateral_quan(RGBInfo& image, double spa_std_devia, double int_std_devia);
//һԪ��̬�ֲ��ܶȺ���
double norm_dis(double x, double expect, double var);

//����RGBͼ��������B��R������λ�ã�ʹ֮���ڴ��а�R,G,B��˳��洢
//����������RGBInfo�����������Ϊ����
//����ֵ������B��R����λ�þ���������������ָ��
BYTE* swapBR(RGBInfo& image);

#endif