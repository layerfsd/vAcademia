//****************************************************************************//
// coremodel.cpp                                                              //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#include "StdAfx.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "coremodel.h"
#include "error.h"
#include "coreskeleton.h"
#include "coreanimation.h"
#include "coremesh.h"
#include "corematerial.h"
#include "facedesc.h"
#include "loader.h"
#include "saver.h"
#include "memory_leak.h"

 /*****************************************************************************/
/** Constructs the core model instance.
  *
  * This function is the default constructor of the core model instance.
  *****************************************************************************/

CalCoreModel::CalCoreModel():
	MP_STRING_INIT(m_strName),
	MP_VECTOR_INIT(m_vectorCoreAnimation),
	MP_VECTOR_INIT(m_vectorCoreMesh),
	MP_VECTOR_INIT(m_vectorCoreMaterial),
	MP_MAP_INIT(m_mapmapCoreMaterialThread)
{
  m_pCoreSkeleton = 0;
  m_userData = 0;
  m_pCoreFace = NULL;
}

 /*****************************************************************************/
/** Destructs the core model instance.
  *
  * This function is the destructor of the core model instance.
  *****************************************************************************/

CalCoreModel::~CalCoreModel()
{
  assert(m_vectorCoreAnimation.empty());
  assert(m_vectorCoreMesh.empty());
  assert(m_vectorCoreMaterial.empty());
}

 /*****************************************************************************/
/** Adds a core animation.
  *
  * This function adds a core animation to the core model instance.
  *
  * @param pCoreAnimation A pointer to the core animation that should be added.
  *
  * @return One of the following values:
  *         \li the assigned animation \b ID of the added core animation
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::addCoreAnimation(CalCoreAnimation *pCoreAnimation)
{
  // get the id of the core animation
  int animationId;
  animationId = m_vectorCoreAnimation.size();

  m_vectorCoreAnimation.push_back(pCoreAnimation);

  return animationId;
}

 /*****************************************************************************/
/** Adds a core material.
  *
  * This function adds a core material to the core model instance.
  *
  * @param pCoreMaterial A pointer to the core material that should be added.
  *
  * @return One of the following values:
  *         \li the assigned material \b ID of the added core material
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::addCoreMaterial(CalCoreMaterial *pCoreMaterial)
{
  // get the id of the core material
  int materialId;
  materialId = m_vectorCoreMaterial.size();

  m_vectorCoreMaterial.push_back(pCoreMaterial);

  return materialId;
}

 /*****************************************************************************/
/** Adds a core mesh.
  *
  * This function adds a core mesh to the core model instance.
  *
  * @param pCoreMesh A pointer to the core mesh that should be added.
  *
  * @return One of the following values:
  *         \li the assigned mesh \b ID of the added core material
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::addCoreMesh(CalCoreMesh *pCoreMesh)
{
  // get the id of the core mesh
  int meshId;
  meshId = m_vectorCoreMesh.size();

  m_vectorCoreMesh.push_back(pCoreMesh);

  return meshId;
}

 /*****************************************************************************/
/** Creates the core model instance.
  *
  * This function creates the core model instance.
  *
  * @param strName A string that should be used as the name of the core model
  *                instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::create(const std::string& strName)
{
  m_strName = strName;

  return true;
}

 /*****************************************************************************/
/** Creates a core material thread.
  *
  * This function creates a new core material thread with the given ID.
  *
  * @param coreMaterialThreadId The ID of the core material thread that should
  *                             be created.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::createCoreMaterialThread(int coreMaterialThreadId)
{
  // insert an empty core material thread with a given id
  MP_MAP_DC<int, int> mapCoreMaterialThreadId;
  MP_MAP_DC_INIT(mapCoreMaterialThreadId);
  m_mapmapCoreMaterialThread.insert(std::make_pair(coreMaterialThreadId, mapCoreMaterialThreadId));

  return true;
}

 /*****************************************************************************/
