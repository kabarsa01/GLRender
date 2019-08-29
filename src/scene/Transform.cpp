#include "scene/Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

Transform::Transform()
	: Location()
	, Rotation()
	, Scale()
	, Matrix(1.0f)
	, IsDirty(true)
{
}

Transform::~Transform()
{
}

void Transform::SetLocation(const glm::vec3 & InLocation)
{
	this->Location = InLocation;
	MarkDirty();
}

void Transform::SetRotation(const glm::vec3 & InRotation)
{
	this->Rotation = InRotation;
	MarkDirty();
}

void Transform::SetScale(const glm::vec3 & InScale)
{
	this->Scale = InScale;
	MarkDirty();
}

void Transform::MarkDirty()
{
	IsDirty = true;
}

glm::mat4 Transform::GetMatrix() const
{
	if (IsDirty)
	{
		return CalculateMatrix();
	}
	return Matrix;
}

glm::mat4& Transform::GetMatrix()
{
	if (IsDirty)
	{
		Matrix = CalculateMatrix();
		IsDirty = false;
	}
	return Matrix;
}

glm::mat4 Transform::CalculateMatrix() const
{
	glm::mat4 Mat(1.0f);
	glm::translate(Mat, Location);
	glm::rotate(Mat, Rotation[2], glm::vec3(0.0f, 0.0, 1.0f));
	glm::rotate(Mat, Rotation[1], glm::vec3(0.0f, 1.0, 0.0f));
	glm::rotate(Mat, Rotation[0], glm::vec3(1.0f, 0.0, 0.0f));
	glm::scale(Mat, Scale);

	return Mat;
}
