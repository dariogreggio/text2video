// text2vidDoc.cpp : implementation of the CText2vidDoc class
//

#include "stdafx.h"
#include "text2vid.h"

#include "text2vidDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CText2vidDoc

IMPLEMENT_DYNCREATE(CText2vidDoc, CDocument)

BEGIN_MESSAGE_MAP(CText2vidDoc, CDocument)
	//{{AFX_MSG_MAP(CText2vidDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CText2vidDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CText2vidDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IText2vid to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {BB51F934-B6AC-4F07-A4F8-50E897EBA5D2}
static const IID IID_IText2vid =
{ 0xbb51f934, 0xb6ac, 0x4f07, { 0xa4, 0xf8, 0x50, 0xe8, 0x97, 0xeb, 0xa5, 0xd2 } };

BEGIN_INTERFACE_MAP(CText2vidDoc, CDocument)
	INTERFACE_PART(CText2vidDoc, IID_IText2vid, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CText2vidDoc construction/destruction

CText2vidDoc::CText2vidDoc()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
}

CText2vidDoc::~CText2vidDoc()
{
	AfxOleUnlockApp();
}

BOOL CText2vidDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CText2vidDoc serialization

void CText2vidDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CText2vidDoc diagnostics

#ifdef _DEBUG
void CText2vidDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CText2vidDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CText2vidDoc commands
