#include "ConstructSuperGraph.h"
#include "utility.h"



void ConstructSuperGraph::construct(int numSuperNode,int numVertex,int numVAttr,int numEAttr,const char* folderName,vector<vector<pair<int,int> > >& topology,int synopsisSize,int vAttrRowSize,int maxDom){

	bool isEdge = false;
	char attrFileName[200],vSynopsisFileName[200],eSynopsisFileName[200],sFileName[200],vToSNMapFileName[200];
	sprintf(vToSNMapFileName,"%s/numVAttr=%dnumEAttr=%dmaxDom=%dnumSN=%dsySize=%dsToSNMap.txt",folderName,numVAttr,numEAttr,maxDom,numSuperNode,synopsisSize);
	sprintf(sFileName,"%s/numVAttr=%dnumEAttr=%dmaxDom=%dnumSN=%dsySize=%dSuperGraph.txt",folderName,numVAttr,numEAttr,maxDom,numSuperNode,synopsisSize);
	sprintf(attrFileName,"%s/numVAttr=%dmaxDom=%dVertexAttr.txt",folderName,numVAttr,maxDom);
	sprintf(vSynopsisFileName,"%s/numVAttr=%dmaxDom=%dnumSN=%dsySize=%dvSynopsis.txt",folderName,numVAttr,maxDom,numSuperNode,synopsisSize);
	sprintf(eSynopsisFileName,"%s/numEAttr=%dmaxDom=%dnumSN=%dsySize=%deSynopsis.txt",folderName,numEAttr,maxDom,numSuperNode,synopsisSize);

	vector<int> S;//to store vertex to super node mapping
	S.assign(numVertex,-1);

	//1. GraphClustering
		//partition vertices into k clusters based on vertex and edge attribute similarity
	numSuperNode = clustering(folderName,S,topology,numSuperNode);//has to be O(n+m) or O((n+m)log(n+m)) I/O

	FILE* outFile = fopen(vToSNMapFileName,"w");

	for(int i=0; i<S.size(); i++)
		fprintf(outFile,"%d\t%d\n",i,S[i]);

	fclose(outFile);

	//2. build and save super graph
		//assign edge into super Edge


	buildSuperGraph(sFileName,S,topology,numSuperNode);

	//3. build synopsis for super node
		//draw sample and compute stat (a tree? do linear scan to compute stat first) from sample
	buildSynopsis2(vSynopsisFileName,S,isEdge,numSuperNode,synopsisSize,attrFileName,vAttrRowSize);

	//4. build synopsis for super edge
		//draw sample and compute stat (a tree? do linear scan to compute stat first) from sample
	isEdge = true;
//	buildSynopsis(eSynopsisFileName,S,isEdge,numSuperNode,synopsisSize);

}

void ConstructSuperGraph::buildSynopsis(const char* synopsisFileName,vector<int>& S,bool isEdge,int numSuperNode,int synopsisSize){
	srand(time(NULL));

	vector<vector<int> > superNodes;
	vector<int> superNode;
	superNodes.assign(numSuperNode,superNode);

	//put vertex ID in to corresponding supernode
	for(int i=0; i<S.size(); i++)
		superNodes[S[i]].push_back(i);

	FILE* outFile = fopen(synopsisFileName,"w");

	for(int i=0; i<numSuperNode; i++){
		string row="";
		row.append(to_string(i+1));
		row.append(",");
		for(int j=0; j<synopsisSize; j++){
			int s = rand()%superNodes[i].size();
			row.append(to_string(superNodes[i][s]));
			row.append(",");
		}
		row.append(",");
//		printf("Row length=%d row.length=%d\n",synopsisSize*(10+1+1)+1,row.length());
		int extraSpace = (synopsisSize+1)*10 + synopsisSize + 1 + 1 - row.size();
//		synopsisSize*(10+1+1)+1 - row.length();//numDigit;//assume maximum 10 digits per sample id
		for(int j=0; j<extraSpace; j++)
			row.append(" ");

		fprintf(outFile,"%s\n",row.c_str());
	}
	fclose(outFile);
}

