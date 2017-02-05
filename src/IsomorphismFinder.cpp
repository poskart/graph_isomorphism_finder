/*
 * IsomorphismFinder.cpp
 *
 *  Created on: 19 sty 2017
 *      Author: piotr
 */

#include "IsomorphismFinder.h"

IsomorphismFinder::IsomorphismFinder()
{
	isomorphicGroupsCount = 0;
	isomorphismGroupTable = nullptr;
	isomorphismTable = nullptr;
}

IsomorphismFinder::IsomorphismFinder(int nodesCount)
{
	isomorphicGroupsCount = 0;
	isomorphismGroupTable = new isomorphismGroup[nodesCount];
	isomorphismTable = nullptr;
}

IsomorphismFinder::~IsomorphismFinder()
{
	delete [] isomorphismGroupTable;
	delete [] isomorphismTable;
}

/*
 * This functions creates isomorphism groups. Isomorphism group
 * contains several vertexes from the first graph (g1) and the
 * same amount of vertexes from the second graph (g2) which can
 * be swapped one another (have the same neighbour nodes degree sequence).
 */
bool IsomorphismFinder::createIsomorphismGroups(Graf & g1, Graf & g2)
{
	std::vector<int> * nodesDegreeTable1, *nodesDegreeTable2;
	nodesDegreeTable1 = new std::vector<int>[g1.nodesCount];
	nodesDegreeTable2 = new std::vector<int>[g2.nodesCount];
	fillNodesDegreeTable(g1, nodesDegreeTable1);
	fillNodesDegreeTable(g2, nodesDegreeTable2);

	sortNodesDegreeTable(nodesDegreeTable1, g1.nodesCount);
	sortNodesDegreeTable(nodesDegreeTable2, g2.nodesCount);

	// arrays which says if node has been already assigned to the group
	bool nodeAssignedFlag1[g1.nodesCount];
	bool nodeAssignedFlag2[g2.nodesCount];
	for(int i = 0; i < g1.nodesCount; i++)
	{
		nodeAssignedFlag1[i] = false;
		nodeAssignedFlag2[i] = false;
	}

	// fill isomorphism groups of the first graph
	int groupCounter = 0;
	for(int i = 0; i < g1.nodesCount; i++)
	{
		if(nodeAssignedFlag1[i] == false)
		{
			isomorphismGroupTable[groupCounter].firstGraph.push_back(i);
			nodeAssignedFlag1[i] = true;
			for(int j = i; j < g1.nodesCount; j++)
			{
				if(nodeAssignedFlag1[j] == false && nodesDegreeTable1[j] == nodesDegreeTable1[i])
				{
					isomorphismGroupTable[groupCounter].firstGraph.push_back(j);
					nodeAssignedFlag1[j] = true;
				}
			}
			groupCounter++;
		}
	}

	// fill isomorphism groups of the second graph and try to match it to the first graph isomorphism groups
	int tmpNode;
	for(int i = 0; i < groupCounter; i++)
	{
		tmpNode = isomorphismGroupTable[i].firstGraph.front();	// take first node in the i-th group
		std::vector<int> currentNodesDegreeVectToMatch = nodesDegreeTable1[tmpNode];
		for(int j = 0; j < g2.nodesCount; j++)
		{
			if(nodeAssignedFlag2[j] == false && nodesDegreeTable2[j] == currentNodesDegreeVectToMatch)
			{
				isomorphismGroupTable[i].secondGraph.push_back(j);
				nodeAssignedFlag2[j] = true;
			}
		}
		if(isomorphismGroupTable[i].firstGraph.size() != isomorphismGroupTable[i].secondGraph.size())
			return false;
	}
	isomorphicGroupsCount = groupCounter;
	return true;
}

/*
 * Fill table with degree of neighbours for all vertexes in g graph.
 * Nodes degree table is similar to graph structure, but with neighbour's
 * degrees instead of the neighbour's numbers.
 */
void IsomorphismFinder::fillNodesDegreeTable(Graf & g, std::vector<int> * nodesDegreeTable)
{
	int currentDegree, currentNode;

	for(int i = 0; i < g.nodesCount; i++)
	{
		for(int j = 0; j < (int)g.graphTable[i].size(); j++)
		{
			currentNode = g.graphTable[i][j];
			currentDegree = g.graphTable[currentNode].size();
			nodesDegreeTable[i].push_back(currentDegree);
		}
	}
}

