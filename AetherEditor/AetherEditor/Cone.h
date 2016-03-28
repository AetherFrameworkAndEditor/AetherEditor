#ifndef _CONE_H
#define _CONE_H

#include "ModelBase.h"
class Cone :
	public aetherClass::ModelBase
{
public:
	Cone();
	Cone(float);
	~Cone();
private:
	float m_slice;
private:

	bool InitializeBuffers()override;
	void FinalizeBuffers()override;
	void RenderBuffers()override;

};

#endif