void ConstructSuperGraph::buildSynopsis2(const char* synopsisFileName,vector<int>& S,bool isEdge,int numSuperNode,int synopsisSize,
										const char* attrFileName,int attrRowSize){
	srand(time(NULL));

	vector<vector<int> > superNodes;
	vector<int> superNode;
	superNodes.assign(numSuperNode,superNode);

	//put vertex ID in to corresponding supernode
	for(int i=0; i<S.size(); i++)
		superNodes[S[i]].push_back(i);

	FILE* outFile = fopen(synopsisFileName,"w");
	ifstream inf(attrFileName);

	for(int i=0; i<numSuperNode; i++){
		string row="";

		for(int j=0; j<synopsisSize; j++){
			int s = rand()%superNodes[i].size();
			vector<int> attr;
			string attrData;
			utility::IOAttr(superNodes[i][s],attr,inf,attrRowSize,attrData);

//			printf("superNodes[i][s]=%d length=%d %s\n",superNodes[i][s],attrData.length(),attrData.c_str());
			row.append(attrData);
			row.append(";");
		}
		row.append(",");
//		printf("Row length=%d row.length=%d\n",synopsisSize*(10+1+1)+1,row.length());
		int extraSpace = (synopsisSize+1)*attrRowSize + synopsisSize + 1 + 1 - row.size();
//		synopsisSize*(10+1+1)+1 - row.length();//numDigit;//assume maximum 10 digits per sample id
		for(int j=0; j<extraSpace; j++)
			row.append(" ");

//		printf("%s\n",row.c_str());
		fprintf(outFile,"%s\n",row.c_str());
	}
	fclose(outFile);
	inf.close();
}

void ConstructSuperGraph::buildSuperGraph(const char* sFileName,vector<int>& S,vector<vector<pair<int,int> > >& topology,int numSuperNode){

	vector<unordered_set<int> > stopology;
	unordered_set<int> adjList;
	stopology.assign(numSuperNode,adjList);

	for(int i=0; i<topology.size(); i++){
		for(int j=0; j<topology[i].size(); j++){
			int adjVertex = topology[i][j].first;

//			printf("S[%d]=%d stopology.size()=%d\n",i,S[i],stopology.size());

			if(stopology[S[i]].find(S[adjVertex]) == stopology[S[i]].end()){
				//insert

				if(S[adjVertex] >= stopology.size()){
					printf("S[%d]=%d stopology.size()=%d\n",adjVertex,S[adjVertex],stopology.size());
					sleep(100000);
				}
				stopology[S[i]].insert(S[adjVertex]);
			}
		}
	}

	FILE* outFile = fopen(sFileName,"w");

	for(int i=0; i<stopology.size(); i++){
		for(unordered_set<int>::iterator itr = stopology[i].begin(); itr != stopology[i].end(); itr++){
			fprintf(outFile,"%d\t%d\n",i,(*itr));
		}
	}

	fclose(outFile);
}

int ConstructSuperGraph::clustering(const char* attrFolderName,vector<int>& S,vector<vector<pair<int,int> > >& topology,int numSuperNode){
	srand(time(NULL));

	//just try a simple version of clustering //make another one later
	int partitionSize = S.size()/(numSuperNode-1);

	vector<bool> visited;
	visited.assign(topology.size(),false);

	int count = 0, p = -1;
	for(int i=0; i<numSuperNode; i++){
		int seed=0;

		do{
			seed = rand()%topology.size();
		}while(visited[seed] == true);

		queue<int> qu;
		qu.push(seed);

		p++;
		count = 0;
		while(!qu.empty()){
			int cur = qu.front();
			qu.pop();

			if(visited[cur] == true)
				continue;
			visited[cur] = true;

			S[cur] = p;
			count++;
			if(count == partitionSize)
				break;

			for(int i=0; i<topology[cur].size(); i++){
				int adjVertex = topology[cur][i].first;
				qu.push(adjVertex);
			}
		}
	}
	printf("num of partition=%d\n",p+1);

	p++;
	for(int i=0; i<topology.size(); i++){
		if(S[i] == -1){
			S[i] = p;
		}
	}

	//make every island to be a partition
/*	for(int i=0; i<topology.size(); i++){
		if(S[i] == -1){
			//do a BFS to make a new parition
			queue<int> qu;
			qu.push(i);

			p++;//make every island into a partition so p++ here.
			count = 0;
			while(!qu.empty()){
				int cur = qu.front();
				qu.pop();

				if(visited[cur] == true)
						continue;
				visited[cur] = true;

				S[cur] = p;
				count++;
				if(count == partitionSize)
					break;

				for(int i=0; i<topology[cur].size(); i++){
					int adjVertex = topology[cur][i].first;
					qu.push(adjVertex);
				}
			}
		}
	}*/
	printf("num of partition + island=%d\n",p+1);

	return p+1;//have to +1 here as count from 0 to p has p+1 entries.
/*
	int count = 0, p = 0;
	for(int i=0; i<S.size(); i++){
		S[i] = p;
		count++;

		if(count == partitionSize){
			p++;
			count = 0;
		}
	}*/
}