/*
 * Check if g1 is isomorphic with g2, print result. If g1 & g2 are
 * isomorphic then print the projection of g1 vertexes into g2 vertexes.
 */
void IsomorphismFinder::checkIsomorphism(Graf & g1, Graf & g2)
{
	bool isomorphic = false;
	int index;

	// check if g1 & g2 are isomorphic
	if(g1.getVertexesCount() == g2.getVertexesCount() && g1.getEdgesCount() == g2.getEdgesCount())
		if(	createIsomorphismGroups(g1, g2) == true)
		{
			isomorphic = true;
			index = findGraphsProjection(g1, g2);
		}
	if(!isomorphic)
		std::cout<<"Nieizomorficzne"<<std::endl;
	else
	{
		std::cout<<"Izomorficzne"<<std::endl;
		printProjection(index, g1);
	}
}

/*
 * Sort nodesDegreeTable in non-decreasing order so that whole vectors can be compared.
 */
inline void IsomorphismFinder::sortNodesDegreeTable(std::vector<int> * table, int size)
{
	for(int i = 0; i < size; i++)
		std::sort(table[i].begin(), table[i].end());
}

/*
 * Print table of nodes degree.
 */
void IsomorphismFinder::printNodesDegreeTable(std::vector<int> * table, int size)
{
	std::cout<<"Tablica stopni:"<<std::endl;

	for(int i = 0; i < size; i++)
	{
		std::vector<int>::iterator iter;
		for(iter = table[i].begin(); iter != table[i].end(); iter++)
		{
			std::cout<<(*iter)<<", ";
		}
		std::cout<<std::endl;
	}
}

/*
 * Find g1 transformation into g2. Return index of the valid permutation
 * which makes this transformation.
 */
int IsomorphismFinder::findGraphsProjection(Graf & g1, Graf & g2)
{
	// isomorphism table: nodesCount x permutations count
	isomorphismTable  = new std::vector<int>[g1.nodesCount];
	int tmp = computeIsomorphismCount();

	// allocate vector spaces for all isomorphism permutations
	for(int i = 0; i < g1.nodesCount; i++)
		isomorphismTable[i].reserve(tmp);
	// fill isomorphism table separately for each group (permute each group)
	for(int i = 0; i < isomorphicGroupsCount; i++)
		tmp = fillIsomprphismTable(isomorphismTable, isomorphismGroupTable[i], tmp);

	Graf tmpGraph(g1);
	int i, node2Swap1, node2Swap2;
	char ifSwappedTable[tmpGraph.nodesCount];
	memset(ifSwappedTable, 0, tmpGraph.nodesCount);
	tmp = computeIsomorphismCount();
	// take next permutation, swap nodes which differ from previous permutation and check graph equality
	for(i = 0; i < tmp; i++)
	{
		// if i = 0, compare with subsequent nodes 0 - nodesCount
		if(i == 0)
		{
			for(int j = 0; j < tmpGraph.nodesCount; j++)
			{
				if(isomorphismTable[j].at(i) != j && !ifSwappedTable[j])
				{
					node2Swap1 = j;
					node2Swap2 = isomorphismTable[j].at(i);
					swap2Nodes(tmpGraph, node2Swap1, node2Swap2);
					ifSwappedTable[node2Swap1] = 1;
					ifSwappedTable[node2Swap2] = 1;
				}
			}
		}
		// if i > 0, compare to previous permutation
		else
		{
			for(int j = 0; j < tmpGraph.nodesCount; j++)
			{
				if(isomorphismTable[j].at(i) != isomorphismTable[j].at(i - 1) && !ifSwappedTable[isomorphismTable[j].at(i)])
				{
					node2Swap1 = isomorphismTable[j].at(i - 1);
					node2Swap2 = isomorphismTable[j].at(i);
					swap2Nodes(tmpGraph, node2Swap1, node2Swap2);
					ifSwappedTable[node2Swap1] = 1;
					ifSwappedTable[node2Swap2] = 1;
				}
			}
		}
		// sort graph so that it could be compared by ==
		tmpGraph.sortGraph();
		memset(ifSwappedTable, 0, tmpGraph.nodesCount);
		if(tmpGraph == g2)
			break;
	}
	return i;
}

