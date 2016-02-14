#include "utility.h"
#include <unistd.h>

/*void utility::readSynopsis(const char* synopsisFileName,vector<double>& synopsis){
	FILE* syFile = fopen(synopsisFileName,"r");



	fclose(syFile);
}*/

void utility::split(const string &s, char delim, vector<int> &elems,bool skipFirst){
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
    	if(item.compare("")==0)
    		break;

    	if(skipFirst == false)
        	elems.push_back(stoi(item));
        skipFirst = false;
    }
}

void utility::splitToString(const string &s, char delim, vector<string> &elems,bool skipFirst){
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
    	if(item.compare("")==0)
    		break;

    	if(skipFirst == false)
        	elems.push_back(item);
        skipFirst = false;
    }
}

void utility::IOAttr(int id,vector<int>& attr,ifstream& inf,int rowSize,string& strData){
	//get the id^th row in attrFileName
	int addr = id*rowSize;
	inf.seekg(addr);
	getline(inf,strData);

	split(strData,',',attr,true);
}

void utility::readVertexToSuperNodeMapping(const char* superNodeMappingFileName,vector<int>& S,vector<int>& partitionSize){
	printf("Read Vertex To SuperNode Mapping\n");

	FILE* inFile = fopen(superNodeMappingFileName,"r");

	int id,SN;
	while(fscanf(inFile,"%d\t%d\n",&id,&SN)!=EOF){
		S.push_back(SN);
	}

	fclose(inFile);

	for(int i=0; i<S.size(); i++)
		partitionSize[S[i]] = partitionSize[S[i]] + 1;
}

int utility::countIntDigit(int x){
	int length = 1;
	while ( x /= 10 )
	   length++;

	return length;
}

int utility::readTopology(const char* fileName,vector<vector<pair<int,int> > >& topology,int numSuperNode){
	printf("Read Topology\n");

	int numEdge = 0;

	string sFileName(fileName);
/*	if(sFileName.find("super-soc-pokec-relationships")!=string::npos)
		numEdge = readTopologyFormat1(fileName,topology,....);
	else if(sFileName.find("soc-pokec-relationships")!=string::npos)
		numEdge = readTopologyFormat1(fileName,topology,soc_pokec_vertex_size);
*/
/*	if(sFileName.find("super-graph_dblp")!=string::npos)
		numEdge = readTopologyFormat1(fileName,topology,....);
	else if(sFileName.find("graph_dblp")!=string::npos)
		numEdge = readTopologyFormat1(fileName,topology,dblp_vertex_size);
*/
	if(sFileName.find("PA_RoadNetwork")!=string::npos){
		if(sFileName.find("SuperGraph")!=string::npos)
			numEdge = readTopologyFormat1(fileName,topology,numSuperNode,'\t');
		else if(sFileName.find("roadNet-PA")!=string::npos)
			numEdge = readTopologyFormat1(fileName,topology,PA_road_network,'\t');
	}

	if(sFileName.find("FB_BFS1")!=string::npos){
		if(sFileName.find("SuperGraph")!=string::npos)
			numEdge = readTopologyFormat1(fileName,topology,numSuperNode,' ');
		else if(sFileName.find("bfs-1")!=string::npos)
			numEdge = readTopologyFormat1(fileName,topology,FB_BFS1,' ');
	}

	return numEdge;
}

int utility::scanTopologyFormat1(const char* fileName,vector<vector<pair<int,int> > >& topology,char split_char){
	string line;
//	char split_char='\t';
	int maxID=0;
	int minID=INT_MAX;
	int lineCount=0;

//	unordered_map<pair<int,int>,int,pairHash> edgeMap;

	ifstream inFile(fileName);
	while(getline(inFile,line)){
		istringstream s(line);
		vector<int> tokens;

		for(string each; getline(s,each,split_char); ){
			tokens.push_back(stoi(each));
			maxID=max(stoi(each),maxID);
			minID=min(stoi(each),minID);
		}

		lineCount++;
		if(lineCount%10000==0)
			printf("Reading line %d\n",lineCount);


		for(int i=1;i<tokens.size(); i++){
			int eID = 0;
		
//			pair<int,int> p = make_pair(min(tokens[0],tokens[i]),max(tokens[0],tokens[i]));
//			unordered_map<pair<int,int>,int,pairHash>::const_iterator got = edgeMap.find(p);

//			if(got == edgeMap.end()){
//				edgeMap.insert(make_pair(p,edgeIDCount));
//				eID = edgeIDCount;
//				edgeIDCount++;
//			}else{
//				eID = got->second;
//			}

			topology[tokens[0]].push_back(pair<int,int>(tokens[i],eID));
		}
//		printf("VID=%d topology.size()=%d\n",tokens[0]);
	}

	int edgeIDCount = assignEdgeID(topology);
	inFile.close();

	printf("min Vertex ID=%d max Vertex ID=%d lineCount=%d EdgeCount=%d\n",minID,maxID,lineCount,edgeIDCount);

	return edgeIDCount;
}

int utility::assignEdgeID(vector<vector<pair<int,int>> >& topology){
	int eID = 0;
	for(int i=0; i<topology.size(); i++){
		for(int j=0; j<topology[i].size(); j++){
			int adjVertex = topology[i][j].first;
			if(i < adjVertex){
				topology[i][j].second = eID;
				eID++;
			}else{
				for(int k=0; k<topology[adjVertex].size(); k++){
					if(topology[adjVertex][k].first == i){
						topology[i][j].second = topology[adjVertex][k].second;
						break;
					}
				}
			}
		}

		if(i%10000==0)
			printf("Reading line %d  edgeIDCount %d\n",i,eID);
	}

	return eID;
}

int  utility::readTopologyFormat1(const char* fileName,vector<vector<pair<int,int> > >& topology,int numVertex,char split_char){

//	scanTopologyFormat1(fileName,topology,true); //collect info only
	vector<pair<int,int> > adj;
	topology.assign(numVertex,adj);
	return scanTopologyFormat1(fileName,topology,split_char); //put tings into topology

}

void utility::readAttrHash(const char* fileName,vector<unsigned long long>& hashValues,bool isEdge){
	printf("Read Hash Value\n");

/*	char fileName[200];
	if(isEdge==true)
		sprintf(fileName,"%s/EdgeAttrHash.txt",attrFolderName);
	else
		sprintf(fileName,"%s/VertexAttrHash.txt",attrFolderName);
*/
	FILE* inFile = fopen(fileName,"r");

	int id = 0;
	unsigned long long hashValue = 0;
	int count = 0;
	while(fscanf(inFile,"%d %lld\n",&id,&hashValue)!=EOF){
		hashValues.push_back(hashValue);

		if(count%10000 == 0 && isEdge == true)
			printf("Reading Edge Hash Value %d\n",count);
		else if(count%10000 == 0 && isEdge == false)
			printf("Reading Vertex Hash Value %d\n",count);

		count++;
	}

	fclose(inFile);
}


