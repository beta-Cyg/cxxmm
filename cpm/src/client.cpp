#include<iostream>
#include<string>
#include<boost/array.hpp>
#include<boost/asio.hpp>

int main(int argc,char **argv){
	try{
		if(argc!=3){
			std::cerr<<"Usage: client <host> <package>"<<std::endl;
			return 1;
		}
		boost::asio::io_context io;
		boost::asio::ip::tcp::resolver resolver(io);
		boost::asio::ip::tcp::resolver::results_type endpoints=resolver.resolve(argv[1],"daytime");
		boost::asio::ip::tcp::socket socket(io);
		boost::asio::connect(socket,endpoints);
		boost::asio::write(socket,boost::asio::buffer(std::string(argv[2])));
		for(;;){
			boost::array<char,128>buf;
			boost::system::error_code error;
			size_t len=socket.read_some(boost::asio::buffer(buf),error);
			if(error==boost::asio::error::eof)
				break;
			else if(error)
				throw boost::system::system_error(error);
			std::cout.write(buf.data(),len);
		}
	}
	catch(std::exception& e){
		std::cerr<<e.what()<<std::endl;
	}

	return 0;
}
