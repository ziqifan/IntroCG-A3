#include "Transform.h"
#include "IO.h"

Transform::Transform()
	: color(vec4(1.0f))
{
}

void Transform::setPosition(vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setRotationAngleX(float newAngle)
{
	m_pRotX = newAngle;
}

void Transform::setRotationAngleY(float newAngle)
{
	m_pRotY = newAngle;
}

void Transform::setRotationAngleZ(float newAngle)
{
	m_pRotZ = newAngle;
}

void Transform::setScale(float newScale)
{
	m_pScale = newScale;
}

vec3 Transform::getPosition()
{
	return m_pLocalPosition;
}

float Transform::getRotationAngleY()
{
	return m_pRotY;
}

mat4 Transform::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix

	mat4 rx; 
	mat4 ry; 
	mat4 rz; 

	rx.RotateX(m_pRotX);
	ry.RotateY(m_pRotY);
	rz.RotateZ(m_pRotZ);
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	mat4 tran; 
	tran.Translate(m_pLocalPosition);

	// Create scale matrix
	mat4 scale; 
	scale.Scale(m_pScale);

	// Combine all above transforms into a single matrix
	m_pLocalToWorldMatrix = tran * m_pLocalRotation * scale;
}

void Transform::draw()
{
	SAT_DEBUG_LOG("DRAWING TRANSFORM INSTEAD OF GAMEOBJECT");
}
