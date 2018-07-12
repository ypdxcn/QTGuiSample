#include "ProcessInterfaceCmd.h"
#include "LinePacket.h"
#include "Logger.h"
#include "strutils.h"

using namespace strutils;

int CProcessInterfaceCmd::m_blInited = false;

CProcessInterfaceCmd::CProcessInterfaceCmd()
:m_nIndex(0)
{
	m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
	m_stRcvBuf.pBuf = m_szBuf;
}

CProcessInterfaceCmd::~CProcessInterfaceCmd()
{
}

int CProcessInterfaceCmd::Init(CConfig * pCfg)
{
	if (!m_blInited)
	{
		m_blInited = true;
		
		m_pCfg = pCfg;
	}
	return 0;
}

// ������˽��յ����Ӻ�ص�
int CProcessInterfaceCmd::OnAccept()
{
	SendData("RiskMgr->",9);
	return 0;
}		
/******************************************************************************
��������:socket�������������,�жϱ����Ƿ��������,��ͨ���ص������յ�����������
         �ϴ�
��������:GetPacketLength
�������:char* pBuf�ѽ��յ����ݻ���,nSize���泤��
����ֵ  :0��ȷ���� -1���մ���
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
int CProcessInterfaceCmd::OnRecv(char* pBuf,int nSize)
{	
	return 0;
}

/******************************************************************************
��������:socket�������������,�жϱ����Ƿ��������,��ͨ���ص������յ�����������
         �ϴ�,���漰�����Ѿ�����m_szBuf,m_nIndex
��������:GetPacketLength
����ֵ  :0��ȷ���� -1���մ���
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
int CProcessInterfaceCmd::OnRecv()
{
	try
	{
		//ͳ��ƽ������
		m_csStatics.Lock();
		int nInterval = m_oBeginStatTm.IntervalToNow();
		if (nInterval >= 5)
		{
			m_uiInAveBytes = m_uiInNowStat / nInterval;
			m_oBeginStatTm.ToNow();
			m_uiInNowStat = 0;
		}
		m_uiInNowStat += m_stRcvBuf.uiLen;
		m_csStatics.Unlock();

		m_nIndex += m_stRcvBuf.uiLen;
		if (m_nIndex < 2)
		{
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
			m_stRcvBuf.pBuf = m_szBuf + m_nIndex;

			return 0;
		}
	
		char * pcTmp = m_szBuf;
		for (unsigned int i = 1; i < m_nIndex; i++)
		{
			if (m_szBuf[i-1] == '\r' && m_szBuf[i] == '\n')
			{
				string sLocalIp,sPeerIp;
				int nLocalPort,nPeerPort;
				GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
				string sKey = sPeerIp + ToString(nPeerPort);

				if (i > 1)
				{
					CPacketLineReq pkt;
					pkt.Decode(pcTmp,static_cast<unsigned int>(m_szBuf+i-1-pcTmp));
					
					if (pkt.GetCmdID() == "exit")
					{
						ReqClose();
						return 0;
					}

					pkt.AddRouteKey(sKey);
					OnRecvPacket(sKey,pkt);
				}
				else
				{
					CPacketLineReq pkt("");
					pkt.AddRouteKey(sKey);
					OnRecvPacket(sKey,pkt);
				}

				pcTmp = pcTmp + i + 1;
			}
		}

		if (pcTmp > m_szBuf)
		{
			if (pcTmp < m_szBuf + m_nIndex)
			{
				memcpy(m_szBuf,pcTmp,m_szBuf + m_nIndex - pcTmp);
				m_nIndex = m_szBuf + m_nIndex - pcTmp + 1;
			}
			else
			{
				m_stRcvBuf.uiLen = MAX_PACKET_SIZE;
				m_stRcvBuf.pBuf = m_szBuf;
				m_nIndex = 0;
			}
		}
		else
		{
			m_stRcvBuf.uiLen = MAX_PACKET_SIZE - m_nIndex;
			m_stRcvBuf.pBuf = m_szBuf + m_nIndex;
		}

		return 0;
	}
	catch(std::exception e)
	{
		CRLog(E_CRITICAL,"exception:%s", e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_CRITICAL,"%s","Unknown exception");
		return -1;
	}
}

/******************************************************************************
��������:�����ṩ����Ҫ�ӿں���,��Э�������ͱ���ʱ����
���ú���:��ΪЭ������İ�װ��������
��������:SendData,�����ֱ��ͨ��socket����
�������:CTradePacket & SendGessPacket�����ͱ���
�������:
�������:
����ֵ  :int �������⴦��
������  :��ΰ
��������:2008.07.22
�޸ļ�¼:
******************************************************************************/
int CProcessInterfaceCmd::SendPacket(CPacket & SendPacket)
{
	try
	{
		//if (!IsLogin())
		//	return -1;

		CPacketLineRsp & pkt = dynamic_cast<CPacketLineRsp&>(SendPacket);

		unsigned int  nLen = 0;
		const char* pcBuf = pkt.Encode(nLen);
		
		int nRtn = 0;
		int nSend = SendData(pcBuf,nLen);
		if (nSend < 0)
		{
			nRtn = -1;
			CRLog(E_ERROR,"%s","SendData err");
		}
		return nRtn;
	}
	catch(std::bad_cast)
	{
		CRLog(E_ERROR,"%s","packet error!");
		return -1;
	}
	catch(std::exception e)
	{
		CRLog(E_ERROR,"exception:%s!",e.what());
		return -1;
	}
	catch(...)
	{
		CRLog(E_ERROR,"%s","Unknown exception!");
		return -1;
	}
}

void CProcessInterfaceCmd::OnClose()
{
	string sLocalIp,sPeerIp;
	int nLocalPort,nPeerPort;
	GetPeerInfo(sLocalIp,nLocalPort,sPeerIp,nPeerPort);
	CRLog(E_PROINFO,"Telnet CmdLine Long Connection Close %s %d",sPeerIp.c_str(),nPeerPort);
}	

int CProcessInterfaceCmd::GetNmKey(string& sKey)
{
	sKey = "Զ��Telnet�ӿڷ��������.";
	int nSockeID = static_cast<int>( SocketID());
	if (INVALID_SOCKET != nSockeID)
	{
		sKey += ToString<int>(nSockeID);
	}
	else
	{
		srand(static_cast<unsigned int>(time(0)));
		int RANGE_MIN = 1;
		int RANGE_MAX = 99;
		int nRandom = rand() % (RANGE_MAX - RANGE_MIN) + RANGE_MIN;
		sKey += ToString<int>(nRandom);
	}

	return 0;
}