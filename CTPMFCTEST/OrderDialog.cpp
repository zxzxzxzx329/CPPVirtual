// OrderDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CTPMFCTEST.h"
#include "OrderDialog.h"


// COrderDialog �Ի���

IMPLEMENT_DYNAMIC(COrderDialog, CDialog)

COrderDialog::COrderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COrderDialog::IDD, pParent)
{

}

COrderDialog::~COrderDialog()
{
}

void COrderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COrderDialog, CDialog)
END_MESSAGE_MAP()


// COrderDialog ��Ϣ�������
