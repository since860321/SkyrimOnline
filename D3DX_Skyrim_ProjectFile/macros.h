#pragma once

//< 매크로 함수

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p) {if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) {if(p) { delete[] (p); (p) = NULL; } }

#define SAFE_RETAIN(p) if(p) { (p)->Retain(); }

#define SYNTHESIZE_RETAIN(varType, varName, funName) \
private: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var) \
{ \
 if (varName != var) \
 { \
 SAFE_RETAIN(var); \
 SAFE_RELEASE(varName); \
 varName = var; \
 } \
} 


#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(varType var){ varName = var; }

#define sgDevice cDevice::GetInstance().GetDevice()
#define NetworkMgr cNetworkMgr::GetInstance()
#define sgFrustum cFrustum::GetInstance()

