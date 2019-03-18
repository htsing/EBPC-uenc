// TestUpnpCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestUpnpC.h"
#include "TestUpnpCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"WS2_32.lib")
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestUpnpCDlg dialog
CList<SOCKET_INFO *,SOCKET_INFO *> CTestUpnpCDlg::m_SockList;

CTestUpnpCDlg::CTestUpnpCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestUpnpCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestUpnpCDlg)
	m_nPort = 9005;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThread = NULL;
	m_hUpnpThread = NULL;
	m_iTcpPort = 0;
}

void CTestUpnpCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestUpnpCDlg)
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestUpnpCDlg, CDialog)
	//{{AFX_MSG_MAP(CTestUpnpCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_BN_CLICKED(IDC_BTN_MAP, OnBtnMap)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LISTEN, OnBtnListen)
	ON_BN_CLICKED(IDC_BTN_STOPLISTEN, OnBtnStoplisten)
	ON_BN_CLICKED(IDC_BTN_UNMAP, OnBtnUnmap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestUpnpCDlg message handlers
CString ResultCode2String(HRESULT hr)
{
	CString		str;
	
	if (FAILED(hr))
		str = _T("失败：");
	else
		str.Empty();
	
	switch(hr)
	{
	case S_OK:
		str = str + _T("成功");
		break;
	case E_UNAT_BAD_ARGUMENTS:
		str = str + _T("参数错误");
		break;
	case E_UNAT_NOT_IN_LAN:
		str = str + _T("已是公网IP");
		break;
	case E_UNAT_CANNOT_FIND_ROUTER:
		str = str + _T("找不到路由器");
		break;
	case E_UNAT_TIMEOUT:
		str = str + _T("超时无回应");
		break;
	case E_UNAT_ENTRY_MAYBE_FULL:
		str = str + _T("端口映射的表项可能已满");
		break;
	case E_UNAT_REACH_RAND_PORT_RETRY_TIMES:
		str = str + _T("达到随机端口的重试次数");
		break;
	case E_UNAT_CREATE_SOCKET_FAILED:
		str = str + _T("创建Socket失败");
		break;
	case E_UNAT_SOCKET_BIND_FAILED:
		str = str + _T("Socket绑定失败");
		break;
	case E_UNAT_ACTION_HTTP_ERRORCODE:
		str = str + _T("操作调用失败");
		break;
	case E_UNAT_UNKNOWN_ERROR:
	default:
		str = str + _T("未知错误");
		break;
	}
	
	return str;
}

CString	Result2String(HRESULT hr, DWORD dwActionErrorCode)
{
	CString	strOut;
	strOut = ResultCode2String(hr);
	
	CString strErrorCode;
	if (E_UNAT_ACTION_HTTP_ERRORCODE == hr)
	{
		switch(dwActionErrorCode)
		{
		case 718:
			strErrorCode = _T("其他机器已使用该外部端口映射。");
			break;
		default:
			strErrorCode.Empty();
			break;
		}
		
		CString	str;
		str.Format(_T("(#%d : %s)"), dwActionErrorCode, strErrorCode);
		strOut += str;
	}
	
	return strOut;
}


BOOL CTestUpnpCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	// TODO: Add extra initialization here
	m_hExitEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestUpnpCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestUpnpCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestUpnpCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestUpnpCDlg::AddLogLine(CString str)
{
	((CListBox *)GetDlgItem(IDC_LIST_INFO))->AddString(str);
}

void CTestUpnpCDlg::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	((CListBox *)GetDlgItem(IDC_LIST_INFO))->ResetContent();
}

void CTestUpnpCDlg::StopListen()
{
	if(m_hThread)
	{
		m_bThreadEnd = TRUE;
		SetEvent(m_hExitEvent);
		WaitForSingleObject(m_hThread,INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
		AddLogLine("已关闭SOCKET");
	}
	for(POSITION pos = m_SockList.GetHeadPosition() ; pos != NULL ;)
	{
		SOCKET_INFO *pInfo = m_SockList.GetNext(pos);
		pInfo->bThreadEnd = TRUE;
		HANDLE hThread = pInfo->hThread;
		WSASetEvent(pInfo->hEvent);
		WaitForSingleObject(hThread,INFINITE);
		CloseHandle(hThread);
	}
}

SOCKET CTestUpnpCDlg::CreateSocket()
{
	SOCKET hSock = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == hSock)
	{
		return INVALID_SOCKET;
	}
	int opt = 1;
	if(SOCKET_ERROR == setsockopt(hSock,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(int)))
	{
		CString strError;
		strError.Format("设置失败:%d",WSAGetLastError());
		AddLogLine(strError);
		closesocket(hSock);
		return INVALID_SOCKET;
	}
	return hSock;
}

