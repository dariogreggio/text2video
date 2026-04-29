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
	m_FpS = -1;
	m_Durata = 0;
	m_FontSize = 0;
	m_AutoSize = FALSE;
	//}}AFX_DATA_INIT
}


void COpzioniDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpzioniDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_DimensioneImmagini);
	DDX_Radio(pDX, IDC_RADIO5, m_AlignHoriz);
	DDX_Radio(pDX, IDC_RADIO9, m_AlignVert);
	DDX_CBIndex(pDX, IDC_COMBO1, m_FpS);
	DDX_Text(pDX, IDC_EDIT1, m_Durata);
	DDV_MinMaxInt(pDX, m_Durata, 1, 3600);
	DDX_Text(pDX, IDC_EDIT2, m_FontSize);
	DDV_MinMaxInt(pDX, m_FontSize, 2, 255);
	DDX_Check(pDX, IDC_CHECK1, m_AutoSize);
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
	
	m_FontSize=theApp.TextSize;
	m_ForeColor=theApp.ColorFore;
	m_BackColor=theApp.ColorBack;
	m_Font=theApp.Font;
	m_DimensioneImmagini=theApp.ImageSize;
	m_FpS=theApp.FpS;
	m_Durata=theApp.DurataFrame;
	m_AlignHoriz=LOWORD(theApp.Align);
	m_AlignVert=HIWORD(theApp.Align);

	UpdateData(FALSE);

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
	LOGFONT lf;

  ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize=sizeof(CHOOSEFONT);

    CDC *dc = theApp.m_pMainWnd->GetDC();
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfCharSet = (BYTE)GetTextCharset(dc->m_hDC);
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH;
    lf.lfWeight = FW_NORMAL;
    lf.lfHeight = -MulDiv(10, GetDeviceCaps(dc->m_hDC, LOGPIXELSY), 2);

		theApp.m_pMainWnd->ReleaseDC(dc);
  
	cf.lpLogFont = &lf;
	cf.hwndOwner=theApp.m_pMainWnd->m_hWnd;

	if(ChooseFont(&cf)) {
		m_Font=cf.lpLogFont->lfFaceName;
		}
	
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


