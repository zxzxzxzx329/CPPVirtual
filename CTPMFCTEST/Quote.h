#pragma once
#include "stdafx.h"
#include "ThostFtdcMdApi.h"
#include "Common.h"
#include "Mutex.h"
#include <map>
using namespace std;

// MdApi����
extern CThostFtdcMdApi *pMdApi;

class CQuote : public CThostFtdcMdSpi
{
public:
	CQuote();
	~CQuote();
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	void GetTime(CString& strTime);
	void OnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	void Info(const CString& strMsg);
	void Info(const int iResult, CString& strMsg);
	void ReqUserLogin();

private:
	//void ReqUserLogin();
	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

public:
	static map<CString,CString> m_mapTimeMsg;
	static Mutex m_mutex;
	static CThostFtdcReqUserLoginField m_req;
	//static CString m_sz
};