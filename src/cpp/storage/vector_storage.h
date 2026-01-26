class VectorStore {
public:
    VectorStore(size_t dim);

    int64_t add_vector(int64_t doc_id, const float* vector);

    const float* get_vector_by_internal_id(int64_t internal_id) const;

    size_t size() const;
    size_t dim() const;

private:
    size_t dim_;
    std::vector<float> data_;      // contiguous
    std::vector<int64_t> doc_ids_; // internal_id → doc_id
};
