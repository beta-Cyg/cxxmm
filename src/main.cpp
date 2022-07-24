#include<cstdio>
#include"make.h"
#include"version.h"

std::vector<std::string>args;
constexpr char helpinfo[]{
	"Usage: cxxmm <command> <options> <args>\n"
	"cxxmm " CXXMM_VERSION " by beta-cyg\n"
	"GPLv3.0 License\n"
};

int main(int argc,char **argv){
	if(argc==1){
		std::printf(helpinfo);
		return 0;
	}
	for(int i=1;i<argc;i++)
		args.push_back(argv[i]);
	std::string command=args[0];
	if(command=="help"){
		if(command==args.back())
			std::printf(helpinfo);
		else{}//todo: print details about specified command
	}else
	if(command=="compile"){
		if(command==args.back())
			cygnus::error(1,"no input file");
		else{
			std::vector<std::string>file_name;
			for(std::size_t i=1;i<args.size();i++)
				file_name.push_back(args[i]);
			cygnus::make(file_name);
		}
	}

	return 0;
}
