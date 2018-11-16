/////////////////////////////////////////////////////////////////////////

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "TraderSpi.h"

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcTraderApi* pUserApi;

void CTraderSpi::OnFrontConnected()
{
	CString strMsg = _T("���׵�¼��Trade Connected");
    m_QuoteTrade.Info(strMsg);

	// �û���¼����
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	req = m_QuoteTrade.m_req;
	memcpy(req.Password, "910406", 41);
	memcpy(req.TradingDay, "20160728", 9);
	//req.Password = "910406";
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);

	CString m_strMsg = _T("���ͽ����û���¼����:");;
	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(IsErrorRspInfo(pRspInfo))
	{
		
	}
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);


		// test
		// �˵ǳɹ�,��������¼������
		CThostFtdcInputOrderField ord;
		memset(&ord, 0, sizeof(ord));
		//���͹�˾����
		strcpy(ord.BrokerID, "2030");
		//Ͷ���ߴ���
		strcpy(ord.InvestorID, "88888888");
		// ��Լ����
		strcpy(ord.InstrumentID, "IC1608");
		///��������
		strcpy(ord.OrderRef, "000000000001");
		// �û�����
		strcpy(ord.UserID, "88888888");
		// �����۸�����
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		// ��������
		ord.Direction = THOST_FTDC_D_Buy;
		// ��Ͽ�ƽ��־
		strcpy(ord.CombOffsetFlag, "0");
		// ���Ͷ���ױ���־
		strcpy(ord.CombHedgeFlag, "1");
		// �۸�
		ord.LimitPrice = 6160;
		// ����
		ord.VolumeTotalOriginal = 10;
		// ��Ч������
		ord.TimeCondition = THOST_FTDC_TC_GFD;
		// GTD����
		strcpy(ord.GTDDate, "");
		// �ɽ�������
		ord.VolumeCondition = THOST_FTDC_VC_AV;
		// ��С�ɽ���
		ord.MinVolume = 0;
		// ��������
		ord.ContingentCondition = THOST_FTDC_CC_Immediately;
		// ֹ���
		ord.StopPrice = 0;
		// ǿƽԭ��
		ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		// �Զ������־
		ord.IsAutoSuspend = 0;
		int iResult = pUserApi->ReqOrderInsert(&ord, 1);

		///Ͷ���߽�����ȷ��
		//ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));

	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	//�����ѯ������Ϣȷ��
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);

	CString m_strMsg = _T("Ͷ���߽�����ȷ��:");

	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///�����ѯ��Լ
		TThostFtdcInstrumentIDType InstrumentID;
		memcpy(InstrumentID, "IC1608",31);
		ReqQryInstrument(InstrumentID);
	}
}


void CTraderSpi::ReqQryInstrument(const TThostFtdcInstrumentIDType InstrumentID)
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));

	//req.InstrumentID = InstrumentID;
	memcpy(req.InstrumentID, InstrumentID, 31);
	int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);

	CString m_strMsg = _T("�����ѯ��Լ:");
	CString strInstrumentID = ANSIToUnicode(req.InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;

	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///�����ѯ��Լ

		ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));

	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);

	int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);

	CString m_strMsg = _T("�����ѯ�ʽ��˻�:");

	m_QuoteTrade.Info(iResult, m_strMsg);
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//cerr << "--->>> ��ȡ�ʽ�: " << pTradingAccount->WithdrawQuota  << endl;
		//��̬Ȩ��=���ս���-������+�����
		double preBalance = pTradingAccount->PreBalance - pTradingAccount->Withdraw + pTradingAccount->Deposit;

		//��̬Ȩ��=��̬Ȩ��+ ƽ��ӯ��+ �ֲ�ӯ��- ������
		double current = preBalance+ pTradingAccount->CloseProfit + pTradingAccount->PositionProfit - pTradingAccount->Commission;
	

		////��齻����־�ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
		//if (LogFilePaths[0] == '\0')
		//{
		//	strcpy(LogFilePaths, "./AutoTrade/");
		//	strcat_s(LogFilePaths, pTradingAccount->TradingDay);
		//	strcat_s(LogFilePaths, ".txt");

		//	//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
		//	ifstream inf;
		//	ofstream ouf;
		//	inf.open(LogFilePaths, ios::out);
		//}

		///�����ѯͶ���ֲ߳�

		ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));

	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	//req.InstrumentID = m_QuoteTrade.m_req.
	int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);

	CString m_strMsg = _T("�����ѯͶ���ֲ߳�:");

	m_QuoteTrade.Info(iResult, m_strMsg);
}

