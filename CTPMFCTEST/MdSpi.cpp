/////////////////////////////////////////////////////////////////////////
///���ƣ�AutoTrader
///�汾��v1.0.x
///���ڣ�2014xxxx
///˵������������CTP����
///���ߣ�Сƽ����
///  QQ��1396144618
/////////////////////////////////////////////////////////////////////////
//#include <iostream>
//#include <fstream>
//#include <sstream> 
//#include <string>
//#include <time.h>
//#include <iomanip>
//#include <windows.h>
//#include <math.h>
//using namespace std;

#include "TraderSpi.h"
#include "MdSpi.h"
#include "stdafx.h"

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcMdApi* pMdApi;

// ���ò���	
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];	
extern int iInstrumentID;

// ������
extern int iRequestID;

// User��������
extern	int		RunMode;
extern	bool	ReceiveTick[20];
extern	bool	FristTick[20];
extern	bool	LastTick[20];
extern	TThostFtdcDateExprType	TradingDay;

extern	string	InstrumentID_name;	//

extern	int		Q_BarTime_1;		//
extern	double	Q_BarTime_2;		//
extern	int		Q_BarTime_1n[20];	//

extern	double  Mn_open[20][60];	//����K�߿�
extern	double  Mn_high[20][60];	//����K�߸�
extern	double  Mn_low[20][60];		//����K�ߵ�
extern	double  Mn_close[20][60];	//����K����
extern	bool	MnKlinesig[20];		//1����K�߱�־

extern	double  Day_open[20][60];	//��K�߿�
extern	double  Day_high[20][60];	//��K�߸�
extern	double  Day_low[20][60];	//��K�ߵ�
extern	double  Day_close[20][60];	//��K����

extern	char    LogFilePaths[80];				//������־
extern	char	TickFileWritepaths[20][80];		//
extern	char	InstrumentID_n[20][10];			//
extern	double  InstrumentID_minmove[20];		//
extern	double  InstrumentID_diff[20];			//

extern double	tick_data[20][10];				//TICK��������
extern	double	tick_AskPrice1[20][60];			//��һ��
extern	double	tick_BidPrice1[20][60];			//��һ��
extern	double	tick_AskVolume1[20][60];		//��һ��
extern	double	tick_BidVolume1[20][60];		//��һ��
extern	double	tick_Volume[20][60];			//�ɽ���
extern	double	tick_OpenInterest[20][60];		//�ֲ���

extern	double	Day_CloseProfit[20];
extern	double	Day_CloseProfitA[20];
extern	double	Day_CloseProfitB[20];

extern double	Day_TradeNumb[20];



void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << "�����¼��..." << endl;

	///�û���¼����
	ReqUserLogin();

}


void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pMdApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> ���������û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;

    if(IsErrorRspInfo(pRspInfo))
    {
		cerr << "--->>> �����¼����: " << pRspInfo->ErrorID<< pRspInfo->ErrorMsg << endl;
    }
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		cerr << "--->>> ��ȡ��ǰ������ = " << pMdApi->GetTradingDay() << endl;
		// ����������
		SubscribeMarketData();	
	}
}

