
//#include "utility.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <climits>
#include <time.h>
#include <stdlib.h>

using namespace std;

class AttrGenerator{

public:
//	int countIntDigit(int);
	void _generateAttribute(const char*,vector<vector<pair<int,int> > >&,int,int,int,bool,int);
	void generateAttribute(const char*,vector<vector<pair<int,int> > >&,int,int,int,int,int,int);

};