///�����ѯͶ���ֲ߳���Ӧ
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		CString strMsg = _T("��Լ��֤���Լ������Ѳ�ѯ����");
		m_QuoteTrade.Info(strMsg);
		m_szInstrumentID.Format(_T("%s"),ANSIToUnicode(pInvestorPosition ->InstrumentID));	
		ReqQryInstrumentMarginRate();
		ReqQryInstrumentCommissionRate();
		Sleep(1000);
	}
}

///�����ѯ��Լ��֤����
void CTraderSpi::ReqQryInstrumentMarginRate()
{
	CThostFtdcQryInstrumentMarginRateField req;
	memset(&req, 0, sizeof(req));

	///���͹�˾����
	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	///Ͷ���ߴ���
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	///��Լ����
	memcpy(req.InstrumentID,m_szInstrumentID,m_szInstrumentID.GetLength()); 
	//////Ͷ���ױ���־
	req.HedgeFlag = THOST_FTDC_HF_Speculation;	//Ͷ��

	int iResult = pUserApi->ReqQryInstrumentMarginRate(&req, ++iRequestID);

	CString m_strMsg = _T("��ѯ��Լ��֤����: ");
	m_QuoteTrade.Info(iResult, m_strMsg);
}

///�����ѯ��Լ��������
void CTraderSpi::ReqQryInstrumentCommissionRate()
{
	CThostFtdcQryInstrumentCommissionRateField req;
	memset(&req, 0, sizeof(req));

	///���͹�˾����
	//req.BrokerID = m_QuoteTrade.m_req.BrokerID;
	memcpy(req.BrokerID, m_QuoteTrade.m_req.BrokerID,11);
	///Ͷ���ߴ���
	//req.InvestorID = m_QuoteTrade.m_req.UserID;
	memcpy(req.InvestorID,m_QuoteTrade.m_req.UserID,13);
	///��Լ����
	memcpy(req.InstrumentID,m_szInstrumentID,m_szInstrumentID.GetLength());

	int iResult = pUserApi->ReqQryInstrumentCommissionRate(&req, ++iRequestID);

	CString m_strMsg = _T("��ѯ��Լ��������: ");
	m_QuoteTrade.Info(iResult, m_strMsg);
}

//����-�޼�,��ʱδʹ��
void CTraderSpi::ReqOrderInsert()
{

}

// �������粻��ͨ��THOSTУ�飬THOST�ܾ�����������OnRspOrderInsert����������룩�����У��ͨ����THOST���ձ�����ת����������
// �������յ�������ͨ��У�顣�û����յ�OnRtnOrder��OnRtnTrade��
// �����������Ϊ���������û��ͻ��յ�OnErrRtnOrder
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{

	}
}

//����-�޸ģ�����
void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//�Ƿ����˱���
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, pOrder->BrokerID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, pOrder->InvestorID);
	///������������
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	strcpy(req.OrderRef, pOrder->OrderRef);
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	req.FrontID = FRONT_ID;
	///�Ự���
	req.SessionID = SESSION_ID;
	///����������
	//	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///�۸�
	//	TThostFtdcPriceType	LimitPrice;
	///�����仯
	//	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///��Լ����
	strcpy(req.InstrumentID, pOrder->InstrumentID);
	//cerr << "--->>> ������������: "<< endl;
	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);

	CString m_strMsg = _T("������������: ");
	if(pOrder->Direction == '0')
	{
		m_strMsg.Append(_T("������ -->"));
	}
	else
	{
		m_strMsg.Append(_T("������ -->"));
	}

	if(pOrder->CombOffsetFlag[0] == '0')
	{
		m_strMsg.Append(_T("��ƽ���򣺿��� -->"));
	}
	else
	{
		m_strMsg.Append(_T("��ƽ����ƽ�� -->"));
	}

	m_QuoteTrade.Info(iResult, m_strMsg);
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
    CString m_strMsg = _T("���׶Ͽ�: ");
	m_QuoteTrade.Info(nReason, m_strMsg);
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	CString m_strMsg = _T("�����ж�ʱ��: ");
	m_QuoteTrade.Info(nTimeLapse, m_strMsg);
}