DWORD CALLBACK SocketProc(LPVOID pPara)
{
	SOCKET_INFO *pInfo = (SOCKET_INFO *)pPara;
	WSANETWORKEVENTS NetEvents;
//	NET_HEAD	NetHead;
	char		Buf[1024];
	while(!pInfo->bThreadEnd)
	{
		WSAWaitForMultipleEvents(1,&pInfo->hEvent,FALSE,WSA_INFINITE,FALSE);
		WSAEnumNetworkEvents(pInfo->hSock,pInfo->hEvent,&NetEvents);
		if((NetEvents.lNetworkEvents & FD_READ) && (0 == NetEvents.iErrorCode[FD_READ_BIT]))
		{
			if(-1 == recv(pInfo->hSock,Buf,1024,0))
				break;
		}
		else if(NetEvents.lNetworkEvents & FD_CLOSE)
			break;
	}
	POSITION pos = CTestUpnpCDlg::m_SockList.Find(pInfo);
	if(pos)
		CTestUpnpCDlg::m_SockList.RemoveAt(pos);
	if(pInfo->pRegInfo)
		delete pInfo->pRegInfo;
	closesocket(pInfo->hSock);
	WSACloseEvent(pInfo->hEvent);
	delete pInfo;
	return 0;
}

DWORD CALLBACK ConnectProc(LPVOID Para)
{
	CONNECT_INFO *pConInfo = (CONNECT_INFO *)Para;
	SOCKET hSock = socket(AF_INET,SOCK_STREAM,0);
	CString str;
	str.Format("开始连接:%s(%d)",inet_ntoa(pConInfo->Addr.sin_addr),ntohs(pConInfo->Addr.sin_port));
	pConInfo->pDlg->AddLogLine(str);
	if(SOCKET_ERROR == connect(hSock,(SOCKADDR *)&pConInfo->Addr,sizeof(SOCKADDR_IN)))
		str.Format("连接:%s(%d)失败,错误码:%d",inet_ntoa(pConInfo->Addr.sin_addr),ntohs(pConInfo->Addr.sin_port),WSAGetLastError());
	else
		str.Format("连接:%s(%d)成功",inet_ntoa(pConInfo->Addr.sin_addr),ntohs(pConInfo->Addr.sin_port));
	pConInfo->pDlg->AddLogLine(str);
	closesocket(hSock);
	delete pConInfo;
	return 0;
}

DWORD CTestUpnpCDlg::ListenProc(LPVOID pPara)
{
	CTestUpnpCDlg *pDlg = (CTestUpnpCDlg *)pPara;
	pDlg->ProcessListen();
	return 0;
}

void CTestUpnpCDlg::ProcessListen()
{
	WSANETWORKEVENTS NetworkEvents;
	SOCKADDR_IN LocalAddr;
	ZeroMemory(&LocalAddr,sizeof(SOCKADDR_IN));
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = 0;
	LocalAddr.sin_port = htons(m_nPort);
	SOCKET hListenSock = CreateSocket();
	if(INVALID_SOCKET == hListenSock)
	{
		AddLogLine("创建SOCKET失败!");
		return ;
	}
	if(SOCKET_ERROR == bind(hListenSock,(SOCKADDR *)&LocalAddr,sizeof(SOCKADDR_IN)))
	{
		AddLogLine("绑定端口失败");
		closesocket(hListenSock);
		return ;
	}
	if(SOCKET_ERROR == listen(hListenSock,200))
	{
		AddLogLine("侦听端口失败");
		closesocket(hListenSock);
		return ;
	}
	AddLogLine("创建SOCKET成功,已在指定端口侦听...");

	WSAEVENT cEvents[2]={WSACreateEvent(),m_hExitEvent};
	WSAEventSelect(hListenSock,cEvents[0],FD_ACCEPT);
	while(!m_bThreadEnd)
	{
		DWORD dwWait = WSAWaitForMultipleEvents(2,cEvents,FALSE,WSA_INFINITE,FALSE);
		if(WSA_WAIT_EVENT_0 + 1 == dwWait)	//要退出了
		{
			break;
		}
		if(WSA_WAIT_EVENT_0 == dwWait)
		{
			WSAEnumNetworkEvents(hListenSock,cEvents[0],&NetworkEvents);
			if((NetworkEvents.lNetworkEvents & FD_ACCEPT) && 0 == NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
			{
				SOCKADDR_IN SockAddr;
				int nLen = sizeof(SOCKADDR_IN);
				SOCKET hSock = accept(hListenSock,(SOCKADDR *)&SockAddr,&nLen);
				if(INVALID_SOCKET != hSock)
				{
					SOCKET_INFO *pInfo = new SOCKET_INFO;
					ZeroMemory(pInfo,sizeof(SOCKET_INFO));
					m_SockList.AddTail(pInfo);
					//				pInfo->dwClient_ID = g_nID ++;
					pInfo->hSock = hSock;
					pInfo->hEvent = WSACreateEvent();
					pInfo->SockAddr = SockAddr;
					pInfo->bThreadEnd = FALSE;
					WSAEventSelect(pInfo->hSock,pInfo->hEvent,FD_READ | FD_CLOSE);
					DWORD dwTempID;
					pInfo->hThread = CreateThread(NULL,0,SocketProc,pInfo,0,&dwTempID);
					CONNECT_INFO *pConnectInfo = new CONNECT_INFO;
					memcpy(&pConnectInfo->Addr,&SockAddr,sizeof(SOCKADDR_IN));
					pConnectInfo->pDlg = this;
					HANDLE hThread = CreateThread(NULL,0,ConnectProc,pConnectInfo,0,&dwTempID);
					CloseHandle(hThread);
					CString str;
					str.Format(_T("有客户端连接上来:%s,%d"),inet_ntoa(SockAddr.sin_addr),ntohs(SockAddr.sin_port));
					AddLogLine(str);
				}
			}
		}
	}
}

DWORD CTestUpnpCDlg::UpnpProc(LPVOID pPara)
{
	CTestUpnpCDlg *pDlg = (CTestUpnpCDlg *)pPara;
	pDlg->ProcessUpnp();
	return 0;
}

DWORD GetLocalIp()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0),&wsaData);
	CString sHostName;
	DWORD	nNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(sHostName.GetBuffer(MAX_COMPUTERNAME_LENGTH + 1),&nNameLen);
	sHostName.ReleaseBuffer();
	HOSTENT *host = gethostbyname(sHostName);
	int i = 0;
	int nLocalIp = 0;
	if(NULL != host)
	{
		if(host->h_addr_list[0])
		{
			memcpy(&nLocalIp,host->h_addr_list[0],4);
			return nLocalIp;
			i++;	
		}
	}
	WSACleanup();
	return 0;
}

