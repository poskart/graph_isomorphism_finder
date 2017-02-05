/*
 * Graf.h
 *
 *  Created on: 7 sty 2017
 *      Author: piotr
 */

#ifndef GRAF_H_
#define GRAF_H_

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

class IsomorphismFinder;

class Graf {
public:
	friend class IsomorphismFinder;
	Graf();
	Graf(std::ifstream & plik);
	Graf(const Graf & g);
	Graf & operator=(const Graf & g);
	bool operator==(const Graf & g);
	virtual ~Graf();

	void loadGraph(std::ifstream & plik);
	void printGraph(void) const;
	int getVertexesCount(void);
	int getEdgesCount(void);
	void sortGraph(void);
private:
	int nodesCount;
	std::vector<int> * graphTable;	// vectors table
};

#endif /* GRAF_H_ */
