// text2vid.h : main header file for the TEXT2VID application
//

#if !defined(AFX_TEXT2VID_H__638BFDAD_A175_4252_B19C_1D7B3E99B3C4__INCLUDED_)
#define AFX_TEXT2VID_H__638BFDAD_A175_4252_B19C_1D7B3E99B3C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


class CExBitmap : public CBitmap {
	// dedicato a Roberta B, 29-31 maggio 2004
public:
	int Resample(CBitmap *,RECT *rcDest);
	int Resample(RECT *rcDest);
	int superImposeDateTime(BYTE *d);
	int superImposeText(char *s);
	int ConvertColorToBN();
	int Mirror();
	int Flip();
	BOOL CreateBitmap(RECT *);
	CExBitmap(CBitmap);
	CExBitmap();
	};

class CStringEx : public CString {
	public:
		enum Options {
			NO_OPTIONS=0,
			COMMA_DELIMIT=1,
			};
		static const int BASE64_MAXLINE;
		static const char *EOL;
		static const char decimalChar,thousandChar;
		static const char CRchar,LFchar,TABchar;
	public:
		static const char m_base64tab[];
	public:
		CString Tokenize(CString delimiter, int& first);
		static CStringEx CommaDelimitNumber(const char *);
		static CStringEx CommaDelimitNumber(CString);
		static CStringEx CommaDelimitNumber(DWORD);
		CStringEx SubStr(int begin, int len) const;					// substring from s[begin] to s[begin+len]
		BYTE Asc(int);
		int Val(int base=10);
		double Val();
		void Repeat(int);
		void Repeat(const char *,int);
		void Repeat(char,int);
		void AddCR() { CStringEx::operator+=(LFchar); }
		void RemoveLeft(int n) { CStringEx::operator=(Mid(n)); }
		void RemoveRight(int n) { CStringEx::operator=(Mid(1,GetLength()-n)); }		// era un'idea per fare LEFT$ di tot char, ma Trimright c' gi anche se diversa...
		static BOOL IsAlpha(char);
		BOOL IsAlpha(int);
		static BOOL IsAlnum(char);
		BOOL IsAlnum(int);
		static BOOL IsDigit(char);
		BOOL IsDigit(int);
		static BOOL IsPrint(char);
		BOOL IsPrint(int);
		int FindNoCase(CString substr,int start=0);
		WORD GetAsciiLength();
		CStringEx Encode64();
		int Decode64();
		CStringEx FormatTime(int m=0,CTime mT=0);
		CStringEx FormatSize(DWORD);
		CStringEx SplitPath(LPCTSTR,BYTE mode);
		void Print();
		void Debug();
		CStringEx() : CString() {};		// servono tutti i costruttori "perché non ne ha di virtual, la CString" !
		// https://www.codeguru.com/cpp/cpp/string/ext/article.php/c2793/CString-Extension.htm
		// https://www.codeproject.com/Articles/2396/Simple-CString-Extension
		CStringEx(const CString& stringSrc) : CString(stringSrc) {};
		// bah, eppure non sembra... 2021...
//		CStringEx(const CStringEx& stringSrc) : CString(stringSrc) {};
		CStringEx(TCHAR ch, int nRepeat = 1) : CString(ch, nRepeat) {};
//		CStringEx(LPCTSTR lpch, int nLength) : CString(lpch, nLength) {};
//		CStringEx(const unsigned char *psz) : CString(psz) {};
		CStringEx(LPCWSTR lpsz) : CString(lpsz) {};
		CStringEx(LPCSTR lpsz) : CString(lpsz) {};
//		CStringEx(const char c) {char s[2]={'\0', '\0'}; s[0]=c; CString::operator=(s);}
		CStringEx(int i, const char* format="%d", DWORD options=NO_OPTIONS);
		CStringEx(double d, const char* format="%02lf", DWORD options=NO_OPTIONS);
		virtual ~CStringEx() {};

private:
	CString InsertSeparator(DWORD);
	};


//#define _CPRIVATEPROFILE_USEINI 1

