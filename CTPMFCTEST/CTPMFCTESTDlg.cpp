
// CTPMFCTESTDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CTPMFCTEST.h"
#include "CTPMFCTESTDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// UserApi����
CThostFtdcTraderApi *pUserApi = NULL;
// MdApi����
CThostFtdcMdApi *pMdApi = NULL;

HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCTPMFCTESTDlg �Ի���



CCTPMFCTESTDlg::CCTPMFCTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCTPMFCTESTDlg::IDD, pParent)
	, m_IPADDRESS(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTPMFCTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_IPADDRESS);
	DDX_Control(pDX, IDC_EDIT_PORT, m_PORT);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPADDRESSCTRL);
	DDX_Control(pDX, IDC_EDIT_MSG, m_ControlMsg);
	DDX_Control(pDX, IDC_EDIT_SECID, m_SECID);
	DDX_Control(pDX, IDC_EDIT_NEWPRICE, m_NewPrice);
	DDX_Control(pDX, IDC_EDIT_BUYPRICE, m_BuyPrice);
	DDX_Control(pDX, IDC_EDIT_SELLPRICE, m_SellPrice);
	DDX_Control(pDX, IDC_EDIT_BROKERID, m_BorkerID);
	DDX_Control(pDX, IDC_EDIT_INVESTORID, m_InvestorID);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_IPADDRESS2, m_IPADDRESS2);
	DDX_Control(pDX, IDC_EDIT_PORT2, m_Port2);
	DDX_Control(pDX, IDC_EDIT_NEWVOLUMN, m_NewVolumn);
	DDX_Control(pDX, IDC_EDIT_BUYVOLUMN, m_BuyVolumn);
	DDX_Control(pDX, IDC_EDIT_SELLVOLUMN, m_SellVolumn);
	DDX_Control(pDX, IDC_EDIT_LASTTIME, m_UpdateLastTime);
	DDX_Control(pDX, IDC_LIST_HOLDORDER, m_HoldOrder);
}

BEGIN_MESSAGE_MAP(CCTPMFCTESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SHOW_QUOTE, OnShowQuoteMessage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CCTPMFCTESTDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_SUBSECID, &CCTPMFCTESTDlg::OnBnClickedButtonSubsecid)
	ON_BN_CLICKED(IDC_BUTTON_BUYORDER, &CCTPMFCTESTDlg::OnBnClickedButtonBuyorder)
	ON_BN_CLICKED(IDC_BUTTON_SELLORDER, &CCTPMFCTESTDlg::OnBnClickedButtonSellorder)
END_MESSAGE_MAP()


// CCTPMFCTESTDlg ��Ϣ�������

BOOL CCTPMFCTESTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	SetTimer(0, 1000, NULL);


	// TEST
	m_BorkerID.SetWindowText(_T("9999"));
	m_InvestorID.SetWindowText(_T("053248"));
	m_Password.SetWindowText(_T("910406"));
	m_PORT.SetWindowText(_T("10003"));
	m_Port2.SetWindowText(_T("10013"));
	m_IPADDRESSCTRL.SetWindowText(_T("180.168.146.187"));
	m_IPADDRESS2.SetWindowText(_T("180.168.146.187"));
	//m_BorkerID.SetWindowText(_T("2030"));
	//m_InvestorID.SetWindowText(_T("88888888"));
	//m_Password.SetWindowText(_T("123456"));
	//m_PORT.SetWindowText(_T("41213"));
	//m_Port2.SetWindowText(_T("41205"));
	//m_IPADDRESSCTRL.SetWindowText(_T("180.168.212.53"));
	//m_IPADDRESS2.SetWindowText(_T("180.168.212.51"));

	m_redcolor=RGB(255,0,0);                      // ��ɫ  
	m_bluecolor=RGB(0,0,255);                     // ��ɫ  
	m_textcolor=RGB(0,0,0);                       // �ı���ɫ����Ϊ��ɫ  
	m_newPricecolor=RGB(0,0,0);                       // �ı���ɫ����Ϊ��ɫ 
	m_buyPricecolor=RGB(0,0,0);                       // �ı���ɫ����Ϊ��ɫ 
	m_sellPricecolor=RGB(0,0,0);                       // �ı���ɫ����Ϊ��ɫ 
	m_redbrush.CreateSolidBrush(m_redcolor);      // ��ɫ����ɫ  
	m_bluebrush.CreateSolidBrush(m_bluecolor);    // ��ɫ����ɫ
	m_blackbrush.CreateSolidBrush(m_textcolor);   // 

	UpdateData(TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCTPMFCTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCTPMFCTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCTPMFCTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCTPMFCTESTDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	memset(&CQuote::m_req, 0, sizeof(CQuote::m_req));
	m_BorkerID.GetWindowText(m_strBorkerID);
	m_InvestorID.GetWindowText(m_strInvestorID);
	m_Password.GetWindowText(m_strPassword);

	//memcpy(CQuote::m_req.BrokerID,m_strBorkerID,m_strBorkerID.GetLength());
	sprintf(CQuote::m_req.BrokerID,"%S",m_strBorkerID);
	sprintf(CQuote::m_req.UserID, "%S",m_strInvestorID);
	sprintf(CQuote::m_req.Password, "%S",m_strPassword);

	OnReady();
}

