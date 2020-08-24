#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "irrklang/irrKlang.h"
using namespace irrklang;

#include "Obj.h"


namespace AudioManager
{
	class audioManager
	{
	public:
		audioManager()
		{
			SoundEngine = createIrrKlangDevice();
		}
		~audioManager()
		{
			SoundEngine->drop();
		}
		void insertPath(string index, string path)
		{
			pathArchive[index] = path;
		}
		void play(string index, bool repeat)
		{
			if (repeat == true)
				SoundEngine->play2D(pathArchive[index].c_str(), GL_TRUE);
			else
				SoundEngine->play2D(pathArchive[index].c_str(), GL_FALSE);
		}
	private:
		ISoundEngine* SoundEngine;
		map<string, string> pathArchive;
	};
}

namespace ModelManager
{
	class modelManager
	{
	public:
		modelManager(){}
		~modelManager(){}

		void insertModel(string name, string path)
		{
			OBJ* temp = new OBJ(path);
			modelArchive[name] = temp;
			temp->reader();
		}

		void drawModel(string name)
		{
			OBJ* temp = modelArchive[name];
			glBindVertexArray(temp->VAO_OBJ());
			glDrawElements(GL_TRIANGLES, temp->get_index_num(), GL_UNSIGNED_INT, 0);
		}

	private:
		map<string, OBJ*> modelArchive;
	};
}
#endif // !_RESOURCE_MANAGER_
