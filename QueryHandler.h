
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
#include <string.h>
#include <unistd.h>
#include <unordered_set>
#include <tuple>

#include "query.h"
#include "triple.h"

using namespace std;

class QueryHandler{

	class mycomparison{
	  	bool reverse;
		public:
			mycomparison(const bool& revparam=false)
	 		{reverse=revparam;}
	  	
	  		bool operator() (const triple& lhs, const triple& rhs) const{
				if(reverse)
				    	return (lhs.dist>rhs.dist);
			    	else 
			    		return (lhs.dist<rhs.dist);
	  		}
	};

	public:
		void PathRecovery(vector<int>&,unordered_set<int>&,int,int);
		void computeAllSynopsis(query&,vector<double>&,const char*,int,const char*,int,vector<vector<int> >&);
		void computeSynopsis(query&,vector<double>&,int,const char*,int,const char*,int,vector<vector<int> >&);
		void computeSynopsis2(query&,vector<double>&,int,const char*,int,const char*,int,vector<vector<int> >&);
		void SuperGraphShortestPath(query&,int,int,vector<vector<pair<int,int> > >&,vector<double>&,vector<double>&,unordered_set<int>&,int,
									const char*,const char*,int,const char*,int,int);
		bool SimpleBFS(query&,vector<vector<pair<int,int> > >&);
		pair<bool,pair<int,int> > CReachabilityQuery(vector<vector<pair<int,int> > >&,vector<unsigned long long>&,vector<unsigned long long>&,query&,const char*,int,int,bool,bool,vector<vector<pair<int,int> > >&,vector<double>&,vector<double>&,vector<int>&,const char* ,const char*,int,bool,vector<int>&,int,int,int);
		bool BFS_C(int,vector<vector<pair<int,int> > >&,vector<unsigned long long>&,vector<unsigned long long>&,query&,queue<pair<int,int> >&,vector<bool>&,unordered_map<unsigned long long,bool>&,unordered_map<unsigned long long,bool>&,const char*,const char*,ifstream&,ifstream&,int,int,bool,bool,unordered_set<int>&,vector<int>&,bool,vector<double>&,vector<int>&);
		bool CheckConstraint(int,vector<unsigned long long>&,vector<vector<int> > &,unordered_map<unsigned long long,bool>&,const char*,ifstream&,int,bool);
		bool CheckConstraintWithSynopsisUpdate(int,vector<unsigned long long>&,vector<vector<int> > &,unordered_map<unsigned long long,bool>&,const char*,ifstream&,int,bool,vector<int>&,vector<double>&,vector<int>&);
		void split(const string&,char,vector<int>&,bool);
		//void IOAttr(int,const char*,vector<int>&,ifstream&,int);
		bool CheckAttr(vector<int>&,vector<vector<int> >&);
		bool isWithinSSP(unordered_set<int>&,int);
		void updateSynopsis(vector<double>&,int,int);
};


