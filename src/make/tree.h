#ifndef TREE_H
#define TREE_H

#include<string>
#include<queue>
#include<list>

namespace cygnus{
	class tree{
	private:
		struct node{
			std::string name{""};
			std::list<node*>children{};
		}*root;
	public:
		tree(const std::string& Name):root(new node()){
			root->name=Name;
		}
	private:
		void basic_clear(node* n);

		node* find_node(node* n,const std::string& Name);

		void basic_gen(node* n,std::queue<std::string>&que);
	public:
		~tree(){
			basic_clear(root);
		}

		void insert(const std::string& Node,const std::string& Name);

		std::queue<std::string> gen_queue();
	};
}

#endif
