#ifndef MAKE_H
#define MAKE_H

#include<cstdio>
#include<string>
#include<vector>
#include<queue>
#include<list>
#include<regex>
#include<cstring>
#include<algorithm>

#include"make/log.h"
#include"make/result.h"
#include"make/split.h"
#include"make/tree.h"

namespace cygnus{
	void make(const std::vector<std::string>& args);
}

#endif
