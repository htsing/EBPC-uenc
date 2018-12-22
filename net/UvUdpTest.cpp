#include "UvUdpTest.h"

CUvUdpTest::CUvUdpTest(){
}

CUvUdpTest::~CUvUdpTest(){
}

int CUvUdpTest::OnInit() {
    mpUdp = &mstUdp;
    
    return Start();
}

int CUvUdpTest::OnRecv(ssize_t nRead, const uv_buf_t* pBuf, const struct sockaddr* pAddr, unsigned iFlag) {
    LOG_INFO("Enter CUvUdpTest::OnRecv iFlag:%d", iFlag);
    if (nRead <= 0 || nullptr == pBuf) {
        return 0;
    }

    return Send(pBuf->base, nRead, pAddr);
}

int CUvUdpTest::OnSend(int iStatus) {
    LOG_INFO("Enter CUvUdpTest::OnSend iStatus:%d", iStatus);
    return 0;
}