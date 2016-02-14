#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <climits>
#include <time.h>
#include <stdlib.h>
#include <unordered_map>
#include <functional>

using namespace std;

class utility{

	class pairHash{
		public:
	    	size_t operator()(const std::pair<int, int> &k) const{
	        	return k.first * soc_pokec_vertex_size + k.second;
	    	}
	};

	public:
		static const unsigned long long prime = 9223372036854775783;
		static const unsigned long long constant = 101; 
		static const int soc_pokec_vertex_size = 1632803 + 1;
		static const int dblp_vertex_size = 42251 + 1;
		static const int PA_road_network = 1090918 + 1;
		static const int FB_BFS1 = 1189767 + 1;

		static void split(const string &,char,vector<int>&,bool);
		static void splitToString(const string &,char,vector<string>&,bool);
		static void IOAttr(int,vector<int>&,ifstream&,int,string& attrData);
		static int countIntDigit(int);
		void readVertexToSuperNodeMapping(const char*,vector<int>&,vector<int>&);
		int readTopology(const char*,vector<vector<pair<int,int> > >&,int numSuperNode = 0) ;
		int readTopologyFormat1(const char*,vector<vector<pair<int,int> > >&,int,char);
		int scanTopologyFormat1(const char*,vector<vector<pair<int,int> > >&,char);
		void readAttrHash(const char*,vector<unsigned long long>&,bool);
//		void readSynopsis(const char*,vector<double>&);
};

