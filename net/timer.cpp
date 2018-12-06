#include <iostream>
#include <unistd.h>

#include "./include/uv.h"

using namespace std;

static void time_cb(uv_timer_t * handle)
{
	while(1)
	{ 
	  sleep(1);
      cout << "2222" << endl;
    }
}

int main()
{
    uv_timer_t timer;
    
	uv_loop_t * loop = uv_loop_new();

	uv_timer_init(loop,&timer);

	uv_timer_start(&timer,time_cb,5000,1000);

    sleep(100);

    

/*
	while(1)
	{
	   sleep(1);
	   cout << "111111" << endl;
	}
*/
}
