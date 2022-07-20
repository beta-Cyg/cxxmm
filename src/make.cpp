#include"make.h"
#include"make/init.h"

namespace cygnus{
	static std::queue<std::string>pragma_queue;

	static std::string prefix;
	
	void gen_name_queue(tree& module_files,const char* name){
		//fprintf(stderr,"gen_name_queue name: %s\n",name);//debug
		FILE* file=std::fopen(name,"r");
		if(file==nullptr){
			char context[BUFFER_SIZE];
			std::sprintf(context,"cannot find file '%s'",name);
			error(1,context);
		}
		char tmp[BUFFER_SIZE];
		std::string module_name;
		while(fgets(tmp,sizeof tmp,file)!=nullptr){
			std::string Tmp{tmp};
			Tmp.pop_back();
			Tmp=std::regex_replace(Tmp,std::regex("/\\*(.*?)\\*/")," ");
			auto f=[](std::string& s){
				while(s[0]==' ' or s[0]=='\t')s.erase(0,1);
			};
			f(Tmp);
			std::vector<std::string>line=cygnus::split(Tmp," ");
			for(auto& i:line)
				f(i);
			//for(int i=0;i<line.size();i++)std::fprintf(stderr,"line[%d]: %c %s\n",i,line[i].back(),line[i].c_str());//debug
			//std::fprintf(stderr,"if line[0]==\"import\": %d\n",int(line[0]=="import"));//debug
			if(line[0]=="export"){
				if(line.size()>=3){
					if(line[1]=="import"){
						std::string module_file{line[2]};
						if(module_file[0]==':')module_file.insert(0,module_name);
						module_file.erase(module_file.find(";"));
						module_file+=SUFFIX;
						module_file.insert(0,prefix);
						module_files.insert(name,module_file);
						gen_name_queue(module_files,module_file.c_str());
					}
					else if(line[1]=="module"){
						std::string module_file{line[2]};
						module_file.erase(module_file.find(";"));
						module_name=module_file;
					}
				}
			}
			else if(line[0]=="module"){
				if(line.size()>=2){
					std::string module_file{line[1]};
					module_file.erase(module_file.find(";"));
					module_name=module_file;
				}
			}
			else if(line[0]=="import"){
				if(line.size()>=2){
					std::string module_file{line[1]};
					module_file.erase(module_file.find(";"));
					module_file+=SUFFIX;
					module_file.insert(0,prefix);
					//std::fprintf(stderr,"insert(%s,%s)\n",name,module_file.c_str());//debug
					module_files.insert(name,module_file);
					gen_name_queue(module_files,module_file.c_str());
				}
			}
		}
		std::fclose(file);
	}
	
	std::string gen_pragma(const char* name){
		std::string object_name{name};
		std::reverse(object_name.begin(),object_name.end());
		object_name.erase(0,object_name.find(".")+1);
		std::reverse(object_name.begin(),object_name.end());
		
		tree module_files(name);
		gen_name_queue(module_files,name);
		std::queue<std::string> module_que{module_files.gen_queue()};
	
		//gen pragma
		std::string pragma{COMPILER};
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

	result compile(std::string pragma){
		std::printf("%s\n",pragma.c_str());
		FILE* pipe=popen(pragma.c_str(),"r");
		if(pipe==nullptr){
			char context[BUFFER_SIZE];
			std::sprintf(context,"cannot open pipe");
			error(1,context);
		}
		std::string info;
		char tmp;
		while((tmp=std::fgetc(pipe))!=EOF)info.push_back(tmp);
		pclose(pipe);
		if(info.find("error")<info.size())return result(1,info);
		else if(info.find("warning")<info.size())return result(-1,info);
		return result(0,"compile successfully");
	}

	void make(const std::vector<std::string>& args){
		init();
		for(auto i:args){
			auto tmp=i;
			std::reverse(tmp.begin(),tmp.end());
			std::size_t pos=tmp.find("/");
			if(pos<tmp.size()){
				prefix=tmp.substr(pos);
				std::reverse(prefix.begin(),prefix.end());
			}
			pragma_queue.push(gen_pragma(i.c_str()));
			prefix="";
		}
		while(not pragma_queue.empty()){
			compile(pragma_queue.front()).log();
			pragma_queue.pop();
		}
	}
}
