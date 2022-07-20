#ifndef LOG_HPP
#define LOG_HPP

namespace cygnus{
	void error(int rst,const char* context){
		fprintf(stderr,"\033[0m\033[31merror\033[0m: %s\n",context);
		std::exit(rst);
	}

	void warning(const char* context){
		fprintf(stderr,"\033[0m\033[33mwarning\033[0m: %s\n",context);
	}
}

#endif
