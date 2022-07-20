#include"tree.h"

namespace cygnus{
	void tree::basic_clear(tree::node* n){
		if(n==nullptr)return;
		if(not n->children.empty())
			for(auto i:n->children)
				basic_clear(i);
		delete n;
	}

	tree::node* tree::find_node(tree::node* n,const std::string& Name){
		if(n==nullptr)return nullptr;
		if(n->name==Name)
			return n;
		if(not n->children.empty())
			for(auto i:n->children){
				tree::node* fn=find_node(i,Name);
				if(fn!=nullptr)
					return fn;
			}
		return nullptr;
	}

	void tree::basic_gen(tree::node* n,std::queue<std::string>&que){
		if(n==nullptr)return;
		if(not n->children.empty())
		for(auto i:n->children)
			basic_gen(i,que);
		que.push(n->name);
	}

	void tree::insert(const std::string& Node,const std::string& Name){
		tree::node* pos=find_node(root,Node);
		if(pos!=nullptr){
			pos->children.push_back(new node());
			pos->children.back()->name=Name;
		}
	}

	std::queue<std::string> tree::gen_queue(){
		std::queue<std::string>result;
		basic_gen(root,result);
		return result;
	}
}
