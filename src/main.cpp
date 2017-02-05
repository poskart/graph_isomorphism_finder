//============================================================================
// Name        : main.cpp
// Author      : Piotr Poskart
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Graf.h"
#include "IsomorphismFinder.h"
using namespace std;

const char * file1 = "tests/Test6_1.txt";
const char * file2 = "tests/Test6_2.txt";

int main()
{
	ifstream plik1, plik2;
	plik1.open(file1, std::iostream::in);
	plik2.open(file2, std::iostream::in);

	Graf graph1, graph2;

	if(plik1.is_open())
		graph1.loadGraph(plik1);
	if(plik2.is_open())
		graph2.loadGraph(plik2);

	IsomorphismFinder finder(graph1.getVertexesCount());
	finder.checkIsomorphism(graph1, graph2);

	return 0;
}
