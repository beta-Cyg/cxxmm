#ifndef SPLIT_HPP
#define SPLIT_HPP

namespace cygnus{
	std::vector<std::string> split(std::string str,const std::string& delimiter){
		std::vector<std::string>result;
		std::size_t pos;
		while((pos=str.find(delimiter))<str.size()){
			result.push_back(str.substr(0,pos));
			str.erase(0,pos+delimiter.size());
		}
		result.push_back(str);
		return result;
	}
}

#endif
