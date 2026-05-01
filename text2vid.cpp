// text2vid.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "text2vid.h"

#include "MainFrm.h"
#include "text2vidDoc.h"
#include "text2vidView.h"
#include "text2vidDlg.h"
#include <mmsystem.h>
#include <cjpeg.h>
#include <vfw.h>
//#include "..\o7area\o7dshow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CText2vidApp

BEGIN_MESSAGE_MAP(CText2vidApp, CWinApp)
	//{{AFX_MSG_MAP(CText2vidApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_CREAVIDEO, OnFileCreavideo)
	ON_COMMAND(ID_OPZIONI_IMMAGINI, OnOpzioniImmagini)
	ON_COMMAND(ID_FILE_CREAJPEG, OnFileCreajpeg)
	ON_UPDATE_COMMAND_UI(ID_FILE_CREAVIDEO, OnUpdateFileCreavideo)
	ON_UPDATE_COMMAND_UI(ID_FILE_CREAJPEG, OnUpdateFileCreajpeg)
	ON_COMMAND(ID_VISUALIZZA_ANTEPRIMA, OnVisualizzaAnteprima)
	ON_UPDATE_COMMAND_UI(ID_VISUALIZZA_ANTEPRIMA, OnUpdateVisualizzaAnteprima)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CText2vidApp construction

const struct VIDEO_SIZE CText2vidApp::qsv[4]= {
	{320,240},
	{640,480},
	{800,600},
	{1024,768}
	};
const BYTE CText2vidApp::qfr[5]= { 1,2,5,10,25 };

CText2vidApp::CText2vidApp() {
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	}

/////////////////////////////////////////////////////////////////////////////
// The one and only CText2vidApp object

CText2vidApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {686A1A8B-5885-4C6C-B4C6-2C4C1E214070}
static const CLSID clsid =
{ 0x686a1a8b, 0x5885, 0x4c6c, { 0xb4, 0xc6, 0x2c, 0x4c, 0x1e, 0x21, 0x40, 0x70 } };

/////////////////////////////////////////////////////////////////////////////
// CText2vidApp initialization

BOOL CText2vidApp::InitInstance() {
	int i;
	char myBuf[256];
	CString S;

	// Initialize OLE libraries
	if (!AfxOleInit()) {
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
		}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	m_iSavedVersionMajor=HIWORD(theApp.getVersione());
	m_iSavedVersionMinor=LOWORD(theApp.getVersione());

	// Change the registry key under which our settings are stored.
	SetRegistryBase(_T("ADPM Synthesis"));

	prStore=new CProfileStore(m_hInstance,m_pszRegistryKey,m_pszAppName);
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	INITCOMMONCONTROLSEX iccex = { sizeof(INITCOMMONCONTROLSEX), ICC_DATE_CLASSES };
  // Init the common controls.
  InitCommonControlsEx(&iccex);
	
  // Rest of InitInstance goes here
	EnableLoadWindowPlacement(TRUE);
	{
	CRect rc;		// qua cmq non lo uso... mettere RC in winApp??
	int nFlags,nCmd;
/*	rc.right=min(GetSystemMetrics(SM_CXSCREEN)-24,800);
	rc.bottom=min(GetSystemMetrics(SM_CYSCREEN)-48,600);
	rc.left=220;
	rc.top=120;*/
	if(theApp.m_bLoadWindowPlacement)
		theApp.LoadWindowPlacement(rc,nFlags,nCmd);
	}

	m_bSaveState=TRUE;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CText2vidDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CText2vidView));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// Note: SDI applications register server objects only if /Embedding
		//   or /Automation is present on the command line.

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

  outputFile=GetString(MAKEINTRESOURCE(IDS_NOMEFILE));
	if(outputFile.IsEmpty())
		outputFile="image";
	ColorFore=GetInt(MAKEINTRESOURCE(IDS_COLOREFORE));
	if(!ColorFore)
		ColorFore=RGB(255,255,255);
	ColorBack=GetInt(MAKEINTRESOURCE(IDS_COLOREBACK));
	if(!ColorBack)			// :)
		ColorBack=RGB(0,0,0);
  Font=GetString(MAKEINTRESOURCE(IDS_FONT));
	if(Font.IsEmpty())
		Font="Arial";

	Align=GetInt(MAKEINTRESOURCE(IDS_ALIGNMENT));
	TextSize=GetInt(MAKEINTRESOURCE(IDS_TEXTSIZE));
	if(!TextSize)
		TextSize=32;
	ImageSize=GetInt(MAKEINTRESOURCE(IDS_IMGSIZE));
	if(!ImageSize)
		ImageSize=0;		// :) 320*240
	FpS=GetInt(MAKEINTRESOURCE(IDS_FPS));		// 0 è ok
	DurataFrame=GetInt(MAKEINTRESOURCE(IDS_DURATA));
	if(!DurataFrame)
		DurataFrame=1;
	Transizione=GetInt(MAKEINTRESOURCE(IDS_TRANSIZIONE));
	Codec=GetInt(MAKEINTRESOURCE(IDS_CODEC));
	if(!Codec)
		Codec=mmioFOURCC('I','V','5','0');
	
	bPreview=GetInt(MAKEINTRESOURCE(IDS_PREVIEW));

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
	}


DWORD CText2vidApp::getVersione(char *n,char *n1,const char *language) const {
	char szGetName[128];
	int wRootLen,bRetCode,i,j;
	LPSTR lpVersion,lpV2;
	DWORD	dwVersion=-1;
	DWORD	dwResult;
	UINT uVersionLen,uV2;

	if(n)
		*n=0;
	if(n1)
		*n1=0;


	try {

/*  HMODULE hLib = AfxGetResourceHandle();

	// questo sistema va, ma NON quando è chiamato dal task TCPIP (o meglio, solo se prima è stato chiamato dal task principale...)
  
  HRSRC hVersion = FindResource(hLib, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
  if(hVersion != NULL) {
    HGLOBAL hGlobal = LoadResource(hLib, hVersion); 
    if(hGlobal != NULL) {
			LPVOID lpstrVffInfo = LockResource(hGlobal);  

	*/

	// ... quindi faccio così!
		//read file name, example: application exe or dll
		char  strPath[255];
		GetModuleFileNameA(NULL,strPath, _MAX_PATH);

		//read file size
		DWORD nVerSize = GetFileVersionInfoSize(strPath,0);
		void *lpstrVffInfo = new DWORD [nVerSize];

		//read data related to version
		GetFileVersionInfo(strPath,0, nVerSize, lpstrVffInfo);

			if(lpstrVffInfo != NULL) {
				_tcscpy(szGetName, "\\StringFileInfo\\041004b0\\");	
				//language USARE!
				wRootLen = _tcslen(szGetName);
					
				_tcscat(szGetName, "ProductName");
				uVersionLen=uV2=0;
				lpVersion=lpV2=NULL;
				bRetCode = VerQueryValue((LPVOID)lpstrVffInfo,(LPSTR)szGetName,
								(LPVOID *)&lpVersion,(UINT *)&uVersionLen);
				szGetName[wRootLen] = 0;
				_tcscat(szGetName, "FileVersion");
				bRetCode = VerQueryValue((LPVOID)lpstrVffInfo,(LPSTR)szGetName,
								(LPVOID *)&lpV2,(UINT *)&uV2);

				if(bRetCode && uVersionLen && lpVersion && uV2 && lpV2) {
					DWORD i1,i2,i3,i4;
					char n2[64];
					sscanf(lpV2,"%u,%u,%u,%u",&i1,&i2,&i3,&i4);		// tolgo spazi e metto puntini al posto di virgole
					wsprintf(n2,"%u.%u.%u.%u",i1,i2,i3,i4);
					if(n) {
						_tcscpy(n,lpVersion);
						_tcscat(n," ");
						_tcscat(n,n2);
						}
					dwVersion=MAKELONG(i2,i1);
					}
				else {
					dwResult = GetLastError();
					if(n)
						wsprintf(n, "Error %lu", dwResult);
					}

				if(n1) {
					szGetName[wRootLen] = (char)0;
					_tcscat(szGetName, "LegalCopyright");
					uVersionLen   = 0;
					lpVersion     = NULL;
					bRetCode      =  VerQueryValue((LPVOID)lpstrVffInfo,(LPSTR)szGetName,
									(LPVOID *)&lpVersion,(UINT *)&uVersionLen);

					if(bRetCode && uVersionLen && lpVersion) {
						_tcscpy(n1, lpVersion);
						}
					else {
						dwResult = GetLastError();
						wsprintf(n1, "Error %lu", dwResult);
						}
					}
				delete lpstrVffInfo;
				}		/*
			UnlockResource(hGlobal);
			FreeResource(hGlobal);
			}
	  }*/

	}
	catch(...) {
		dwVersion=-1;
	}	

	return dwVersion;
	}



//----------------------------------------------------------------------------------
TCHAR *CProfileStore::emptyString=_T("");
TCHAR *CProfileStore::mainSubKey=_T("Software");

