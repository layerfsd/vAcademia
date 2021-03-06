//****************************************************************************//
// coreanimation.cpp                                                          //
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

#include "coreanimation.h"
#include "coretrack.h"
#include "skeleton.h"
#include "coreskeleton.h"
#include "corekeyframe.h"
#include "corebone.h"
#include "coreanimationflags.h"
#include "../nengine/ncommon/StringFunctions.h"

 /*****************************************************************************/
/** Constructs the core animation instance.
  *
  * This function is the default constructor of the core animation instance.
  *****************************************************************************/

CalCoreAnimation::CalCoreAnimation():
	MP_STRING_INIT(m_strLock),
	MP_LIST_INIT(m_listCoreTrack),
	MP_VECTOR_INIT(m_listCallbacks)
{
	m_bSaveFirst = false;
	m_bSaveLast = false;
	m_dwKeepLast=0;
	m_strLock="";
	m_bRelative=0;
	m_duration = 1;
	dwFlags=0;
	m_isBVH = false;
	//InitializeCriticalSection(&cs);
}

 /*****************************************************************************/
/** Destructs the core animation instance.
  *
  * This function is the destructor of the core animation instance.
  *****************************************************************************/

CalCoreAnimation::~CalCoreAnimation()
{
  assert(m_listCoreTrack.empty());
  //DeleteCriticalSection(&cs);
}

 /*****************************************************************************/
/** Adds a core track.
  *
  * This function adds a core track to the core animation instance.
  *
  * @param pCoreTrack A pointer to the core track that should be added.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreAnimation::addCoreTrack(CalCoreTrack *pCoreTrack)
{
  m_listCoreTrack.push_back(pCoreTrack);

  return true;
}

bool CalCoreAnimation::removeCoreTrack(DWORD coreBoneId)
{
	// loop through all core bones
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
	{
		// get the core bone
		CalCoreTrack *pCoreTrack;
		pCoreTrack = *iteratorCoreTrack;

		// check if we found the matching core bone
		if(pCoreTrack->getCoreBoneId() == (int)coreBoneId)
		{
			m_listCoreTrack.erase(iteratorCoreTrack);
			return true;
		}
	}

	// no match found
	return false;
}
 /*****************************************************************************/
/** Creates the core animation instance.
  *
  * This function creates the core animation instance.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalCoreAnimation::create()
{
  return true;
}

 /*****************************************************************************/
/** Destroys the core animation instance.
  *
  * This function destroys all data stored in the core animation instance and
  * frees all allocated memory.
  *****************************************************************************/

void CalCoreAnimation::destroy()
{
//EnterCriticalSection(&cs);
  // destroy all core tracks
  while(!m_listCoreTrack.empty())
  {
    CalCoreTrack *pCoreTrack;
    pCoreTrack = m_listCoreTrack.front();
    m_listCoreTrack.pop_front();

    pCoreTrack->destroy();
    MP_DELETE( pCoreTrack);
  }
//LeaveCriticalSection(&cs);
}

 /*****************************************************************************/
/** Provides access to a core track.
  *
  * This function returns the core track for a given bone ID.
  *
  * @param coreBoneId The core bone ID of the core track that should be
  *                   returned.
  *
  * @return One of the following values:
  *         \li a pointer to the core track
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreTrack *CalCoreAnimation::getCoreTrack(int coreBoneId)
{
  // loop through all core bones
  std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
  for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
  {
    // get the core bone
    CalCoreTrack *pCoreTrack;
    pCoreTrack = *iteratorCoreTrack;

    // check if we found the matching core bone
    if(pCoreTrack->getCoreBoneId() == coreBoneId) return pCoreTrack;
  }

  // no match found
  return 0;
}

CalCoreTrack *CalCoreAnimation::getCoreTrack(const char* trackName)
{
	if( !IsCommon())	return NULL;
	std::string	sTrackName = trackName;
	StringToLower(sTrackName);	
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack)
	{
		// get the core bone
		CalCoreTrack *pCoreTrack;
		pCoreTrack = *iteratorCoreTrack;

		std::string	sCoreBoneName = pCoreTrack->getCoreBoneName();
		sCoreBoneName = StringToLower(sCoreBoneName);
		if( sCoreBoneName.find(sTrackName) != -1)	return	pCoreTrack;
	}
	return NULL;
}

 /*****************************************************************************/
