#ifndef RESULT_H
#define RESULT_H

#include<string>
#include"log.h"

namespace cygnus{
	class result{
	private:
		int rst;
		std::string info;
	public:
		result(int&& Result,const std::string& Info):rst(Result),info(Info){}

		void log()const;

		int status()const;
	};
}

#endif
