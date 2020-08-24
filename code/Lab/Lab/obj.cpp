#include "Obj.h"
int obj_filenum;
using namespace std;

void OBJ::reader() {
	ifstream fin(filename);
	//判断文件是否打开成功
	if (!fin.is_open()) {
		cout << "open file error" << endl;
		return;
	}
	string type; //记录类型（v\vt\vn\f...）
	fin >> type;
	do {
		//记录顶点数据
		if (type == "v") {
			float temp;
			for (int i = 1; i <= 3; i++) {
                fin >> temp;
			    vertex.push_back(temp);
			}
		}
		//记录每个面的顶点顺序
		else if (type == "f") {
			string str;
			getline(fin, str);

			//提取索引信息
			int i = 0;
			while (i < str.length() - 1) {
				while (str[i++] != ' ') {
					if (i == str.length()) break;
				}
				if (i == str.length()) break;
				int num = 0;
				while (str[i] != '/') {
					num = num * 10 + str[i] - '0';
					i++;
				}
				//记录索引信息
				index.push_back(num);
			}

		}
		fin >> type;
	} while (!fin.eof());

	fin.close();
}

GLuint OBJ::VAO_OBJ() {

    float *vertices = NULL;
	vertices = new float[vertex.size()];
	vector <float> ::iterator p;
	int i = 0;
	for (p = vertex.begin(); p < vertex.end(); p++) {
		vertices[i] = *p;
		i++;
	}

	unsigned int *indices = NULL;
	indices = new unsigned int[index.size()];
	vector <int> ::iterator q;
	int j = 0;
	for (q = index.begin(); q < index.end(); q++) {
		indices[j] = *q-1;
		j++;
	}

	GLuint VAOid, VBOid, EBOid;
	//step1
	glGenVertexArrays(1, &VAOid);
	glBindVertexArray(VAOid);

	//step2
	glGenBuffers(1, &VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex.size(), vertices, GL_STATIC_DRAW);

	//step3
	glGenBuffers(1, &EBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*index.size(), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	delete[] vertices;
	delete[] indices;

	return VAOid;
}
int OBJ::get_index_num() {
	return index.size();
}

int OBJ::get_vertex_num() {
	return vertex.size();
}

void obj_writer(float vertex[],int num) {
	ofstream fout;
	string filename = "obj\\obj_output" + to_string(obj_filenum) + ".obj";
	fout.open(filename);
	for (int i = 0; i < num; i=i+3) {
		fout << "v " << vertex[i] << " " << vertex[i + 1] << " " << vertex[i + 2] << endl;
	}
	for (int i = 1; i <= num/3; i++) {
        if (i % 3 == 1) fout << endl << "f";
		fout << " " << i;
	}
	fout.close();
	obj_filenum++;
}