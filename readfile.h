#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;


struct Material
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;
	int textureIdx;
};

struct Vertex
{
	float x,y,z;
};

struct Coordinate
{
	float x,y;
};

struct Triangle
{
	 unsigned int vi1,ti1,ni1,vi2,ti2,ni2,vi3,ti3,ni3;
};

struct Model{
public:
	int TriangleNum;
	int MaterialIdx;
	vector<Triangle> vecTriangle;
};

class RoseFile
{
public:

	string name;
	int textureNum;
	vector<string> vecTexture;

	int MaterialNum;
	vector<Material> vecMaterial;

	int VertexNum;
	vector<Vertex> vecVertex;

	int CoordinateNum;
	vector<Coordinate> vecCoordinate;

	//法线
	int NormalNum;
	vector<Vertex> vecNormal;

	int ModelNum;
	float Scale[3];
	vector<Model> vecModel;
	GLfloat xmin,xmax,ymin,ymax,zmin,zmax;
public:
	RoseFile(char *fileName)
	{
		xmin = 0;xmax = 0;
		ymin = 0;ymax = 0;
		zmin = 0;zmax = 0;
		name = fileName;
		ifstream fileIn(fileName);
		fileIn>>textureNum;
		for (int i = 0;i<textureNum;++i)
		{
			string wenli;
			fileIn>>wenli;
			vecTexture.push_back(wenli);
		}
		fileIn>>MaterialNum;
		for (int i = 0;i<MaterialNum;++i)
		{
			Material C;
			fileIn>>C.ambient[0]>>C.ambient[1]>>C.ambient[2]>>C.ambient[3];
			fileIn>>C.diffuse[0]>>C.diffuse[1]>>C.diffuse[2]>>C.diffuse[3];
			fileIn>>C.specular[0]>>C.specular[1]>>C.specular[2]>>C.specular[3];
			fileIn>>C.emission[0]>>C.emission[1]>>C.emission[2]>>C.emission[3];
			fileIn>>C.shininess>>C.textureIdx;
			vecMaterial.push_back(C);
		}
		fileIn>>VertexNum;
		for (int i = 0;i<VertexNum;++i)
		{
			Vertex D;
			fileIn>>D.x>>D.y>>D.z;
			if (D.x>xmax)xmax = D.x;
			else if(D.x<xmin) xmin = D.x;
			if (D.y>ymax)ymax = D.y;
			else if(D.y<ymin) ymin = D.y;
			if (D.z>zmax)zmax = D.z;
			else if(D.z<zmin) zmin = D.z;
			vecVertex.push_back(D);
		}

		fileIn>>CoordinateNum;
		for (int i = 0;i<CoordinateNum;++i)
		{
			Coordinate Z;
			fileIn>>Z.x>>Z.y;
			vecCoordinate.push_back(Z);
		}

		fileIn>>NormalNum;
		for (int i = 0;i<NormalNum;++i)
		{
			Vertex FX;
			fileIn>>FX.x>>FX.y>>FX.z;
			vecNormal.push_back(FX);
		}

		fileIn>>ModelNum;
		fileIn>>Scale[0]>>Scale[1]>>Scale[2];
		for (int i = 0;i<ModelNum;++i)
		{
			Model MF;
			fileIn>>MF.TriangleNum;
			fileIn>>MF.MaterialIdx;
			for (int j = 0;j<MF.TriangleNum;++j)
			{
				Triangle SJX;
				fileIn>>SJX.vi1>>SJX.ti1>>SJX.ni1;
				fileIn>>SJX.vi2>>SJX.ti2>>SJX.ni2;
				fileIn>>SJX.vi3>>SJX.ti3>>SJX.ni3;
				MF.vecTriangle.push_back(SJX);
			}
			vecModel.push_back(MF);
		}
	}
};

class Images
{
public:
	string name;
	GLint ImageWidth, ImageHeight, PixelLength;
	GLubyte *pixelDatas;

	void loadPixel()   //读入图像文件
	{
		FILE *pFile;
		pFile = fopen(name.c_str(), "r");
		if(pFile == 0)
		{
			printf("picture open error\n");
			exit(0);
		}
		fseek(pFile, 0x0012, SEEK_SET);   //从第18个字节开始的8个字节存储图像的宽和高！！！！
		fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
		fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);
		ImageWidth = abs(ImageWidth);
		ImageHeight = abs(ImageHeight);
		// 计算像素数据长度
		PixelLength = ImageWidth * 3;   //一个像素有R、G、B成分，所以乘以3
		while( PixelLength % 4 != 0 )  //四字节对齐
			++PixelLength;
		PixelLength *= ImageHeight;

		// 读取像素数据
		pixelDatas = (GLubyte*)malloc(PixelLength);
		if( pixelDatas == 0 )
			exit(0);
		fseek(pFile, 54, SEEK_SET);  //从第54个字节往后是图像的真正内容
		fread(pixelDatas, PixelLength, 1, pFile);
		// 关闭文件
		fclose(pFile);
	}
};