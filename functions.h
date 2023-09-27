#pragma once


#include <string>
#include <vector>
#include "SearchServer.h"
using namespace std;

string ReadLine();
int ReadLineWithNumber();
vector<string> SplitIntoWords(const string& text);
SearchServer CreateSearchServer();