#include <bits/stdc++.h>
#include <mutex>

class CountMinSketch
{
private:
    size_t width;
    size_t depth;

    uint64_t* sketch;

    std::mutex sketchMutex;

    size_t getRowHash(size_t row, const std::string& item)
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

        for(size_t col = 0; col < width; col++)
        {
            std::cout << '-';
        }

        std::cout << std::endl;
    }

public:
    // constructor, creates a count-min sketch with width columns (buckets) and depth rows (hash functions).
    CountMinSketch(size_t _width, size_t _depth) : width(_width), depth(_depth)
    {
        sketch = new uint64_t[width * depth]();
    }

    ~CountMinSketch() 
    { 
        delete[] sketch; 
    }

    // inserts the specified item into the count-min sketch.
    void Insert(const std::string& item)
    {
        const std::lock_guard<std::mutex> lock(sketchMutex);
        
        for(size_t row = 0; row < depth; row++)
        {
            size_t hash = getRowHash(row, item);


            sketch[row * width + hash]++;
        }

        //printTable();
    }

    // returns the estimated frequency of the item.
    uint64_t Count(const std::string& item)
    {
        uint64_t res = UINT64_MAX;
        
        const std::lock_guard<std::mutex> lock(sketchMutex);

        for(size_t row = 0; row < depth; row++)
        {
            int hash = getRowHash(row, item);

            res = std::min(res, sketch[row * width + hash]);
        }

        return res;
    }

    // resets the data structure from previous streams.
    void Clear()
    {
        const std::lock_guard<std::mutex> lock(sketchMutex);

        for(size_t row = 0; row < depth; row++)
        {
            for(size_t col = 0; col < width; col++)
            {
                sketch[row * width + col] = 0;
            }
        }

        //printTable();
    }

    // creates a new sketch by combining counter values from two compatible sketches.
    void Merge(CountMinSketch &other)
    {
        if (width != other.width || depth != other.depth)
        {
            throw std::invalid_argument("Cannot merge sketches of different dimensions.");
        }

        std::scoped_lock lock(sketchMutex, other.sketchMutex);

        for (size_t i = 0; i < (width * depth); i++)
        {
            sketch[i] += other.sketch[i];
        }
    }

    // returns the k candidates with the most estimated counts.
    std::vector<std::string> TopK(size_t k, std::vector<std::string>& candidates)
    {
        std::priority_queue<std::pair<uint64_t, std::string>, std::vector<std::pair<uint64_t, std::string>>, std::greater<std::pair<uint64_t, std::string>>> min_heap;

        for (const std::string& item : candidates)
        {
            uint64_t current_count = Count(item); 

            min_heap.push({current_count, item});

            if (min_heap.size() > k)
            {
                min_heap.pop(); 
            }
        }

        std::vector<std::string> result;

        while (!min_heap.empty())
        {
            result.push_back(min_heap.top().second);
            min_heap.pop();
        }

        std::reverse(result.begin(), result.end());

        return result;
    }
};


