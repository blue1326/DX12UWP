#include "pch.h"
#include "FbxLoader.h"

CFbxLoader::CFbxLoader()
{
	
	m_pMgr = FbxManager::Create();
	//m_Mgr = make_unique<FbxManager>(FbxManager::Create()) ;
}

CFbxLoader::~CFbxLoader()
{
	m_pMgr->Destroy();
}

