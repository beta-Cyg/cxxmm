#ifndef INIT_H
#define INIT_H

#include<cstdio>
#include<cstring>
#include"log.h"
#include"macro.h"

namespace cygnus{
	void init(){
		//printf("init the program...\n");
		FILE* file=fopen("cxxmmfile","r");
		std::strcpy(COMPILER,"g++");
		std::strcpy(OPTION,"-fmodules-ts");
		std::strcpy(SUFFIX,".cxx");
		if(file==nullptr){
			warn("cannot find cxxmmfile");
			return;
		}
		char var[BUFFER_SIZE],val[BUFFER_SIZE];
		bool flag=false;
		while(fscanf(file,"%s = %s",var,val)!=EOF){
			if(strcmp(var,"compiler")==0){
				std::strcpy(COMPILER,val);
			}
			else if(strcmp(var,"option")==0){
				if(flag==false){
					std::strcpy(OPTION,val);
					flag=true;
				}
				//every time you use 'option = xxx', if OPTION isn't empty, then 'xxx' will fill OPTION, else 'xxx' will append to the end of USER_OPTION
				else{
					std::strcat(USER_OPTION," ");
					std::strcat(USER_OPTION,val);
				}
			}
			else if(strcmp(var,"suffix")==0){
				std::strcpy(SUFFIX,val);
			}
			//will be more vars
		}
	}
}

#endif
