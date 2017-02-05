/*
 * IsomorphismFinder.h
 *
 *  Created on: 19 sty 2017
 *      Author: piotr
 */

#ifndef ISOMORPHISMFINDER_H_
#define ISOMORPHISMFINDER_H_

#include "Graf.h"

class IsomorphismFinder {
public:

	typedef struct
	{
		// nodes from first graph which have neighbours with the same degree sequence
		std::vector<int> firstGraph;
		// nodes from the second graph with the same sequence of neigh. degrees as above
		std::vector<int> secondGraph;
	}isomorphismGroup;

	IsomorphismFinder();
	IsomorphismFinder(int nodesCount);
	virtual ~IsomorphismFinder();

	bool createIsomorphismGroups(Graf & g1, Graf & g2);
	void fillNodesDegreeTable(Graf & g, std::vector<int> * nodesDegreeTable);
	void checkIsomorphism(Graf & g1, Graf & g2);
	void sortNodesDegreeTable(std::vector<int> * table, int size);
	void printNodesDegreeTable(std::vector<int> * table, int size);
	int findGraphsProjection(Graf & g1, Graf & g2);
	int computeIsomorphismCount();
	int fillIsomprphismTable(std::vector<int> * isomorphismTable, isomorphismGroup & group, int placesToDeploy);
	void fillUpRemainingPermutations(std::vector<int> * isomorphismTable, isomorphismGroup & group);
	void swap2Nodes(Graf & g1, int nodeNumber1, int nodeNumber2);
	void printIsomorphismTable(std::vector<int> * isomorphismTable, int nRows);
	void printProjection(const int index, const Graf & g);
private:
	isomorphismGroup * isomorphismGroupTable;		// table of nodesCount isomorphismGroup structures
	std::vector<int> * isomorphismTable;			// table of vectors, each column - another permutation
	int isomorphicGroupsCount;						// number of possible isomorphism groups
};

int factorial(int n);

#endif /* ISOMORPHISMFINDER_H_ */
