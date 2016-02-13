#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <climits>
#include <time.h>
#include <stdlib.h>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string.h>
#include <unistd.h>


using namespace std;



class ConstructSuperGraph{

	public:
		void buildSynopsis(const char*,vector<int>&,bool,int,int);
		void buildSynopsis2(const char*,vector<int>&,bool,int,int,const char*,int);
		void construct(int,int,int,int,const char*,vector<vector<pair<int,int> > >&,int,int,int);
		void buildSuperGraph(const char*,vector<int>&,vector<vector<pair<int,int> > >&,int);
		int clustering(const char*,vector<int>&,vector<vector<pair<int,int> > >&,int);
};


