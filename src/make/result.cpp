#include"result.h"

namespace cygnus{
	void result::log()const{
		if(rst>0)error(rst,("\n"+info).c_str());
		if(rst<0)warn(("\n"+info).c_str());
	}

	int result::status()const{
		return rst;
	}
}
