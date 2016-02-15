#include "QueryHandler.h"
#include "utility.h"


int IOCount = 0;
int nodeVisited = 0;
double totalSSPTime = 0, totalBFS_CTime = 0;


pair<bool,pair<int,int> > QueryHandler::CReachabilityQuery(vector<vector<pair<int,int> > >& topology,vector<unsigned long long>& vertexHashValues,vector<unsigned long long>& edgeHashValues,query& q,const char* attrFolderName,int vRowSize,int eRowSize,bool useConstraint,bool hashOpt,vector<vector<pair<int,int> > >& stopology,vector<double>& vSynopsis,vector<double>& eSynopsis,vector<int>& S,const char* vSynopsisFileName,const char* eSynopsisFileName,int vSyRowSize,bool heuristic,vector<int>& partitionSize,int numVAttr,int numEAttr,int maxDom){

	IOCount = 0;
	nodeVisited = 0;

	queue<pair<int,int> > qu;
	vector<bool> visited;
	visited.assign(topology.size()+1,false);
	unordered_map<unsigned long long,bool> satTableE,satTableV;

	char vertexAttrFileName[200],edgeAttrFileName[200];
	sprintf(vertexAttrFileName,"%s/numVAttr=%dmaxDom=%dVertexAttr.txt",attrFolderName,numVAttr,maxDom);
	sprintf(edgeAttrFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttr.txt",attrFolderName,numEAttr,maxDom);
	ifstream infV(vertexAttrFileName);
	ifstream infE(edgeAttrFileName);

	clock_t start = clock();
//	q.src  = 553556;
//	q.dest = 780384;

//4.63 sec
//	q.src = 403070;
//	q.dest =695505;

	printf("For debug: src %d dest %d topology.size()=%ld\n",q.src,q.dest,topology.size());

	bool firstLoop = true;
	//no need to check constraint of src


	qu.push(make_pair(q.src,-1));//the second element of this pair is the super-edge to be ignored in super graph SP search
	while(!qu.empty()){
		pair<int,int> h = qu.front();
		qu.pop();

		if(visited[h.first]==true)
			continue;
		visited[h.first]=true;

		unordered_set<int> SSP;
		if(firstLoop == false){
			int SuperEIgnore = h.second;
			if(heuristic == true){
				clock_t start = clock();
				SuperGraphShortestPath(q,S[h.first],S[q.dest],stopology,vSynopsis,eSynopsis,SSP,SuperEIgnore,vSynopsisFileName,eSynopsisFileName,vSyRowSize,vertexAttrFileName,vRowSize,eRowSize);
				double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
				totalSSPTime = totalSSPTime + duration;
				printf("SSP Time=%f\n",duration);
			}
		}else{
			SSP.insert(S[h.first]);
		}
		firstLoop = false;
		clock_t start = clock();
		bool result = BFS_C(h.first,topology,vertexHashValues,edgeHashValues,q,qu,visited,satTableE,satTableV,vertexAttrFileName,edgeAttrFileName,infV,infE,vRowSize,eRowSize,useConstraint,hashOpt,SSP,S,heuristic,vSynopsis,partitionSize);
		double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		totalBFS_CTime = totalBFS_CTime + duration;
		printf("BFS_C Time=%f nodeVisited=%d\n",duration,nodeVisited);

		if(result == true){
//			printf("totalSSPTime=%f totalBFS_CTime=%f\n",totalSSPTime,totalBFS_CTime);
			return make_pair(true,make_pair(IOCount,nodeVisited));
		}
	}
	infV.close();
	infE.close();

//	printf("totalSSPTime=%f totalBFS_CTime=%f\n",totalSSPTime,totalBFS_CTime);
	return make_pair(false,make_pair(IOCount,nodeVisited));
}

void QueryHandler::computeAllSynopsis(query& q,vector<double>& vSynopsis,const char* vSynopsisFileName,int rowSize,const char* attrFileName,int attrRowSize,vector<vector<int> >& attrCon){
	for(int i=0; i<vSynopsis.size(); i++){
//		printf("i=%d vSynopsis.size()=%d\n",i,vSynopsis.size());
		computeSynopsis(q,vSynopsis,i,vSynopsisFileName,rowSize,attrFileName,attrRowSize,attrCon);
	}
}

void QueryHandler::computeSynopsis2(query& q,vector<double>& synopsis,int id,const char* synopsisFileName,int rowSize,
									const char* attrFileName,int attrRowSize,vector<vector<int> >& attrCon){
	//use fseek here!
	ifstream inf(synopsisFileName);

	//get the id^th row in attrFileName
	string strData;
	int addr = id*rowSize;
	inf.seekg(addr);
	getline(inf,strData);

	IOCount++;
	vector<string> samples;
	utility::splitToString(strData,';',samples,false);//true means to skip the first element.
	if(samples.size()==0)
		printf("+++%s\n",strData.c_str());

	inf.close();

	//compute synopsis value
	int satCount = 0;
	for(int i=0; i<samples.size(); i++){
		vector<int> attr;
		utility::split(samples[i],',',attr,true);

		if(CheckAttr(attr,attrCon))
			satCount++;
	}
//	printf("satCount=%d  samples.size()=%d\n",satCount,samples.size());
	synopsis[id] = (double)satCount/(double)samples.size();
}

void QueryHandler::computeSynopsis(query& q,vector<double>& synopsis,int id,const char* synopsisFileName,int rowSize,const char* attrFileName,int attrRowSize,vector<vector<int> >& attrCon){
	//use fseek here!
	ifstream inf(synopsisFileName);

	//get the id^th row in attrFileName
	string strData;
	int addr = id*rowSize;
	inf.seekg(addr);
	getline(inf,strData);

	IOCount++;
	vector<int> samples;
	utility::split(strData,',',samples,true);
	if(samples.size()==0)
		printf("%s\n",strData.c_str());

	inf.close();

	//compute synopsis value
	ifstream inf2(attrFileName);
	int satCount = 0;
	for(int i=0; i<samples.size(); i++){
		string strData2;
		int addr = i*attrRowSize;
		inf2.seekg(addr);
		getline(inf2,strData2);

		IOCount++;
		vector<int> attr;
		utility::split(strData2,',',attr,true);
		if(CheckAttr(attr,attrCon))
			satCount++;
	}
//	printf("satCount=%d  samples.size()=%d\n",satCount,samples.size());
	synopsis[id] = (double)satCount/(double)samples.size();
	inf2.close();
}

void QueryHandler::SuperGraphShortestPath(query& q,int src,int dest,vector<vector<pair<int,int> > >& stopology,vector<double>& vSynopsis,vector<double>& eSynopsis,unordered_set<int>& SSP,int SuperEIgnore,const char* vSynopsisFileName,const char* eSynopsisFileName,int vSyRowSize,const char* attrFileName,int vRowSize,int eRowSize){
	vector<int> parents;
	parents.assign(stopology.size(),-1);

	vector<bool> visited;
	visited.assign(stopology.size(),false);

	typedef	priority_queue<triple,vector<triple>,mycomparison> myPriorityQueue;
	myPriorityQueue qu;

	triple p; p.v=src; p.dist=0; p.parent=-1;
	qu.push(p);

	double totalDuration2 = 0;
//	printf("parents.size()=%d\n",parents.size());

	while(!qu.empty()){
		triple cur = qu.top();
		qu.pop();
		if(visited[cur.v] == true)
			continue;

//		printf("dist=%f\n",cur.dist);
		parents[cur.v] = cur.parent;
		visited[cur.v] = true;
		if(cur.v == dest)
			break;

//		printf("stopology[%d].size()=%d\n",cur.v,stopology[cur.v].size());
		for(int i=0; i<stopology[cur.v].size(); i++){
			int adjVertex = stopology[cur.v][i].first;
			int e = stopology[cur.v][i].second;
//			printf("adjVertex=%d\n",adjVertex);
			if(visited[adjVertex] == false && e!=SuperEIgnore){
				triple adj;
				adj.v = adjVertex;
//				printf("cur.v=%d adjVetex=%d\n",cur.v,adjVertex);
//				adj.dist = cur.dist*eSynopsis[e]*vSynopsis[adjVertex];
				if(vSynopsis[adjVertex]==-1){
//					printf("vSynopsis[%d]=%f\n",adjVertex,vSynopsis[adjVertex]);
					clock_t start = clock();
//					char attrFileName[200];
//					sprintf(attrFileName,"%s/VertexAttr.txt",attrFolderName);
					computeSynopsis2(q,vSynopsis,adjVertex,vSynopsisFileName,vSyRowSize,attrFileName,vRowSize,q.vertexAttrCon);
					double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
					totalDuration2 = totalDuration2 + duration;
//					printf("after compute vSynopsis[%d]=%f\n",adjVertex,vSynopsis[adjVertex]);
				}
//				printf("cur.dist=%f vSynopsis[adjVertex]=%f\n",cur.dist,vSynopsis[adjVertex]);
				adj.dist = cur.dist+(1-vSynopsis[adjVertex]);
				adj.parent = cur.v;
				qu.push(adj);
			}
		}
//		printf("After For\n");
	}
//	printf("SSP IO Time=%f\n",totalDuration2);

//	printf("super node src=%d dest=%d\n",src,dest);
	PathRecovery(parents,SSP,src,dest);
}

void QueryHandler::PathRecovery(vector<int>& parents,unordered_set<int>& SSP,int src,int dest){

	int cur = dest;
//	int count =0;
	SSP.insert(cur);
//	count++;
//	printf("%d<-",cur);
	while(cur != src){
		SSP.insert(parents[cur]);
		cur = parents[cur];
//		printf("%d<-",cur);
//		count++;
	}
//	printf("count=%d\n",count);
//	sleep(100000);
}

bool QueryHandler::BFS_C(int cur,vector<vector<pair<int,int> > >& topology,vector<unsigned long long>& vertexHashValues,vector<unsigned long long>& edgeHashValues,query& q,queue<pair<int,int> >& quGlobal,vector<bool>& visited,unordered_map<unsigned long long,bool>& satTableE,unordered_map<unsigned long long,bool>& satTableV,const char* vertexAttrFileName,const char* edgeAttrFileName,ifstream& infV,ifstream& infE,int vRowSize,int eRowSize,bool useConstraint,bool hashOpt,unordered_set<int>& SP,vector<int>& S,bool heuristic,vector<double>& vSynopsis,vector<int>& partitionSize){

	queue<pair<int,int> > qu;
	qu.push(make_pair(cur,-1));
	while(!qu.empty()){
		pair<int,int> h = qu.front();
		qu.pop();

		int cur = h.first;
		if(cur == q.dest)
			return true;

//		printf("cur=%d S[%d]=%d\n",cur,cur,S[cur]);

		for(int i=0; i<topology[cur].size();i++){
			int adjVertex = topology[cur][i].first;
//			printf("adjVertex=%d S[%d]=%d\n",adjVertex,adjVertex,S[adjVertex]);

			if(visited[adjVertex]==true)
				continue;

			nodeVisited++;

//			if(heuristic == true){
				bool withinSP = isWithinSSP(SP,S[adjVertex]);
				if(withinSP == true)
					visited[adjVertex] = true;
//			}

			//check Edge constraint
			int adjEdgeID = topology[cur][i].second;
			if(useConstraint && (!CheckConstraint(adjEdgeID,edgeHashValues,q.edgeAttrCon,satTableE,edgeAttrFileName,infE,eRowSize,hashOpt)))
				continue;
			//check Vertex constraint
			if(useConstraint && (!CheckConstraintWithSynopsisUpdate(adjVertex,vertexHashValues,q.vertexAttrCon,satTableV,vertexAttrFileName,infV,vRowSize,hashOpt,S,vSynopsis,partitionSize)))
				continue;

			//if the vertex is in the super path, do below
			if(!heuristic || withinSP){
				//SP is a unordered_set that store the super node in spuer path
				//S is a vector that store which super node this adjVertex belong to
				visited[adjVertex] = true;
				qu.push(make_pair(adjVertex,-1));
			}else{
				//put this vertex in the quGlobal
				quGlobal.push(make_pair(adjVertex,-1));
			}
		}
	}
	return false;
}

bool QueryHandler::isWithinSSP(unordered_set<int>& SP,int node){
	if(SP.find(node)==SP.end())
		return false;
	else
		return true;
}

void QueryHandler::updateSynopsis(vector<double>& synopsis,int id,int paritionSize){
	synopsis[id]=(synopsis[id]*paritionSize-1)/(double)paritionSize;
}

bool QueryHandler::CheckConstraint(int id, vector<unsigned long long>& hashValues,vector<vector<int> >& con,unordered_map<unsigned long long,bool>& satTable,const char* attrFileName,ifstream& inf,int rowSize,bool hashOpt){

	unordered_map<unsigned long long,bool>::const_iterator got = satTable.find(hashValues[id]);
	if(hashOpt && (got!=satTable.end())){
		return got->second; 
	}else{
		vector<int> attr;
		string attrData;
		utility::IOAttr(id,attr,inf,rowSize,attrData);
		IOCount++;

		if(CheckAttr(attr,con)){
			satTable.insert(make_pair(hashValues[id],true));
			return true;
		}else{
			satTable.insert(make_pair(hashValues[id],false));
			return false;
		}
	}
	return false;
}

bool QueryHandler::CheckConstraintWithSynopsisUpdate(int id, vector<unsigned long long>& hashValues,vector<vector<int> >& con,unordered_map<unsigned long long,bool>& satTable,const char* attrFileName,ifstream& inf,int rowSize,bool hashOpt,vector<int>& S,vector<double>& synopsis,vector<int>& partitionSize){

	unordered_map<unsigned long long,bool>::const_iterator got = satTable.find(hashValues[id]);
	if(hashOpt && (got!=satTable.end())){
		if(synopsis[S[id]]!=-1 && got->second==true)
			updateSynopsis(synopsis,S[id],partitionSize[S[id]]);
		return got->second; 
	}else{
		vector<int> attr;
		string attrData;
		utility::IOAttr(id,attr,inf,rowSize,attrData);
		IOCount++;

		if(CheckAttr(attr,con)){
			if(synopsis[S[id]]!=-1)
				updateSynopsis(synopsis,S[id],partitionSize[S[id]]);
			satTable.insert(make_pair(hashValues[id],true));
			return true;
		}else{
			satTable.insert(make_pair(hashValues[id],false));
			return false;
		}
	}
	return false;
}

bool QueryHandler::CheckAttr(vector<int>& attr,vector<vector<int> >& con){
	for(int i=0; i<attr.size(); i++){
		bool sat = false;
		for(int j=0; j<con[i].size(); j++){
			if(attr[i]==con[i][j]){
				sat=true;
				break;
			}
		}
		if(sat == false)
			return false;
	}
	return true;
}

bool QueryHandler::SimpleBFS(query& q,vector<vector<pair<int,int> > >& topology){
	//Really Not Reachable? Yes as 
	//In PA dataset:
	//47922 and 47923 is an island!!!!
	//946588 and 946589 is an island!!!
	//606555 and 606556 is an island!!!

	queue<pair<int,int> > qu;
	vector<bool> visited;
	visited.assign(topology.size()+1,false);

	qu.push(make_pair(q.src,-1));
	while(!qu.empty()){
		pair<int,int> h = qu.front();
		qu.pop();

		int cur = h.first;
		if(cur==q.dest)
			return true;
		for(int i=0; i<topology[h.first].size();i++){
			int adjVertex = topology[cur][i].first;
			if(visited[adjVertex]==true)
				continue;
			visited[adjVertex]=true;
			qu.push(make_pair(adjVertex,-1));
		}
	}

	return false;
}


