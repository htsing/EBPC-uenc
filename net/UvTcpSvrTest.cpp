#include "UvTcpSvrTest.h"

CUvTcpSvrTest::CUvTcpSvrTest(){
}

CUvTcpSvrTest::~CUvTcpSvrTest(){
}

int CUvTcpSvrTest::OnInit() {
    LOG_INFO("Enter CUvTcpSvrTest::OnInit");
    mpTcpSvr = &mcTcpSvr;
    return Listen();
}

int CUvTcpSvrTest::OnAccept(uv_tcp_t* pUvTcp) {
    LOG_INFO("Enter CUvTcpSvrTest::OnAccept");
    ASSERT_RET_VALUE(nullptr != pUvTcp, 1);
    mcCli.SetUvLoop(GetUvLoop());
    mcCli.Init();
    mcCli.SetTcpCli(pUvTcp);
    
    return 0;
}