/** Destroys the core model instance.
  *
  * This function destroys all data stored in the core model instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalCoreModel::destroy()
{
  // destroy all core materials
  std::vector<CalCoreMaterial *>::iterator iteratorCoreMaterial;
  for(iteratorCoreMaterial = m_vectorCoreMaterial.begin(); iteratorCoreMaterial != m_vectorCoreMaterial.end(); ++iteratorCoreMaterial)
  {
    (*iteratorCoreMaterial)->destroy();
    MP_DELETE( *iteratorCoreMaterial);
  }
  m_vectorCoreMaterial.clear();

  //OutputDebugString("2\n");
  // destroy all core meshes
  std::vector<CalCoreMesh *>::iterator iteratorCoreMesh;
  for(iteratorCoreMesh = m_vectorCoreMesh.begin(); iteratorCoreMesh != m_vectorCoreMesh.end(); ++iteratorCoreMesh)
  {
    (*iteratorCoreMesh)->destroy();
    MP_DELETE( *iteratorCoreMesh);
  }
  m_vectorCoreMesh.clear();

  //OutputDebugString("3\n");
  // destroy all core animations
/*  std::vector<CalCoreAnimation *>::iterator iteratorCoreAnimation;
  for(iteratorCoreAnimation = m_vectorCoreAnimation.begin(); iteratorCoreAnimation != m_vectorCoreAnimation.end(); ++iteratorCoreAnimation)
  {
    (*iteratorCoreAnimation)->destroy();
    delete (*iteratorCoreAnimation);
  }*/
  m_vectorCoreAnimation.clear();

  //OutputDebugString("4\n");
  if(m_pCoreSkeleton != 0)
  {
    m_pCoreSkeleton->destroy();
    MP_DELETE( m_pCoreSkeleton);
    m_pCoreSkeleton = 0;
  }
  //OutputDebugString("5\n");

  if( m_pCoreFace != NULL)
	  MP_DELETE( m_pCoreFace);

  m_pCoreFace=NULL;
  //OutputDebugString("6\n");
  //m_strName.erase();
  //m_strName = 0;
}

void CalCoreModel::destroy_geometry()
{
  // destroy all core materials
  std::vector<CalCoreMaterial *>::iterator iteratorCoreMaterial;
  for(iteratorCoreMaterial = m_vectorCoreMaterial.begin(); iteratorCoreMaterial != m_vectorCoreMaterial.end(); ++iteratorCoreMaterial)
  {
    (*iteratorCoreMaterial)->destroy();
    MP_DELETE( *iteratorCoreMaterial);
  }
  m_vectorCoreMaterial.clear();

  // destroy all core meshes
  std::vector<CalCoreMesh *>::iterator iteratorCoreMesh;
  for(iteratorCoreMesh = m_vectorCoreMesh.begin(); iteratorCoreMesh != m_vectorCoreMesh.end(); ++iteratorCoreMesh)
  {
    (*iteratorCoreMesh)->destroy();
    MP_DELETE( *iteratorCoreMesh);
  }
  m_vectorCoreMesh.clear();  
}

 /*****************************************************************************/
/** Provides access to a core animation.
  *
  * This function returns the core animation with the given ID.
  *
  * @param coreAnimationId The ID of the core animation that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core animation
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreAnimation *CalCoreModel::getCoreAnimation(int coreAnimationId)
{
  if((coreAnimationId < 0) || (coreAnimationId >= (int)m_vectorCoreAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreAnimation[coreAnimationId];
}

 /*****************************************************************************/
/** Returns the number of core animations.
  *
  * This function returns the number of core animations in the core model
  * instance.
  *
  * @return The number of core animations.
  *****************************************************************************/

int CalCoreModel::getCoreAnimationCount()
{
  return m_vectorCoreAnimation.size();
}

 /*****************************************************************************/
/** Provides access to a core material.
  *
  * This function returns the core material with the given ID.
  *
  * @param coreMaterialId The ID of the core material that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core material
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMaterial *CalCoreModel::getCoreMaterial(int coreMaterialId)
{
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreMaterial[coreMaterialId];
}

 /*****************************************************************************/
/** Returns the number of core materials.
  *
  * This function returns the number of core materials in the core model
  * instance.
  *
  * @return The number of core materials.
  *****************************************************************************/

int CalCoreModel::getCoreMaterialCount()
{
  return m_vectorCoreMaterial.size();
}

 /*****************************************************************************/
