#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "data/Resource.h"

class Texture : public Resource
{
public:
	Texture(const char* path, bool InputUsesAlpha);
	virtual ~Texture();

	unsigned int GetID() const;
	unsigned char* GetData() const;
	void Use(GLenum textureUnit) const;
protected:
	unsigned int ID = -1;
	unsigned char* data;
	int width;
	int height;
	int numChannels;
};
