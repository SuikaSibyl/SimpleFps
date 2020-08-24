#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Texture
{
public:
    unsigned int ID;
    // constructor generates the texture
    // ------------------------------------------------------------------------
    Texture(const char* imgPaht)
    {
        // 1. load a texture 
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // 2. set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 3. set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // 4. load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(imgPaht, &width, &height, &nrChannels, 0);
       
        // 5. create the texture
        if (data)
        {
            if (nrChannels == 3)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else if (nrChannels == 4)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        // 5. free the imae
        stbi_image_free(data);

        ID = texture;
    }

private:
};

class CubeTexture
{
public:
    unsigned int ID;
    // constructor generates the texture
    // ------------------------------------------------------------------------
    CubeTexture(string Path)
    {
        // 1. load a texture 
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        // 4. load image, create texture and generate mipmaps
        vector<string> textures_faces;
        textures_faces.push_back(Path + string("/right.bmp"));
        textures_faces.push_back(Path + string("/left.bmp"));
        textures_faces.push_back(Path + string("/top.bmp"));
        textures_faces.push_back(Path + string("/bottom.bmp"));
        textures_faces.push_back(Path + string("/front.bmp"));
        textures_faces.push_back(Path + string("/back.bmp"));


        int width, height, nrChannels;
        unsigned char* data;
        for (unsigned int i = 0; i < textures_faces.size(); i++)
        {
            //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
            unsigned char* data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << textures_faces[i] << std::endl;
                stbi_image_free(data);
            }
        }

        // 2. set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // 3. set texture filtering parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        ID = texture;
    }

private:
};
#endif