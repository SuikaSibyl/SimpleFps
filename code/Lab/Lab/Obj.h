#ifndef _OBJREADER_
#define _OBJREADER_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class OBJ
{
public:
	OBJ(string s) :filename(s) {	}
	void reader();//载入obj文件中的顶点、索引的信息
	GLuint VAO_OBJ();
	int get_index_num();
	int get_vertex_num();
private:
	string filename;
	vector <float> vertex;
	vector <int> index;
};

void obj_writer(float vertex[],int num);

#endif // !_OBJREADER_