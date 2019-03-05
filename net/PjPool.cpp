#include "PjPool.h"
#include "PjGlobal.h"

#define DEFAULT_POOL_NAME   "pjpool"
#define DEFAULT_INITIAL_SIZE    1024
#define DEFAULT_INCREMENT_SIZE  1024

CPjPool::CPjPool(){
    mpPjPool = nullptr;
    mstrPoolName = DEFAULT_POOL_NAME;
    miInitialSize = DEFAULT_INITIAL_SIZE;
    miIncrementSize = DEFAULT_INCREMENT_SIZE;
    InitPool();
}

CPjPool::~CPjPool(){
    if (nullptr != mpPjPool){
        pj_pool_release(mpPjPool);
        mpPjPool = nullptr;
    }
}

CPjPool::CPjPool(std::string strPoolName,  int iInitialSize, int iIncrementSize) {
    miInitialSize = iInitialSize;
    miIncrementSize = iIncrementSize;
    mstrPoolName = strPoolName;
    InitPool();
}

int CPjPool::InitPool() {
    mpPjPool = pj_pool_create(sPjGlobal->GetGlobalPool()->factory, mstrPoolName.c_str(), miInitialSize, miIncrementSize, nullptr);
    ASSERT_RET_VALUE(nullptr != mpPjPool, 1);

    return 0;
}

void* CPjPool::Alloc(pj_size_t iSize) {
    ASSERT_RET_VALUE(nullptr != mpPjPool && iSize > 0, nullptr);
    return pj_pool_zalloc(mpPjPool, iSize);
}



cout << "test" << endl;
cout << "i am so timid" << endl;
