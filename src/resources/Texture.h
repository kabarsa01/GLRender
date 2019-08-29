#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/ObjectBase.h"

class Texture : public ObjectBase
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
