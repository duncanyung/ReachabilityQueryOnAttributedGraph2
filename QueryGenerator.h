#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <climits>
#include <stdlib.h>
#include <queue>
#include <unordered_set>
#include <string.h>

#include "QueryHandler.h"


class QueryGenerator{


	public:
		void eraseConstraints(vector<vector<int> >&,int);
		int getAttributeInfo(char*,vector<int>&);
		void generateQuery(vector<query>&,int,const char*,vector<vector<pair<int,int> > >&,int,int,int,int);
		void writeQueries(vector<query>&,const char*,int,int,int);
		void readSrcDest(query&,ifstream&);
		void readAttrCon(vector<vector<int> >&,ifstream&,int);
		void readQueries(const char*,vector<query>&,int,int,int);
};



