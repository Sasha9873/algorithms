#include <iostream>
#include <vector>
#include <cstring>

using std::vector;

const static vector<int> dx = {1, -1, 0, 0};
const static vector<int> dy = {0, 0, 1, -1};

void read_matrix(vector< vector< char > >& matrix, const size_t& height, const size_t& width, vector< vector< int > >& number)
{
	int num = 0;

	for(size_t i = 0; i < height; ++i)
    {
        for(size_t j = 0; j < width; ++j)
        {
            std::cin >> matrix[i][j];
            number[i][j] = num++;
        }
    }
}

bool is_ok(int i, int j, size_t height, size_t width)
{
	if(i < 0 || j < 0 || i >= height || j >= width)
		return 0;
	return 1;
}

vector<vector<int>> make_graph(const vector< vector< char > >& matrix, size_t height, size_t width, 
	const vector< vector< int > >& number, int& k_stars) 
{
	k_stars = 0;

    vector<vector<int>> graph(height * width, std::vector<int>());
    for(size_t i = 0; i < height; ++i) 
    	for(size_t j = 0; j < width; ++j)
    	{
    		if(matrix[i][j] == '*')
    		{
    			++k_stars;

    			for(int k = 0; k < 4; ++k)
    			{
    				if(is_ok(i + dx[k], j + dy[k], height, width) && matrix[i + dx[k]][j + dy[k]] == '*')
    				{
                        //std::cout << number[i + dx[k]][j + dy[k]] << " " << number[i][j] << " aaaaa\n";
    					graph[number[i][j]].push_back(number[i + dx[k]][j + dy[k]]);
    					graph[number[i + dx[k]][j + dy[k]]].push_back(number[i][j]);
    				}
    			}
    		}
    		
    	}

    return graph;
}

void dfs(const vector<vector<int>>& graph, vector<char>& used, vector<int>& color, int vertex, int c)
{
    color[vertex] = c;
    used[vertex] = 1;

    for(int to : graph[vertex])
    {
        if(!used[to])
        {
            dfs(graph, used, color, to, (c ^ 1));
        }
    }
}

void make_any_bipartite_graph(const vector<vector<char>>& matrix, size_t height, size_t width, 
	const vector<vector<int>>& number, const vector<vector<int>>& graph, vector<char>& used, 
	vector<int>& color)
{
	for(size_t i = 0; i < height; ++i)
    {
        for(size_t j = 0; j < width; ++j)
        {
            if(matrix[i][j] == '*' && !used[number[i][j]])
                dfs(graph, used, color, number[i][j], 0);
        }
    }
}

int kuhn(int vertex, vector<int>&  matched, const vector<vector<int>>& graph, vector<char>& used)
{
    if(used[vertex])
        return 0;
    used[vertex] = 1;

    for(int to : graph[vertex])
    {
        if(matched[to] == -1 || kuhn(matched[to], matched, graph, used))
        {
            matched[to] = vertex;
            return 1;
        }
    }

    return 0;
}

int make_max_matching(const vector<vector<char>>& matrix, size_t height, size_t width, 
	const vector<vector<int>>& number, const vector<vector<int>>& graph, vector<char>& used, 
	vector<int>& color)
{
	vector<int> matched(graph.size(), -1);
	int ans = 0;

	for(size_t i = 0; i < height; ++i)
    {
        for(size_t j = 0; j < width; ++j)
        {
            if(matrix[i][j] == '*' && color[number[i][j]] == 0)
            {
            	used.assign(height * width, 0);
                ans += kuhn(number[i][j], matched, graph, used);
            }
        }
    }

    return ans;
}



int main()
{
	size_t height = 0, width = 0;
    int time1 = 0, time2 = 0;
	std::cin >> height >> width >> time2 >> time1;

	vector< vector< char > > matrix(height, vector< char >(width));
	vector< vector< int > > number(height, vector< int >(width));   //условная нумерация клеток в matrix
	read_matrix(matrix, height, width, number);

	int k_stars = 0;
	vector<vector<int>> graph = make_graph(matrix, height, width, number, k_stars);

	vector<char> used(height * width, 0);
	vector<int> color(height * width, 0);
	make_any_bipartite_graph(matrix, height, width, number, graph, used, color);

	int ans = make_max_matching(matrix, height, width, number, graph, used, color);

	if(2 * time1 <= time2)
        std::cout << k_stars * time1;
    else
        std::cout << (ans * time2) + (k_stars - ans * 2) * time1;
/**/
	return 0;
}