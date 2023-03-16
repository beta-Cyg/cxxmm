#include<fstream>
#include<iostream>
#include<string>
#include<map>
#include<boost/asio.hpp>
#include<boost/array.hpp>
using boost::asio::ip::tcp;

std::map<std::string,std::string>config;

void init(const char* conf){
	std::fstream getconf(conf,std::ios::in);
	if(getconf.fail())
		throw std::ios_base::failure("cannot open server.conf");
	std::string path;
	getconf>>path;
	config["PATH"]=path;
}

int main(int argc,char **argv){
	try{
		if(argc!=2){
			std::cerr<<"Usage: server <config file>"<<std::endl;
			return 1;
		}
		init(argv[1]);
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1000));
		for(;;){
			tcp::socket socket(io_context);
			acceptor.accept(socket);
			
			std::cout<<"new connection"<<std::endl;

			std::string pkgname;
			boost::array<char,128>buf;
			boost::system::error_code error;
			size_t len=socket.read_some(boost::asio::buffer(buf),error);
			if(error)
				throw boost::system::system_error(error);
			for(size_t i=0;i<len;i++)
				pkgname.push_back(buf[i]);

			for(int i=0;i<128;i++)
				buf[i]=0;
			std::fstream fin(config["PATH"]+pkgname+".tar",std::ios::in|std::ios::binary);
			if(fin.fail())
				std::cerr<<"cannot open "+config["PATH"]+pkgname+".tar"<<std::endl;
			while(len=fin.readsome(buf.data(),128)){
				//std::cout.write(buf.data(),len);
				boost::asio::write(socket,boost::asio::buffer(buf,len),error);
			}
		}
	}
	catch(std::exception& e){
		std::cerr<<e.what()<<std::endl;
	}

	return 0;
}
