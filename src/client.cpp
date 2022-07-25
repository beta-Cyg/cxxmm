#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<cstdlib>
#include<boost/array.hpp>
#include<boost/asio.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>

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

boost::property_tree::ptree get_install_config(std::string path){
	boost::property_tree::ptree config;
	boost::property_tree::read_xml(path,config);
	return config;
}

int install(const std::string& pkg){
	if(command==args.back())
		cygnus::error(1,"no input package");
	std::string real_path{std::string(std::getenv("HOME"))+"/inst_pkg/"+pkg};

	std::fstream check(real_path+"/package.xml",std::ios::in);
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

	auto config=get_install_config(real_path+"/package.xml");

	for(auto& i:config.get_child("cpm")){
		if(i.second.data()=="null")i.second.data()="";
	}
	//std::cerr<<"dependence="<<config.get<std::string>("cpm.dependence");

	if(not config.get<std::string>("cpm.dependence").empty()){
		auto f=[](std::string& s){
			while(s.front()==' ')
				s.erase(0,1);
		};
		std::string dependence=config.get<std::string>("cpm.dependence");
		f(dependence);
		auto dependences=cygnus::split(dependence," ");
		for(auto& i:dependences){
			f(i);
			install(i);
		}
	}
	//std::cerr<<config["install"]<<std::endl;//debug
	chdir(real_path.c_str());
	FILE* pipe=popen(config.get<std::string>("cpm.install").c_str(),"r");
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
