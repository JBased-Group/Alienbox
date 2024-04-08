#pragma once
#include "squirrel/squirrel.h"
#include "c_baseentity.h"

class C_SquirrelEntity : public C_BaseEntity, public IClientModelRenderable
{
public:
	DECLARE_CLASS(C_SquirrelEntity, C_BaseEntity);
	DECLARE_CLIENTCLASS();

	C_SquirrelEntity();
	~C_SquirrelEntity();

	virtual void Spawn();
	virtual int DrawModel(int flags, const RenderableInstance_t& instance);
	virtual bool GetRenderData(void* pData, ModelDataCategory_t nCategory);
	IClientModelRenderable* GetClientModelRenderable() { return this; }
	bool SetupBones(matrix3x4a_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

	SquirrelObject obj;
	SquirrelScript script;
	ClientClass* clientclass;
};

