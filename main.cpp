#include "stlReader.hpp"

int main() {
    stlReader stl("/home/vijai.kumar/Documents/Data/bunny_new.stl");
    std::vector<Triangle> test_triangles = stl.readSTL();
    stl.writeSTL(test_triangles, "test_new_bunny.stl");
    return 0;
}
