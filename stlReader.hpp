#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>


struct Vertex {
public:
    Vertex(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

public:
    void setX(double _x) { x = _x; }

    void setY(double _y) { y = _y; }

    void setZ(double _z) { z = _z; }

public:
    double getX() { return x; }

    double getY() { return y; }

    double getZ() { return z; }

private:
    double x, y, z;
};

struct Triangle {
public:
    Triangle(Vertex &_v1, Vertex &_v2, Vertex &_v3) : v1(_v1), v2(_v2), v3(_v3) {};

public:
    Vertex getVertexOne() { return v1; }

    Vertex getVertexTwo() { return v2; }

    Vertex getVertexThree() { return v3; }

private:
    Vertex v1, v2, v3;
};

class stlReader {
public:
    stlReader(const std::string _input_geometry) : input_geometry(_input_geometry){};

    ~stlReader() {};

public:
    std::vector<Triangle> readSTL() {
        return readSTLInefficient(input_geometry);
    }

    void writeSTL(std::vector<Triangle> &all_triangles, const std::string output_name) {
        std::ofstream myfile(output_name);
        if (myfile.is_open()) {
            myfile << "solid stlfile" << std::endl;
            for (auto triangle: all_triangles) {
                myfile << "  facet normal 0.0 0.0 0.0" << std::endl;
                myfile << "    outer loop" << std::endl;
                myfile << "      vertex " << triangle.getVertexOne().getX() << " " << triangle.getVertexOne().getY()
                       << " "
                       << triangle.getVertexOne().getZ() << std::endl;
                myfile << "      vertex " << triangle.getVertexTwo().getX() << " " << triangle.getVertexTwo().getY()
                       << " "
                       << triangle.getVertexTwo().getZ() << std::endl;
                myfile << "      vertex " << triangle.getVertexThree().getX() << " " << triangle.getVertexThree().getY()
                       << " "
                       << triangle.getVertexThree().getZ() << std::endl;
                myfile << "    endloop" << std::endl;
                myfile << "  endfacet" << std::endl;
            }
            myfile << "endsolid stlfile" << std::endl;
            myfile.close();
        }
    }

private:
    void removeDuplicateStrings(std::vector<std::string> &v) {
        std::vector<std::string>::iterator itr = v.begin();
        std::unordered_set<std::string> s;
        for (auto curr = v.begin(); curr != v.end(); ++curr) {
            if (s.insert(*curr).second) {
                *itr++ = *curr;
            }
        }
        v.erase(itr, v.end());
    }

    void printVector(std::vector<std::string> &vS) {
        std::string final_vector;
        final_vector.append("{ ");
        for (auto v: vS) {
            final_vector.append(v);
            final_vector.append(",");
        }
        final_vector.pop_back();
        final_vector.append(" }");
        std::cout << final_vector << std::endl;
    }

    std::vector<Triangle> readSTLInefficient(const std::string input_stl) {
        std::vector<Triangle> all_triangles;
        std::string line;
        std::ifstream stlFile(input_stl);
        std::vector<std::string> globalTriangles;
        if (stlFile.is_open()) {
            bool insideTriangle = false;
            std::string vertex_one, vertex_two, vertex_three;
            unsigned int vC = 0;
            while (getline(stlFile, line)) {
                boost::trim(line);
                if (boost::starts_with(line, "outer loop")) {
                    insideTriangle = true;
                }
                if (boost::starts_with(line, "endloop")) {
                    insideTriangle = false;
                }
                if (insideTriangle) {
                    if (boost::starts_with(line, "vertex")) {
                        if (vC == 1) {
                            vertex_one = line;
                            boost::erase_all(vertex_one, "vertex");
                            boost::trim(vertex_one);
                        }
                        if (vC == 2) {
                            vertex_two = line;
                            boost::erase_all(vertex_two, "vertex");
                            boost::trim(vertex_two);
                        }
                        if (vC == 3) {
                            vertex_three = line;
                            boost::erase_all(vertex_three, "vertex");
                            boost::trim(vertex_three);
                        }
                    }
                    vC++;
                } else {
                    vC = 0;
                    std::string combined_string;
                    if ((vertex_one.size() > 0) && (vertex_two.size() > 0) && (vertex_three.size() > 0)) {
                        combined_string.append(vertex_one);
                        combined_string.append("/");
                        combined_string.append(vertex_two);
                        combined_string.append("/");
                        combined_string.append(vertex_three);
                        globalTriangles.push_back(combined_string);
                    }
                }

            }
            stlFile.close();
        }
        removeDuplicateStrings(globalTriangles);
        for (auto triangle: globalTriangles) {
            std::string current_triangle = triangle;
            boost::trim(current_triangle);
            std::vector<std::string> current_triangle_vertices;
            boost::split(current_triangle_vertices, current_triangle, boost::is_any_of("/"));
            std::string vertex_one = current_triangle_vertices.at(0);
            std::string vertex_two = current_triangle_vertices.at(1);
            std::string vertex_three = current_triangle_vertices.at(2);
            boost::trim(vertex_one);
            boost::trim(vertex_two);
            boost::trim(vertex_three);
            std::istringstream issOne(vertex_one);
            std::istringstream issTwo(vertex_two);
            std::istringstream issThree(vertex_three);
            double v1v1, v1v2, v1v3;
            issOne >> v1v1 >> v1v2 >> v1v3;
            Vertex v1(v1v1, v1v2, v1v3);
            double v2v1, v2v2, v2v3;
            issTwo >> v2v1 >> v2v2 >> v2v3;
            Vertex v2(v2v1, v2v2, v2v3);
            double v3v1, v3v2, v3v3;
            issThree >> v3v1 >> v3v2 >> v3v3;
            Vertex v3(v3v1, v3v2, v3v3);
            Triangle new_triangle(v1, v2, v3);
            all_triangles.push_back(new_triangle);
        }
        return all_triangles;
    }

public:
    std::string input_geometry;
};