void CMdSpi::SubscribeMarketData()
{
	//int iResult0 = pMdApi->UnSubscribeMarketData(ppInstrumentID, iInstrumentID);
	//cerr << "--->>> ȡ�����鶩������: " << ((iResult0 == 0) ? "�ɹ�" : "ʧ��") << endl;
	Sleep(1000);
	int iResult = pMdApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "��������Ʒ�֣�" <<pSpecificInstrument->InstrumentID<< endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//cerr << "--->>> " << __FUNCTION__ << endl;
	///���ݻ���
	string dbtoch(double nums) ;
	double GetLocalTimeMs1();
	void WriteConfiguration(char *filepaths);
	void Count_Day_BuySellpos(int i);

	char names[10] = "";
	char times[10] = "";
	
	strcpy(names, pDepthMarketData->InstrumentID);
	strcpy(times, pDepthMarketData->UpdateTime);
	strcpy(TradingDay, pDepthMarketData->TradingDay);

	InstrumentID_name = names;
	string str0 = names;
	string str1 = times;
	string str2 = times;
	string str3 = times;

	str1 = str1.substr(0, 2);		//
	str2 = str2.substr(3, 2);		//
	str3 = str3.substr(6, 2);		//
	int hours = atoi(str1.c_str());
	int minutes = atoi(str2.c_str());
	int seconds = atoi(str3.c_str());
	int Millisecs = pDepthMarketData->UpdateMillisec;

	double NewPrice = pDepthMarketData->LastPrice;

	Q_BarTime_1 = hours * 60 * 60 + minutes * 60 + seconds;						//ʱ��������
	Q_BarTime_2 = (1 / 10e1)*hours + (1 / 10e3)*minutes + (1 / 10e5)*seconds;	//ʱ���ʽ0.145100 = 14��51��00



	for (int i = 0; i < 20; i++)
	{
		if (InstrumentID_name == InstrumentID_n[i])
		{
			ReceiveTick[i] = true;

			Q_BarTime_1n[i]=Q_BarTime_1;

			tick_data[i][0] = 2;		//���ñ�־λ
			tick_data[i][1] = atof(pDepthMarketData->TradingDay);
			tick_data[i][2] = Q_BarTime_2;//pDepthMarketData->UpdateTime;
			tick_data[i][3] = pDepthMarketData->UpdateMillisec;

			tick_data[i][4] = pDepthMarketData->LastPrice;
			tick_data[i][5] = pDepthMarketData->AveragePrice;
			tick_data[i][6] = pDepthMarketData->HighestPrice;
			tick_data[i][7] = pDepthMarketData->LowestPrice;
			
			//***************************************************	

			bool check0=(tick_data[i][2]>0.0910 && tick_data[i][2]<0.0915 && i>17)  || (tick_data[i][2]<0.1518 && tick_data[i][2]>0.0914 && !FristTick[i] && i>17);
			bool check1=(tick_data[i][2]>0.0856 && tick_data[i][2]<0.0900 && i<=17) || ( tick_data[i][2]<0.1505 && tick_data[i][2]>0.0859  && !FristTick[i] && i<=17);
			bool check2=(tick_data[i][2]>0.2056 && tick_data[i][2]<0.2100 && i<=17) || ((tick_data[i][2]<0.0235 || tick_data[i][2]>0.2059) && !FristTick[i] && i<=17);

			if ( RunMode && ( check0 || check1 || check2) )
			{
					MnKlinesig[i]=true;
					Day_open[i][0]=pDepthMarketData->OpenPrice;

					Mn_open[i][1]	=Day_open[i][0]; 
					Mn_high[i][1]	=Day_open[i][0];
					Mn_low[i][1]	=Day_open[i][0];
					Mn_close[i][1]	=Day_open[i][0];

					Mn_open[i][0]	=Day_open[i][0]; 
					Mn_high[i][0]	=Day_open[i][0];
					Mn_low[i][0]	=Day_open[i][0];
					Mn_close[i][0]	=Day_open[i][0];

					tick_data[i][8]=pDepthMarketData->UpperLimitPrice;	//��ͣ��
					tick_data[i][9]=pDepthMarketData->LowerLimitPrice;	//��ͣ��					

					Day_CloseProfit[i]=0;
					Day_CloseProfitA[i]=0;
					Day_CloseProfitB[i]=0;

					Day_TradeNumb[i]=0;

			}
			
			//***************************************************	

			for (int j = 59; j >0; j--)
			{
			tick_AskPrice1[i][j]	= tick_AskPrice1[i][j-1];
			tick_BidPrice1[i][j]	= tick_BidPrice1[i][j-1];
			tick_AskVolume1[i][j]	= tick_AskVolume1[i][j-1];
			tick_BidVolume1[i][j]	= tick_BidVolume1[i][j-1];
			tick_Volume[i][j]		= tick_Volume[i][j-1];
			tick_OpenInterest[i][j] = tick_OpenInterest[i][j-1];
			}

			//ʵ�̿���ǰ��¼���������˴���������ֵ�������δ���
			if ( (tick_data[i][2]>0.0250 && tick_data[i][2]<0.0850) || (tick_data[i][2]>0.1550 && tick_data[i][2]<0.2050) )
			//if (AskPrice1t>99999 || BidPrice1t>99999)
			{
			tick_AskPrice1[i][0]	= 0;
			tick_BidPrice1[i][0]	= 0;
			}
			else
			{
			tick_AskPrice1[i][0]	= pDepthMarketData->AskPrice1;
			tick_BidPrice1[i][0]	= pDepthMarketData->BidPrice1;
			}

			tick_AskVolume1[i][0]	= pDepthMarketData->AskVolume1;
			tick_BidVolume1[i][0]	= pDepthMarketData->BidVolume1;
			tick_Volume[i][0]		= pDepthMarketData->Volume;
			tick_OpenInterest[i][0] = pDepthMarketData->OpenInterest;
			
			//***************************************************	

			bool Timemore0=tick_data[i][2]!=0.0859 && tick_data[i][2]!=0.0900 && tick_data[i][2]!=0.1015 && tick_data[i][2]!=0.1130 && tick_data[i][2]!=0.1500;
			bool Timemore1=tick_data[i][2]!=0.2059 && tick_data[i][2]!=0.2100 && tick_data[i][2]!=0.0230;
			bool Timemore2=(tick_data[i][2]>0.0900 && tick_data[i][2]<0.1500) ||  tick_data[i][2]>0.2100 || tick_data[i][2]<0.0230;

			if (Timemore0 && Timemore1  && Timemore2 && seconds>=0 && seconds<40 && tick_Volume[i][0]>tick_Volume[i][1] && MnKlinesig[i]==false)
			{
				MnKlinesig[i]=true;

				Mn_open[i][1]	=Mn_open[i][0]; 
				Mn_high[i][1]	=Mn_high[i][0];
				Mn_low[i][1]	=Mn_low[i][0];
				Mn_close[i][1]	=Mn_close[i][0];

				Mn_open[i][0]	=NewPrice; 
				Mn_high[i][0]	=NewPrice;
				Mn_low[i][0]	=NewPrice;
				Mn_close[i][0]	=NewPrice;
				//��ӡK��������
				//cerr << "--->>> " << InstrumentID_n[i] << "_" <<dbtoch(tick_data[i][1])<< "_" << tick_data[i][2]<< "_" << Mn_open[i][1]<< "_" << Mn_high[i][1]<< "_" << Mn_low[i][1]<< "_" << Mn_close[i][1] << endl;
			}
			else
			{
				Mn_high[i][0]	=max(Mn_high[i][0],NewPrice);
				Mn_low[i][0]	=min(Mn_low[i][0],NewPrice);
				Mn_close[i][0]	=NewPrice;
			}

			if (seconds>45 && seconds<55 && MnKlinesig[i]==true)
			{
				MnKlinesig[i]=false;
			}
			//***************************************************
			
			//tick_data[i][0]= 0;		//���ñ�־λ

			if (TickFileWritepaths[i][0] == '\0')
			{
				tick_data[i][8] = pDepthMarketData->UpperLimitPrice;
				tick_data[i][9] = pDepthMarketData->LowerLimitPrice;

				strcpy_s(TickFileWritepaths[i], "./TickData/");
				strcat_s(TickFileWritepaths[i], pDepthMarketData->InstrumentID);
				strcat_s(TickFileWritepaths[i], "_");
				strcat_s(TickFileWritepaths[i], TradingDay);
				strcat_s(TickFileWritepaths[i], ".txt");

				//����ļ��Ƿ���ڣ��Ƿ���Ҫ�½��ı��ļ�
				ifstream inf;
				ofstream ouf;
				inf.open(TickFileWritepaths[i], ios::out);
			}
			
			//��¼TICK����
			ofstream o_file(TickFileWritepaths[i], ios::app);

			if ( RunMode && ( check0 || check1 || check2) )
			{
			o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[i][2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< "\t" << Day_open[i][2] << "\t" << Day_high[i][2] << "\t" << Day_low[i][2] << "\t" << Day_close[i][2] <<"\t"<< Day_open[i][1] << "\t" << Day_high[i][1] << "\t" << Day_low[i][1] << "\t" <<Day_close[i][1] << "\t" << tick_data[i][8] << "\t" << tick_data[i][9] << endl;
			}
			else
			{
			o_file << dbtoch(tick_data[i][1]) << "\t" << tick_data[i][2] << "\t" << Millisecs << "\t" << tick_AskPrice1[i][0] << "\t" << tick_AskVolume1[i][0] << "\t" << tick_BidPrice1[i][0] << "\t" << tick_BidVolume1[i][0] << "\t" << tick_data[i][4] << "\t" << dbtoch(tick_Volume[i][0]) << "\t" << dbtoch(tick_OpenInterest[i][0])<< endl; //������д�뵽�ı��ļ���
			}
			o_file.close();						//�ر��ļ�

			
			if (tick_data[i][2]>0.145950 && (tick_Volume[i][0]-tick_Volume[i][1])<0.01 &&  (tick_OpenInterest[i][0]-tick_OpenInterest[i][1])<0.01)// && LastTick[i]==false)
			{
				//LastTick[i]=true;

				Day_open[i][0]	=pDepthMarketData->OpenPrice;
				Day_high[i][0]	=pDepthMarketData->HighestPrice;
				Day_low[i][0]	=pDepthMarketData->LowestPrice;
				Day_close[i][0]	=pDepthMarketData->LastPrice;				
				//cerr << "--->>> " <<"WriteConfiguration!" << endl;
				//WriteConfiguration("./AutoTrader.dat");
			}

		if ( (tick_data[i][2]>0.0913 && i>17) || (tick_data[i][2]>0.0858 && i<=17) )
		{
			FristTick[i]=true;
		}

		ReceiveTick[i] = false;
		tick_data[i][0] = 1;		//���ñ�־λ

		}

	}

	//cerr << "--->>> " << InstrumentID_name << "Tick is OK!" << endl;
	//cerr << "--->>> " << "Time is:" <<setprecision(9)<< GetLocalTimeMs1() << endl;

}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}


