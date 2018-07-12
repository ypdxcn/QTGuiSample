#ifndef _PROCESS_INTERFACE_CMD_H
#define _PROCESS_INTERFACE_CMD_H

#include "CommAp.h"
#include "ProtocolProcess.h"
#include "GessDateTime.h"

using namespace std;

class COMMAP_CLASS CProcessInterfaceCmd:virtual public CAppProcessServer
{
public:
	CProcessInterfaceCmd();
	~CProcessInterfaceCmd();

	int OnRecv(char* pBuf,int nSize);
	int OnRecv();
	PRCV_BUF GetRcvBuf(){return &m_stRcvBuf;}

	int Init(CConfig * pCfg);
	int SendPacket(CPacket & packet);
	int OnSend(int nSize){return 0;}

	int OnAccept();					// ������˽��յ����Ӻ�ص�
	void OnClose();					// �Ͽ�����ʱ����

	//ȡ����ͳ�ƽӿ�
	void GetFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes,unsigned int & uiOutNowStat,unsigned int & uiOutAveBytes)
	{
		GetInFlowStatics(uiInNowStat,uiInAveBytes);
		GetOutFlowStatics(uiOutNowStat,uiOutAveBytes);
	}
	int GetNmKey(string& sKey);
protected:
	
private:
	int HandleTimeout(unsigned long& ulTmSpan){return 0;}

	//ͨ�Ų㱨��ͳ�ƴ���
	void UpdateStatics(eCommStaticType eType, int nCount = 1); 
	void GetInFlowStatics(unsigned int & uiInNowStat,unsigned int & uiInAveBytes)
	{
		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiInAveBytes = m_uiInNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiInNowStat = 0;
		}

		uiInNowStat = m_uiInNowStat;
		uiInAveBytes = m_uiInAveBytes;
		m_csStatics.Unlock();
	}

	RCV_BUF     m_stRcvBuf;				// ���ջ���ṹ��ָ�����������ַ
	size_t m_nIndex;					// �ѽ��ձ���λ��
	char m_szBuf[MAX_PACKET_SIZE];		// ���ջ���

	static int	m_blInited;

	CConfig *	m_pCfg;

	unsigned int	m_uiLastStatics[eCommMaxStatic];
	unsigned int	m_uiStatics[eCommMaxStatic];

	unsigned int	m_uiInNowStat;
	unsigned int	m_uiInAveBytes;
	CGessDateTime	m_oBeginStatTm;
	CGessMutex		m_csStatics;
};
#endif
