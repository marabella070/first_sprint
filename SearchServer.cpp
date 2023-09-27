#include <algorithm>
#include <cmath>
#include <iostream>
#include "SearchServer.h"
#include "functions.h"
#include "constants.h"

void SearchServer::SetStopWords(const string& text) {
    for (const string& word : SplitIntoWords(text)) {
        stop_words_.insert(word);
    }
}

void SearchServer::AddDocument(int document_id, const string& document) {
    const vector<string> words = SplitIntoWordsNoStop(document);
    for (const string& word : words)
        documents_[word][document_id] += (1.0 / words.size());
    ++document_count_;
}

vector<Document> SearchServer::FindTopDocuments(const string& raw_query) const {
    const Query query = ParseQuery(raw_query);
    vector<Document> matched_documents = FindAllDocuments(query);

    sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return lhs.relevance > rhs.relevance;
        });
    if (matched_documents.size() > constant::MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(constant::MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}
bool SearchServer::IsStopWord(const string& word) const {
    return stop_words_.count(word) > 0;
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string& text) const {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

SearchServer::Query SearchServer::ParseQuery(const string& text) const {
    Query query;
    for (const string& word : SplitIntoWordsNoStop(text)) {
        if (word[0] != '-')
            query.plus_words.insert(word);
        else
            query.minus_words.insert(word);
    }
    
    return query;
}

vector<Document> SearchServer::FindAllDocuments(const Query& query) const {
    map<int, double> matched_documents;
    vector<Document> result;

    for (const string& plus_word : query.plus_words)
    {
        if (documents_.count(plus_word))
        {
            const map<int, double>& document = documents_.at(plus_word);
            double IDF = log(document_count_ / static_cast<double>(document.size()));
            for (auto& [id, TF] : document)
                matched_documents[id] += IDF * TF;
        }
    }

    for (const string& minus_word : query.minus_words)
    {
        string sub_minus_word = minus_word.substr(1, minus_word.size() - 1);
        if (documents_.count(minus_word))
        {
            for (auto& [id, TF] : documents_.at(sub_minus_word))
                matched_documents.erase(id);
        }
    }

    for (auto& [id, relevance] : matched_documents)
        result.push_back({ id, relevance });

    return result;
}