/** Returns a specified core material ID.
  *
  * This function returns the core material ID for a specified core material
  * thread / core material set pair.
  *
  * @param coreMaterialThreadId The ID of the core material thread.
  * @param coreMaterialSetId The ID of the core material set.
  *
  * @return One of the following values:
  *         \li the \b ID of the core material
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::getCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId)
{
  // find the core material thread
  MP_MAP<int, MP_MAP_DC<int, int> >::iterator iteratorCoreMaterialThread;
  iteratorCoreMaterialThread = m_mapmapCoreMaterialThread.find(coreMaterialThreadId);
  if(iteratorCoreMaterialThread == m_mapmapCoreMaterialThread.end())
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // get the core material thread
  std::map<int, int>& coreMaterialThread = (*iteratorCoreMaterialThread).second;

  // find the material id for the given set
  std::map<int, int>::iterator iteratorSet;
  iteratorSet = coreMaterialThread.find(coreMaterialSetId);
  if(iteratorSet == coreMaterialThread.end())
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  return (*iteratorSet).second;
}

 /*****************************************************************************/
/** Provides access to a core mesh.
  *
  * This function returns the core mesh with the given ID.
  *
  * @param coreMeshId The ID of the core mesh that should be returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core mesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreMesh *CalCoreModel::getCoreMesh(int coreMeshId)
{
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return 0;
  }

  return m_vectorCoreMesh[coreMeshId];
}

 /*****************************************************************************/
/** Returns the number of core meshes.
  *
  * This function returns the number of core meshes in the core model instance.
  *
  * @return The number of core meshes.
  *****************************************************************************/

int CalCoreModel::getCoreMeshCount()
{
  return m_vectorCoreMesh.size();
}

 /*****************************************************************************/
/** Provides access to the core skeleton.
  *
  * This function returns the core skeleton.
  *
  * @return One of the following values:
  *         \li a pointer to the core skeleton
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSkeleton *CalCoreModel::getCoreSkeleton()
{
  return m_pCoreSkeleton;
}

 /*****************************************************************************/
/** Provides access to the user data.
  *
  * This function returns the user data stored in the core model instance.
  *
  * @return The user data stored in the core model instance.
  *****************************************************************************/

Cal::UserData CalCoreModel::getUserData()
{
  return m_userData;
}

 /*****************************************************************************/
/** Loads a core animation.
  *
  * This function loads a core animation from a file.
  *
  * @param strFilename The file from which the core animation should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core animation
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::loadCoreAnimation(const std::string& strFilename)
{
  // the core skeleton has to be loaded already
  if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core animation
  CalLoader loader;
  CalCoreAnimation *pCoreAnimation;
  pCoreAnimation = loader.loadCoreAnimation(strFilename);
  if(pCoreAnimation == 0) return -1;

  // add core animation to this core model
  int animationId;
  animationId = addCoreAnimation(pCoreAnimation);
  if(animationId == -1)
  {
    MP_DELETE( pCoreAnimation);
    return -1;
  }

  return animationId;
}

int CalCoreModel::loadCoreAnimation(void *apData, DWORD adwSize)
{
  // the core skeleton has to be loaded already
  if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }

  // load a new core animation
  CalLoader loader;
  CalCoreAnimation *pCoreAnimation;
  pCoreAnimation = loader.loadCoreAnimation( apData, adwSize);
  if(pCoreAnimation == 0) return -1;

  // add core animation to this core model
  int animationId;
  animationId = addCoreAnimation(pCoreAnimation);
  if(animationId == -1)
  {
    MP_DELETE( pCoreAnimation);
    return -1;
  }

  return animationId;
}

 /*****************************************************************************/
/** Loads a core material.
  *
  * This function loads a core material from a file.
  *
  * @param strFilename The file from which the core material should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core material
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::loadCoreMaterial(const std::string& strFilename)
{
  // the core skeleton has to be loaded already
  /*if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }*/

  // load a new core material
  CalLoader loader;
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = loader.loadCoreMaterial(strFilename);
  if(pCoreMaterial == 0) return -1;

  // add core material to this core model
  int materialId;
  materialId = addCoreMaterial(pCoreMaterial);
  if(materialId == -1)
  {
    MP_DELETE( pCoreMaterial);
    return -1;
  }

  return materialId;
}

int CalCoreModel::loadFakeMaterial(const char* textureName)
{
  // load a new core material
  CalLoader loader;
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = loader.loadFakeMaterial(textureName);
  if(pCoreMaterial == 0) return -1;

  // add core material to this core model
  int materialId;
  materialId = addCoreMaterial(pCoreMaterial);
  if(materialId == -1)
  {
    MP_DELETE( pCoreMaterial);
    return -1;
  }

  return materialId;
}

