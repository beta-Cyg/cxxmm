#include<iostream>
#include<string>
#include<boost/asio.hpp>
#include<boost/array.hpp>
using boost::asio::ip::tcp;

int main(){
	try{
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));
		for(;;){
			tcp::socket socket(io_context);
			acceptor.accept(socket);
			std::string pkgname;
			boost::array<char,128>buf;
			boost::system::error_code error;
			size_t len=socket.read_some(boost::asio::buffer(buf),error);
			if(error)
				throw boost::system::system_error(error);
			for(int i=0;i<len;i++)
				pkgname.push_back(buf[i]);
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(pkgname), ignored_error);
		}
	}
	catch(std::exception& e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
