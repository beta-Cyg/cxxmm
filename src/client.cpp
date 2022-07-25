#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<cstdlib>
#include<boost/array.hpp>
#include<boost/asio.hpp>

#include"make/log.h"
#include"make/split.h"

std::vector<std::string>args;
std::string command;

//get mirror address
const char* getadd(){
	std::fstream getconf(std::string(std::getenv("HOME"))+"/.cpmadd",std::ios::in);
	if(getconf.fail())throw std::ios_base::failure("cannot open .cpmadd");
	std::string address;
	getconf>>address;
	return address.c_str();
}

int get(const std::string& pkg){
	try{
		using boost::asio::ip::tcp;
		boost::asio::io_context io;
		tcp::resolver resolver(io);
		tcp::resolver::results_type endpoints=resolver.resolve(getadd(),"1000");
		tcp::socket socket(io);
		boost::asio::connect(socket,endpoints);
		boost::asio::write(socket,boost::asio::buffer(std::string(pkg)));

		std::fstream fout(std::string(std::getenv("HOME"))+"/inst_pkg/"+pkg+".tar",std::ios::out|std::ios::binary);
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
		
		fout.close();
	}
	catch(std::exception& e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}

int pack(const std::string& pkg){
	if(command==args.back())
		cygnus::error(1,"no input directory");
	FILE* pipe=popen(("pack.py pack "+pkg).c_str(),"r");
	if(pipe==nullptr)
		std::fprintf(stderr,"%s\n",strerror(errno));
	std::string errorInfo{""};
	char tmp{'\0'};
	while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
	std::fprintf(stderr,"%s",errorInfo.c_str());
	return pclose(pipe)/256;
}

int unpack(const std::string& pkg){
	if(command==args.back())
		cygnus::error(1,"no input directory");
	FILE* pipe=popen(("pack.py unpack "+pkg).c_str(),"r");
	if(pipe==nullptr)
		std::fprintf(stderr,"%s\n",strerror(errno));
	std::string errorInfo{""};
	char tmp{'\0'};
	while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
	std::fprintf(stderr,"%s",errorInfo.c_str());
	return pclose(pipe)/256;
}

int remove(const std::string& pkg){
	if(command==args.back())
		cygnus::error(1,"no input package");
	FILE* pipe=popen(("pack.py remove "+std::string(std::getenv("HOME"))+"/inst_pkg/"+pkg).c_str(),"r");
	if(pipe==nullptr)
		std::fprintf(stderr,"%s\n",strerror(errno));
	std::string errorInfo{""};
	char tmp{'\0'};
	while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
	std::fprintf(stderr,"%s",errorInfo.c_str());
	return pclose(pipe)/256;
}

std::map<std::string,std::string> get_install_config(std::string path){
	std::map<std::string,std::string>config;
	std::fstream getconf(path,std::ios::in);
	if(getconf.fail()){
		getconf.close();
		throw std::ios_base::failure("cannot find package.conf");
	}
	std::string line;
	while(getline(getconf,line)){
		if(line=="[package]"){
			getline(getconf,line);
			config["package"]=line;
		}else
		if(line=="[version]"){
			getline(getconf,line);
			config["version"]=line;
		}else
		if(line=="[dependence]"){
			getline(getconf,line);
			if(line=="null")line.clear();
			config["dependence"]=line;
		}else
		if(line=="[install]"){
			while(getline(getconf,line)){
				if(line=="[remove]")goto remove;
				if(line=="null")line.clear();
				if(config.find("install")==config.end())
					config["install"]=line+"\n";
				else config["install"]+=line+"\n";
			}
		}else
		remove:if(line=="[remove]"){
			while(getline(getconf,line)){
				if(line=="null")line.clear();
				if(config.find("remove")==config.end())
					config["remove"]=line+"\n";
				else config["remove"]+=line+"\n";
			}
		}
	}
	return config;
}

int install(const std::string& pkg){
	if(command==args.back())
		cygnus::error(1,"no input package");
	std::string real_path{std::string(std::getenv("HOME"))+"/inst_pkg/"+pkg};

	std::fstream check(real_path+"/package.conf",std::ios::in);
	if(not check.fail()){
		check.close();
		cygnus::warn((pkg+" has been installed").c_str());
	}

	std::fstream test(real_path+".tar",std::ios::in);
	if(test.fail()){
		test.close();
		get(pkg);
	}

	unpack(real_path);

	auto config=get_install_config(real_path+"/package.conf");
	if(not config["dependence"].empty()){
		auto f=[](std::string& s){
			while(s.front()==' ')
				s.erase(0,1);
		};
		f(config["dependence"]);
		auto dependences=cygnus::split(config["dependence"]," ");
		for(auto& i:dependences){
			f(i);
			install(i);
		}
	}
	//std::cerr<<config["install"]<<std::endl;//debug
	chdir(real_path.c_str());
	FILE* pipe=popen(config["install"].c_str(),"r");
	if(pipe==nullptr)
		std::fprintf(stderr,"%s\n",strerror(errno));
	std::string errorInfo{""};
	char tmp{'\0'};
	while((tmp=std::fgetc(pipe))!=EOF)errorInfo.push_back(tmp);
	std::fprintf(stderr,"%s",errorInfo.c_str());
	return pclose(pipe)/256;
}

int main(int argc,char **argv){
	if(argc!=3){
		std::cerr<<"Usage: client <option> <package>"<<std::endl;
		return 1;
	}
	try{
		for(int i=1;i<argc;i++)
			args.push_back(argv[i]);
		command=args[0];
		if(command=="get"){
			return get(args[1]);
		}else
		if(command=="pack"){
			return pack(args[1]);
		}else
		if(command=="unpack"){
			return unpack(args[1]);
		}else
		if(command=="remove"){
			return remove(args[1]);
		}else
		if(command=="install"){
			return install(args[1]);
		}
	}
	catch(std::exception& e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}

        return 0;
}
