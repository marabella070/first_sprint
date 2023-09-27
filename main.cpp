#include <iostream>
#include "SearchServer.h"
#include "functions.h"
using namespace std;

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", " << "relevance = "s << relevance << " }"s << endl;
    }

    return 0;
}