/** Returns the duration.
  *
  * This function returns the duration of the core animation instance.
  *
  * @return The duration in seconds.
  *****************************************************************************/

float CalCoreAnimation::getDuration()
{
  return m_duration;
}

 /*****************************************************************************/
/** Returns the core track list.
  *
  * This function returns the list that contains all core tracks of the core
  * animation instance.
  *
  * @return A reference to the core track list.
  *****************************************************************************/

std::list<CalCoreTrack *>& CalCoreAnimation::getListCoreTrack()
{
  return m_listCoreTrack;
}

 /*****************************************************************************/
/** Sets the duration.
  *
  * This function sets the duration of the core animation instance.
  *
  * @param duration The duration in seconds that should be set.
  *****************************************************************************/

void CalCoreAnimation::setDuration(float duration)
{
	/*if (m_duration == duration)
	{
		return;
	}

    // get the list of core tracks of above core animation
    std::list<CalCoreTrack *>& listCoreTrack = getListCoreTrack();

    // loop through all core tracks of the core animation
    std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
    for(iteratorCoreTrack = listCoreTrack.begin(); iteratorCoreTrack != listCoreTrack.end(); ++iteratorCoreTrack)
    {
      (*iteratorCoreTrack)->setDuration(duration, m_duration);//getState((*iteratorAnimationAction)->getTime(), duration, translation, rotation);
    }*/
	m_duration = duration;
}

//****************************************************************************//

void CalCoreAnimation::setRelMode(bool bRelMode)
{
	m_bRelative = bRelMode;
}

void CalCoreAnimation::saveFState(bool bSave)
{
	m_bSaveFirst = bSave;
}

void CalCoreAnimation::saveLState(bool bSave)
{
	m_bSaveLast = bSave;
}

bool CalCoreAnimation::containBoneFrom(CalCoreAnimation* pCheckAnim, std::list<TRACKPAIR>*	tracksout)
{
    std::list<CalCoreTrack *>& listCoreTrack1 = getListCoreTrack();
	std::list<CalCoreTrack *>& listCoreTrack2 = pCheckAnim->getListCoreTrack();

    std::list<CalCoreTrack *>::iterator iteratorCoreTrack1;
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack2;

    for(iteratorCoreTrack1 = listCoreTrack1.begin(); iteratorCoreTrack1 != listCoreTrack1.end(); ++iteratorCoreTrack1)
    {
		for(iteratorCoreTrack2 = listCoreTrack2.begin(); iteratorCoreTrack2 != listCoreTrack2.end(); ++iteratorCoreTrack2)
		{
			if( (*iteratorCoreTrack1)->getCoreBoneId() == (*iteratorCoreTrack2)->getCoreBoneId())
			{
				if(!tracksout)
					return true;
				else
				{
					std::pair<CalCoreTrack *, CalCoreTrack *>	tpair;
					tpair.first = *iteratorCoreTrack1;
					tpair.second = *iteratorCoreTrack2;
					tracksout->push_back(tpair);
				}
			}
		}
	}
	if( !tracksout)	return false;
	if(	tracksout->empty())
		return false;
	else
		return true;
}

void CalCoreAnimation::setKeepLastTime(DWORD ms, float rnd)
{
	m_dwKeepLast = ms;
	m_fKeepRnd = rnd;
}

float CalCoreAnimation::getKeepLastTime()
{
	return (m_dwKeepLast - (m_dwKeepLast*m_fKeepRnd) + 2*(m_dwKeepLast*m_fKeepRnd)*rand()/RAND_MAX) / 1000.0f;
}

