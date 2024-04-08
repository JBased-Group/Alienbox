#include "cbase.h"
#include "c_squirrel_entity.h"

extern ISquirrel* g_pSquirrel;

template <>
static inline bool ConvertToCpp<RenderableInstance_t*>(SquirrelScript script, RenderableInstance_t** valOut, int a)
{
    return g_pSquirrel->GetStackUserPtr(script, a, (void**)valOut);
}


int C_SquirrelEntity::YouForgotToImplementOrDeclareClientClass() 
{
    return 0;
} 
ClientClass* C_SquirrelEntity::GetClientClass() 
{
    return clientclass;
} 

C_SquirrelEntity::C_SquirrelEntity() : BaseClass()
{
}

C_SquirrelEntity::~C_SquirrelEntity()
{
}

void C_SquirrelEntity::Spawn()
{
    g_pSquirrel->CallObjectFunction(script, obj, "Spawn", "");
}

int C_SquirrelEntity::DrawModel(int flags, const RenderableInstance_t& instance)
{
    return g_pSquirrel->CallObjectFunction(script, obj, "DrawModel","iu", flags, instance).val_int;
}

bool C_SquirrelEntity::GetRenderData(void* pData, ModelDataCategory_t nCategory)
{
    *(RenderableLightingModel_t*)pData = LIGHTING_MODEL_STANDARD;
    return true;
}

bool C_SquirrelEntity::SetupBones(matrix3x4a_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
    if (!pBoneToWorldOut)
    {
        return false;
    }
    if (nMaxBones == 0)
    {
        return false;
    }
    *pBoneToWorldOut = RenderableToWorldTransform();
    return true;
}
