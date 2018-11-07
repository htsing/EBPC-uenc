#include <iostream>
#include "./ca.h"


void ca_print()
{ 
	int flag=1;
	while(flag)
	{
		std::cout << "ca..."<< std::endl << std::endl;	

		std::cout << "please input 0 to return:" ;
		std::cin >> flag;
		if(flag!=0)
		{
			std::cout << " input error!!!" << std::endl << std::endl;
		} 

	} 
}
