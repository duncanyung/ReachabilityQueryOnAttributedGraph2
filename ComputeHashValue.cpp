#include "ComputeHashValue.h"
#include "utility.h"

void ComputeHashValue::_computeHashValue(char* attrHashFileName, char* attrFileName,char* attrInfoFileName, bool isNode){
	FILE * attrFile = fopen(attrFileName,"r");
	FILE * attrInfoFile = fopen(attrInfoFileName,"r");
	FILE * hashFile = fopen(attrHashFileName,"w");
	char s[10];
	int numAttr = 0,ID = 0, ID2 = 0,count = 0;
	fscanf(attrInfoFile,"%s %d\n",s,&numAttr);
	fclose(attrInfoFile);

	utility ut;
	const unsigned long long _prime = ut.prime;
	while(fscanf(attrFile,"%d,",&ID)!=EOF){

		unsigned long long hashValue = 0;
		unsigned long long c = ut.constant;
		for(int i=0; i<numAttr; i++){
			int temp=0;
			fscanf(attrFile,"%d,",&temp);
			hashValue = hashValue + temp*c;
			c = c*c;
		}
		fscanf(attrFile,",\n");
		hashValue=hashValue%_prime;
		fprintf(hashFile,"%d %lld\n",ID,hashValue);//assume count all = 1 first!!!

		count++;
		if(count%10000==0 && isNode==true)
			printf("Generating Hash for Vertex %d\n",count);
		else if(count%10000==0 && isNode==false)
			printf("Generating Hash for Edge %d\n",count);
	}

	fclose(attrFile);
	fclose(hashFile);
}

//O(nlog n) CPU, O(n) I/O [Not Yet Implemented]
//void ComputeHashValue::computeHashCount(){
	//1. sort the (hashValue,ID) pair based on hashValue.
	//2. I/O attribute with the same hashValue based on ID
	//3. compute the count
//}

void ComputeHashValue::computeHashValue(const char* folderName,int numVAttr,int numEAttr,int maxDom){

	char vertexAttrFileName[200],edgeAttrFileName[200],vertexAttrHashFileName[200],edgeAttrHashFileName[200],
			vertexAttrInfoFileName[200],edgeAttrInfoFileName[200];
	sprintf(vertexAttrHashFileName,"%s/numVAttr=%dmaxDom=%dVertexAttrHash.txt",folderName,numVAttr,maxDom);
	sprintf(edgeAttrHashFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttrHash.txt",folderName,numEAttr,maxDom);
	sprintf(vertexAttrFileName,"%s/numVAttr=%dmaxDom=%dVertexAttr.txt",folderName,numVAttr,maxDom);
	sprintf(edgeAttrFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttr.txt",folderName,numEAttr,maxDom);
	sprintf(vertexAttrInfoFileName,"%s/numVAttr=%dmaxDom=%dVertexAttrInfo.txt",folderName,numVAttr,maxDom);
	sprintf(edgeAttrInfoFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttrInfo.txt",folderName,numEAttr,maxDom);


	_computeHashValue(vertexAttrHashFileName,vertexAttrFileName,vertexAttrInfoFileName,true);
	_computeHashValue(edgeAttrHashFileName,edgeAttrFileName,edgeAttrInfoFileName,false);
}


