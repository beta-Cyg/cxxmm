#include<cstdio>
#include<string>
#include<vector>
#include<queue>
#include<list>
#include<regex>
#include<cstring>
#include<algorithm>

#include"macro.hpp"

#include"log.hpp"
#include"result.hpp"
#include"split.hpp"
#include"tree.hpp"
#include"init.hpp"

using namespace std;

queue<string>pragma_queue;

void gen_name_queue(cygnus::tree& module_files,const char* name){
	//fprintf(stderr,"gen_name_queue name: %s\n",name);//debug
	FILE* file=fopen(name,"r");
	if(file==nullptr){
		char context[BUFFER_SIZE];
		sprintf(context,"cannot find file '%s'",name);
		cygnus::error(1,context);
	}
	char tmp[BUFFER_SIZE];
	string module_name;
	while(fgets(tmp,sizeof tmp,file)!=nullptr){
		string Tmp{tmp};
		Tmp.pop_back();
		Tmp=regex_replace(Tmp,regex("/\\*(.*?)\\*/")," ");
		auto f=[](string& s){
			while(s[0]==' ' or s[0]=='\t')s.erase(0,1);
		};
		f(Tmp);
		vector<string>line=cygnus::split(Tmp," ");
		for(auto& i:line)
			f(i);
		//for(int i=0;i<line.size();i++)fprintf(stderr,"line[%d]: %c %s\n",i,line[i].back(),line[i].c_str());//debug
		//fprintf(stderr,"if line[0]==\"import\": %d\n",int(line[0]=="import"));//debug
		if(line[0]=="export"){
			if(line.size()>=3){
				if(line[1]=="import"){
					string module_file{line[2]};
					if(module_file[0]==':')module_file.insert(0,module_name);
					module_file.erase(module_file.find(";"));
					module_file+=SUFFIX;
					module_files.insert(name,module_file);
					gen_name_queue(module_files,module_file.c_str());
				}
				else if(line[1]=="module"){
					string module_file{line[2]};
					module_file.erase(module_file.find(";"));
					module_name=module_file;
				}
			}
		}
		else if(line[0]=="module"){
			if(line.size()>=2){
				string module_file{line[1]};
				module_file.erase(module_file.find(";"));
				module_name=module_file;
			}
		}
		else if(line[0]=="import"){
			if(line.size()>=2){
				string module_file{line[1]};
				module_file.erase(module_file.find(";"));
				module_file+=SUFFIX;
				//fprintf(stderr,"insert(%s,%s)\n",name,module_file.c_str());//debug
				module_files.insert(name,module_file);
				gen_name_queue(module_files,module_file.c_str());
			}
		}
	}
	fclose(file);
}

string gen_pragma(const char* name){
	string object_name{name};
	reverse(object_name.begin(),object_name.end());
	object_name.erase(0,object_name.find(".")+1);
	reverse(object_name.begin(),object_name.end());
	
	cygnus::tree module_files(name);
	gen_name_queue(module_files,name);
	queue<string> module_que{module_files.gen_queue()};

	//gen pragma
	string pragma{COMPILER};
	pragma+=" ";
	pragma+=OPTION;
	if(strcmp(USER_OPTION,"")!=0){
		pragma+=USER_OPTION;
		pragma+=" ";
	}
	while(not module_que.empty()){
		pragma+=" ";
		pragma+=module_que.front();
		module_que.pop();
	}
	pragma+=" -o ";
	pragma+=object_name;
	return pragma;
}

cygnus::result compile(std::string pragma){
	printf("%s\n",pragma.c_str());
	FILE* pipe=popen(pragma.c_str(),"r");
	if(pipe==nullptr){
		char context[BUFFER_SIZE];
		sprintf(context,"cannot open pipe");
		cygnus::error(1,context);
	}
	string info;
	char tmp;
	while((tmp=fgetc(pipe))!=EOF)info.push_back(tmp);
	pclose(pipe);
	if(info.find("error")<info.size())return cygnus::result(1,info);
	else if(info.find("warning")<info.size())return cygnus::result(-1,info);
	return cygnus::result(0,"compile successfully");
}

int main(int argc,char** args){
	if(argc==1){
		printf("Usage: cxxmm [file1] [file2] ...\n");
		printf("cxxmm v0.0.1 by beta-Cyg\n");
		printf("GPLv3.0 License\n");
		return 0;
	}
	cygnus::init();
	for(int i=1;i<argc;i++)
		pragma_queue.push(gen_pragma(args[i]));
	while(not pragma_queue.empty()){
		compile(pragma_queue.front()).log();
		pragma_queue.pop();
	}

	return 0;
}