class CProfileStore {
//CSettingsStore ... 2025

public:
	static TCHAR *emptyString,*mainSubKey;

private:
	HKEY theRoot;
	HINSTANCE m_hInstance;
	CString regRoot;
	TCHAR *variabiliKey;

public:
#ifdef _CPRIVATEPROFILE_USEINI
	int WritePrivateProfileInt(const TCHAR *, CString, int, const TCHAR *);
#endif
	int WritePrivateProfileString(const TCHAR *,CString,const TCHAR *);
	int WritePrivateProfileString(const TCHAR *s, int k, const TCHAR *v=NULL) { CString ks; ks.LoadString(k); return WritePrivateProfileString(s,ks,v);};
	int WritePrivateProfileInt(const TCHAR *, CString, int);
	int WritePrivateProfileInt(const TCHAR *s, int k, int v)  { CString ks; ks.LoadString(k); return WritePrivateProfileInt(s,ks,v);};
	int WritePrivateProfileDouble(const TCHAR *, CString, double);
	int WritePrivateProfileDouble(const TCHAR *s, int k, double v)  { CString ks; ks.LoadString(k); return WritePrivateProfileDouble(s,ks,v);};
	int GetPrivateProfileInt(const TCHAR *s, CString k, int def=0);
	int GetPrivateProfileInt(const TCHAR *s, int k, int def=0) { CString ks; ks.LoadString(k); return GetPrivateProfileInt(s,ks,def);};
	int GetPrivateProfileString(const TCHAR *s, CString k, TCHAR *v, int len, const TCHAR *def=emptyString);
	int GetPrivateProfileString(const TCHAR *s, int k, TCHAR *v, int len, const TCHAR *def=emptyString) { CString ks; ks.LoadString(k); return GetPrivateProfileString(s,ks,v,len,def);};
	double GetPrivateProfileDouble(const TCHAR *s, CString k, double def=0);
	double GetPrivateProfileDouble(const TCHAR *s, int k, double def=0) { CString ks; ks.LoadString(k); return GetPrivateProfileDouble(s,ks,def);};

	CTime GetPrivateProfileTime(const TCHAR *,CString);
	CTime GetPrivateProfileTime(const TCHAR *s,int k) { CString ks; ks.LoadString(k); return GetPrivateProfileTime(s,ks); }
	CTimeSpan GetPrivateProfileTimeSpan(const TCHAR *,CString);
	CTimeSpan GetPrivateProfileTimeSpan(const TCHAR *s,int k) { CString ks; ks.LoadString(k); return GetPrivateProfileTimeSpan(s,ks); }
	int WritePrivateProfileTime(const TCHAR *, CString , CTime );
	int WritePrivateProfileTime(const TCHAR *s, int k, CTime t) { CString ks; ks.LoadString(k); return WritePrivateProfileTime(s,ks,t); }
	int WritePrivateProfileTime(const TCHAR *, CString , CTimeSpan );
	int WritePrivateProfileTime(const TCHAR *s, int k, CTimeSpan t) { CString ks; ks.LoadString(k); return WritePrivateProfileTime(s,ks,t); }

	int WriteProfileVariabileInt(int k, int v)  { return WritePrivateProfileInt(variabiliKey,k,v);};
	int GetProfileVariabileInt(int k) { return GetPrivateProfileInt(variabiliKey,k);};
	int WriteProfileVariabileString(int k, CString v)  { return WritePrivateProfileString(variabiliKey,k,v); };
	int GetProfileVariabileString(int k,TCHAR *v,int len,const TCHAR *def=emptyString) { return GetPrivateProfileString(variabiliKey,k,v,len,def); };
	int FlushProfile();
#ifndef _CPRIVATEPROFILE_USEINI
  CProfileStore(HINSTANCE,CString,CString);
#else
  CProfileStore(HINSTANCE,CString);
#endif
	CString getProfileRoot() const { return regRoot; };
	CString getVariabiliKey() const;

//private:
	CString getProfileKey(CString s=emptyString) const;

	};


class CWinAppEx : public CWinApp {

