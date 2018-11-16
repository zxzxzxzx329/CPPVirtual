
// CTPMFCTESTDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Quote.h"
#include "TraderSpi.h"

// CCTPMFCTESTDlg �Ի���
class CCTPMFCTESTDlg : public CDialog
{
// ����
public:
	CCTPMFCTESTDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CTPMFCTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_IPADDRESS;
	CEdit m_PORT;
	CIPAddressCtrl m_IPADDRESSCTRL;
	CEdit m_ControlMsg;
	afx_msg void OnBnClickedButtonLogin();
	CEdit m_SECID;
	afx_msg void OnBnClickedButtonSubsecid();
	afx_msg void OnBnClickedButtonBuyorder();
	afx_msg void OnBnClickedButtonSellorder();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnShowQuoteMessage(WPARAM wParam, LPARAM lParam);
	CEdit m_NewPrice;
	CEdit m_BuyPrice;
	CEdit m_SellPrice;
	CEdit m_BorkerID;
	CEdit m_InvestorID;
	CEdit m_Password;

	map<CString,CString> m_tmpmapTimeMsg;
	CString m_szMsg;

public:
	void Print(CString strMsg);
	COLORREF SetTextColor(TThostFtdcPriceType NewPrice, TThostFtdcPriceType PriceSettlePrice);
	CString m_strBorkerID;
	CString m_strInvestorID;
	CString m_strPassword;
	CQuote  m_Quote;
	CString m_strSecID;

	void OnReady();
	CIPAddressCtrl m_IPADDRESS2;
	CEdit m_Port2;
	CEdit m_NewVolumn;
	CEdit m_BuyVolumn;
	CEdit m_SellVolumn;
	CEdit m_UpdateLastTime;
	CListCtrl m_HoldOrder;
	CBrush m_redbrush,m_bluebrush,m_blackbrush; 
	COLORREF m_redcolor,m_bluecolor,m_textcolor;
	COLORREF m_newPricecolor,m_buyPricecolor,m_sellPricecolor;
};