///����֪ͨ
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	CString m_strMsg = _T("����״̬��");
	if('0' == pOrder->OrderSubmitStatus)
	{
		m_strMsg.Append(_T("�Ѿ��ύ::"));
	}
	else if('3' == pOrder->OrderSubmitStatus)
	{
		m_strMsg.Append(_T("�Ѿ�����::"));
	}

	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
		{
			m_strMsg.Append(_T("����ִ�У�"));
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
		{
		    m_strMsg.Append(_T("�����ɹ���"));
		}
	}

	m_QuoteTrade.Info(m_strMsg);
}

void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	CString m_strMsg = _T("--->>> �ɽ�֪ͨ:::");

	CString strInstrumentID = _T("");
	strInstrumentID = ANSIToUnicode(pTrade->InstrumentID);

	m_strMsg = m_strMsg + strInstrumentID;

	if('0' == pTrade->Direction)
	{
		m_strMsg.Append(_T("��:::"));
	}
	else
	{
		m_strMsg.Append(_T("��:::"));
	}

	if('0' == pTrade->OffsetFlag)
	{
		m_strMsg.Append(_T("����:::"));
	}
	else
	{
		m_strMsg.Append(_T("ƽ��:::"));
	}

	m_strMsg.Append(_T("������%d"),pTrade->Volume);
	m_strMsg.Append(_T("�۸�%lf"),pTrade->Price);

	m_QuoteTrade.Info(m_strMsg);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	CString m_strTime = _T(""); 
	CString m_strMsg = _T("");
	m_strMsg.Format(_T("%d::::"), pRspInfo ->ErrorID);
	CString strTmp = _T("");
	strTmp.Format(_T("%s"), ANSIToUnicode(pRspInfo ->ErrorMsg));
	m_strMsg.Append(strTmp);

	m_QuoteTrade.GetTime(m_strTime);
	//if(bResult)
	{
		Locker locker(m_QuoteTrade.m_mutex);

		m_QuoteTrade.m_mapTimeMsg.insert(make_pair(m_strTime, m_strMsg));
	}

	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
		(pOrder->SessionID == SESSION_ID) &&
		(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

///�����ѯ��Լ��֤������Ӧ
void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	double bzj_b=100*(pInstrumentMarginRate->LongMarginRatioByMoney);

	CString m_strMsg = _T("��Լ��֤���ʣ�");

	CString strInstrumentID = _T("");
	strInstrumentID = ANSIToUnicode(pInstrumentMarginRate->InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;
	m_strMsg.Append(_T("%lf"),bzj_b);

	m_QuoteTrade.Info(m_strMsg);
}
///�����ѯ��Լ����������Ӧ
void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	double sxf_w=100*100*(pInstrumentCommissionRate->OpenRatioByMoney);

	CString m_strMsg = _T("��Լ�������ʣ�");

	CString strInstrumentID = _T("");
	strInstrumentID = ANSIToUnicode(pInstrumentCommissionRate->InstrumentID);
	m_strMsg = m_strMsg + strInstrumentID;
	m_strMsg.Append(_T("%lf"),sxf_w);
	m_strMsg.Append(_T("%lf"),pInstrumentCommissionRate->CloseTodayRatioByVolume);

	m_QuoteTrade.Info(m_strMsg);
}