int CalCoreModel::loadCoreMaterial(void *apData, DWORD adwSize)
{
  // load a new core material
  CalLoader loader;
  CalCoreMaterial *pCoreMaterial;
  pCoreMaterial = loader.loadCoreMaterial(apData, adwSize);
  if(pCoreMaterial == 0) return -1;

  // add core material to this core model
  int materialId;
  materialId = addCoreMaterial(pCoreMaterial);
  if(materialId == -1)
  {
    MP_DELETE( pCoreMaterial);
    return -1;
  }

  return materialId;
}

 /*****************************************************************************/
/** Loads a core mesh.
  *
  * This function loads a core mesh from a file.
  *
  * @param strFilename The file from which the core mesh should be loaded from.
  *
  * @return One of the following values:
  *         \li the assigned \b ID of the loaded core mesh
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalCoreModel::loadCoreMesh(const std::string& strFilename)
{
  // the core skeleton has to be loaded already
  /*if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return -1;
  }*/

  // load a new core mesh
  CalLoader loader;
  CalCoreMesh *pCoreMesh;
  pCoreMesh = loader.loadCoreMesh(strFilename);
  if(pCoreMesh == 0) return -1;

  // add core mesh to this core model
  int meshId;
  meshId = addCoreMesh(pCoreMesh);
  if(meshId == -1)
  {
    MP_DELETE( pCoreMesh);
    return -1;
  }

  return meshId;
}

int CalCoreModel::loadCoreMesh(void *apData, DWORD adwSize)
{
  // load a new core mesh
  CalLoader loader;
  CalCoreMesh *pCoreMesh;
  pCoreMesh = loader.loadCoreMesh(apData, adwSize);
  if(pCoreMesh == 0) return -1;

  // add core mesh to this core model
  int meshId;
  meshId = addCoreMesh(pCoreMesh);
  if(meshId == -1)
  {
    MP_DELETE( pCoreMesh);
    return -1;
  }

  return meshId;
}

 /*****************************************************************************/
/** Loads the core skeleton.
  *
  * This function loads the core skeleton from a file.
  *
  * @param strFilename The file from which the core skeleton should be loaded
  *                    from.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::loadCoreSkeleton(const std::string& strFilename)
{
  // destroy the current core skeleton
  if(m_pCoreSkeleton != 0)
  {
    m_pCoreSkeleton->destroy();
    MP_DELETE( m_pCoreSkeleton);
  }

  // load a new core skeleton
  CalLoader loader;
  m_pCoreSkeleton = loader.loadCoreSkeleton(strFilename);
  if(m_pCoreSkeleton == 0) return false;

  return true;
}

bool CalCoreModel::loadCoreSkeleton(void *apData, DWORD adwSize)
{
  // destroy the current core skeleton
  if(m_pCoreSkeleton != 0)
  {
    m_pCoreSkeleton->destroy();
    MP_DELETE( m_pCoreSkeleton);
  }

  // load a new core skeleton
  CalLoader loader;
  m_pCoreSkeleton = loader.loadCoreSkeleton(apData, adwSize);
  if(m_pCoreSkeleton == 0) return false;

  return true;
}

 /*****************************************************************************/
/** Saves a core animation.
  *
  * This function saves a core animation to a file.
  *
  * @param strFilename The file to which the core animation should be saved to.
  * @param coreAnimationId The ID of the core animation that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::saveCoreAnimation(const std::string& strFilename, int coreAnimationId)
{
  // check if the core animation id is valid
  if((coreAnimationId < 0) || (coreAnimationId >= (int)m_vectorCoreAnimation.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  CalSaver saver;
  if(!saver.saveCoreAnimation(strFilename, m_vectorCoreAnimation[coreAnimationId]))
  {
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core material.
  *
  * This function saves a core material to a file.
  *
  * @param strFilename The file to which the core material should be saved to.
  * @param coreMaterialId The ID of the core material that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::saveCoreMaterial(const std::string& strFilename, int coreMaterialId)
{
  // check if the core material id is valid
  if((coreMaterialId < 0) || (coreMaterialId >= (int)m_vectorCoreMaterial.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  CalSaver saver;
  if(!saver.saveCoreMaterial(strFilename, m_vectorCoreMaterial[coreMaterialId]))
  {
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves a core mesh.
  *
  * This function saves a core mesh to a file.
  *
  * @param strFilename The file to which the core mesh should be saved to.
  * @param coreMeshId The ID of the core mesh that should be saved.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::saveCoreMesh(const std::string& strFilename, int coreMeshId)
{
  // check if the core mesh id is valid
  if((coreMeshId < 0) || (coreMeshId >= (int)m_vectorCoreMesh.size()))
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core animation
  CalSaver saver;
  if(!saver.saveCoreMesh(strFilename, m_vectorCoreMesh[coreMeshId]))
  {
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Saves the core skeleton.
  *
  * This function saves the core skeleton to a file.
  *
  * @param strFilename The file to which the core skeleton should be saved to.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::saveCoreSkeleton(const std::string& strFilename)
{
  // check if we have a core skeleton in this code model
  if(m_pCoreSkeleton == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // save the core skeleton
  CalSaver saver;
  if(!saver.saveCoreSkeleton(strFilename, m_pCoreSkeleton))
  {
    return false;
  }

  return true;
}

 /*****************************************************************************/
