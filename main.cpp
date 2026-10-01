#include "count_min_sketch.hpp"

int main()
{
    CountMinSketch* filter = new CountMinSketch(20, 5);

    filter->Insert("harsh");

    std::cout << filter->Count("harsh") << std::endl;

    filter->Clear();

    filter->Insert("window");
    filter->Insert("window");
    filter->Insert("harsh");

    std::cout << filter->Count("waymen") << std::endl;
    std::cout << filter->Count("harsh") << std::endl;
}