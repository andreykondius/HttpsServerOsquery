#include "iostream"
#include "forwardmanager.h"

int main()
{
	try {
		lib::ForwardManager forwardManager;
	} catch (...) {
		std::cout << "Error ! Что-то пошло не так." << std::endl;
	}

    return 0;
}

