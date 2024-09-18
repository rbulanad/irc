#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>
#include <arpa/inet.h>
#include <cstdlib>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

class	firstLog
{
	private:
	
	public:
	firstLog();
	~firstLog();
	firstLog(const firstLog &dup);
	firstLog &operator=(const firstLog &dup);
};

#endif
