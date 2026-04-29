// text2vidDoc.h : interface of the CText2vidDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT2VIDDOC_H__B6C08B22_B14E_4200_96EA_87FC589E6254__INCLUDED_)
#define AFX_TEXT2VIDDOC_H__B6C08B22_B14E_4200_96EA_87FC589E6254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CText2vidDoc : public CDocument
{
protected: // create from serialization only
	CText2vidDoc();
	DECLARE_DYNCREATE(CText2vidDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CText2vidDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CText2vidDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CText2vidDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CText2vidDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT2VIDDOC_H__B6C08B22_B14E_4200_96EA_87FC589E6254__INCLUDED_)
