// CText2vidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "text2vid.h"
#include "Text2vidDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpzioniDlg dialog


COpzioniDlg::COpzioniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpzioniDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpzioniDlg)
	m_DimensioneImmagini = -1;
	m_AlignHoriz = -1;
	m_AlignVert = -1;
	//}}AFX_DATA_INIT
}


void COpzioniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpzioniDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_DimensioneImmagini);
	DDX_Radio(pDX, IDC_RADIO5, m_AlignHoriz);
	DDX_Radio(pDX, IDC_RADIO9, m_AlignVert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpzioniDlg, CDialog)
	//{{AFX_MSG_MAP(COpzioniDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpzioniDlg message handlers

BOOL COpzioniDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_Size=theApp.TextSize;
	m_ForeColor=theApp.ColorFore;
	m_BackColor=theApp.ColorBack;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

void COpzioniDlg::OnButton1() {
	CColorDialog ccd;
	
	if(ccd.DoModal() == IDOK) {
		m_ForeColor=ccd.m_cc.rgbResult;
		}
	}

void COpzioniDlg::OnButton2() {		// colore sfondo
	CColorDialog ccd;

	if(ccd.DoModal() == IDOK) {
		m_BackColor=ccd.m_cc.rgbResult;
		}
	}

void COpzioniDlg::OnButton3() {		// font
	CHOOSEFONT cf;

	cf.lStructSize=sizeof(CHOOSEFONT);

	ChooseFont(&cf);
	
	}
/////////////////////////////////////////////////////////////////////////////
// CCreaDlg dialog


CCreaDlg::CCreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreaDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CCreaDlg)
	m_NomeFile = _T("");
	//}}AFX_DATA_INIT
}


void CCreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreaDlg)
	DDX_Text(pDX, IDC_EDIT1, m_NomeFile);
	DDV_MaxChars(pDX, m_NomeFile, 127);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreaDlg, CDialog)
	//{{AFX_MSG_MAP(CCreaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreaDlg message handlers

BOOL CCreaDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_NomeFile=theApp.outputFile;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}


