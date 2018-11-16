#include "stdafx.h"
#include "Quote.h"
#include "CTPMFCTEST.h"

map<CString,CString> CQuote::m_mapTimeMsg;
Mutex CQuote::m_mutex;
CThostFtdcReqUserLoginField CQuote::m_req;


CQuote::CQuote()
{
}

CQuote::~CQuote()
{
}

void CQuote::OnRspError(CThostFtdcRspInfoField *pRspInfo,
						int nRequestID, bool bIsLast)
{
    IsErrorRspInfo(pRspInfo);
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CQuote::OnFrontDisconnected(int nReason)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	GetTime(m_strTime);
	m_strMsg.Format(_T("%d"),nReason);

	m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
}

///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CQuote::OnHeartBeatWarning(int nTimeLapse)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	GetTime(m_strTime);
	m_strMsg.Format(_T("%d"),nTimeLapse);

	m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
}

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CQuote::OnFrontConnected()
{
	CString m_strMsg = _T("�����¼��.....");

	Info(m_strMsg);

	ReqUserLogin();
}

void CQuote::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	req = m_req;
	int iResult = pMdApi->ReqUserLogin(&req, ++iRequestID);

	CString m_strMsg = _T("���������û���¼����:");
	Info(iResult, m_strMsg);
}

///��¼������Ӧ
void CQuote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		CString strMsg = _T("��ȡ��ǰ������:");
		CString strTradingDay = _T("");

		char *pTradingDay = const_cast<char*>(pMdApi->GetTradingDay());
		strTradingDay = ANSIToUnicode(pTradingDay);
		strMsg = strMsg + strTradingDay;

		Info(strMsg);
	}
}

void CQuote::OnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
	int iResult = pMdApi->SubscribeMarketData(ppInstrumentID, nCount);

	CString m_strTime = _T(""); 
	CString m_strMsg = _T("�������鶩������:");

	Info(iResult, m_strMsg);
}

///��������Ӧ��
void CQuote::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CString m_strMsg = _T("��������Ʒ�֣�");
	m_strMsg.Append(_T("%d::::"), pRspInfo ->ErrorID);
	CString strTmp = _T("");
	CString strInstrumentID = _T("");
	strTmp = ANSIToUnicode(pRspInfo ->ErrorMsg);
	m_strMsg.Append(strTmp);
	strInstrumentID = ANSIToUnicode(pSpecificInstrument->InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;

	Info(m_strMsg);
}

///ȡ����������Ӧ��
void CQuote::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

///�������֪ͨ
void CQuote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//CThostFtdcDepthMarketDataField *tmpData = new CThostFtdcDepthMarketDataField;
	//tmpData = pDepthMarketData;
	//strncpy(tmpData->InstrumentID , pDepthMarketData->InstrumentID, 31);
	//strncpy(tmpData->UpdateTime , pDepthMarketData->UpdateTime, 9);
	//strncpy(tmpData->TradingDay , pDepthMarketData->TradingDay, 9);

	//CString strTmp = _T("");
	//CString strTradingDay = _T("");
	//strTmp.Format(_T("%s"),ANSIToUnicode(pDepthMarketData ->UpdateTime));   // ����ʱ��
	//strTradingDay.Format(_T("%s"),ANSIToUnicode(pDepthMarketData ->TradingDay));
	//tmpData->AskPrice1 = pDepthMarketData->AskPrice1;
	//tmpData->BidPrice1 = pDepthMarketData->BidPrice1;
	//tmpData->AskVolume1 = pDepthMarketData->AskVolume1;
	//tmpData->BidVolume1 = pDepthMarketData->BidVolume1;
	//tmpData->LastPrice = pDepthMarketData->LastPrice;
	//tmpData->Volume = pDepthMarketData->Volume;

	theApp.m_pMainWnd ->PostMessage(WM_SHOW_QUOTE, (WPARAM)pDepthMarketData);

}

// 
bool CQuote::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	m_strMsg.Format(_T("%d::::"), pRspInfo ->ErrorID);
	CString strTmp = _T("");
	strTmp.Format(_T("%s"), ANSIToUnicode(pRspInfo ->ErrorMsg));
	m_strMsg.Append(strTmp);

	GetTime(m_strTime);
	//if(bResult)
	{
        Locker locker(m_mutex);

        m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
	}

	return bResult;
}

void CQuote::GetTime(CString& strTime)
{
	CTime tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S-->"); 
}

void CQuote::Info(const CString& strMsg)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 
	GetTime(m_strTime);
    m_mapTimeMsg.insert(make_pair(m_strTime, strMsg));
}

void CQuote::Info(const int iResult, CString& strMsg)
{
	Locker locker(m_mutex);
	CString m_strTime = _T(""); 

	GetTime(m_strTime);
	if(iResult == 0)
	{
		strMsg.Append(_T("�ɹ�"));
	}
	else
	{
		strMsg.Append(_T("ʧ��"));
	}
	m_mapTimeMsg.insert(make_pair(m_strTime, strMsg));
}

