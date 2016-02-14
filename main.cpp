#include "AttrGenerator.h"
#include "ComputeHashValue.h"
#include "utility.h"
//#include "query.h"
//#include "QueryHandler.h"
#include "QueryGenerator.h"
#include "ConstructSuperGraph.h"

#include <ctime>

void Preprocessing(char const *argv[]){
	printf("Do Preprocessing\n");
	/*Pre-processing Functions*/
	int function = atoi(argv[1]);
	switch(function +'0'){
		case '1':{//attribute generator
			printf("Attribute Generator\n");

			//prase parameters
			const char* fileName = argv[2];
			const char* folderName = argv[3];
			int numVAttr = atoi(argv[4]);
			int numEAttr = atoi(argv[5]);
			int maxDomainSize = atoi(argv[6]);
			int vRowSize = atoi(argv[7]);
			int eRowSize = atoi(argv[8]);
			printf("%s %s %d %d %d %d %d\n",fileName,folderName,numVAttr,numEAttr,maxDomainSize,vRowSize,eRowSize);
			
			
			//read topology
			utility ut;
			vector<vector<pair<int,int> > > topology;
			int numEdge = ut.readTopology(fileName,topology);

			//generate attribute on the topology
			AttrGenerator genAttr;
			genAttr.generateAttribute(folderName,topology,numVAttr,numEAttr,maxDomainSize,numEdge,vRowSize,eRowSize);

			break;
		}case '2':{//pre-processing real data attributes
			printf("pre-processing real data attributes\n");
			
			//One vertex/edge attribute 1 file. Is that correct?
				//read attribute and put them into individual files
				//different data have different format

			break;
		}case '3':{//hash scheme index construction
			printf("hash scheme index construction\n");

			//prase parameters
			const char* folderName = argv[3];
			int numVAttr = atoi(argv[4]);
			int numEAttr = atoi(argv[5]);
			int maxDom = atoi(argv[6]);

			//Compute and store Hash Value to File
			ComputeHashValue ch;
			ch.computeHashValue(folderName,numVAttr,numEAttr,maxDom);

			break;
		}case '4':{//super-graph construction [not to use super graph first]
			printf("super-graph construction\n");

			const char* fileName = argv[2];
			const char* folderName = argv[3];
			int numVAttr = atoi(argv[4]);
			int numEAttr = atoi(argv[5]);
			int numSuperNode = atoi(argv[6]);
			int synopsisSize = atoi(argv[7]);
			int vRowSize = atoi(argv[8]);
			int maxDom = atoi(argv[9]);

			//read topology
			utility ut;
			vector<vector<pair<int,int> > > topology;
			int numEdge = ut.readTopology(fileName,topology);
			int numVertex = topology.size();

			ConstructSuperGraph csg;
			csg.construct(numSuperNode,numVertex,numVAttr,numEAttr,folderName,topology,synopsisSize,vRowSize,maxDom);

			break;
		}case '5':{
			printf("Generate Query\n");
			const char* fileName = argv[2];
			int numQuery = atoi(argv[3]);
			const char* folderName = argv[4];
			int numVAttr = atoi(argv[5]);
			int numEAttr = atoi(argv[6]);
			int numConstraint = atoi(argv[7]);
			int maxDom = atoi(argv[8]);

			utility ut;
			vector<vector<pair<int,int> > > topology;

//			string sFolderName(folderName);
//			if(sFolderName.find("FB_BFS1")!=string::npos)
//				graphSize = FB_BFS1;
//			else if(sFolderName.find("PA_RoadNetwork")!=string::npos)
//				graphSize = PA_road_network;

			int numEdge = ut.readTopology(fileName,topology);

			vector<query> queries;
			QueryGenerator qg;
			qg.generateQuery(queries,numQuery,folderName,topology,numConstraint,numVAttr,numEAttr,maxDom);
			qg.writeQueries(queries,folderName,numVAttr,numEAttr,maxDom);
			break;
		}default:{
			printf("No Such Function....\n");
		}
	}
}