void CCTPMFCTESTDlg::OnBnClickedButtonSubsecid()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(NULL == pMdApi)
	{
		CString strMsg = _T("�û���δ��¼");
		m_Quote.Info(strMsg);
	}
	m_SECID.GetWindowText(m_strSecID);
	char *pInstrumentID = "";

	LPTSTR pf=(LPTSTR)(LPCTSTR)m_strSecID;
	pInstrumentID =(char *)malloc( 2*wcslen(pf)+1 );
	wcstombs(pInstrumentID , pf, 2*wcslen(pf)+1 );

	char *ppInstrumentID[1];
    ppInstrumentID[0] = pInstrumentID;
	int iCount = 1;
	m_Quote.OnSubscribeMarketData(ppInstrumentID, iCount);

	free(pInstrumentID);
}

void CCTPMFCTESTDlg::OnBnClickedButtonBuyorder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(NULL == pUserApi)
	{
		CString strMsg = _T("�û���δ��¼");
		m_Quote.Info(strMsg);
	}
}

void CCTPMFCTESTDlg::OnBnClickedButtonSellorder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(NULL == pUserApi)
	{
		CString strMsg = _T("�û���δ��¼");
		m_Quote.Info(strMsg);
	}

	
}


void CCTPMFCTESTDlg::Print(CString strMsg)
{
    m_ControlMsg.SetWindowText(strMsg);
}

void CCTPMFCTESTDlg::OnTimer(UINT_PTR nIDEvent)
{
	Locker locker(CQuote::m_mutex);
	map<CString, CString>::iterator pCurTimeMsg = CQuote::m_mapTimeMsg.begin();
    for( ; pCurTimeMsg != CQuote::m_mapTimeMsg.end(); )
	{
		m_szMsg = pCurTimeMsg ->first + ":" + pCurTimeMsg ->second;
		m_ControlMsg.SetSel(-1,-1);
		m_ControlMsg.ReplaceSel(m_szMsg + "\r\n");
		CQuote::m_mapTimeMsg.erase(pCurTimeMsg++);
	}

	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}

void CCTPMFCTESTDlg::OnReady()
{
	// ��ʼ��UserApi
	char *pszFrontAddress1 = "";
	char *pszFrontAddress2 = "";
	CString strTCP = _T("tcp://");
	CString strIP = _T("");
	CString strPort = _T("");

	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll");			// ����UserApi//"./thosttraderapi.dll"
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			// ע��˽����

	m_IPADDRESSCTRL.GetWindowText(strIP);
	m_PORT.GetWindowText(strPort);

	strTCP = strTCP + strIP + ":" + strPort;
	//pszFrontAddress = (LPSTR)(LPCTSTR)strTCP;
	//strncpy(pszFrontAddress, (LPSTR)(LPCTSTR)strTCP, strTCP.GetLength());

	//char *p1 = "tcp://180.168.146.187:10003";
	LPTSTR pf1=(LPTSTR)(LPCTSTR)strTCP;
	pszFrontAddress1 =(char *)malloc( 2*wcslen(pf1)+1 );
	wcstombs(pszFrontAddress1 , pf1, 2*wcslen(pf1)+1 );
	//p1 = strTCP.GetBuffer(0);
	pUserApi->RegisterFront(pszFrontAddress1);							// connect

	pUserApi->Init();


	// ��ʼ��MdApi
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");					// ����MdApi//"./thostmduserapi.dll"
	CThostFtdcMdSpi* pMdSpi = new CQuote();
	pMdApi->RegisterSpi(pMdSpi);									// ע���¼���

	m_IPADDRESS2.GetWindowText(strIP);
	m_Port2.GetWindowText(strPort);

	strTCP = _T("tcp://");
	strTCP = strTCP + strIP + ":" + strPort;
	//pszFrontAddress = (LPSTR)(LPCTSTR)strTCP;
	//char *p2 = "tcp://180.168.146.187:10003";
	LPTSTR pf2=(LPTSTR)(LPCTSTR)strTCP;
	pszFrontAddress2 =(char *)malloc( 2*wcslen(pf2)+1 );
	wcstombs(pszFrontAddress2 , pf2, 2*wcslen(pf2)+1 );
	pMdApi->RegisterFront(pszFrontAddress2);							// connect

	pMdApi->Init();


	//WaitForSingleObject(g_hEvent, INFINITE);
	//pMdApi->Join();
	//pMdApi->Release();

	CString m_strTime = _T(""); 
	CString m_strMsg("��ʼ�����.....");
	m_Quote.GetTime(m_strTime);

	m_Quote.m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));

	free(pszFrontAddress1);
	free(pszFrontAddress2);

}