	DECLARE_DYNAMIC(CWinAppEx)
public:
	friend class CMainFrame;

	CWinAppEx(BOOL bResourceSmartUpdate = TRUE);
	virtual ~CWinAppEx();

	virtual int ExitInstance();

	LPCTSTR SetRegistryBase(LPCTSTR lpszSectionName = NULL);
	LPCTSTR	GetRegistryBase() { return m_strRegSection; }

	// Saved data version:
	int GetDataVersionMajor() const { return m_iSavedVersionMajor; }
	int GetDataVersionMinor() const { return m_iSavedVersionMinor; }
	int GetDataVersion() const;

	BOOL InitMouseManager();
	BOOL InitContextMenuManager();
	BOOL InitKeyboardManager();
	BOOL InitShellManager();
	BOOL InitTooltipManager();


	BOOL IsResourceSmartUpdate() const { return m_bResourceSmartUpdate; }
	void EnableLoadWindowPlacement(BOOL bEnable = TRUE) { m_bLoadWindowPlacement = bEnable; }


	// Call one of these in CMyApp::InitInstance just after ProcessShellCommand() and before pMainFrame->ShowWindow().
	BOOL LoadState(CMDIFrameWnd* pFrame, LPCTSTR lpszSectionName = NULL);
	BOOL LoadState(CFrameWnd* pFrame, LPCTSTR lpszSectionName = NULL);

	virtual BOOL CleanState(LPCTSTR lpszSectionName = NULL);

	BOOL SaveState(CMDIFrameWnd* pFrame, LPCTSTR lpszSectionName = NULL);
	BOOL SaveState(CFrameWnd* pFrame, LPCTSTR lpszSectionName = NULL,int bTutto=TRUE);

	BOOL IsStateExists(LPCTSTR lpszSectionName /*=NULL*/);

	virtual BOOL OnViewDoubleClick(CWnd* pWnd, int iViewId);
	virtual BOOL ShowPopupMenu(UINT uiMenuResId, const CPoint& point, CWnd* pWnd);

	CString GetRegSectionPath(LPCTSTR szSectionAdd = _T(""));

	// These functions load and store values from the "Custom" subkey
	// To use subkeys of the "Custom" subkey use GetSectionInt() etc. instead
	int GetInt(LPCTSTR lpszEntry, int nDefault = 0);
	CString GetString(LPCTSTR lpszEntry, LPCTSTR lpszDefault = _T(""));
	BOOL GetBinary(LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes);
	BOOL GetObject(LPCTSTR lpszEntry, CObject& obj);
	BOOL WriteInt(LPCTSTR lpszEntry, int nValue );
	BOOL WriteString(LPCTSTR lpszEntry, LPCTSTR lpszValue );
	BOOL WriteBinary(LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);
	BOOL WriteObject(LPCTSTR lpszEntry, CObject& obj);

	// These functions load and store values from a given subkey
	// of the "Custom" subkey. For simpler access you may use GetInt() etc.
	int GetSectionInt( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nDefault = 0);
	CString GetSectionString( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = _T(""));
	BOOL GetSectionBinary(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes);
	BOOL GetSectionObject(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, CObject& obj);
	BOOL WriteSectionInt( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nValue );
	BOOL WriteSectionString( LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszValue );
	BOOL WriteSectionBinary(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);
	BOOL WriteSectionObject(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, CObject& obj);

	void SetTitle();
  BOOL FirstInstance(int nCmdShow=SW_SHOW);

	// WinHelp override:
	virtual void OnAppContextHelp(CWnd* pWndControl, const DWORD dwHelpIDArray []);

	// Idle processing override:
	virtual BOOL OnWorkspaceIdle(CWnd* /*pWnd*/) { return FALSE; }

public:
	BOOL m_bLoadUserToolbars;
	CProfileStore *prStore;
	TCHAR szDocPath[MAX_PATH];

protected:

	// Overidables for customization
	virtual void OnClosingMainFrame();
	
	virtual void PreLoadState() {}    // called before anything is loaded
	virtual void LoadCustomState() {} // called after everything is loaded
	virtual void PreSaveState() {}    // called before anything is saved
	virtual void SaveCustomState() {} // called after everything is saved

