#ifndef FbxLoader_h__
#define FbxLoader_h__
#include "Template.h"

using namespace Engine::Singleton;

class CFbxLoader :public CSingleton<CFbxLoader>
{
public:
	CFbxLoader();
	~CFbxLoader();

private:
	FbxManager* m_pMgr;
	//unique_ptr<FbxManager> m_Mgr;
};


#endif // FbxLoader_h__
