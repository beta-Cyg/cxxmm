#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<boost/array.hpp>
#include<boost/asio.hpp>

#include"make/log.h"

std::vector<std::string>args;

//get mirror address
const char* getadd(){
	std::fstream getconf(std::string(getenv("HOME"))+"/.cpmadd",std::ios::in);
	if(getconf.fail())throw std::ios_base::failure("cannot open .cpmadd");
	std::string address;
	getconf>>address;
	return address.c_str();
}

int main(int argc,char **argv){
	if(argc!=3){
		std::cerr<<"Usage: client <option> <package>"<<std::endl;
		return 1;
	}
	for(int i=1;i<argc;i++)
		args.push_back(argv[i]);
	std::string& command=args[0];
	if(command=="get"){
		try{
			boost::asio::io_context io;
			boost::asio::ip::tcp::resolver resolver(io);
			boost::asio::ip::tcp::resolver::results_type endpoints=resolver.resolve(getadd(),"1000");
			boost::asio::ip::tcp::socket socket(io);
			boost::asio::connect(socket,endpoints);
			boost::asio::write(socket,boost::asio::buffer(std::string(argv[2])));

			std::fstream fout(std::string(argv[2])+".tar",std::ios::out|std::ios::binary);
			if(fout.fail())throw std::ios_base::failure("cannot open .cpmadd");
			for(;;){
				boost::array<char,128>buf;
				boost::system::error_code error;
				size_t len=socket.read_some(boost::asio::buffer(buf),error);
				if(error==boost::asio::error::eof)
					break;
				else if(error)
					throw boost::system::system_error(error);
				fout.write(buf.data(),len);
			}
		}
		catch(std::exception& e){
			std::cerr<<e.what()<<std::endl;
		}
	}else
	if(command=="pack"){
		if(command==args.back())
			cygnus::error(1,"no input directory");
		else{
			FILE* pipe=popen(("pack.py pack "+args[1]).c_str(),"r");
			if(pipe==nullptr)
				std::fprintf(stderr,"%s\n",strerror(errno));
			std::string errorInfo{""};
			char tmp{'\0'};
			while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
			std::fprintf(stderr,"%s",errorInfo.c_str());
			return pclose(pipe)/256;
		}
	}else
	if(command=="unpack"){
		if(command==args.back())
			cygnus::error(1,"no input package");
		else{
			FILE* pipe=popen(("pack.py unpack "+args[1]).c_str(),"r");
			if(pipe==nullptr)
				std::fprintf(stderr,"%s\n",strerror(errno));
			std::string errorInfo{""};
			char tmp{'\0'};
			while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
			std::fprintf(stderr,"%s",errorInfo.c_str());
			return pclose(pipe)/256;
		}
	}else
	if(command=="remove"){
		if(command==args.back())
			cygnus::error(1,"no input package");
		else{
			FILE* pipe=popen(("pack.py remove "+args[1]).c_str(),"r");
			if(pipe==nullptr)
				std::fprintf(stderr,"%s\n",strerror(errno));
			std::string errorInfo{""};
			char tmp{'\0'};
			while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
			std::fprintf(stderr,"%s",errorInfo.c_str());
			return pclose(pipe)/256;
		}
	}
	else;

        return 0;
}