	virtual BOOL LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd);
	virtual BOOL StoreWindowPlacement(const CRect& rectNormalPosition, int nFflags, int nShowCmd);
	virtual BOOL ReloadWindowPlacement(CFrameWnd* pFrame);

protected:
	CString m_strRegSection;
  BOOL bClassRegistered;
	BOOL AppInited;

	BOOL m_bKeyboardManagerAutocreated;
	BOOL m_bContextMenuManagerAutocreated;
	BOOL m_bMouseManagerAutocreated;
	BOOL m_bUserToolsManagerAutoCreated;
	BOOL m_bTearOffManagerAutoCreated;
	BOOL m_bShellManagerAutocreated;
	BOOL m_bTooltipManagerAutocreated;
	BOOL m_bForceDockStateLoad; // Load dock bars state even it's not valid
	BOOL m_bLoadSaveFrameBarsOnly;
	BOOL m_bSaveState;          // Automatically save state when the main frame is closed.
	BOOL m_bForceImageReset;    // Force image reset every time when the frame is loaded
	BOOL m_bLoadWindowPlacement;

	const BOOL m_bResourceSmartUpdate; // Automatic toolbars/menu resource update

	int m_iSavedVersionMajor;
	int m_iSavedVersionMinor;
	};


/////////////////////////////////////////////////////////////////////////////
// CText2vidApp:
// See text2vid.cpp for the implementation of this class
//

struct VIDEO_SIZE {
	SIZE imageSize;
	};

class CText2vidApp : public CWinAppEx {
public:
	static const struct VIDEO_SIZE qsv[4];
	static const BYTE qfr[5];
	COLORREF ColorFore,ColorBack;
	short int TextSize;			// -1 se auto
	BYTE ImageSize;
	DWORD Align;
	WORD DurataFrame;
	BYTE FpS;
	BYTE Transizione;
	CString inputFile,outputFile;
	CString Font;
	CString BackFile;
	DWORD Codec;

	DWORD inputPos;

	bool bPreview;

public:
	CText2vidApp();
	CString readString();
	void resetInput() { inputPos=0; }
	int writeTextToBitmap(const CBitmap *b,const char *s,int x,int y,BYTE ha,BYTE va,
		COLORREF colorf,COLORREF colorb,WORD size);

	DWORD getVersione(char *n=NULL,char *n1=NULL,const char *language=NULL) const;

	BYTE *mergeImages(LPBITMAPINFOHEADER pBitmap,BYTE *image1,double val1,BYTE *image2,double val2,BYTE *imageOut);

	int renderBitmap(CDC *dc,int res,RECT *r);
	int renderBitmap(CDC *dc,const CBitmap *b,RECT *r);
	int renderBitmap(CDC *dc,const BITMAPINFO *bi,const BYTE *p,const RECT *r);
	int renderBitmap(CDC *dc,const char *aBitmapFile,const RECT *r,int m);	// m=0 stretch, 1=tile
	int renderBitmap(CBitmap b,const char *aBitmapFile,const RECT *r,int m);	// m=0 stretch, 1=tile
	int renderBitmap(BYTE *d,const char *aBitmapFile,const RECT *r,BYTE bpp,int m,bool f);	// m=0 stretch, 1=tile; TRUE=flip
	BYTE *scaleBitmap(const BITMAPINFO *sb,BYTE *s,BITMAPINFO *db,BYTE *d);
	int adjustBitmap(BYTE *p,short int l,short int c,short int s);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CText2vidApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CText2vidApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileCreavideo();
	afx_msg void OnOpzioniImmagini();
	afx_msg void OnFileCreajpeg();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileCreavideo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileCreajpeg(CCmdUI* pCmdUI);
	afx_msg void OnVisualizzaAnteprima();
	afx_msg void OnUpdateVisualizzaAnteprima(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

extern CText2vidApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXT2VID_H__638BFDAD_A175_4252_B19C_1D7B3E99B3C4__INCLUDED_)
