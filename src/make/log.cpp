#include"log.h"

namespace cygnus{
	void error(int rst,const char* context){
		std::fprintf(stderr,"\033[0m\033[31merror\033[0m: %s\n",context);
		std::exit(rst);
	}

	void warn(const char* context){
		std::fprintf(stderr,"\033[0m\033[33mwarning\033[0m: %s\n",context);
	}
}
