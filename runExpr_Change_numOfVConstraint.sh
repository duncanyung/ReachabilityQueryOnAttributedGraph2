#!/bin/bash

make main

echo start_Run_Expr_Script


	function=1
	fileName='/home/skyline/Documents/Duncan/ReachabilityQueryOnAttributedGraph_Data/PA_RoadNetwork/roadNet-PA.txt'
	folderName='/home/skyline/Documents/Duncan/ReachabilityQueryOnAttributedGraph_Data/PA_RoadNetwork'
	outFolderName='/home/skyline/Documents/Duncan/ReachabilityQueryOnAttributedGraph_Data/PA_RoadNetwork/ExprResult'

	fileName='/home/skyline/Documents/Duncan/ReachabilityQueryOnAttributedGraph_Data/FB_BFS1/roadNet-PA.txt'
	folderName='/home/skyline/Documents/Duncan/ReachabilityQueryOnAttributedGraph_Data/FB_BFS1'
	outFolderName='/home/skyline/Documents/Duncan/ReachabilityQueryOnAttributedGraph_Data/FB_BFS1/ExprResult'

	numEAttr=5
	numVAttr=5
	maxDomainSize=20
	vRowSize=30
	eRowSize=30
	numQuery=100
	useConstraint=1
	numSuperNode=100
	synopsisSize=20

	variable='VConstraint'

	#generate Attribute
	./main 1 $fileName $folderName $numVAttr $numEAttr $maxDomainSize $vRowSize $eRowSize

	#generate hash value
	./main 3 $fileName $folderName $numVAttr $numEAttr $maxDomainSize

	#construct super graph and synopsis
	./main 4 $fileName $folderName $numVAttr $numEAttr $numSuperNode $synopsisSize $vRowSize $maxDomainSize

###########################################################################################################################################33

	numConstraint=0
	#generate Queries
	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

	numConstraint=1
	#generate Queries
	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

	numConstraint=2
	#generate Queries
	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

	numConstraint=5
	#generate Queries
	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

	numConstraint=10
	#generate Queries
	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

	numConstraint=20
	#generate Queries
	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

#	numConstraint=50
	#generate Queries
#	./main 5 $fileName $numQuery $folderName $numVAttr $numEAttr $numConstraint $maxDomainSize
	#query algorithm
#	./main 6 $fileName $folderName $numEAttr $numVAttr $vRowSize $eRowSize $numQuery $useConstraint $numSuperNode $synopsisSize $maxDomainSize $outFolderName $numConstraint $variable

#	COUNT=10000
#	while [ $COUNT -le 100000 ]; do
#	FN=$COUNT'_quick_server_test_with_pruning'
#	./qmain -query_data $QUERY_DATA -show_comment $SC -alg $alg -r $COUNT -testing $testing -client_update $CLIENT_UPDATE > $FN
#	echo $COUNT
#	let COUNT=COUNT+10000
#	done