#ifndef _CPRIVATEPROFILE_USEINI
CProfileStore::CProfileStore(HINSTANCE h,CString s,CString s1) {
#else
CProfileStore::CProfileStore(HINSTANCE h,CString s) {
#endif

	m_hInstance=h;
	theRoot=/* HKEY_CURRENT_USER */ HKEY_LOCAL_MACHINE;
	variabiliKey=_T("");
	regRoot=s;
#ifndef _CPRIVATEPROFILE_USEINI
	regRoot+="\\";
	regRoot+=s1;
	regRoot+="\\";
	regRoot+="Settings";
#endif
	}

CString CProfileStore::getProfileKey(CString s) const {
	CString d;

	d=regRoot;
	if(!s.IsEmpty()) {
		d+=_T("\\");
		d+=s;
		}
	return d;
	}

CString CProfileStore::getVariabiliKey() const {
	CString s;

#ifdef _CPRIVATEPROFILE_USEINI
	s=variabiliKey;
#else
//	s=regRoot;		bah no... 2026
//	s+=variabiliKey;
	s=variabiliKey;		
#endif
	return s;
	}




int CProfileStore::WritePrivateProfileString(const TCHAR *s,CString v,const TCHAR *n) {

#ifdef _CPRIVATEPROFILE_USEINI

	return ::WritePrivateProfileString(s,v,n,regRoot);

#else

	CString myBuf;
	HKEY pk,pksub,pksub2;
	int i,retVal=-1;

	myBuf=getProfileKey(s);
	if(!RegCreateKeyEx(theRoot,mainSubKey,0L,emptyString,REG_OPTION_NON_VOLATILE,
		KEY_WRITE,NULL,&pk,NULL)) {
		if(!RegCreateKeyEx(pk,myBuf,0L,emptyString,REG_OPTION_NON_VOLATILE,
			KEY_WRITE,NULL,&pksub,NULL)) {
			retVal=RegSetValueEx(pksub,v,0,REG_SZ,(const BYTE *)(LPCTSTR)n,_tcslen(n)*sizeof(TCHAR));
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}

	return retVal;
#endif

  }

int CProfileStore::WritePrivateProfileInt(const TCHAR *s,CString v,int n) {
	CString myBuf;

#ifdef _CPRIVATEPROFILE_USEINI

	myBuf.Format(_T("%d"),n);
	return ::WritePrivateProfileString(s,v,myBuf,regRoot);

#else

	HKEY pk,pksub,pksub2;
	int i,retVal=-1;

	myBuf=getProfileKey(s);
	if(!RegCreateKeyEx(theRoot,mainSubKey,0L,emptyString,REG_OPTION_NON_VOLATILE,
		KEY_WRITE,NULL,&pk,NULL)) {
		if(!RegCreateKeyEx(pk,myBuf,0L,emptyString,REG_OPTION_NON_VOLATILE,
			KEY_WRITE,NULL,&pksub,NULL)) {
			retVal=RegSetValueEx(pksub,v,0,REG_DWORD,(const BYTE *)&n,4);
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}

	return retVal;

#endif

  }

int CProfileStore::WritePrivateProfileDouble(const TCHAR *s,CString v,double n) {
	CString myBuf;
	CString myBuf3;

#ifdef _CPRIVATEPROFILE_USEINI

//	_gcvt(n,9,myBuf);
	myBuf.Format(_T("%g"),n);
	return ::WritePrivateProfileString(s,v,myBuf,regRoot);

#else

	HKEY pk,pksub,pksub2;
	int i,retVal=-1;

	myBuf=getProfileKey(s);
	if(!RegCreateKeyEx(theRoot,mainSubKey,0L,emptyString,REG_OPTION_NON_VOLATILE,
		KEY_WRITE,NULL,&pk,NULL)) {
		if(!RegCreateKeyEx(pk,myBuf,0L,emptyString,REG_OPTION_NON_VOLATILE,
			KEY_WRITE,NULL,&pksub,NULL)) {
			myBuf3.Format(_T("%g"),n);
//			_gcvt(n,9,myBuf3);
			retVal=RegSetValueEx(pksub,v,0,REG_SZ,(BYTE *)(LPCTSTR)myBuf3,myBuf3.GetLength());
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}

	return retVal;

#endif

  }

int CProfileStore::GetPrivateProfileString(const TCHAR *s, CString k, TCHAR *v, int len, const TCHAR *def) {

#ifdef _CPRIVATEPROFILE_USEINI

	return ::GetPrivateProfileString(s,k,def,v,len,regRoot);

#else

	CString myBuf;
	HKEY pk,pksub;
	int i,retVal=-1;
	DWORD vType,vLen=len;

	myBuf=getProfileKey(s);
	if(def)
		_tcscpy(v,def);
	else
		*v=0;
	if(!RegOpenKeyEx(theRoot,mainSubKey,0L,KEY_READ,&pk)) {
		if(!RegOpenKeyEx(pk,myBuf,0L,KEY_READ,&pksub)) {
			retVal=RegQueryValueEx(pksub,k,0,NULL /*&vType*/,(BYTE*)v,&vLen);
			if(retVal == ERROR_SUCCESS)
				;
			else
				_tcsncpy(v,def,len);
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}

	return retVal;

#endif

	}

int CProfileStore::GetPrivateProfileInt(const TCHAR *s, CString k, int def) {

#ifdef _CPRIVATEPROFILE_USEINI

	return ::GetPrivateProfileInt(s,k,def,regRoot);

#else

	CString myBuf;
	HKEY pk,pksub;
	int i,v,retVal=def;
	DWORD vType,vLen=4;

	myBuf=getProfileKey(s);
	v=def;
	if(!RegOpenKeyEx(theRoot,mainSubKey,0L,KEY_READ,&pk)) {
		if(!RegOpenKeyEx(pk,myBuf,0L,KEY_READ,&pksub)) {
			i=RegQueryValueEx(pksub,k,0,NULL /*&vType*/,(BYTE *)&v,&vLen);
			if(i == ERROR_SUCCESS)
				retVal=v;
			else
				retVal=def;
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}
	return retVal;

#endif

	}

double CProfileStore::GetPrivateProfileDouble(const TCHAR *s, CString k, double def) {
	CString myBuf;
	char myBuf2[128];
	char myBuf3[128];

	_gcvt(def,9,myBuf2);

#ifdef _CPRIVATEPROFILE_USEINI

	::GetPrivateProfileString(s,k,myBuf2,myBuf3,32,regRoot);
	return atof(myBuf3);

#else

	HKEY pk,pksub;
	int i;
	double retVal=0;
	DWORD vType,vLen=128;

	myBuf=getProfileKey(s);
	if(!RegOpenKeyEx(theRoot,mainSubKey,0L,KEY_READ,&pk)) {
		if(!RegOpenKeyEx(pk,myBuf,0L,KEY_READ,&pksub)) {
			i=RegQueryValueEx(pksub,k,0,NULL /*&vType*/,(BYTE *)myBuf3,&vLen);
			if(i == ERROR_SUCCESS)
				retVal=atof(myBuf3);
			else
				retVal=def;
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}
	return retVal;

#endif

	}

CTime CProfileStore::GetPrivateProfileTime(const TCHAR *s,CString v) {
	TCHAR myBuf[64];
	int i,h,d,m,y;

	GetPrivateProfileString(s,v,myBuf,18 /*"01/01/1997 00:00"*/);
	d=*myBuf ? _ttoi(myBuf) : 1;
	m=*myBuf ? _ttoi(myBuf+3) : 1;
	y=*myBuf ? _ttoi(myBuf+6) : 1997;
	h=*myBuf ? _ttoi(myBuf+11) : 0;
	i=*myBuf ? _ttoi(myBuf+14) : 0;
	{
		CTime t(y,m,d,h,i,0);
		return t;
		}
	}

CTimeSpan CProfileStore::GetPrivateProfileTimeSpan(const TCHAR *s,CString v) {
	TCHAR myBuf[64];
	int i,h,m;

	GetPrivateProfileString(s,v,myBuf,8 /*"00:00"*/);
	h=*myBuf ? _ttoi(myBuf) : 1;
	m=*myBuf ? _ttoi(myBuf+3) : 1;
	{
		CTimeSpan ts(0,h,m,0);
		return ts;
		}
	}

int CProfileStore::WritePrivateProfileTime(const TCHAR *s,CString v,CTime t) {
	CString c;

#ifndef _WIN32_WCE
	c=t > 0 ? t.Format("%d/%m/%Y %H:%M") : emptyString;
#else
// in CE non linka CTime::Format... boh?
	CString S;
	if(t>0)
		S.Format(_T("%u/%u/%u %u:%u"),t.GetDay(),t.GetMonth(),t.GetYear(),t.GetHour(),t.GetMinute());

	c=t > 0 ? S : emptyString;
#endif
	
	return WritePrivateProfileString(s,v,(LPCTSTR)c);
	}

int CProfileStore::WritePrivateProfileTime(const TCHAR *s,CString v,CTimeSpan t) {
	CString c;

	c=t.Format(_T("%H:%M"));
	return WritePrivateProfileString(s,v,c);
	}


#ifdef _CPRIVATEPROFILE_USEINI
int CProfileStore::WritePrivateProfileInt(const TCHAR *s,CString v,int n,const TCHAR *nf) {
	TCHAR myBuf[64];

	itoa(n,myBuf,10);
	return ::WritePrivateProfileString(s,v,myBuf,nf);
  }
#endif


int CProfileStore::FlushProfile() {

#ifdef _CPRIVATEPROFILE_USEINI
	return ::WritePrivateProfileString(NULL,NULL,NULL,regRoot);
#else
	HKEY pk,pksub;
	CString myBuf;
	int i;

	myBuf=regRoot;
	if(!RegOpenKeyEx(theRoot,mainSubKey,0L,KEY_WRITE,&pk)) {
		if(!RegOpenKeyEx(pk,myBuf,0L,KEY_READ,&pksub)) {
			i=RegFlushKey(pksub);
//			theApp.FileSpool->print(CLogFile::flagInfo,"Flushkey: %x lstErr=%x",i,i);
			RegCloseKey(pksub);
			}
		RegCloseKey(pk);
		}
	return i;
#endif
	}



int CWinAppEx::ReloadWindowPlacement(class CFrameWnd *w) {
	CRect r;
	int n,n2;
	return LoadWindowPlacement(r,n,n2);  // boh...
	}
int CWinAppEx::StoreWindowPlacement(const CRect& rectNormalPosition,int nFlags,int nShowCmd) {
	char myBuf[128];

	if(!nShowCmd) {		// non  esattamente cos ma ok
		wsprintf(myBuf,"%d,%d,%d,%d",rectNormalPosition.left,rectNormalPosition.top,rectNormalPosition.right,rectNormalPosition.bottom);
		prStore->WriteProfileVariabileString(IDS_COORDINATE,myBuf);
		}

	return 1;
	}
int CWinAppEx::LoadWindowPlacement(class CRect &rc,int &n,int &n2) {
	CString S;

	S=GetString(MAKEINTRESOURCE(IDS_COORDINATE),"20,20");
	return sscanf((LPCTSTR)S,"%d,%d,%d,%d",&rc.left,&rc.top,&rc.right,&rc.bottom);
	}
void CWinAppEx::OnClosingMainFrame() {
	CRect r;
	if(m_bLoadWindowPlacement) {
		if(m_pMainWnd) {
			m_pMainWnd->GetWindowRect(&r);
			StoreWindowPlacement(r,0,m_pMainWnd->IsIconic());
			}
		}
	}
void CWinAppEx::OnAppContextHelp(class CWnd *w,unsigned long const * const n) {
	}
int CWinAppEx::ShowPopupMenu(unsigned int n,class CPoint const &pt,class CWnd *w) {
	CMenu myMenu;
	// n FINIRE se serve :)
	myMenu.GetSubMenu(2)->GetSubMenu(2)->GetSubMenu(0)->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, pt.x, pt.y, w);
	return 1;
	}
int CWinAppEx::OnViewDoubleClick(class CWnd *w,int n) {
	return 1;
	}
int CWinAppEx::CleanState(char const *s) {
	return 1;
	}
struct CRuntimeClass *CWinAppEx::GetRuntimeClass() const {
	return CWinApp::GetRuntimeClass();
	}
CWinAppEx::CWinAppEx(int n) : m_bResourceSmartUpdate(0) {
	}
int CWinAppEx::ExitInstance() {
	CRect r;

//	if(m_bSaveState)		// qua non la prende... :(
//		SaveState((CFrameWnd*)m_pMainWnd,NULL,TRUE);
	delete prStore;
	return CWinApp::ExitInstance();
	}
CWinAppEx::~CWinAppEx() {
//AfxMessageBox("~");
	}
BOOL CWinAppEx::WriteInt(LPCTSTR lpszEntry, int nValue) {
	if(prStore)
		return prStore->WritePrivateProfileInt(prStore->getVariabiliKey(), lpszEntry, nValue);
	}
BOOL CWinAppEx::WriteString(LPCTSTR lpszEntry, LPCTSTR lpszValue) {
	if(prStore)
		return prStore->WritePrivateProfileString(prStore->getVariabiliKey(), lpszEntry, lpszValue);
	}
BOOL CWinAppEx::WriteSectionInt(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nValue) {
	if(prStore)
		return prStore->WritePrivateProfileInt(lpszSubSection, lpszEntry, nValue);
	}
BOOL CWinAppEx::WriteSectionString(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszValue) {
	if(prStore)
		return prStore->WritePrivateProfileString(lpszSubSection, lpszEntry,lpszValue);
	}
int CWinAppEx::GetInt(LPCTSTR lpszEntry, int nDefault) {
	if(prStore)
		return prStore->GetPrivateProfileInt(prStore->getVariabiliKey(), lpszEntry, nDefault);
	}
CString CWinAppEx::GetString(LPCTSTR lpszEntry, LPCTSTR lpszDefault) {
	char myBuf[256];
	CString S;
	myBuf[0]=myBuf[1]=0;
	if(prStore)
		prStore->GetPrivateProfileString(prStore->getVariabiliKey(), lpszEntry, myBuf,255, lpszDefault);
	S=myBuf;
	return S;
	}
int CWinAppEx::GetSectionInt(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, int nDefault) {
	if(prStore)
		return prStore->GetPrivateProfileInt(lpszSubSection,lpszEntry,nDefault);
	}
CString CWinAppEx::GetSectionString(LPCTSTR lpszSubSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault) {
	char myBuf[256];
	CString S;
	myBuf[0]=myBuf[1]=0;
	if(prStore) {
		prStore->GetPrivateProfileString(lpszSubSection,lpszEntry,myBuf,255,lpszDefault);
		}
	S=myBuf;
	return S;
	}
LPCTSTR CWinAppEx::SetRegistryBase(LPCTSTR lpszSectionName) {
	SetRegistryKey(lpszSectionName);
	return m_pszRegistryKey;
	}
CString CWinAppEx::GetRegSectionPath(LPCTSTR szSectionAdd) {
	if(prStore) {
		return prStore->getProfileKey(szSectionAdd);
		}
	}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog {
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CText2vidApp::OnAppAbout() {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	}


/////////////////////////////////////////////////////////////////////////////
// CText2vidApp message handlers

int CText2vidApp::writeTextToBitmap(const CBitmap *b,const char *s,int x,int y,BYTE ha,BYTE va,
																		COLORREF colorf,COLORREF colorb,WORD size) {
	int xSize,ySize;
	int i,n;
	DWORD flags;
//	BYTE *p,*p1;
	CFont myFont,*oldFont;
	CDC *dc,dc1;
	BITMAP bmp;
	RECT rc,sz;
	CBitmap *oldB;

	((CBitmap*)b)->GetBitmap(&bmp);
	rc.top=y;
	rc.left=x;
	rc.bottom=bmp.bmHeight;
	rc.right=bmp.bmWidth;
	xSize=bmp.bmWidth/4,ySize=bmp.bmHeight/2;		// mah, tanto per autoadeguarsi ... ad altezza

  myFont.CreateFont(size,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,Font);
	dc=theApp.m_pMainWnd->GetDC();
//	dc=GetDesktopDC();
	dc1.CreateCompatibleDC(dc);
//	dc2.CreateCompatibleDC(&dc1);

//					CBitmap pBitmap2; pBitmap2.CreateCompatibleBitmap(&dc1,rc.right,rc.bottom);
//					oldB2=(CBitmap*)dc3.SelectObject((CBitmap*)&pBitmap2);
//					BITMAP bmp2;
//					pBitmap2.GetBitmap(&bmp2);

	oldB=(CBitmap*)dc1.SelectObject((CBitmap*)b);
	oldFont=(CFont*)dc1.SelectObject(myFont);

	dc1.SetTextColor(colorf);
	dc1.SetBkColor(colorb);			// 



	sz=rc;
	i=dc1.DrawText(s,&sz,DT_WORDBREAK | DT_CALCRECT);

	flags=DT_WORDBREAK /*0*/;
//	i=dc1.TextOut(x,y,s,_tcslen(s));
	switch(ha) {
		case 0:
			flags |= DT_LEFT;
			break;
		case 1:
		case 3:		// per ora...
			flags |= DT_CENTER;
			break;
		case 2:
			flags |= DT_RIGHT;
			break;
		}
	// DT_VCENTER  solo se single line
	switch(va) {
		case 0:
			break;
		case 1:
			rc.top=(rc.bottom-sz.bottom)/2;
			break;
		case 2:
			rc.top=rc.bottom-sz.bottom;
			break;
		}
	i=dc1.DrawText(s,&rc,flags);



	if(bPreview) {
		CDC dc2;
		CBitmap *pBitmap;
	CBitmap *oldB;
//	dc->SetOutputDC(hDC);
	dc2.CreateCompatibleDC(&dc1);		// non e' necessario Delete: lo fa il distruttore!
	pBitmap=new CBitmap;
	if(!pBitmap)
		goto salva_non_ok;
//	pBitmap->CreateBitmap(myRect.right,myRect.bottom,1,8,NULL);
	pBitmap->CreateCompatibleBitmap(dc,rc.right,rc.bottom);
	oldB=(CBitmap*)dc2.SelectObject(*pBitmap);
	i=dc->BitBlt(0,0,rc.right,rc.bottom,&dc1,0,0,SRCCOPY);
//	dc2.SelectObject(oldB);

	delete pBitmap;
salva_non_ok: ;
	}

//	dc2.SelectObject(oldB2);
	dc1.SelectObject(oldB);
//	dc1.SelectObject(oldFont);


	dc1.DeleteDC();
//	dc2.DeleteDC();
	theApp.m_pMainWnd->ReleaseDC(dc);
  DeleteObject(myFont);

	return 1;
	}

void CText2vidApp::OnFileCreajpeg() {
	BITMAPINFO biRawDef,biCompDef;
	char myBuf[256],myBuf2[256];
  int i,n;
	int retVal;
	RECT rc,rc2;
	HANDLE hBitmap;
	HBITMAP *phBitmap=NULL;
	LPBITMAPINFOHEADER pBitmap;		// NON HBITMAP !!
	char NomeIn[256],Note[256];
	HRESULT hr; 
	static int xSize,ySize,xSizeCap,ySizeCap;
	DWORD l,len,ti;
	BYTE *p,*p1;
	CFile mF;
	CStringEx S,S2;
	CSize sz;


	CCreaDlg ccd;

	S2=((CMainFrame*)m_pMainWnd)->GetActiveView()->GetDocument()->GetTitle();
	_splitpath((LPCTSTR)S2,NULL,NULL,NomeIn,NULL);
	if(ccd.DoModal(NomeIn,0) == IDOK) {
		CBitmap b;
		BITMAP bmp;


		biRawDef.bmiHeader.biWidth=qsv[ImageSize].imageSize.cx;
		biRawDef.bmiHeader.biHeight=qsv[ImageSize].imageSize.cy;
		biRawDef.bmiHeader.biClrUsed=biRawDef.bmiHeader.biClrImportant=0;
		biRawDef.bmiHeader.biCompression=BI_RGB;
		biRawDef.bmiHeader.biPlanes=1;
		biRawDef.bmiHeader.biXPelsPerMeter=biRawDef.bmiHeader.biYPelsPerMeter=0;;
		biRawDef.bmiHeader.biBitCount=32;
		biRawDef.bmiHeader.biSizeImage=(biRawDef.bmiHeader.biWidth*biRawDef.bmiHeader.biHeight*biRawDef.bmiHeader.biBitCount)/8;

		biCompDef=biRawDef;


//			i=b.LoadBitmap(IDB_MONOSCOPIO);

		rc2.top=rc2.left=0;
		rc2.bottom=biCompDef.bmiHeader.biHeight;
		rc2.right=biCompDef.bmiHeader.biWidth;


		n=0;
		do {
			S=readString();
			if(S.IsEmpty())
				break;

			{
			i=theApp.m_pMainWnd->GetDC()->GetDeviceCaps(BITSPIXEL);
			b.CreateBitmap(rc2.right,rc2.bottom,1,i /*32 va! altri valori, 24, no */,NULL);
			DWORD *pBmp=(DWORD*)GlobalAlloc(GPTR,rc2.right*rc2.bottom*4),*pBmp1=pBmp;
			int x,y;
			for(y=0; y<rc2.bottom; y++) {
				for(x=0; x<rc2.right; x++) {
					*(BYTE*)pBmp1=LOBYTE(HIWORD(ColorBack));
					*(((BYTE*)pBmp1)+1)=HIBYTE(LOWORD(ColorBack));
					*(((BYTE*)pBmp1)+2)=LOBYTE(LOWORD(ColorBack));
					pBmp1=(DWORD*)(((DWORD)pBmp1)+4);
					}
				}
			b.SetBitmapBits((rc2.right*rc2.bottom*i)/8,pBmp);
			GlobalFree(pBmp);
			}
			writeTextToBitmap(&b,S /*"culo"*/,0,0,LOWORD(Align),HIWORD(Align),ColorFore,ColorBack,TextSize);

			CJpeg *myJPEG;
			CString S2=CTime::GetCurrentTime().Format("text2vid_%d_%m_%Y_%H_%M");

			// PRENDERE NOME DOCUMENTO INPUT!

			theApp.getVersione(myBuf2);
			myJPEG=new CJpeg(myBuf2);
//				p1=myJPEG->buildJPEG(IDB_MONOSCOPIO,&len,75);
			p1=myJPEG->buildJPEG(&b,&len,FALSE,70,S2,0,inputFile /*"text2vid"*/);

			S2.Format("%s_%03u.jpg",ccd.m_NomeFile,n++);
			mF.Open(S2,CFile::modeCreate | CFile::modeWrite);
			mF.Write(p1,len);
			mF.Close();

			outputFile=ccd.m_NomeFile;

			b.DeleteObject();
			HeapFree(GetProcessHeap(),0,p1);
			delete myJPEG;
			} while(!S.IsEmpty() && n<1000);		// safety!

		resetInput();

		((CMainFrame*)m_pMainWnd)->SetStatusText("fatto");


		retVal=1;
		}
	
	}

BYTE *CText2vidApp::mergeImages(LPBITMAPINFOHEADER pBitmap,BYTE *image1,double val1,BYTE *image2,double val2,BYTE *imageOut) {
	BYTE r1,g1,b1, r2,g2,b2, r3,g3,b3;
	int x,y;
	BYTE *pImg=imageOut;


	for(y=0; y<pBitmap->biHeight; y++) {
		for(x=0; x<pBitmap->biWidth; x++) {		// verificare pad dword...
			r1=GetRValue(*(DWORD*)image1);
			g1=GetGValue(*(DWORD*)image1);
			b1=GetBValue(*(DWORD*)image1);
			r2=GetRValue(*(DWORD*)image2);
			g2=GetGValue(*(DWORD*)image2);
			b2=GetBValue(*(DWORD*)image2);

			r3=r1*val1+r2*val2;
			g3=g1*val1+g2*val2;
			b3=b1*val1+b2*val2;

			*pImg++=b3;
			*pImg++=g3;
			*pImg++=r3;
			image1+=3;
			image2+=3;
			}
		}

	return imageOut;
	}

void CText2vidApp::OnFileCreavideo() {
	BITMAPINFO biRawDef,biCompDef;
	PAVIFILE aviFile;
	PAVISTREAM psVideo, psAudio, psText;
	DWORD vFrameNum4Save,aFrameNum4Save,saveWait4KeyFrame;
	DWORD maxFrameSize;
	HIC hICCo=NULL;
	char myBuf[256],myBuf2[256];
  int i,n;
	int retVal;
	RECT rc,rc2;
	HANDLE hBitmap;
	HBITMAP *phBitmap=NULL;
	LPBITMAPINFOHEADER pBitmap;		// NON HBITMAP !!
	char NomeIn[256],Note[256];
	AVISTREAMINFO strhdr;
	HRESULT hr; 
	AVICOMPRESSOPTIONS opts; 
	LPAVICOMPRESSOPTIONS aopts[1] = {&opts}; 
	PAVISTREAM myps=NULL;
	DWORD dwTextFormat; 
//	DWORD compressor=mmioFOURCC('I','V','5','0');	/*fisso per ora*/
	int xSize,ySize,xSizeCap,ySizeCap;
	double step1,step2,sstep;
	DWORD l,len,ti;
	BYTE *p,*p1;
	DWORD *pBmpBack=NULL,*pBmpText=NULL,*pBmpPrec=NULL,*pBmpSeg=NULL;
	CFile mF;
	CString S;
	CStringEx S2;

	CCreaDlg ccd;

	S2=((CMainFrame*)m_pMainWnd)->GetActiveView()->GetDocument()->GetTitle();
	_splitpath((LPCTSTR)S2,NULL,NULL,NomeIn,NULL);
	if(ccd.DoModal(NomeIn,1) == IDOK) {

		CBitmap b;
		BITMAP bmp;


		biRawDef.bmiHeader.biWidth=qsv[ImageSize].imageSize.cx;
		biRawDef.bmiHeader.biHeight=qsv[ImageSize].imageSize.cy;
		biRawDef.bmiHeader.biClrUsed=biRawDef.bmiHeader.biClrImportant=0;
		biRawDef.bmiHeader.biCompression=BI_RGB;
		biRawDef.bmiHeader.biPlanes=1;
		biRawDef.bmiHeader.biXPelsPerMeter=biRawDef.bmiHeader.biYPelsPerMeter=0;;
		biRawDef.bmiHeader.biBitCount=24;		// sarebbero 32 ma poi non va... faccio io a mano, v.sotto
		biRawDef.bmiHeader.biSizeImage=(biRawDef.bmiHeader.biWidth*biRawDef.bmiHeader.biHeight*biRawDef.bmiHeader.biBitCount)/8;



//			i=b.LoadBitmap(IDB_MONOSCOPIO);

/*		rc2.top=rc2.left=0;
		rc2.bottom=biCompDef.bmiHeader.biHeight;
		rc2.right=biCompDef.bmiHeader.biWidth;
*/


		psVideo=NULL, psAudio=NULL, psText = NULL;
		aviFile=NULL;

		maxFrameSize=(biRawDef.bmiHeader.biWidth*biRawDef.bmiHeader.biHeight*biRawDef.bmiHeader.biBitCount)/8;

		biCompDef=biRawDef;
		biCompDef.bmiHeader=biRawDef.bmiHeader;
		biCompDef.bmiHeader.biCompression=theApp.Codec;
		biCompDef.bmiHeader.biBitCount=24;		// buono per IR50
		biCompDef.bmiHeader.biSizeImage=(biCompDef.bmiHeader.biWidth*biCompDef.bmiHeader.biHeight*biCompDef.bmiHeader.biBitCount)/8;

		AVIFileInit();  // Open the movie file for writing....

		vFrameNum4Save=0;

		S2.Format("%s.avi",ccd.m_NomeFile);
		remove(S2);		// non cancella MAI il file precedente... €£$%&
		hr = AVIFileOpen(&aviFile,    // returned file pointer
			S2,            // file name
			/*OF_WRITE | */ OF_CREATE | OF_SHARE_EXCLUSIVE,    // mode to open file with
			NULL);    // use handler determined from file extension....
		if(hr != AVIERR_OK)
			goto errorSaveVideo;
		
		ZeroMemory(&strhdr, sizeof(strhdr));
		strhdr.fccType                = streamtypeVIDEO;// stream type
		strhdr.fccHandler             = theApp.Codec;
		strhdr.dwScale                = 1;
		strhdr.dwRate                 = qfr[FpS];
		strhdr.dwSuggestedBufferSize  = maxFrameSize;
		SetRect(&strhdr.rcFrame, 0, 0,    // rectangle for stream
					(int)biCompDef.bmiHeader.biWidth,
					(int)-biCompDef.bmiHeader.biHeight);  // And create the stream;
		hr = AVIFileCreateStream(aviFile,    // file pointer
														 &myps,    // returned stream pointer
														 &strhdr);    // stream header
		if(hr != AVIERR_OK)
			goto errorSaveVideo;
		hr = AVIStreamSetFormat(myps, 0, &biCompDef.bmiHeader, sizeof(BITMAPINFOHEADER)); 
		if(hr != AVIERR_OK) 
			goto errorSaveVideo;

//	ZeroMemory(&opts, sizeof(opts));
//	if(!AVISaveOptions(NULL, 0, 1, &myps, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
//		goto errorSaveVideo;

#if 0			

		ZeroMemory(&strhdr, sizeof(strhdr)); 
		strhdr.fccType                = streamtypeTEXT; 
		strhdr.fccHandler             = mmioFOURCC('D', 'R', 'A', 'W'); 
		strhdr.dwScale                = 1; 
		strhdr.dwRate                 = 1;
		strhdr.dwSuggestedBufferSize  = 25;
		SetRect(&strhdr.rcFrame, 0, (int) biCompDef.bmiHeader.biHeight,
					(int)biCompDef.bmiHeader.biWidth,     
					(int)biCompDef.bmiHeader.biHeight+(biCompDef.bmiHeader.biHeight/8));  // And create the stream; 
		hr = AVIFileCreateStream(aviFile, &psText, &strhdr); 
		if(hr != AVIERR_OK) 
			goto errorSaveVideo; 
		dwTextFormat = sizeof(dwTextFormat); 
		hr = AVIStreamSetFormat(psText, 0, &dwTextFormat, sizeof(dwTextFormat)); 
		if(hr != AVIERR_OK) 
			goto errorSaveVideo;
#endif

		{
			long n;
			int x,y;
			DWORD t,l;
			CBitmap b;
			BITMAP bmp;
			BYTE *p,*p2,*p3,*pOut;
			RECT rc2;

/*						b.CreateBitmap(biCompDef.bmiHeader.biWidth,biCompDef.bmiHeader.biHeight,1,biCompDef.bmiHeader.biBitCount,NULL);
			b.GetBitmap(&bmp);
			n=biCompDef.bmiHeader.biWidth*biCompDef.bmiHeader.biHeight*biCompDef.bmiHeader.biBitCount/8;
			p=(BYTE *)GlobalAlloc(GPTR,n);
			bmp.bmBits=p;
			for(i=0; i<n; i+=3) {
				*(WORD *)p=0xc0;
				*(BYTE *)(p+2)=0;
				p+=3;
				}*/


			hICCo=ICOpen(ICTYPE_VIDEO,theApp.Codec,ICMODE_FASTCOMPRESS);
			if(hICCo) {

				rc2.top=rc2.left=0;
				rc2.bottom=biCompDef.bmiHeader.biHeight;
				rc2.right=biCompDef.bmiHeader.biWidth;

				i=theApp.m_pMainWnd->GetDC()->GetDeviceCaps(BITSPIXEL)/8;
				{
				pBmpBack=(DWORD*)GlobalAlloc(GPTR,rc2.right*rc2.bottom*i /* (32bpp fisso*/);
				DWORD *pBmp1=pBmpBack;
				int x,y;
				for(y=0; y<rc2.bottom; y++) {
					for(x=0; x<rc2.right; x++) {		// invertire colorback e scrivere dword, ma occhio alla fine!
						*(BYTE*)pBmp1=LOBYTE(HIWORD(ColorBack));
						*(((BYTE*)pBmp1)+1)=HIBYTE(LOWORD(ColorBack));
						*(((BYTE*)pBmp1)+2)=LOBYTE(LOWORD(ColorBack));
						pBmp1=(DWORD*)(((DWORD)pBmp1)+4);
						}
					}
				}
				pBmpPrec=(DWORD*)GlobalAlloc(GPTR,rc2.right*rc2.bottom*i /*32bpp fisso*/);
				pBmpSeg=(DWORD*)GlobalAlloc(GPTR,rc2.right*rc2.bottom*i /*32bpp fisso*/);
				pBmpText=(DWORD*)GlobalAlloc(GPTR,rc2.right*rc2.bottom*i /*32bpp fisso*/);

				maxFrameSize=ICCompressGetSize(hICCo,&biRawDef,&biCompDef);
				i=ICCompressBegin(hICCo,&biRawDef,&biCompDef);

				switch(Transizione) {
					case 0:		// nulla
						step1=step2=0;
						sstep=0;
						break;
					case 1:		// fadein
						step1=0; step2=1;
						sstep=1.0/(qfr[FpS]+1);
						break;
					case 2:		// fadeout
						step1=1; step2=0;
						sstep=1.0/(qfr[FpS]+1);
						break;
					case 3:		// fade in & out
						step1=0; step2=1;
						sstep=1.0/(qfr[FpS]+1);
						break;
					case 4:		// crossfade
						step1=1; step2=0;
						sstep=1.0/(qfr[FpS]+1);
						break;
					}
				do {
					S=readString();
					if(S.IsEmpty())
						break;

					i=theApp.m_pMainWnd->GetDC()->GetDeviceCaps(BITSPIXEL);
					b.CreateBitmap(rc2.right,rc2.bottom,1,i /*32 va! altri valori, 24, no */,NULL);

					b.SetBitmapBits((rc2.right*rc2.bottom*i)/8,pBmpBack);		// ev separare colore sfondo da colore back dei caratteri...
					writeTextToBitmap(&b,S,0,0,LOWORD(Align),HIWORD(Align),ColorFore,ColorBack,TextSize);

//			b2.Resample(&b,&rc2);

					i=b.GetBitmap(&bmp);
					i=bmp.bmWidth*bmp.bmHeight*bmp.bmBitsPixel/8;
					bmp.bmBits=GlobalAlloc(GMEM_FIXED,i);
					i=b.GetBitmapBits(i,bmp.bmBits);
					{
					BYTE *pBmp1=(BYTE*)pBmpText;
					int x,y;
					for(y=rc2.bottom-1; y; y--) {
						BYTE *pBmp=((BYTE*)bmp.bmBits)+y*rc2.right*4   /* ev. pad dword ?*/;
						for(x=0; x<rc2.right; x++) {
							*(BYTE*)pBmp1=*(BYTE*)pBmp;
							*(((BYTE*)pBmp1)+1)=*(((BYTE*)pBmp)+1);
							*(((BYTE*)pBmp1)+2)=*(((BYTE*)pBmp)+2);
							pBmp=pBmp+4;
							pBmp1=pBmp1+3;
							}
						}
					}
// non fa nulla cmq					biCompDef.bmiHeader.biHeight=-240;


					pOut=(BYTE *)GlobalAlloc(GPTR,maxFrameSize+100);

					{
						BYTE j,j1;

						j=qfr[FpS];// durano SEMPRE 1 Secondo per ora
						j1=ICCOMPRESS_KEYFRAME;

						while(j--) {
							switch(Transizione) {
								case 1:		// fadein
								case 3:		// fade in & out
									step1+=sstep;
									step2-=sstep;
									mergeImages(&biRawDef.bmiHeader,(BYTE*)pBmpBack,step1,(BYTE*)pBmpText,step2,(BYTE*)pBmpSeg);
									break;
								case 4:		// crossfade
									step1-=sstep;
									step2+=sstep;
									mergeImages(&biRawDef.bmiHeader,(BYTE*)pBmpPrec,step1,(BYTE*)pBmpText,step2,(BYTE*)pBmpSeg);
									break;
								default:
									continue;
									break;
								}

							t=l=0;
							i=ICCompress(hICCo,j1,
								&biCompDef.bmiHeader,pOut,&biRawDef.bmiHeader,pBmpSeg,
								&l,&t,0,0/*2500*/,7500 /*quality per ora fisso*/,
								NULL,NULL);
							if(i == ICERR_OK) {
								n=AVIStreamWrite(myps,// stream pointer 
									vFrameNum4Save, // time of this frame 
									1,// number to write 
									pOut,
									biCompDef.bmiHeader.biSizeImage,
									t, // flags.... 
									NULL, NULL);
								vFrameNum4Save++;
								j1=0;
								}
							}

						j=DurataFrame*qfr[FpS];
						j1=ICCOMPRESS_KEYFRAME;
						while(j--) {
							t=l=0;
							i=ICCompress(hICCo,j1,
								&biCompDef.bmiHeader,pOut,&biRawDef.bmiHeader,pBmpText,
								&l,&t,0,0/*2500*/,7500 /*quality per ora fisso*/,
								NULL,NULL);
							if(i == ICERR_OK) {
								n=AVIStreamWrite(myps,// stream pointer 
									vFrameNum4Save, // time of this frame 
									1,// number to write 
									pOut,
									biCompDef.bmiHeader.biSizeImage,
									t, // flags.... 
									NULL, NULL);
								vFrameNum4Save++;
								j1=0;
								}
							}
						}

					{
						BYTE j,j1;

						j=qfr[FpS];// durano SEMPRE 1 Secondo per ora
						j1=ICCOMPRESS_KEYFRAME;

						while(j--) {
							switch(Transizione) {
								case 2:		// fadeout
								case 3:		// fade in & out
									step1-=sstep;
									step2+=sstep;
									mergeImages(&biRawDef.bmiHeader,(BYTE*)pBmpText,step1,(BYTE*)pBmpBack,step2,(BYTE*)pBmpSeg);
									break;
								case 0:		// nulla
								case 1:		// fadein
								case 4:		// crossfade
									continue;
									break;
								}

							t=l=0;
							i=ICCompress(hICCo,j1,
								&biCompDef.bmiHeader,pOut,&biRawDef.bmiHeader,pBmpSeg,
								&l,&t,0,0/*2500*/,7500 /*quality per ora fisso*/,
								NULL,NULL);
							if(i == ICERR_OK) {
								n=AVIStreamWrite(myps,// stream pointer 
									vFrameNum4Save, // time of this frame 
									1,// number to write 
									pOut,
									biCompDef.bmiHeader.biSizeImage,
									t, // flags.... 
									NULL, NULL);
								vFrameNum4Save++;
								j1=0;
								}
							}
					}

					GlobalFree(pOut);
					GlobalFree(bmp.bmBits);
					b.DeleteObject();

					memcpy(pBmpPrec,pBmpSeg,biRawDef.bmiHeader.biSizeImage);

					} while(!S.IsEmpty() && vFrameNum4Save<10000);		// safety :)

				resetInput();


				((CMainFrame*)m_pMainWnd)->SetStatusText("fatto");


				}



#if 0
			if(psText && imposeTime) {		// crea pista testo/data... opzionale
				n=AVIStreamLength(psText);
				CString S;
				S=CTime::GetCurrentTime().Format("%d/%m/%Y %H:%M:%S");
				AVIStreamWrite(psText,// stream pointer 
					n+1, // time of this frame 
					1,// number to write 
					(LPSTR)(LPCTSTR)S,
					S.GetLength()+1,
					AVIIF_KEYFRAME, // flags.... 
					NULL, NULL);
				}
#endif

			}

		aFrameNum4Save=0;
		saveWait4KeyFrame=1;
		psVideo=myps;
		retVal=1;
	goto okSaveVideo;

errorSaveVideo:
	retVal=0;
okSaveVideo:


	if(pBmpSeg)
		GlobalFree(pBmpSeg);
	if(pBmpPrec)
		GlobalFree(pBmpPrec);
	if(pBmpText)
		GlobalFree(pBmpText);
	if(pBmpBack)
		GlobalFree(pBmpBack);
	if(psVideo) {
		myps=psVideo;
		psVideo=NULL;
		AVIStreamClose(myps);
		}
	if(psAudio) {
		myps=psAudio;
		psAudio=NULL;
		AVIStreamClose(myps);  
		}
	if(psText) 
		AVIStreamClose(psText);  
	psText=NULL;
	if(aviFile) 
		AVIFileClose(aviFile);  
	aviFile=NULL;
	if(hICCo) {
		ICCompressEnd(hICCo);
		ICClose(hICCo);
		hICCo=NULL;
		}

	AVIFileExit(); 
	retVal=1;

		}

	}

void CText2vidApp::OnOpzioniImmagini() {
	COpzioniDlg cod;

	if(cod.DoModal() == IDOK) {
		ColorFore=cod.m_ForeColor;
		ColorBack=cod.m_BackColor;
		Font=cod.m_Font;
		TextSize=cod.m_FontSize;
		ImageSize=cod.m_DimensioneImmagini;
		FpS=cod.m_FpS;
		DurataFrame=cod.m_Durata;
		Transizione=cod.m_Transizione;
		Codec=cod.m_CompressorV;

		Align=MAKELONG(cod.m_AlignHoriz,cod.m_AlignVert);
		}
	
	}

void CText2vidApp::OnUpdateFileCreavideo(CCmdUI* pCmdUI) {
	
	pCmdUI->Enable(!inputFile.IsEmpty());
	}

void CText2vidApp::OnUpdateFileCreajpeg(CCmdUI* pCmdUI) {

	pCmdUI->Enable(!inputFile.IsEmpty());
	}



void CText2vidApp::OnVisualizzaAnteprima() {

	bPreview=!bPreview;	
	}

void CText2vidApp::OnUpdateVisualizzaAnteprima(CCmdUI* pCmdUI) {
	
	pCmdUI->SetCheck(bPreview != 0);
	}

CDocument* CText2vidApp::OpenDocumentFile(LPCTSTR lpszFileName) {
	CText2vidDoc *p;

	p=(CText2vidDoc*)CWinApp::OpenDocumentFile(lpszFileName);
	if(p) {
		inputFile=lpszFileName;
		inputPos=0;
		}
	return p;
	}

CString CText2vidApp::readString() {
	CStringEx S;
	CStdioFile myFile;
	char myBuf[256];
	int i;
	char *p;

	if(!inputFile.IsEmpty()) {
		myFile.Open(inputFile,CFile::modeRead);
		myFile.Seek(inputPos,CFile::begin);
		do {
			*myBuf=0;
			p=myFile.ReadString(myBuf,255);
			S+=myBuf;
			} while(p && *p != '\n');
		if(S.Right(1)=='\n')
			S=S.Left(S.GetLength() - 1);
		if(S.Right(1)=='\n')
			S=S.Left(S.GetLength() - 1);
		inputPos=myFile.Seek(0,CFile::current);
		myFile.Close();
		}
	return S;
	}

int CText2vidApp::ExitInstance() {

	SaveCustomState();

	WriteString(MAKEINTRESOURCE(IDS_NOMEFILE),outputFile);
	WriteInt(MAKEINTRESOURCE(IDS_COLOREFORE),ColorFore);
	WriteInt(MAKEINTRESOURCE(IDS_COLOREBACK),ColorBack);
	WriteInt(MAKEINTRESOURCE(IDS_ALIGNMENT),Align);
  WriteString(MAKEINTRESOURCE(IDS_FONT),Font);

	WriteInt(MAKEINTRESOURCE(IDS_TEXTSIZE),TextSize);
	WriteInt(MAKEINTRESOURCE(IDS_IMGSIZE),ImageSize);
	WriteInt(MAKEINTRESOURCE(IDS_FPS),FpS);
	WriteInt(MAKEINTRESOURCE(IDS_DURATA),DurataFrame);
	WriteInt(MAKEINTRESOURCE(IDS_TRANSIZIONE),Transizione);
	WriteInt(MAKEINTRESOURCE(IDS_CODEC),Codec);
	WriteInt(MAKEINTRESOURCE(IDS_PREVIEW),bPreview);
	
	return CWinAppEx::ExitInstance();
	}






// -----------------------------------------------------------------------

int CExBitmap::Resample(CBitmap *b,RECT *rcDest) {
	BITMAP bi;
	BYTE *s,*d;
	register BYTE *p1,*p3;
	int x,y,x2,y2,xRatio,yRatio;
	int i,j,j1;
	register DWORD n;

	b->GetBitmap(&bi);
	x=bi.bmWidth;
	y=bi.bmHeight;
	x2=rcDest->right;
	y2=rcDest->bottom;

	n=bi.bmWidth*bi.bmHeight*(bi.bmBitsPixel/8)*bi.bmPlanes;
	s=(BYTE *)GlobalAlloc(GPTR,n);
	b->GetBitmapBits(n,s);

	// in Output solo 24 bit... faccio le conversioni da input se mi servono...
	switch(bi.bmBitsPixel) {
		case 24:
			break;
		case 32:					// questa, in particolare, è come diventa una Risorsa a 24 bit con LoadBitmap...
			for(j=0; j<y; j++) {
				p1=s+j*(x*4);
				p3=s+j*(x*3);
				for(i=0; i<x; i++) {
					n=*(DWORD *)p1;
					*(WORD *)p3=n;
					*(p3+2)=LOBYTE(HIWORD(n));
					p1+=4;
					p3+=3;
					}
				}

			bi.bmBitsPixel=24;				// ...quindi correggo...
			n=bi.bmWidth*bi.bmHeight*(bi.bmBitsPixel/8)*bi.bmPlanes;

//				return -1;
			break;
		case 8:
			return -1;
			break;
		default:
			return -1;
			break;
		}

	xRatio=0;
	yRatio=0;
	d=(BYTE *)GlobalAlloc(GPTR,x2*y2*3 +10000 /* PATCH!! verificare dove sfora...*/);
	if(!d)
		return -1;
//	p3=d;

	if(x==x2 && y==y2) {
		memcpy(d,s,n);
		goto fine;
		}

	for(j=0,j1=0; j<y; j++) {
		p1=s+j*(x*3);
		p3=d+j1*(x2*3);
//		p1=p2;
		xRatio=0;
		for(i=0; i<x; i++) {
			n=*(DWORD *)p1;

			if(x2 < x) {				// se la dest è minore della source...
				*(WORD *)p3=n;
				*(p3+2)=LOBYTE(HIWORD(n));
				do {
					xRatio+=x2;
					p1+=3;
					} while(xRatio<x);
				xRatio-=x;
				p3+=3;
				// forse andrebbe anche incrementato i... non si nota ma...
				}
			else {
				while(xRatio<x2) {
					*(WORD *)p3=n;
					*(p3+2)=LOBYTE(HIWORD(n));
					p3+=3;
					xRatio+=x;
					}
				xRatio-=x2;
				p1+=3;
				} 

			}
		if(y2 < y) {				// se la dest è minore della source...
			while(yRatio<y) {
				yRatio+=y2;
				j++;
				}
			yRatio-=y;
			j1++;
			}
		else {
			int j2=j1+1;
			int x3=x2*3;
			p1=d+j1*x3;
			while(yRatio < (y2-yRatio)) {
				p3=d+j2*x3;
				memcpy(p3,p1,x3);
				yRatio+=y;
				j2++;
				j1++;
				}
			yRatio-=y2;
			} 
		}

fine:
	i=SetBitmapBits(x2*y2*3,d);
	GlobalFree(s);
	GlobalFree(d);

	return 1;
	}

int CExBitmap::Mirror() {
	BYTE *p,*p1,*p2;
	int x,y;
	int i,j;
	DWORD n;
	BITMAP bi;
	BYTE *d;

	GetBitmap(&bi);

	if(bi.bmBitsPixel != 24)		// piccola protezione...
		return -1;
	x=bi.bmWidth;
	y=bi.bmHeight;
	n=bi.bmWidth*bi.bmHeight*(bi.bmBitsPixel/8)*bi.bmPlanes;
	d=(BYTE *)GlobalAlloc(GPTR,n);
	GetBitmapBits(n,d);

	p=(BYTE *)GlobalAlloc(GPTR,x*3+3);
	if(p) {
		for(j=0; j<y; j++) {
			p2=d+j*(x*3);
			memcpy(p,p2,x*3);
			p1=p;
			p2=p2+((x-1)*3);
			for(i=0; i<x; i++) {
				n=*(DWORD *)p1;
				*(WORD *)p2=n;
				*(p2+2)=LOBYTE(HIWORD(n));
				p1+=3;
				p2-=3;
				}
			}
		GlobalFree(p);
		i=SetBitmapBits(n,d);
		GlobalFree(d);
		}

	return 1;
	}

int CExBitmap::Flip() {
	BYTE *p,*ps,*pd;
	int x,y;
	int i;
	DWORD n;
	BITMAP bi;
	BYTE *d;

	GetBitmap(&bi);

	if(bi.bmBitsPixel != 24)		// piccola protezione...
		return -1;
	x=bi.bmWidth*3;
	y=bi.bmHeight;
	n=bi.bmWidth*bi.bmHeight*(bi.bmBitsPixel/8)*bi.bmPlanes;
	d=(BYTE *)GlobalAlloc(GPTR,n);
	GetBitmapBits(n,d);
	p=(BYTE *)GlobalAlloc(GPTR,x+3);
	if(p) {
		ps=d;
		pd=d+x*(y-1);
		y/=2;
		while(y--) {
			memcpy(p,pd,x);
			memcpy(pd,ps,x);
			memcpy(ps,p,x);
			ps+=x;
			pd-=x;
			}
		GlobalFree(p);
		i=SetBitmapBits(n,d);
		GlobalFree(d);
		}

	return 1;
	}

BOOL CExBitmap::CreateBitmap(RECT *rc) {
	return CBitmap::CreateBitmap(rc->right,rc->bottom,1,24,NULL);
	}

CExBitmap::CExBitmap() : CBitmap() {
	}

CExBitmap::CExBitmap(CBitmap b) {
	BITMAP bi;
	BYTE *s;
	int x,y;
	int i;
	DWORD n;

	b.GetBitmap(&bi);
	x=bi.bmWidth;
	y=bi.bmHeight;
	n=bi.bmWidth*bi.bmHeight*(bi.bmBitsPixel/8)*bi.bmPlanes;
	//sarebbe giusto bloccare/convertire a 24 bit, visto che la usiamo solo per questo... (v.sopra)

	s=(BYTE *)GlobalAlloc(GPTR,n);
	b.GetBitmapBits(n,s);
	i=SetBitmapBits(n,s);
	GlobalFree(s);
	}


CString CStringEx::Tokenize(CString delimiter, int& first) {
  CString token;
  int end = Find(delimiter, first);

  if(end != -1) {
    int count = end-first;
    token = Mid(first,count);
    first = end+delimiter.GetLength();
    return token;
	  }
  else {
    int count = GetLength() - first;
    if(count <= 0)
      return "";

    token = Mid(first,count);
    first = GetLength();
    return token;
		}
	}
CStringEx CStringEx::SubStr(int begin, int len) const {
	return CString::Mid(begin, len);
	}
int CStringEx::FindNoCase(CString substr,int start) {
	CString s1=*this,s2;
	s1.MakeUpper();
	s2=substr;
	s2.MakeUpper();
	return s1.Find(s2,start);
	}
CStringEx::CStringEx(int i, const char *format, DWORD options) {

	Format(format,i);
	if(options & COMMA_DELIMIT)
		CStringEx::operator=(CommaDelimitNumber(*this));
	}
CStringEx::CStringEx(double d, const char *format, DWORD options) {

	Format(format,d);
	if(options & COMMA_DELIMIT)
		CStringEx::operator=(CommaDelimitNumber(*this));
	}
CStringEx CStringEx::CommaDelimitNumber(const char *s) {
	CStringEx s2=s;												// convert to CStringEx
	return CommaDelimitNumber(s2);
	}
CStringEx CStringEx::CommaDelimitNumber(CString s2) {
	CStringEx dp;
	CStringEx q2;											// working string
	CStringEx posNegChar=s2.Left(1);				// get the first char
	bool posNeg=!posNegChar.IsDigit(0);			// if not digit, then assume + or -

	if(posNeg) 											// if so, strip off
		s2=s2.Mid(1);
	if(s2.Find(decimalChar)>=0) {
		dp=s2.Mid(s2.Find(decimalChar)+1);							// remember everything to the right of the decimal point
		s2=s2.Left(s2.Find(decimalChar));				// get everything to the left of the first decimal point
		}
	while(s2.GetLength() > 3) {									// if more than three digits...
		CStringEx s3(thousandChar);
		s3+=s2.Right(3);		// insert a comma before the last three digits (100's)
		q2=s3+q2;											// append this to our working string
		s2=s2.Left(s2.GetLength()-3);							// get everything except the last three digits
		}
	q2=s2+q2;												// prepend remainder to the working string
	if(!dp.IsEmpty()) {									// if we have decimal point...
		q2+=decimalChar;							// append it and the digits
		q2+=dp;							// append it and the digits
		}
	if(posNeg)											// if we stripped off a +/- ...
		q2=posNegChar+q2;			// add it back in

	return q2;											// this is our final comma delimited string
	}

CStringEx CStringEx::CommaDelimitNumber(DWORD n) {
	CStringEx q2;

	q2.Format("%u",n);
	q2=CommaDelimitNumber(q2);
	return q2;
	}

BYTE CStringEx::Asc(int pos) {

	return GetAt(pos);
	}

int CStringEx::Val(int base) {

	switch(base) {
		case 10:
		default:
			return atoi((LPCTSTR)this);
			break;
		case 16:		// fare...
			break;
		}
	}

double CStringEx::Val() {

	return strtod((LPCTSTR)this,NULL);
	}

void CStringEx::Repeat(int n) {
	CString s2=*this;

	Empty();
	while(n--)
		CStringEx::operator+=(s2);
	}

void CStringEx::Repeat(const char *s,int n) {

	Empty();
	while(n--)
		CStringEx::operator+=(s);
	}

void CStringEx::Repeat(char c,int n) {

	Empty();
	while(n--)
		CStringEx::operator+=(c);
	}

BOOL CStringEx::IsAlpha(char ch) {

	return (ch>='A' && ch<='Z') || (ch>='a' && ch<='z');
	}

BOOL CStringEx::IsAlpha(int pos) {

	return IsAlpha(GetAt(pos));
	}

BOOL CStringEx::IsAlnum(char ch) {

	return IsAlpha(ch) || IsDigit(ch);
	}

BOOL CStringEx::IsAlnum(int pos) {

	return IsAlnum(GetAt(pos));
	}

BOOL CStringEx::IsDigit(char ch) {

	return (ch>='0' && ch<='9');
	}

BOOL CStringEx::IsDigit(int pos) {

	return IsDigit(GetAt(pos));
	}

BOOL CStringEx::IsPrint(char ch) {

	return (ch>=' ' && ch<'\x7f');			//127 escluso
	}

BOOL CStringEx::IsPrint(int pos) {

	return IsPrint(GetAt(pos));
	}

void CStringEx::Print() {

	AfxMessageBox(*this);
	}

void CStringEx::Debug() {

#ifdef _DEBUG
	Print();
#endif
	}

WORD CStringEx::GetAsciiLength() {			// utile per saltare ESC ecc in stampa citofono LCD ecc
	WORD i,j;

	for(i=0,j=0; i<GetLength(); i++)
		if(IsPrint(i))
			j++;
	return j;
	}

// _T("%h %l %u %t \"%r\" %>s %b")		v. Apache...

CStringEx CStringEx::FormatTime(int m,CTime mT) {

	if(!(*((DWORD *)&mT)))
		mT=CTime::GetCurrentTime();

	switch(m) {
		case 0:
			CStringEx::operator=(mT.Format("%d/%m/%Y %H:%M:%S"));
			break;
		case 1:
			break;
		case 2:
			CStringEx::operator=(mT.Format(_T("%a, %d %b %Y %H:%M:%S %Z")));
			break;
		}

	return *this;
	}


const char CStringEx::m_base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz0123456789+/";
const int CStringEx::BASE64_MAXLINE=76;
const char *CStringEx::EOL="\r\n";
const char CStringEx::decimalChar=',',CStringEx::thousandChar='.';		// GetLocale ??
const char CStringEx::CRchar='\r',CStringEx::LFchar='\n',CStringEx::TABchar='\t';
CStringEx CStringEx::Encode64() {
	CStringEx S2;

  //Set up the parameters prior to the main encoding loop
  int nInPos  = 0;
  int nLineLen = 0;

  // Get three characters at a time from the input buffer and encode them
  for(int i=0; i<GetLength()/3; ++i) {

    //Get the next 2 characters
    int c1 = Asc(nInPos++) & 0xFF;
    int c2 = Asc(nInPos++) & 0xFF;
    int c3 = Asc(nInPos++) & 0xFF;

    //Encode into the 4 6 bit characters
    S2 += m_base64tab[(c1 & 0xFC) >> 2];
    S2 += m_base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
    S2 += m_base64tab[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
    S2 += m_base64tab[c3 & 0x3F];
    nLineLen += 4;

    //Handle the case where we have gone over the max line boundary
    if(nLineLen >= BASE64_MAXLINE-3) {
      const char *cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      nLineLen = 0;
			}
		}

  // Encode the remaining one or two characters in the input buffer
  const char *cp;
  switch(GetLength() % 3) {
    case 0:
      cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      break;
    case 1:
    {
      int c1 = Asc(nInPos) & 0xFF;
      S2 += m_base64tab[(c1 & 0xFC) >> 2];
      S2 += m_base64tab[((c1 & 0x03) << 4)];
      S2 += '=';
      S2 += '=';
      cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      break;
    }
    case 2:
    {
      int c1 = Asc(nInPos++) & 0xFF;
      int c2 = Asc(nInPos) & 0xFF;
      S2 += m_base64tab[(c1 & 0xFC) >> 2];
      S2 += m_base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
      S2 += m_base64tab[((c2 & 0x0F) << 2)];
      S2 += '=';
      cp = EOL;
      S2 += *cp++;
      if(*cp) {
        S2 += *cp;
				}
      break;
    }
    default: 
      ASSERT(FALSE); 
      break;
	  }

  CStringEx::operator=(S2);
  return *this;
	}

int CStringEx::Decode64() {
	CStringEx sInput;
	int m_nBitsRemaining;
	ULONG m_lBitStorage;

  m_nBitsRemaining = 0;

	sInput=*this;
  Empty();  
	if(sInput.GetLength() == 0)
		return 0;

	//Build Decode Table
  int nDecode[256];
	for(int i=0; i<256; i++) 
		nDecode[i] = -2; // Illegal digit
	for(i=0; i<64; i++) {
		nDecode[m_base64tab[i]] = i;
		nDecode[m_base64tab[i] | 0x80] = i; // Ignore 8th bit
		nDecode['='] = -1; 
		nDecode['=' | 0x80] = -1; // Ignore MIME padding char
		}

	// Decode the Input
  i=0;
  TCHAR* szOutput = GetBuffer(sInput.GetLength());
	for(int p=0; p<sInput.GetLength(); p++) {
		int c = sInput[p];
		int nDigit = nDecode[c & 0x7F];
		if(nDigit < -1) {
      ReleaseBuffer();  
			return 0;
			}
		else if(nDigit >= 0) {
			// i (index into output) is incremented by write_bits()
//			WriteBits(nDigit & 0x3F, 6, szOutput, i);
			UINT nScratch;

			m_lBitStorage = (m_lBitStorage << 6) | (nDigit & 0x3F);
			m_nBitsRemaining += 6;
			while(m_nBitsRemaining > 7) {
				nScratch = m_lBitStorage >> (m_nBitsRemaining - 8);
				szOutput[i++] = (TCHAR) (nScratch & 0xFF);
				m_nBitsRemaining -= 8;
				}
			}
		}	
  szOutput[i] = _T('\0');
  ReleaseBuffer();

	return i;
	}

CString CStringEx::InsertSeparator(DWORD dwNumber) {

  Format("%u", dwNumber);
  
  for(int i=GetLength()-3; i > 0; i -= 3) {
    Insert(i, ",");
    }

  return *this;
  }

CStringEx CStringEx::FormatSize(DWORD dwFileSize) {
  static const DWORD dwKB = 1024;          // Kilobyte
  static const DWORD dwMB = 1024 * dwKB;   // Megabyte
  static const DWORD dwGB = 1024 * dwMB;   // Gigabyte

  DWORD dwNumber, dwRemainder;

  if(dwFileSize < dwKB) {
//    InsertSeparator(dwFileSize) + " B";		// non funziona (usare  *this o Format) e poi non mi piace!
    InsertSeparator(dwFileSize);
		} 
  else {
    if(dwFileSize < dwMB) {
      dwNumber = dwFileSize / dwKB;
      dwRemainder = (dwFileSize * 100 / dwKB) % 100;

      Format("%s.%02d KB", (LPCSTR)InsertSeparator(dwNumber), dwRemainder);
			}
    else {
      if(dwFileSize < dwGB) {
        dwNumber = dwFileSize / dwMB;
        dwRemainder = (dwFileSize * 100 / dwMB) % 100;
        Format("%s.%02d MB", InsertSeparator(dwNumber), dwRemainder);
				}
      else {
        if(dwFileSize >= dwGB) {
          dwNumber = dwFileSize / dwGB;
          dwRemainder = (dwFileSize * 100 / dwGB) % 100;
          Format("%s.%02d GB", InsertSeparator(dwNumber), dwRemainder);
					}
				}
			}
		}

  // Display decimal points only if needed
  // another alternative to this approach is to check before calling str.Format, and 
  // have separate cases depending on whether dwRemainder == 0 or not.
  Replace(".00", "");

	return *this;
	}




