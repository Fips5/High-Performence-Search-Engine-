# High-Performance Search Engine - Project Timeline

Development roadmap for building a search engine with parallel scraping, embeddings, and vector similarity search.

1. Understand and implement Parallel Scraper ([MilanSazdov/parallel-web-scraper](https://github.com/MilanSazdov/parallel-web-scraper))

2. Fetch and store raw HTML data

3. Build custom HTML parser and cleaner

4. Generate embeddings using bge_large_en_v1.5 ([State-of-art Retrieval Augmented LLM](https://medium.com/@marketing_novita.ai/state-of-art-retrieval-augmented-llm-bge-large-en-v1-5-4cd5abbcbf0a))

5. Create FAISS index

6. Test FAISS and alternative indexing solutions, document comparison results

7. After selecting the optimal similarity model, proceed to researching search algorithms