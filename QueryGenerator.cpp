#include "QueryGenerator.h"
#include "utility.h"
#include <time.h>

//srand(time(NULL));

int QueryGenerator::getAttributeInfo(char* attrInfoFileName, vector<int>& attrDomain){
	int numAttr = 0, temp = 0;
	char s[10];

	FILE* attrInfoFile = fopen(attrInfoFileName,"r");
	fscanf(attrInfoFile,"%s %d\n",s,&numAttr);

	for(int i=0; i<numAttr; i++){
		fscanf(attrInfoFile,"%d ",&temp);
		attrDomain.push_back(temp);
	}
	fclose(attrInfoFile);

	return numAttr;
}

void QueryGenerator::eraseConstraints(vector<vector<int> >& AttrCon,int numConstraintToErase){
//	srand(time(NULL));

	for(int i=0;i<numConstraintToErase; i++){
		int chooseAttr = rand()%AttrCon.size();
		int size = AttrCon[chooseAttr].size();
		if(size == 0){
			i--;
			continue;
		}
		int chooseConstraint = rand()%size;

		AttrCon[chooseAttr].erase(AttrCon[chooseAttr].begin()+chooseConstraint,AttrCon[chooseAttr].begin()+chooseConstraint+1);
	}
}

void QueryGenerator::generateQuery(vector<query>& queries,int numQuery,const char* folderName,vector<vector<pair<int,int> > >& topology,int numConstraintToErase,int numVAttr,int numEAttr,int maxDom){

	char vertexAttrFileName[200],edgeAttrFileName[200],vertexAttrInfoFileName[200],edgeAttrInfoFileName[200];
	sprintf(vertexAttrFileName,"%s/numVAttr=%dmaxDom=%dVertexAttr.txt",folderName,numVAttr,maxDom);
	sprintf(edgeAttrFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttr.txt",folderName,numEAttr,maxDom);
	sprintf(vertexAttrInfoFileName,"%s/numVAttr=%dmaxDom=%dVertexAttrInfo.txt",folderName,numVAttr,maxDom);
	sprintf(edgeAttrInfoFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttrInfo.txt",folderName,numEAttr,maxDom);

	vector<int> vertexAttrDomain;	vector<int> edgeAttrDomain;

	getAttributeInfo(vertexAttrInfoFileName,vertexAttrDomain);
	getAttributeInfo(edgeAttrInfoFileName,edgeAttrDomain);

	srand(time(NULL));
	
	for(int i=0; i<numQuery; i++){
		
		query q;
//		printf("topology.size() + 1=%d\n",topology.size() + 1);
		int r1 = rand(), r2 = rand();
	
		q.src = r1%topology.size() + 1;
		q.dest = r2%topology.size() + 1;

//		printf("r1=%d r2=%d src=%d dest=%d \n",r1,r2,q.src,q.dest);


		for(int j=0; j<numVAttr; j++){
			int numCon = vertexAttrDomain[j];//rand()%vertexAttrDomain[j] + 1;
//			int numCon = vertexAttrDomain[j] - numConstraint;
			unordered_set<int> existC;
			vector<int> con;
//			printf("DomainSize=%d\n",numCon);
//			printf("Constraint of vertex Attr %d: ",j+1);
			for(int k=0; k<numCon; k++){
				int c = rand()%vertexAttrDomain[j] + 1;
				if(existC.find(c)==existC.end()){
					existC.insert(c);
					con.push_back(c);
//					printf("%d ",c);
				}else
					k--;
			}
//			printf("\n");
			q.vertexAttrCon.push_back(con);
		}

		eraseConstraints(q.vertexAttrCon,numConstraintToErase);

		for(int j=0; j<numEAttr; j++){
			int numCon = edgeAttrDomain[j];//rand()%edgeAttrDomain[j] + 1;
//			int numCon = edgeAttrDomain[j] - numConstraint;
			unordered_set<int> existC;
			vector<int> con;
//			printf("DomainSize=%d\n",numCon);
//			printf("Constraint of edge Attr %d: ",j+1);
			for(int k=0; k<numCon; k++){
				int c = rand()%edgeAttrDomain[j] + 1;
				if(existC.find(c)==existC.end()){
					existC.insert(c);
					con.push_back(c);
//					printf("%d ",c);
				}else
					k--;
			}
//			printf("\n");
			q.edgeAttrCon.push_back(con);
		}

		if(i%10 == 0)
			printf("Generating Query %d\n",i);




		queries.push_back(q);
	}
}


void QueryGenerator::writeQueries(vector<query>& queries,const char* folderName,int numVAttr,int numEAttr,int maxDom){
	
	char queryFileName[200];
	sprintf(queryFileName,"%s/numVAttr=%dnumEAttr=%dmaxDom=%dquery.txt",folderName,numVAttr,numEAttr,maxDom);

	FILE* outFile = fopen(queryFileName,"w");

	for(int i=0; i<queries.size(); i++){
		fprintf(outFile,"%d,%d\n",queries[i].src,queries[i].dest);
		for(int j=0; j<queries[i].vertexAttrCon.size(); j++){
			for(int k=0; k<queries[i].vertexAttrCon[j].size(); k++){
				fprintf(outFile,"%d,",queries[i].vertexAttrCon[j][k]);
			}
			fprintf(outFile,"\n");
		}

		for(int j=0; j<queries[i].edgeAttrCon.size(); j++){
			for(int k=0; k<queries[i].edgeAttrCon[j].size(); k++){
				fprintf(outFile,"%d,",queries[i].edgeAttrCon[j][k]);
			}
			fprintf(outFile,"\n");
		}
	}

	fclose(outFile);
}

void QueryGenerator::readSrcDest(query& q,ifstream& inf){
	//read src and dest
	string strData;
	getline(inf,strData);
	vector<int> elems;
	utility::split(strData,',',elems,false);
	int src,dest;
	q.src = elems[0];
	q.dest = elems[1];
}

void QueryGenerator::readAttrCon(vector<vector<int> >& attrCon,ifstream& inf,int numAttr){

	for(int i=0; i<numAttr; i++){
		string strData;
		getline(inf,strData);
		vector<int> elems;
		utility::split(strData,',',elems,false);
		attrCon.push_back(elems);
	}
}

void QueryGenerator::readQueries(const char* queryFileName,vector<query>& queries,int numQueries,int numVertexAttr,int numEdgeAttr){
	printf("Read Query\n");

	ifstream inf(queryFileName);

	for(int i=0; i<numQueries; i++){
		query q;
		readSrcDest(q,inf);
		readAttrCon(q.vertexAttrCon,inf,numVertexAttr);
		readAttrCon(q.edgeAttrCon,inf,numEdgeAttr);
		queries.push_back(q);
	}

	inf.close();
}