void CalCoreAnimation::setLock(const std::string& strName)
{
	m_strLock = strName;
}

std::string CalCoreAnimation::getName()const
{
	return m_strLock;
}

void CalCoreAnimation::addCoreTrack(DWORD id)
{
	CalCoreTrack*	ptrack = MP_NEW( CalCoreTrack);
	ptrack->create();
	ptrack->setCoreBoneId(id);
	addCoreTrack( ptrack);
}

void CalCoreAnimation::addTrackKeyframe(DWORD id, CalVector trn, CalQuaternion rtn, float time)
{
    std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack){
		if( (*iteratorCoreTrack)->getCoreBoneId() == (int)id){
			(*iteratorCoreTrack)->addKeyframe( trn, rtn, time);
			return;
		}	
	}
}

CalCoreKeyframe* CalCoreAnimation::getBegPointer()
{
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack = m_listCoreTrack.begin();
	return (*iteratorCoreTrack)->getBegPointer();
}

bool CalCoreAnimation::IsCommon()
{
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
	for(iteratorCoreTrack = m_listCoreTrack.begin(); iteratorCoreTrack != m_listCoreTrack.end(); ++iteratorCoreTrack){
		if((*iteratorCoreTrack)->getCoreBoneName() != "")	return true;
	}
	return false;
}

bool CalCoreAnimation::FallOnSkel(CalCoreSkeleton *pCoreSkel)
{
	if (!IsCommon()) return false;
	std::list<CalCoreTrack *>::iterator iteratorCoreTrack;
    for(iteratorCoreTrack = getListCoreTrack().begin(); iteratorCoreTrack != getListCoreTrack().end(); ++iteratorCoreTrack)
    {
		int id = pCoreSkel->getCoreBoneIdSubName((*iteratorCoreTrack)->getCoreBoneName().c_str());
		(*iteratorCoreTrack)->setCoreBoneId( id);
		if( id == Cal::PROJECTION_BONE_ID)
		{
			// ?? 
			//(*iteratorCoreTrack)->alignFrameTranslation( CalVector);
		}
		else	if (id == 0) {
			float	skelBipHeight = pCoreSkel->getStayedBipHeight();
			if( skelBipHeight != 0.0f)
			{
				float	animBipHeight = getStayedBipHeight(pCoreSkel);
				if( animBipHeight != 0.0f)
				{
					(*iteratorCoreTrack)->adjustKeysTranslation(skelBipHeight/animBipHeight);
				}
			}
		}
		else	if (id > 0) {
			if( m_version<1009 || (*iteratorCoreTrack)->m_coreCommonBoneLength == 0.0f)
			{
				CalCoreBone*	pBone = pCoreSkel->getCoreBone(id);
				(*iteratorCoreTrack)->setKeysTranslation( pBone->getTranslation());
			}
			else
			{
				CalVector	skelBoneTranslation = pCoreSkel->getCoreBone(id)->getTranslation();
				float	skelBoneLength = skelBoneTranslation.length();
				float	animBoneLength = (*iteratorCoreTrack)->m_coreCommonBoneLength;
				(*iteratorCoreTrack)->adjustKeysTranslation( skelBoneLength/animBoneLength);
			}
		}
	}
	return true;
}

