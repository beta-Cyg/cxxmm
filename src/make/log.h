#ifndef LOG_H
#define LOG_H

#include<cstdio>
#include<cstdlib>

namespace cygnus{
	void error(int rst,const char* context);

	void warn(const char* context);
}

#endif
