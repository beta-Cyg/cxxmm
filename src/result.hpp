#ifndef RESULT_HPP
#define RESULT_HPP

namespace cygnus{
	class result{
	private:
		int rst;
		std::string info;
	public:
		result(int&& Result,const std::string& Info):rst(Result),info(Info){}

		void log()const{
			if(rst>0)error(rst,("\n"+info).c_str());
			if(rst<0)warning(("\n"+info).c_str());
		}

		int status()const{
			return rst;
		}
	};
}

#endif
