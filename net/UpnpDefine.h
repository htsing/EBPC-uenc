#ifndef _UPNPDEFINE_H_
#define _UPNPDEFINE_H_

#ifndef MAKE_HRESULT
#define MAKE_HRESULT(sev,fac,code) \
((HRESULT) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )
#endif
//	��������
#define E_UNAT_BAD_ARGUMENTS				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x200)

//	���ǹ���IP
#define E_UNAT_NOT_IN_LAN					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x201)

//	�Ҳ���·����
#define E_UNAT_CANNOT_FIND_ROUTER			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x202)

//	��ʱ�޻�Ӧ
#define E_UNAT_TIMEOUT						MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x203)

//	�˿�ӳ��ı����������
#define E_UNAT_ENTRY_MAYBE_FULL				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x205)

//	δ֪����
#define E_UNAT_UNKNOWN_ERROR				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x206)

//	�ﵽ����˿ڵ����Դ���
#define E_UNAT_REACH_RAND_PORT_RETRY_TIMES	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x207)

//	���������У���û�н��
#define E_UNAT_SEARCH_PENDING				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x208)

//	����Socketʧ��
#define E_UNAT_CREATE_SOCKET_FAILED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x209)

//	Socket��ʧ��
#define E_UNAT_SOCKET_BIND_FAILED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x210)

//	Action����Httpʧ����
#define E_UNAT_ACTION_HTTP_ERRORCODE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x211)

typedef struct tagUPNPNAT_MAPPING
{
	WORD		m_wInternalPort;				// Port mapping internal port
	WORD		m_wExternalPort;				// Port mapping external port
	char		m_strProtocol[16];					// Protocol-> TCP (UPNPNAT_PROTOCOL:UNAT_TCP) || UDP (UPNPNAT_PROTOCOL:UNAT_UDP)
	char		m_strDescription[32];				// Port mapping description
	char		m_strInternalClient[16];
}UPNPNAT_MAPPING;

typedef struct tagMAPPING_RESULT
{
	DWORD				dwCustomParam;
	WORD				wInternalPort;
	WORD				wExternalPort;
	BOOL				bCleanedFillupBug;
	DWORD				dwActionErrorCode;
}MAPPING_RESULT;

typedef struct tagUPNPNAT_MAPPINGKEY
{
	char		m_strRemoteHost[16];
	USHORT		m_usExternalPort;
	char		m_strProtocol[16];
}UPNPNAT_MAPPINGKEY;

#endif