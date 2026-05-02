// CText2vidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "text2vid.h"
#include "Text2vidDlg.h"
#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpzioniDlg dialog

COpzioniDlg::COpzioniDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpzioniDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(COpzioniDlg)
	m_DimensioneImmagini = -1;
	m_AlignHoriz = -1;
	m_AlignVert = -1;
	m_FpS = -1;
	m_Durata = 0;
	m_FontSize = 0;
	m_AutoSize = FALSE;
	m_Transizione = -1;
	m_BackFile = _T("");
	//}}AFX_DATA_INIT
	}


void COpzioniDlg::DoDataExchange(CDataExchange* pDX) {

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpzioniDlg)
	DDX_Control(pDX, IDC_COMBO3, m_ComboCompressorV);
	DDX_Radio(pDX, IDC_RADIO1, m_DimensioneImmagini);
	DDX_Radio(pDX, IDC_RADIO5, m_AlignHoriz);
	DDX_Radio(pDX, IDC_RADIO9, m_AlignVert);
	DDX_CBIndex(pDX, IDC_COMBO1, m_FpS);
	DDX_Text(pDX, IDC_EDIT1, m_Durata);
	DDV_MinMaxInt(pDX, m_Durata, 1, 3600);
	DDX_Text(pDX, IDC_EDIT2, m_FontSize);
	DDV_MinMaxInt(pDX, m_FontSize, 2, 255);
	DDX_Check(pDX, IDC_CHECK1, m_AutoSize);
	DDX_CBIndex(pDX, IDC_COMBO2, m_Transizione);
	DDX_Text(pDX, IDC_EDIT3, m_BackFile);
	//}}AFX_DATA_MAP
	}


BEGIN_MESSAGE_MAP(COpzioniDlg, CDialog)
	//{{AFX_MSG_MAP(COpzioniDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpzioniDlg message handlers

BOOL COpzioniDlg::OnInitDialog() {
	CString S;
	int n;

	CDialog::OnInitDialog();
	
	m_FontSize=theApp.TextSize;
	m_AutoSize = m_FontSize == -1 ? TRUE : FALSE;
	GetDlgItem(IDC_EDIT2)->EnableWindow(!m_AutoSize);
	m_ForeColor=theApp.ColorFore;
	S.Format("%06X",m_ForeColor);
	GetDlgItem(IDC_BUTTON1)->SetWindowText(S);
	m_BackColor=theApp.ColorBack;
	S.Format("%06X",m_BackColor);
	GetDlgItem(IDC_BUTTON2)->SetWindowText(S);
	m_Font=theApp.Font;
	GetDlgItem(IDC_BUTTON3)->SetWindowText(m_Font);
	m_DimensioneImmagini=theApp.ImageSize;
	m_FpS=theApp.FpS;
	m_Durata=theApp.DurataFrame;
	m_Transizione=theApp.Transizione;
	m_AlignHoriz=LOWORD(theApp.Align);
	m_AlignVert=HIWORD(theApp.Align);
	m_BackFile=theApp.BackFile;

	m_CompressorV=theApp.Codec;
	n=enumCompressorV(&m_ComboCompressorV,m_CompressorV);
	m_ComboCompressorV.SetCurSel(HIWORD(n));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

DWORD COpzioniDlg::enumCompressorV(CComboBox *c,DWORD v) {
	int i,j,n=0;
	HIC hic;
	ICINFO ii;
	CString S;

	c->AddString("<nessuna>");
	c->SetItemData(0,0);
	for(i=0; ICInfo(ICTYPE_VIDEO, i, &ii); i++) {
    hic=ICOpen(ii.fccType, ii.fccHandler, ICMODE_QUERY); 
    if(hic) { 
        // Skip this compressor if it can't handle the format. 
/*      if(fccType == ICTYPE_VIDEO && pvIn != NULL && 
        ICDecompressQuery(hic, pvIn, NULL) != ICERR_OK) { 
	      ICClose(hic); 
				continue;
				}*/
      ICGetInfo(hic, &ii, sizeof(ii)); 
      ICClose(hic); 
			}
 		S=ii./*szDescription*/ szName;
		c->AddString(S);
		c->SetItemData(i+1,ii.fccHandler);
		if(v==ii.fccHandler && !n) 
			n=i+1;
		}
	return MAKELONG(i,n);
	}

void COpzioniDlg::OnButton1() {
	CColorDialog ccd;
	CString S;
	
	if(ccd.DoModal() == IDOK) {
		m_ForeColor=ccd.m_cc.rgbResult;
		S.Format("%06X",m_ForeColor);
		GetDlgItem(IDC_BUTTON1)->SetWindowText(S);
		}
	}

void COpzioniDlg::OnButton2() {		// colore sfondo
	CColorDialog ccd;
	CString S;

	if(ccd.DoModal() == IDOK) {
		m_BackColor=ccd.m_cc.rgbResult;
		S.Format("%06X",m_BackColor);
		GetDlgItem(IDC_BUTTON2)->SetWindowText(S);
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
		GetDlgItem(IDC_BUTTON3)->SetWindowText(m_Font);
		// m_FontSize=cf.lpLogFont->lfHeight;  no, non esce...
		// UpdateData(FALSE);
		}
	
	}

void COpzioniDlg::OnSelchangeCombo3() {
	int i=m_ComboCompressorV.GetCurSel();

	if(i != CB_ERR)
		m_CompressorV=m_ComboCompressorV.GetItemData(i);
	
	}

void COpzioniDlg::OnButton4() {
	CString S=m_BackFile;
	CFileDialog myDlg(TRUE,NULL,S,OFN_OVERWRITEPROMPT,
		"File immagine (*.jpg)|*.JPG|File immagine (*.png)|*.PNG|File immagine (*.bmp)|*.BMP|Tutti i file (*.*)|*.*||"
		);

	if(myDlg.DoModal() == IDOK) {
		m_BackFile=myDlg.GetPathName();
		UpdateData(FALSE);
		}
	}

void COpzioniDlg::OnCheck1() {

	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == BST_CHECKED)
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	else {
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		m_AutoSize=FALSE;
		m_FontSize=8;
		UpdateData(FALSE);
		}
	UpdateData();
	
	}


/////////////////////////////////////////////////////////////////////////////
// CCreaDlg dialog

CCreaDlg::CCreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreaDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CCreaDlg)
	m_NomeFile = _T("");
	//}}AFX_DATA_INIT
}


void CCreaDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreaDlg)
	DDX_Text(pDX, IDC_EDIT1, m_NomeFile);
	DDV_MaxChars(pDX, m_NomeFile, 127);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreaDlg, CDialog)
	//{{AFX_MSG_MAP(CCreaDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreaDlg message handlers

BOOL CCreaDlg::OnInitDialog() {
	CString S;

	CDialog::OnInitDialog();
	
	GetWindowText(S);
	S += which ? " video" : " immagine";
	SetWindowText(S);
//	m_NomeFile=theApp.outputFile;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
	}

int CCreaDlg::DoModal(CString T,BYTE w) {

	m_NomeFile=T;
	which=w;
	return CDialog::DoModal();
	}

void CCreaDlg::OnButton1() {
	CString S=m_NomeFile;
	CFileDialog myDlg(FALSE,NULL,S,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		which ? "File video (*.avi)|*.AVI|Tutti i file (*.*)|*.*||" : "File immagine (*.jpg)|*.JPG|Tutti i file (*.*)|*.*||"
		);

	if(myDlg.DoModal() == IDOK) {
		m_NomeFile=myDlg.GetPathName();
		UpdateData(FALSE);
		}
	}