int IsomorphismFinder::computeIsomorphismCount()
{
	int count = 1;
	for(int i = 0; i < isomorphicGroupsCount; i++)
		count *= factorial(isomorphismGroupTable[i].firstGraph.size());
	return count;
}

int factorial(int n)
{
	return (n == 1 || n == 0)? 1 : n * factorial(n -1);
}

/*
 * Fill table with all possible permutations of g1 vertexes into g2 vertexes,
 * based on isomorphism groups created before.
 */
int IsomorphismFinder::fillIsomprphismTable(std::vector<int> * isomorphismTable, isomorphismGroup & group, int placesToDeploy)
{
	int nodesInGroup = group.firstGraph.size();
	int permutationsCountForCurrentGroup = factorial(nodesInGroup);
	int permutationMultiple = placesToDeploy/permutationsCountForCurrentGroup;
	std::vector<int> permutations = group.secondGraph;

	std::vector<int>::iterator it;
	int counter;
	// deploy all possible permutations to isomorphismTable for nodes given in current group
	do
	{
		counter = 0;
		for(it = group.firstGraph.begin(); it != group.firstGraph.end(); it++)
		{
			for(int i = 0; i < permutationMultiple; i++)
			{
				isomorphismTable[*it].push_back(permutations.at(counter));
			}
			counter++;
		}
	} while ( std::next_permutation(permutations.begin(),permutations.end()) );

	// fill remaining isomorphismTable space by multiplying created permutations
	if(placesToDeploy < computeIsomorphismCount())
		fillUpRemainingPermutations(isomorphismTable, group);

	// return minimum modulo divisor value
	return permutationMultiple;
}

/*
 * Repeats permutations for the given group if different permutations for this group were
 * deployed before, did not fill the entire isomorphism table.
 */
void IsomorphismFinder::fillUpRemainingPermutations(std::vector<int> * isomorphismTable, isomorphismGroup & group)
{
	std::vector<int>::iterator tmpIter1, tmpIter2;
	std::vector<int> vec;
	for(tmpIter1 = group.firstGraph.begin(); tmpIter1 != group.firstGraph.end(); tmpIter1++)
	{
		tmpIter2 = isomorphismTable[*tmpIter1].begin();
		while((int)isomorphismTable[*tmpIter1].size() < computeIsomorphismCount())
		{
			isomorphismTable[*tmpIter1].push_back(*tmpIter2);
			tmpIter2++;
		}
	}
}

/*
 * Swap two nodes in the graph. First - swap two vectors corresponding to these nodes,
 * second - swap all these nodes in the all node's vectors
 */
void IsomorphismFinder::swap2Nodes(Graf & g, int nodeNumber1, int nodeNumber2)
{
	std::vector<int> tmpVec(g.graphTable[nodeNumber1]);
	g.graphTable[nodeNumber1] = g.graphTable[nodeNumber2];
	g.graphTable[nodeNumber2] = tmpVec;
	// swap all nodes in vectors
	std::vector<int>::iterator iter;
	for(int i = 0; i < g.nodesCount; i++)
	{
		for(iter = g.graphTable[i].begin(); iter != g.graphTable[i].end(); iter++)
		{
			if(*iter == nodeNumber1)
				*iter = nodeNumber2;
			else if(*iter == nodeNumber2)
				*iter = nodeNumber1;
		}
	}
}

/*
 * Print isomorphism table (debugging).
 */
void IsomorphismFinder::printIsomorphismTable(std::vector<int> * isomorphismTable, int nRows)
{
	std::cout<<"Tablica izomorfizmow: "<<std::endl;
	int tmpNode;
	for(tmpNode = 0; tmpNode < nRows; tmpNode++)
	{
		std::vector<int>::iterator it = isomorphismTable[tmpNode].begin();
		for(; it != isomorphismTable[tmpNode].end(); it++)
		{
			std::cout<<*it<<", ";
		}
		std::cout<<std::endl;
	}
}

/*
 * Print g1 projection into g2 based on given index in isomorphism table.
 */
void IsomorphismFinder::printProjection(const int index, const Graf & g)
{
	for(int i = 0; i < g.nodesCount; i++)
		std::cout<<i<<" -> "<<isomorphismTable[i].at(index)<<std::endl;
}
