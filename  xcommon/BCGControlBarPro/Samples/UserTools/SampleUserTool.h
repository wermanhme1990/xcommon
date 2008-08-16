// SampleUserTool.h: interface for the CSampleUserTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEUSERTOOL_H__579D6A5C_69F9_4407_AA2B_B96FED08873B__INCLUDED_)
#define AFX_SAMPLEUSERTOOL_H__579D6A5C_69F9_4407_AA2B_B96FED08873B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleUserTool : public CBCGPUserTool  
{
	DECLARE_SERIAL(CSampleUserTool)

public:
	CSampleUserTool();
	virtual ~CSampleUserTool();

// Attributes:
public:
	CString	m_strProp1;
	CString	m_strProp2;

// Overrides
	virtual void Serialize (CArchive& ar);
	virtual BOOL Invoke ();
};

#endif // !defined(AFX_SAMPLEUSERTOOL_H__579D6A5C_69F9_4407_AA2B_B96FED08873B__INCLUDED_)