LRESULT CCTPMFCTESTDlg::OnShowQuoteMessage(WPARAM wParam, LPARAM lParam)
{	
	CThostFtdcDepthMarketDataField *lQuoteData = (CThostFtdcDepthMarketDataField*)wParam;
	CString strTmp = _T("");
	CString strTradingDay = _T("");
    
	strTmp.Format(_T("%s"),ANSIToUnicode(lQuoteData ->UpdateTime));   // ����ʱ��

	strTradingDay.Format(_T("%s"),ANSIToUnicode(lQuoteData ->TradingDay));
    strTmp = strTradingDay + " " + strTmp;
	m_UpdateLastTime.SetWindowText(strTmp);

	strTmp.Format(_T("%.2f"),lQuoteData ->AskPrice1);    // ��һ��
	m_SellPrice.SetWindowText(strTmp);
	m_sellPricecolor = SetTextColor(lQuoteData ->AskPrice1, lQuoteData ->PreSettlementPrice);

	strTmp.Format(_T("%.2f"),lQuoteData ->BidPrice1);    // ��һ��
	m_BuyPrice.SetWindowText(strTmp);
	m_buyPricecolor = SetTextColor(lQuoteData ->BidPrice1, lQuoteData ->PreSettlementPrice);

	strTmp.Format(_T("%.2f"),lQuoteData ->LastPrice);    // ���¼�
	m_NewPrice.SetWindowText(strTmp);
	m_newPricecolor = SetTextColor(lQuoteData ->LastPrice, lQuoteData ->PreSettlementPrice);

	strTmp.Format(_T("%d"),lQuoteData ->AskVolume1);    // ��һ����
	m_SellVolumn.SetWindowText(strTmp);

	strTmp.Format(_T("%d"),lQuoteData ->BidVolume1);    // ��һ����
	m_BuyVolumn.SetWindowText(strTmp);

	strTmp.Format(_T("%d"),lQuoteData ->Volume);    // �ɽ�����
	m_NewVolumn.SetWindowText(strTmp);

	//if(lQuoteData != NULL)
	//{
	//	delete lQuoteData;
	//	lQuoteData = NULL;
	//}
	UpdateData(TRUE);

	return 0;
}

COLORREF CCTPMFCTESTDlg::SetTextColor(TThostFtdcPriceType NewPrice, TThostFtdcPriceType PriceSettlePrice)
{
	if(NewPrice > PriceSettlePrice)
	{
        return m_redcolor;
	}
	else if(NewPrice == PriceSettlePrice)
	{
		return m_textcolor;
	}
	else
	{
		return m_bluecolor;
	}
}

HBRUSH CCTPMFCTESTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)  
{  
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);  
 
	switch (pWnd->GetDlgCtrlID())//��ĳһ���ض��ؼ������ж�  
	{      
	case IDC_EDIT_NEWPRICE:         // ��һ���ı���  
		//pDC->SetBkColor(m_bluecolor);    // change the background  
		pDC->SetTextColor(m_newPricecolor); // change the text color  
		//hbr = (HBRUSH) m_bluebrush;    // apply the blue brush   
		break;    

	case IDC_EDIT_BUYPRICE:         // �ڶ����ı���  
		//pDC->SetBkMode(TRANSPARENT);   // make background  
		pDC->SetTextColor(m_buyPricecolor); // change the text color  
		//hbr = (HBRUSH) m_redbrush;     // apply the red brush  
		break; 

	case IDC_EDIT_SELLPRICE:
		//pDC->SetBkMode(TRANSPARENT);   // make background  
		pDC->SetTextColor(m_sellPricecolor); // change the text color  
		//hbr = (HBRUSH) m_redbrush;     // apply the red brush  
		break; 

	case IDC_EDIT_SELLVOLUMN:
		pDC->SetTextColor(m_redcolor); // change the text color  
		break; 

	case IDC_EDIT_BUYVOLUMN:
		pDC->SetTextColor(m_redcolor); // change the text color   
		break; 

	case IDC_EDIT_NEWVOLUMN:
		pDC->SetTextColor(m_redcolor); // change the text color    
		break; 

	default:  
		hbr=CDialog::OnCtlColor(pDC,pWnd,nCtlColor);  
		break;  
	}  
   
	return hbr;  
}  


// ��Լ  ����   �ֲܳ�  ���   ��ƽ����  �ֲ־���  �ֲ�ӯ��   ռ�ñ�֤��   �ҵ�״̬