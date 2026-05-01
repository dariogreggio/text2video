#if !defined(AFX_CTEXT2VIDDLG_H__0EA5C798_564F_405E_AF31_07370EB1982B__INCLUDED_)
#define AFX_CTEXT2VIDDLG_H__0EA5C798_564F_405E_AF31_07370EB1982B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CText2vidDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpzioniDlg dialog

class COpzioniDlg : public CDialog {
// Construction
public:
	COpzioniDlg(CWnd* pParent = NULL);   // standard constructor
	DWORD enumCompressorV(CComboBox *c,DWORD v);

public:
// Dialog Data
	COLORREF m_ForeColor,m_BackColor;
	CString m_Font;
	WORD m_Frames;
	DWORD m_CompressorV;

	//{{AFX_DATA(COpzioniDlg)
	enum { IDD = IDD_OPZIONI };
	CComboBox	m_ComboCompressorV;
	int		m_DimensioneImmagini;
	int		m_AlignHoriz;
	int		m_AlignVert;
	int		m_FpS;
	int		m_Durata;
	int		m_FontSize;
	BOOL	m_AutoSize;
	int		m_Transizione;
	CString	m_BackFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpzioniDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpzioniDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

/////////////////////////////////////////////////////////////////////////////
// CCreaDlg dialog

class CCreaDlg : public CDialog {
// Construction
public:
	CCreaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreaDlg)
	enum { IDD = IDD_CREA };
	CString	m_NomeFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreaDlg)
	public:
	virtual int DoModal(CString,BYTE);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BYTE which;

	// Generated message map functions
	//{{AFX_MSG(CCreaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTEXT2VIDDLG_H__0EA5C798_564F_405E_AF31_07370EB1982B__INCLUDED_)
