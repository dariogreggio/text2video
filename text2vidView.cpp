// text2vidView.cpp : implementation of the CText2vidView class
//

#include "stdafx.h"
#include "text2vid.h"

#include "text2vidDoc.h"
#include "text2vidView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CText2vidView

IMPLEMENT_DYNCREATE(CText2vidView, CView)

BEGIN_MESSAGE_MAP(CText2vidView, CView)
	//{{AFX_MSG_MAP(CText2vidView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CText2vidView construction/destruction

CText2vidView::CText2vidView()
{
	// TODO: add construction code here

}

CText2vidView::~CText2vidView()
{
}

BOOL CText2vidView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CText2vidView drawing

void CText2vidView::OnDraw(CDC* pDC)
{
	CText2vidDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CText2vidView printing

BOOL CText2vidView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CText2vidView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CText2vidView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CText2vidView diagnostics

#ifdef _DEBUG
void CText2vidView::AssertValid() const
{
	CView::AssertValid();
}

void CText2vidView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CText2vidDoc* CText2vidView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CText2vidDoc)));
	return (CText2vidDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CText2vidView message handlers