/** Sets a core material ID.
  *
  * This function sets a core material ID for a core material thread / core
  * material set pair.
  *
  * @param coreMaterialThreadId The ID of the core material thread.
  * @param coreMaterialSetId The ID of the core maetrial set.
  * @param coreMaterialId The ID of the core maetrial.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreModel::setCoreMaterialId(int coreMaterialThreadId, int coreMaterialSetId, int coreMaterialId)
{
  // find the core material thread
  MP_MAP<int, MP_MAP_DC<int, int> >::iterator iteratorCoreMaterialThread;
  iteratorCoreMaterialThread = m_mapmapCoreMaterialThread.find(coreMaterialThreadId);
  if(iteratorCoreMaterialThread == m_mapmapCoreMaterialThread.end())
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  // get the core material thread
  std::map<int, int>& coreMaterialThread = (*iteratorCoreMaterialThread).second;

  // remove a possible entry in the core material thread
  coreMaterialThread.erase(coreMaterialSetId);

  // set the given set id in the core material thread to the given core material id
  coreMaterialThread.insert(std::make_pair(coreMaterialSetId, coreMaterialId));

  return true;
}

 /*****************************************************************************/
/** Stores user data.
  *
  * This function stores user data in the core model instance.
  *
  * @param userData The user data that should be stored.
  *****************************************************************************/

void CalCoreModel::setUserData(Cal::UserData userData)
{
  m_userData = userData;
}

int CalCoreModel::loadCoreFace(const std::string& strFilename)
{
	CalLoader loader;
	m_pCoreFace = loader.loadFace(strFilename);
	if(m_pCoreFace == 0)
		return -1;
	return 0;
}

int CalCoreModel::loadCoreFace(void *apData, DWORD adwSize)
{
	CalLoader loader;
	m_pCoreFace = loader.loadFace( apData, adwSize);
	if(m_pCoreFace == 0)
		return -1;
	return 0;
}
//****************************************************************************//


int CalCoreModel::attachCoreAnimation(CalCoreAnimation *pAnim)
{
	// get the id of the core animation
	for(unsigned int i=0; i < m_vectorCoreAnimation.size(); i++)
		if( m_vectorCoreAnimation[i] == pAnim) 	return i;

  int animationId = m_vectorCoreAnimation.size();
  m_vectorCoreAnimation.push_back(pAnim);
  return animationId;
}

void CalCoreModel::detachCoreAnimation(CalCoreAnimation *pAnim)
{
	std::vector<CalCoreAnimation *>::iterator it = m_vectorCoreAnimation.begin();
	std::vector<CalCoreAnimation *>::iterator itEnd = m_vectorCoreAnimation.end();

	for ( ; it != itEnd; it++)
	if (*it == pAnim)
	{
		*it = NULL;
	}
}

void	CalCoreModel::get_worldTranslation(CalVector& trn)
{
	if( m_vectorCoreMesh.size()>0)	trn = m_vectorCoreMesh[0]->get_worldTranslation();
	else	trn = CalVector(0,0,0);
}

void	CalCoreModel::get_worldRotation(CalQuaternion& rtn)
{
	if( m_vectorCoreMesh.size()>0)	rtn =m_vectorCoreMesh[0]->get_worldRotation();
	else rtn = CalQuaternion(0,0,0,1);
}