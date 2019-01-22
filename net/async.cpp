#include <iostream>
#include "./include/uv.h"

using namespace std;


void async_cd(uv_async_t * handle, int status)
{
	cout << "my thread id:" << uv_thread_self() << endl;
	cout << "can i successed?" << endl;

}

int main()
{
    uv_loop_t * loop ;

	loop = uv_default_loop();
	
    cout << "main thread id:" << uv_thread_self() << endl;

	uv_async_t async ;
	
    uv_async_init(loop,&async,(uv_async_cb) async_cd);

	uv_async_send(&async);

	uv_run(loop,UV_RUN_DEFAULT);
}
