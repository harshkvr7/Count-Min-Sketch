#include <bits/stdc++.h>
#include <mutex>

class CountMinSketch
{
private:
    size_t width;
    size_t depth;

    int* sketch;

    std::mutex sketchMutex;

    int p = 31;

    int getRowHash(int row, std::string item)
    {
        size_t hash1 = std::hash<std::string>{}(item);
    
        size_t hash2 = std::hash<std::string>{}(item + "_salt"); 

        size_t combinedHash = hash1 + (row * hash2);

        return combinedHash % width;
    }

    void printTable()
    {
        for(size_t row = 0; row < depth; row++)
        {
            for(size_t col = 0; col < width; col++)
            {
                std::cout << sketch[row * width + col];
            }

            std::cout << std::endl;
        }
    }

public:
    CountMinSketch(int _width, int _depth) : width(_width), depth(_depth)
    {
        sketch = new int[width * depth]();
    }

    void Insert(std::string item)
    {
        const std::lock_guard<std::mutex> lock(sketchMutex);
        
        for(size_t row = 0; row < depth; row++)
        {
            int hash = getRowHash(row, item);


            sketch[row * width + hash]++;
        }

        printTable();
    }

    int Count(std::string item)
    {
        int res = INT_MAX;

        for(size_t row = 0; row < depth; row++)
        {
            int hash = getRowHash(row, item);

            res = std::min(res, sketch[row * width + hash]);
        }

        return res;
    }

    void Clear()
    {
        for(size_t row = 0; row < depth; row++)
        {
            for(size_t col = 0; col < width; col++)
            {
                sketch[row * width + col] = 0;
            }
        }
    }

    void Merge(CountMinSketch &other)
    {

    }

    //std::vector<string> TopK(int k, &candidates)
};


