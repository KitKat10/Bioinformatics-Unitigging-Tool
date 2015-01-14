#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "overlapgraph.h"

using namespace std;

std::string path;
std::vector<std::string> reads;
std::vector<overlap> overlaps;

std::vector<std::string> split(std::string input, char splitChar)
{
	std::vector<std::string> substrings;
	std::string::iterator it;
	std::string s = "";
	for (it = input.begin(); it<input.end(); it++)
	{
		if (*it == splitChar)
		{
			substrings.push_back(s);
			s = "";
		}
		else
			s += *it;
	}
	substrings.push_back(s);
	return substrings;
}

int main(int argc, char *argv[])
{
	path = argv[1];

	std::string readsfile = path + "\\reads.txt";
	std::string overlapsfile = path + "\\overlaps.afg";

	ifstream readsMap(path + "\\reads.bnk\\RED.0.map");
	ifstream readsMap2(path + "\\reads.2k.10x.fasta");

	std::ifstream readinput(readsfile.c_str());
	for (std::string line; std::getline(readinput, line);)
	{
		std::vector<std::string> substrings = split(line, '\t');
		reads.push_back(substrings[1]);
	}

	//std::cout << reads.size() << std::endl;

	map<string, int> readMap;
	for (string line; getline(readsMap, line);)
	{
		if (line.substr(0, 3) == "RED")
			continue;

		vector<string> tokens = split(line, '\t');
		pair <string, int> p(tokens[2], stoi(tokens[0]));
		readMap.insert(p);
	}

	int no = 0;
	string read;
	for (string line; getline(readsMap2, line);)
	{

		if (line.substr(0, 1) == ">"){
			no = readMap[line.substr(1)];
			continue;
		}
		else
		{
			reads.push_back(line);
		}
	}


	std::ifstream overlapinput(overlapsfile.c_str());
	overlap *o = NULL;
	string line;
	for (string line; getline(overlapinput, line);)
	{
		if (line[0] == '{')
			o = (overlap *)malloc(sizeof(overlap));
		else if (line.substr(0, 3) == "adj")
			o->orientation = line[4];
		else if (line.substr(0, 3) == "rds")
		{
			string s = line.substr(4);
			vector<string> reads = split(s, ',');
			o->read1 = stoi(reads[0])-1;
			o->read2 = stoi(reads[1])-1;
		}
		else if (line.substr(0, 3) == "ahg")
		{
			o->ahg = stoi(line.substr(4));
		}
		else if (line.substr(0, 3) == "bhg")
		{
			o->bhg = stoi(line.substr(4));
		}
		else if (line[0] == '}')
		{
			overlaps.push_back(*o);
		}
	}

	OverlapGraph overlapGraph(reads, overlaps);
	overlapGraph.runUnitigging();

}
