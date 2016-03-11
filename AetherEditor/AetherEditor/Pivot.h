#ifndef _PIVOT_H
#define _PIVOT_H
#include <Vector3.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
class Pivot
{
public:
	Pivot() = default;
	virtual ~Pivot() = default;
	virtual bool Initialize(aetherClass::ViewCamera*) = 0;
	virtual void Render(aetherClass::ShaderBase*) = 0;
	virtual void MoveDirection(aetherClass::Vector3) = 0;
	virtual void SetScale(float length) = 0;
private:
	virtual void Finalize() = 0;

};

#endif