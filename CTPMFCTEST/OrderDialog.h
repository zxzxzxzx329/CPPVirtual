#pragma once


// COrderDialog �Ի���

class COrderDialog : public CDialog
{
	DECLARE_DYNAMIC(COrderDialog)

public:
	COrderDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrderDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
