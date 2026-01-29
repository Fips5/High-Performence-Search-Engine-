#include <unordered_set>
#include <queue>
#include <iostream>

int main() {
    HttpClient client;
    std::unordered_set<std::string> visited;
    std::queue<std::string> frontier;

    frontier.push("https://books.toscrape.com/");

    while (!frontier.empty()) {
        std::string url = frontier.front();
        frontier.pop();

        if (visited.contains(url)) continue;
        visited.insert(url);

        std::string html;
        if (!client.get(url, html)) {
            std::cerr << "Failed: " << url << "\n";
            continue;
        }

        // ---- PARSE HERE ----
        // extract links, push to frontier
        // extract data

        std::cout << "Fetched " << url << " size=" << html.size() << "\n";
    }
}