void CalCoreAnimation::KeepFrame()
{
	CalCoreTrack*	track = getCoreTrack(Cal::PROJECTION_BONE_ID);
	if( track)
	{
		std::map<float, CalCoreKeyframe *>&	keys = track->getMapCoreKeyframe();
		std::map<float, CalCoreKeyframe *>::iterator	it = keys.end();
		it--;
		// Calculate frame move
		CalVector	st = keys.begin()->second->getTranslation();
		CalVector	en = it->second->getTranslation();
		float	fl = (st-en).length();
		setFlag(CAL3D_COREANIM_KEEPFRAME);
		setFlagData(CAL3D_COREANIM_KEEPFRAME, &fl);

		int i=0;
		if( keys.size() > 1)
		{
			CalVector	startPosition = keys.begin()->second->getTranslation();
			CalVector	endPosition = (--it)->second->getTranslation();
			for( it=keys.begin(); it!=keys.end(); ++it, i++)
			{
				CalCoreKeyframe*	key = it->second;
				CalVector	desPos = startPosition + (endPosition-startPosition)*(float)i/(float)keys.size();
				key->setTranslation(startPosition + (key->getTranslation() - desPos));
			}
		}
	}
}

void CalCoreAnimation::getFlagData(DWORD flag, LPVOID *ppdata)
{
	switch(flag) {
	case CAL3D_COREANIM_KEEPFRAME:
		*ppdata = (float*)&frameMoveLength;
		break;
	}
}

void CalCoreAnimation::setBVH(bool isBVH)
{
	m_isBVH = isBVH;
}

bool CalCoreAnimation::isBVH()
{
	return m_isBVH;
}

void CalCoreAnimation::setFlagData(DWORD flag, LPVOID pdata)
{
	switch(flag) {
	case CAL3D_COREANIM_KEEPFRAME:
		frameMoveLength = *(float*)pdata;
		break;
	}
}

float CalCoreAnimation::getStayedBipHeight(CalCoreSkeleton *m_pCoreSkeleton)
{
	if( !m_pCoreSkeleton->containBone("L Foot"))	return 0.0f;
	if( !m_pCoreSkeleton->containBone("L Calf"))		return 0.0f;

	CalCoreTrack*	thighTrack = getCoreTrack("L Foot");
	CalCoreTrack*	calfTrack = getCoreTrack("L Calf");
	
	float	height=0.0f;
	if( thighTrack)
	{
		if( thighTrack->getMapCoreKeyframe().size()>0)
		{
			CalVector	vkeypos = thighTrack->getMapCoreKeyframe().begin()->second->getTranslation();
			height += vkeypos.length();
		}
	}
	if( calfTrack)
	{
		if( calfTrack->getMapCoreKeyframe().size()>0)
		{
			CalVector	vkeypos = calfTrack->getMapCoreKeyframe().begin()->second->getTranslation();
			height += vkeypos.length();
		}
	}
	return height;
}

