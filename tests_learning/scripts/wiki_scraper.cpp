#include <unordered_set>
#include <queue>
#include <iostream>
#include <string>

#include "httplib.h"

int main() {
    httplib::Client client("https://books.toscrape.com");

    std::unordered_set<std::string> visited;
    std::queue<std::string> frontier;

    frontier.push("/");

    while (!frontier.empty()) {
        std::string path = frontier.front();
        frontier.pop();

        if (visited.find(path) != visited.end())
            continue;

        visited.insert(path);

        auto res = client.Get(path.c_str());
        if (!res || res->status != 200) {
            std::cerr << "Failed: " << path << "\n";
            continue;
        }

        const std::string& html = res->body;

        // ---- PARSE HERE ----
        // extract links
        // extract data

        std::cout << "Fetched " << path
                  << " size=" << html.size() << "\n";
    }
}
