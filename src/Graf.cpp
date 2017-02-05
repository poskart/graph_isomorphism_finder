/*
 * Graf.cpp
 *
 *  Created on: 7 sty 2017
 *      Author: piotr
 */

#include "Graf.h"

Graf::Graf()
{
	nodesCount = 0;
	graphTable = nullptr;
}

Graf::~Graf()
{
	delete [] graphTable;
}

Graf::Graf(const Graf & g)
{
	nodesCount = g.nodesCount;
	graphTable = new std::vector<int>[nodesCount];
	for(int i = 0; i < nodesCount; i++)
		graphTable[i] = g.graphTable[i];
}

Graf & Graf::operator=(const Graf & g)
{
	if(graphTable != nullptr)
		delete [] graphTable;

	nodesCount = g.nodesCount;
	graphTable = new std::vector<int>[nodesCount];
	for(int i = 0; i < nodesCount; i++)
		graphTable[i] = g.graphTable[i];
	return *this;
}

bool Graf::operator==(const Graf & g)
{
	if(nodesCount != g.nodesCount)
		return false;
	for(int i = 0; i < nodesCount; i++)
	{
		if(graphTable[i] != g.graphTable[i])
			return false;
	}
	return true;
}

/*
 * Loads graph structure from given file
 */
void Graf::loadGraph(std::ifstream & plik)
{
	int nCount;
	plik>>nCount;
	nodesCount = nCount;
	graphTable = new std::vector<int>[nCount];
	// Allocate 2-dimensional array of int
	int a, b;
	while (!plik.eof() )
	{
	    plik >> a >> b;
	    graphTable[a].push_back(b);
	    graphTable[b].push_back(a);
	}
	graphTable[a].pop_back();
	graphTable[b].pop_back();
	sortGraph();
}

/*
 * Print graph structure
 */
void Graf::printGraph(void) const
{
	for(int i = 0; i < this->nodesCount; i++)
	{
		std::vector<int>::iterator iter;
		for(iter = graphTable[i].begin(); iter != graphTable[i].end(); iter++)
		{
			std::cout<<(*iter)<<", ";
		}
		std::cout<<std::endl;
	}
}

int Graf::getVertexesCount(void)
{
	return nodesCount;
}

int Graf::getEdgesCount(void)
{
	int edgesCount = 0;
	for(int i = 0; i < nodesCount; i++)
	{
		edgesCount += graphTable[i].size();
	}
	return edgesCount;
}

void Graf::sortGraph(void)
{
	for(int i = 0; i < nodesCount; i++)
		std::sort(graphTable[i].begin(), graphTable[i].end());
}
