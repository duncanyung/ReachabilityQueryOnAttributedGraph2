#include "AttrGenerator.h"
#include "utility.h"

/*
int AttrGenerator::countIntDigit(int x){
	int length = 1;
	while ( x /= 10 )
	   length++;

	return length;
}
*/

void AttrGenerator::_generateAttribute(const char* attrFolderName,vector<vector<pair<int,int> > >& topology,int numAttr,int maxDomainSize,int rowSize,bool isEdge,int numEdge = 0){
	srand(time(NULL));

	char fileName[200],infoFileName[200];
	if(isEdge==true){
		sprintf(fileName,"%s/numEAttr=%dmaxDom=%dEdgeAttr.txt",attrFolderName,numAttr,maxDomainSize);
		sprintf(infoFileName,"%s/numEAttr=%dmaxDom=%dEdgeAttrInfo.txt",attrFolderName,numAttr,maxDomainSize);
	}else{
		sprintf(fileName,"%s/numVAttr=%dmaxDom=%dVertexAttr.txt",attrFolderName,numAttr,maxDomainSize);
		sprintf(infoFileName,"%s/numVAttr=%dmaxDom=%dVertexAttrInfo.txt",attrFolderName,numAttr,maxDomainSize);
	}

	FILE * outInfoFile = fopen(infoFileName,"w");
	FILE * outFile = fopen(fileName,"w");

	fprintf(outInfoFile,"Num_Attr %d\n",numAttr);


	vector<int> attrDomainSize;
	for(int i=0; i<numAttr; i++){
		int d = max(rand()%maxDomainSize,1);
		attrDomainSize.push_back(d);
//		attrDomainSize.push_back(maxDomainSize);
		fprintf(outInfoFile,"%d ",d);
//		fprintf(outInfoFile,"%d ",maxDomainSize);
	}
	fclose(outInfoFile);

	int numRow = 0;
	int numDigit1 = utility::countIntDigit(maxDomainSize);
	int numDigit2;
	if(isEdge == true) 
		numDigit2 = utility::countIntDigit(numEdge);
	else
		numDigit2 = utility::countIntDigit(topology.size());

	if(isEdge==true)
		numRow = numEdge;
	else
		numRow = topology.size();

	for(int i=0;i<=numRow;i++){
		string attr=to_string(i);
		attr.append(",");
		for(int j=0; j<numAttr; j++){
			int temp=rand()%attrDomainSize[j]+1;
//			printf("temp=%d\n",temp);
			attr.append(to_string(temp));
			if(j==numAttr-1)
				attr.append(",,");
			else
				attr.append(",");
		}

//		int extraSpace = numDigit2 + (numAttr*numDigit1+numAttr+1) - attr.length();
		int extraSpace = rowSize - attr.length() - 1;

		for(int j=0; j<extraSpace; j++)
			attr.append(" ");

		fprintf(outFile,"%s\n",attr.c_str());
		if(i%10000==0 && isEdge==false)
			printf("Generating Vertex Attr %d\n",i);
		else if (i%10000==0 && isEdge==true)
			printf("Generating Edge Attr %d\n",i);
	}

	fclose(outFile);
}


void AttrGenerator::generateAttribute(const char* attrFolderName,vector<vector<pair<int,int> > >& topology,int numVAttr,int numEAttr,int maxDomainSize,int numEdge,int vRowSize, int eRowSize){

	_generateAttribute(attrFolderName,topology,numVAttr,maxDomainSize,vRowSize,false);
	_generateAttribute(attrFolderName,topology,numEAttr,maxDomainSize,eRowSize,true,numEdge);

}
