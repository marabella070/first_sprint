#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include "Document.h"
using namespace std;


class SearchServer {
public:
    void SetStopWords(const string& text);
    void AddDocument(int document_id, const string& document);
    vector<Document> FindTopDocuments(const string& raw_query) const;

private:
    struct Query
    {
        bool empty() const
        {
            return !(plus_words.size() + minus_words.size());
        }
        set<string> plus_words;
        set<string> minus_words;
    };

    bool IsStopWord(const string& word) const;
    vector<string> SplitIntoWordsNoStop(const string& text) const;
    Query ParseQuery(const string& text) const;
    vector<Document> FindAllDocuments(const Query& query) const;

    map<string, map<int, double>> documents_;
    set<string> stop_words_;
    int document_count_ = 0;
};