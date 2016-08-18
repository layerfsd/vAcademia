//----------------------------------------------------------------------------//
// AnimationExportSheet.h                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef ANIMATION_EXPORT_SHEET_H
#define ANIMATION_EXPORT_SHEET_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "SkeletonFilePage.h"
#include "SkeletonHierarchyPage.h"
#include "AnimationTimePage.h"
#include "ExportConfig.h"

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//

class CAnimationExportSheet : public CPropertySheet
{
// member variables
public:
	CExportConfig m_exportConfig;
	CSkeletonFilePage m_skeletonFilePage;
protected:
	CSkeletonHierarchyPage m_skeletonHierarchyPage;
	CAnimationTimePage m_animationTimePage;

// constructors/destructor
public:
	CAnimationExportSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CAnimationExportSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAnimationExportSheet();

// member functions
public:
	bool GetRelState();
	bool GetLState();
	bool GetFState();
	bool GetCommonState();
	int GetDisplacement();
	int GetEndFrame();
	int GetFps();
	int GetStartFrame();
	void SetAnimationTime(int startFrame, int endFrame, int displacementFrame, int fps);
	void SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate);
	void SetCAFName(LPCTSTR alpcCAFName){m_exportConfig.SetCAFName(alpcCAFName);}
	void SaveECF();
	bool SetSection(CString sSectionName);
	//bool m_bRelativeAnim;

protected:
	void FillPages();

// MSVC++ stuff
	//{{AFX_VIRTUAL(CAnimationExportSheet)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAnimationExportSheet)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------------------------------//
// MSVC++ stuff                                                               //
//----------------------------------------------------------------------------//

//{{AFX_INSERT_LOCATION}}

#endif

//----------------------------------------------------------------------------//