void CTestUpnpCDlg::ProcessUpnp()
{
	bool bHasCleanedFillupBug = false;
	UPNPNAT_MAPPING mapping;
	ZeroMemory(&mapping,sizeof(UPNPNAT_MAPPING));
	strcpy(mapping.m_strProtocol,"TCP");
	mapping.m_wExternalPort = m_nPort;
	mapping.m_wInternalPort = mapping.m_wExternalPort;
	strcpy(mapping.m_strDescription,"HDVS_DVSNet_Client_Upnp");	
	ZeroMemory(&m_NatMappingKey,sizeof(m_NatMappingKey));
	int nRet = AddNatPortMapping(&mapping,true,true,GetLocalIp(),&m_NatMappingKey);
	CString		strErrorMessage;
	CString		strLogText;
	DWORD dwActionErrorCode = 0;
	if(nRet == E_UNAT_ACTION_HTTP_ERRORCODE)
		GetLastActionErrorCode();
	strErrorMessage = Result2String(nRet, dwActionErrorCode);
	strLogText.Format("UPnp: TCP 端口映射 %d-->%d (%s)",
		mapping.m_wInternalPort,
		mapping.m_wExternalPort,
		strErrorMessage);
	
	AddLogLine(strLogText);
				
		
	if(nRet >= 0)
	{
					//				thePrefs.SetUPnPTCPExternal(pResult->wExternalPort);
		m_iTcpPort = mapping.m_wExternalPort;	
					//				CloseConnection();
					//				StartConnection();
	}
}

void CTestUpnpCDlg::StartListen()
{
	StopListen();
	DWORD dwTempID;
	m_bThreadEnd = FALSE;
	m_hThread = CreateThread(NULL,0,ListenProc,this,0,&dwTempID);
}

void CTestUpnpCDlg::OnBtnMap() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwTempID;
	m_hUpnpThread = CreateThread(NULL,0,UpnpProc,this,0,&dwTempID);
}

void CTestUpnpCDlg::OnDestroy() 
{
	StopListen();
	CloseHandle(m_hExitEvent);
	RemoveNATPortMapping(&m_NatMappingKey);
	WSACleanup();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CTestUpnpCDlg::OnBtnListen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	StartListen();
}

void CTestUpnpCDlg::OnBtnStoplisten() 
{
	// TODO: Add your control notification handler code here
	StopListen();
}

void CTestUpnpCDlg::OnBtnUnmap() 
{
	// TODO: Add your control notification handler code here
	if(RemoveNATPortMapping(&m_NatMappingKey) >= 0)
		AddLogLine("取消映射成功!");
	else
		AddLogLine("取消映射失败!");
}
