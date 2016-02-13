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

class ComputeHashValue{

public:
	void _computeHashValue(char*,char*,char*,bool);
	void computeHashValue(const char*,int,int,int);
};