int CalCoreAnimation::RemoveFakeFrames( )
{
	int cnt = 0;
	CalCoreAnimation* apCoreAnimation = this;
	assert( apCoreAnimation);
	if( !apCoreAnimation)	return 0;
	// 1. ������� ���������� �����. ���� ����������� � ������� �� ������ n..m ���������, ��
	// ����� ������� ��� ����� � ��������� n+1..m+1
	std::list<CalCoreTrack *>&	tracks = apCoreAnimation->getListCoreTrack();
	std::list<CalCoreTrack *>::iterator	it = tracks.begin(),
										end = tracks.end();
	for(; it!=end; ++it)
	{
		CalCoreTrack*	track = *it;
		std::map<float, CalCoreKeyframe *>& keyframes = track->getMapCoreKeyframe();
		std::map<float, CalCoreKeyframe *>::iterator	kit = keyframes.begin(),
														kend = keyframes.end(),
														kbend = keyframes.end();
		kbend--;

		CalVector		translation, _translation;
		CalQuaternion	rotation, _rotation;
		std::map<float, CalCoreKeyframe *>::iterator	startConst = keyframes.end();
		for(; kit!=kend; kit++)
		{
			CalCoreKeyframe*	keyFrame = kit->second;
			if( startConst == keyframes.end())
			{
				translation = keyFrame->getTranslation();
				rotation = keyFrame->getRotation();
				startConst = kit;
			}
			else
			{
				_translation = keyFrame->getTranslation();
				_rotation = keyFrame->getRotation();
				if( _translation != translation ||
					rotation.x != _rotation.x ||
					rotation.y != _rotation.y ||
					rotation.z != _rotation.z ||
					rotation.w != _rotation.w ||
					kit == kbend)
				{
					startConst++;
					while( startConst != kit)
					{
						startConst = keyframes.erase(startConst);
					}
					startConst = keyframes.end();
				}
			}
		}
	}

	// 2. ������� �����, ������� � ��� ���������� ��� ������ ������������ ����� ��������� ����������.
	it = tracks.begin();
	end = tracks.end();
	for(; it!=end; ++it)
	{
		CalCoreTrack*	track = *it;
		std::map<float, CalCoreKeyframe *>& keyframes = track->getMapCoreKeyframe();
		std::map<float, CalCoreKeyframe *>::iterator	kit = keyframes.begin(),
														kend = keyframes.end(), 
														frameit = keyframes.end();

		kit++;

		CalVector		translation, _translation;
		CalQuaternion	rotation, _rotation;
		for(; kit!=kend; kit++)
		{
			frameit = kit;	frameit++;
			if( frameit== kend)	break;
			float				afterTime = frameit->first;
			CalCoreKeyframe*	keyFrameAfter = frameit->second;

			frameit = kit;	frameit--;
			float				beforeTime = frameit->first;
			CalCoreKeyframe*	keyFrameBefore = frameit->second;

			float				keyTime = kit->first;
			CalCoreKeyframe*	keyFrame = kit->second;

			if (afterTime == beforeTime)
			{
				assert(false);
				break;
			}

			float time = (keyTime - beforeTime) / (afterTime - beforeTime);

			CalVector	posBlended = keyFrameBefore->getTranslation();
			posBlended.blend(time, keyFrameAfter->getTranslation());
			if( (posBlended-keyFrame->getTranslation()).length() > 1)	break;

			CalQuaternion rtnBlended = keyFrameBefore->getRotation();
			rtnBlended.blend(time, keyFrameAfter->getRotation());

			float accAngle = 0;
			CalVector	axes[] = {CalVector(1,0,0), CalVector(0,1,0), CalVector(0,0,1)};
			for(int i=0; i<sizeof(axes)/sizeof(axes[0]); i++)
			{
				CalVector v1 = axes[i];	v1 *= keyFrameAfter->getRotation();
				CalVector v2 = axes[i];	v2 *= rtnBlended;
				accAngle += v1.getAngle(v2);
			}
			if( accAngle > 1*3.14f/180.0f)	break;

			keyframes.erase(kit);
			cnt++;
		}
	}
		
	return cnt;
}

void CalCoreAnimation::initialize(CalSkeleton* /*skeleton*/)
{
}

/*****************************************************************************/
/** 
* Add a callback to the current list of callbacks for this CoreAnim.
*
* @param  callback     Ptr to a subclass of this abstract class implementing the callback function.
* @param  min_interval Minimum interval (in seconds) between callbacks.  Specifying 0 means call every update().
*
*****************************************************************************/

void CalCoreAnimation::registerCallback(CalAnimationCallback *callback,float min_interval)
{
	CallbackRecord record;
	record.callback     = callback;
	record.min_interval = min_interval;

	m_listCallbacks.push_back(record);
}

/*****************************************************************************/
/** 
* Remove a callback from the current list of callbacks for this Anim.
* Callback objects not removed this way will be deleted in the dtor of the Anim.
*
* @param  callback     Ptr to a subclass of this abstract class implementing the callback function to remove.
*
*****************************************************************************/

void CalCoreAnimation::removeCallback(CalAnimationCallback *callback)
{
	for (std::vector<CallbackRecord>::iterator i = m_listCallbacks.begin(); i != m_listCallbacks.end(); i++)
	{
		if ((*i).callback == callback)
		{
			m_listCallbacks.erase(i);
			return;
		}
	}
}