#include <parallel_web_scraper/scraper.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <vector>
#include <string>
#include <regex>

namespace fs = std::filesystem;

// Thread-safe metadata writer
std::mutex meta_mutex;
std::ofstream meta_file;

// Utility: clean HTML to plain text (very basic)
std::string html_to_text(const std::string& html) {
    std::string text = html;

    // remove scripts/styles
    text = std::regex_replace(text, std::regex("<script[^>]*>.*?</script>"), "");
    text = std::regex_replace(text, std::regex("<style[^>]*>.*?</style>"), "");

    // remove all HTML tags
    text = std::regex_replace(text, std::regex("<[^>]+>"), " ");

    // collapse whitespace
    text = std::regex_replace(text, std::regex("\\s+"), " ");

    return text;
}

// Process a single article
void process_article(const std::string& url, int doc_id) {
    try {
        std::string html = parallel_scraper::fetch_html(url); // fetch using the scraper lib
        std::string text = html_to_text(html);

        // extract title from <title> tag
        std::string title;
        std::smatch match;
        if (std::regex_search(html, match, std::regex("<title>(.*?)</title>"))) {
            title = match[1].str();
        }

        // extract date if available (optional, Wikipedia usually has <time>)
        std::string date = "2026-01-27"; // fallback to today for simplicity

        // save raw HTML
        fs::create_directories("tests&learning/data/raw/");
        std::ofstream raw_file("tests&learning/data/raw/page_" + std::to_string(doc_id) + ".html");
        raw_file << html;
        raw_file.close();

        // save cleaned text
        fs::create_directories("tests&learning/data/text/");
        std::ofstream text_file("tests&learning/data/text/doc_" + std::to_string(doc_id) + ".txt");
        text_file << text;
        text_file.close();

        // write metadata
        std::lock_guard<std::mutex> lock(meta_mutex);
        meta_file << doc_id << "," << url << "," << title << "," << date << "\n";

        std::cout << "Saved doc_id=" << doc_id << " : " << title << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to fetch " << url << " : " << e.what() << std::endl;
    }
}

// Main entry
int main() {
    fs::create_directories("tests&learning/data/");
    meta_file.open("tests&learning/data/metadata.csv");
    meta_file << "doc_id,url,title,date\n";

    // Step 1: Current Events portal
    std::string portal_url = "https://en.wikipedia.org/wiki/Portal:Current_events";

    // Step 2: Fetch portal HTML and extract article links
    std::string portal_html = parallel_scraper::fetch_html(portal_url);
    std::vector<std::string> article_urls;

    // Regex to find article links
    std::regex link_regex(R"(<a href="(/wiki/[^:#"]+)")");
    std::smatch match;
    std::string::const_iterator search_start(portal_html.cbegin());
    while (std::regex_search(search_start, portal_html.cend(), match, link_regex)) {
        std::string link = "https://en.wikipedia.org" + match[1].str();
        article_urls.push_back(link);
        search_start = match.suffix().first;
    }

    // Remove duplicates
    std::sort(article_urls.begin(), article_urls.end());
    article_urls.erase(std::unique(article_urls.begin(), article_urls.end()), article_urls.end());

    std::cout << "Found " << article_urls.size() << " articles" << std::endl;

    // Step 3: Parallel scrape articles
    parallel_scraper::parallel_for(article_urls.size(), [&](size_t i) {
        process_article(article_urls[i], static_cast<int>(i));
    }, 8); // 8 threads

    meta_file.close();
    std::cout << "Scraping done. Metadata saved to metadata.csv" << std::endl;
    return 0;
}
