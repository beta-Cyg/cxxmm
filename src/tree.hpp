#ifndef TREE_HPP
#define TREE_HPP

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
		void basic_clear(node* n){
			if(n==nullptr)return;
			if(not n->children.empty())
				for(auto i:n->children)
					basic_clear(i);
			delete n;
		}

		node* find_node(node* n,const std::string& Name){
			if(n==nullptr)return nullptr;
			if(n->name==Name)
				return n;
			if(not n->children.empty())
				for(auto i:n->children){
					node* fn=find_node(i,Name);
					if(fn!=nullptr)
						return fn;
				}
			return nullptr;
		}

		void basic_gen(node* n,std::queue<std::string>&que){
			if(n==nullptr)return;
			if(not n->children.empty())
				for(auto i:n->children)
					basic_gen(i,que);
			que.push(n->name);
		}
	public:
		~tree(){
			basic_clear(root);
		}

		void insert(const std::string& Node,const std::string& Name){
			node* pos=find_node(root,Node);
			if(pos!=nullptr){
				pos->children.push_back(new node());
				pos->children.back()->name=Name;
			}
		}

		std::queue<std::string> gen_queue(){
			std::queue<std::string>result;
			basic_gen(root,result);
			return result;
		}
	};
}

#endif