void Query(char const *argv[]){
	printf("Do Query\n");
	/*Query Time Functions*/
	const char* fileName = argv[2];
	const char* folderName = argv[3];
	int numEAttr = atoi(argv[4]);
	int numVAttr = atoi(argv[5]);
	int vRowSize = atoi(argv[6]);
	int eRowSize = atoi(argv[7]);
	int numQuery = atoi(argv[8]);
	int useConstraint = atoi(argv[9]);
	int numSuperNode = atoi(argv[10]);
	int vSySize = atoi(argv[11]);
	int vSyRowSize = vRowSize*vSySize + vSySize;//sySize*(10+1+1)+1;
	int maxDom = atoi(argv[12]);
	const char* outFolderName = argv[13];
	int numConstraint = atoi(argv[14]);
	const char* variable = argv[15];

	char sFileName[200],vSynopsisFileName[200],eSynopsisFileName[200],superNodeMappingFileName[200],queryFileName[200];
	sprintf(sFileName,"%s/numVAttr=%dnumEAttr=%dmaxDom=%dnumSN=%dsySize=%dSuperGraph.txt",folderName,numVAttr,numEAttr,maxDom,numSuperNode,vSySize);
	sprintf(vSynopsisFileName,"%s/numVAttr=%dmaxDom=%dnumSN=%dsySize=%dvSynopsis.txt",folderName,numVAttr,maxDom,numSuperNode,vSySize);
//no use //sprintf(eSynopsisFileName,"%s/numEAttr=%dmaxDom=%dnumSN=%dsySize=%deSynopsis.txt",folderName,numEAttr,maxDom,numSuperNode,eSySize);	
	sprintf(superNodeMappingFileName,"%s/numVAttr=%dnumEAttr=%dmaxDom=%dnumSN=%dsySize=%dsToSNMap.txt",folderName,numVAttr,numEAttr,maxDom,numSuperNode,vSySize);
	sprintf(queryFileName,"%s/numVAttr=%dnumEAttr=%dmaxDom=%dquery.txt",folderName,numVAttr,numEAttr,maxDom);

	//read graph topology into memory
	utility ut;
	vector<vector<pair<int,int> > > topology,stopology;
	int numEdge = ut.readTopology(fileName,topology);
	int numSEdge = ut.readTopology(sFileName,stopology,numSuperNode+1);

	vector<double> vSynopsis,eSynopsis;

	vector<int> S;
	vector<int> partitionSize;
	partitionSize.assign(stopology.size(),0);
	ut.readVertexToSuperNodeMapping(superNodeMappingFileName,S,partitionSize);

	//read hash values into memory
	vector<unsigned long long> vertexHashValues,edgeHashValues;
	char vHashFileName[200],eHashFileName[200];
	sprintf(vHashFileName,"%s/numVAttr=%dmaxDom=%dVertexAttrHash.txt",folderName,numVAttr,maxDom);
	sprintf(eHashFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttrHash.txt",folderName,numEAttr,maxDom);

	ut.readAttrHash(vHashFileName,vertexHashValues,false);
	ut.readAttrHash(eHashFileName,edgeHashValues,true);

	//read query
	vector<query> queries;
	QueryGenerator qg;
	qg.readQueries(queryFileName,queries,numQuery,numVAttr,numEAttr);

	//query algorithm
	printf("topology size=%ld  vertexHashValues size=%ld edgeHashValue size=%ld\n\n",topology.size(),vertexHashValues.size(),edgeHashValues.size());
	QueryHandler qh;

	char queryTimeFileName[200], IOCountFileName[200],nodeVisitedCountFileName[200],notReachableCountFileName[200];
	sprintf(queryTimeFileName,"%s/change_%s_QueryTime.txt",outFolderName,variable);
	sprintf(IOCountFileName,"%s/change_%s_IOCount.txt",outFolderName,variable);
	sprintf(nodeVisitedCountFileName,"%s/change_%s_NodeVisitedCount.txt",outFolderName,variable);
	sprintf(notReachableCountFileName,"%s/change_%s_NotReachableCount.txt",outFolderName,variable);
	FILE* queryTimeFile = fopen(queryTimeFileName,"a");
	FILE* IOCountFile = fopen(IOCountFileName,"a");
	FILE* nodeVisitedCountFile = fopen(nodeVisitedCountFileName,"a");
	FILE* notReachableCountFile = fopen(notReachableCountFileName,"a");

	//3 solutions
	int hashOptList[3]   = {1,1,0};
	int heuristicList[3] = {1,0,0};
	for(int i=0; i<2; i++){
		int hashOpt = hashOptList[i];
		int heuristic = heuristicList[i];
		int notReachableCount = 0;
		int totalIO = 0, totalNodeVisited = 0;
		double maxTime = 0; 
		int maxIOCount = 0, maxNodeVisited = 0;
		double totalDuration = 0, duration;
		pair<bool,pair<int,int> > ans;

		for(int i=0; i<queries.size(); i++){
			vSynopsis.assign(stopology.size(),-1);//reset synopsis
			eSynopsis.assign(numSEdge,-1);
			
			printf("Query %d src=%d dest=%d\n",i+1,queries[i].src,queries[i].dest);

			//Start Timer HERE!
			clock_t start = clock();
			ans = qh.CReachabilityQuery(topology,vertexHashValues,edgeHashValues,queries[i],folderName,vRowSize,eRowSize,useConstraint,hashOpt,stopology,vSynopsis,eSynopsis,S,vSynopsisFileName,eSynopsisFileName,vSyRowSize,heuristic,partitionSize,numVAttr,numEAttr,maxDom);//S is the vertex to supernode mapping vector
			//End Timer HERE!
			duration = (clock() - start) / (double) CLOCKS_PER_SEC;
			totalIO = totalIO + ans.second.first;
			totalNodeVisited = totalNodeVisited + ans.second.second;

			totalDuration = totalDuration + duration;
			if(ans.first==false)
				notReachableCount++;
			if(maxTime < duration)
				maxTime = duration;
			if(maxIOCount < ans.second.first)
				maxIOCount = ans.second.first;
			if(maxNodeVisited < ans.second.second)
				maxNodeVisited = ans.second.second;

			printf("Reachable=%d Time=%f IOCount=%d NodeVisisted=%d\n",ans.first,duration,ans.second.first,ans.second.second);
		}

		double avgQueryTime   = totalDuration/(double)queries.size();
		double avgIOCount     = (double)totalIO/(double)queries.size();
		double avgNodeVisited = (double)totalNodeVisited/(double)queries.size();

		printf("\navg.query time=%fsec avg.IOCount=%f avg.NodeVisisted=%f\n",avgQueryTime,avgIOCount,avgNodeVisited);
		printf("maxTime=%f maxIOCount=%d maxNodeVisisted=%d\n",maxTime,maxIOCount,maxNodeVisited);
		printf("Num of Not Reachable=%d\n\n\n",notReachableCount);

		fprintf(queryTimeFile,"%f %f ",avgQueryTime,maxTime);
		fprintf(IOCountFile,"%f %d ",avgIOCount,maxIOCount);
		fprintf(nodeVisitedCountFile,"%f %d ",avgNodeVisited,maxNodeVisited);
		fprintf(notReachableCountFile,"%d ",notReachableCount);

		if(i == 1){
			fprintf(queryTimeFile,"\n");
			fprintf(IOCountFile,"\n");
			fprintf(nodeVisitedCountFile,"\n");
			fprintf(notReachableCountFile,"\n");
		}
	}

	fclose(queryTimeFile);
	fclose(IOCountFile);
	fclose(nodeVisitedCountFile);
	fclose(notReachableCountFile);
}

int main(int argc, char const *argv[]){
	
	printf("***Reachability Query on Attributed Graph***\n");
	const int function = atoi(argv[1]);

	if(function<6)
		Preprocessing(argv);
	else
		Query(argv);


	return 0;
}



