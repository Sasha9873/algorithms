#pragma once 

struct IGraph {
    virtual ~IGraph() = default;

    virtual void AddEdge(size_t from, size_t to) = 0;
    virtual size_t VerticesCount() const noexcept = 0;
    virtual void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;
    virtual void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;
};


struct ArcGraph : public IGraph { //представленный списком рёбер
    ArcGraph(size_t vertex) : k_vertex_(vertex);
    ArcGraph(const IGraph* graph);

    ArcGraph(const ArcGraph& graph) = delete;

    ArcGraph& operator=(const ArcGraph& graph) = delete;

    void AddEdge(size_t from, size_t to) = 0;
    size_t VerticesCount() const noexcept = 0;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;


    ~ArcGraph();

private:
    vector<pair<int, int>> data_ = nullptr;
    size_t k_vertex_ = 0;
};

struct MatrixGraph : public IGraph { //представленный матрицей смежности
    MatrixGraph(size_t vertex);
    MatrixGraph(const IGraph* graph);

    MatrixGraph(const MatrixGraph& graph) = delete;
    
    MatrixGraph& operator=(const MatrixGraph& graph) = delete;

    void AddEdge(size_t from, size_t to) = 0;
    size_t VerticesCount() const noexcept = 0;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;


    ~MatrixGraph();
};

struct ListGraph : public IGraph { //представленный списком смежности
    ListGraph(size_t vertex);
    ListGraph(const IGraph* graph);

    ListGraph(const ListGraph& graph) = delete;
    
    ListGraph& operator=(const ListGraph& graph) = delete;

    void AddEdge(size_t from, size_t to) = 0;
    size_t VerticesCount() const noexcept = 0;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;

    
    ~ListGraph();
};

//(похож на предыдущий, но для хранения списка следующих и предыдущих вершин 
//используется не последовательный, а ассоциативный контейнер)
struct SetGraph : public IGraph { //представленный списком множеств
    SetGraph(size_t vertex);
    SetGraph(const IGraph* graph);

    SetGraph(const SetGraph& graph) = delete;
    
    SetGraph& operator=(const SetGraph& graph) = delete;

    void AddEdge(size_t from, size_t to) = 0;
    size_t VerticesCount() const noexcept = 0;
    void GetNextVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;
    void GetPrevVertices(size_t vertex, std::vector<size_t>& vertices) const noexcept = 0;

    
    ~SetGraph();
};