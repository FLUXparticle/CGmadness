#include "foo.hpp"

#include "text/dataBigAlpha.hpp"
#include "text/dataDigits.hpp"
#include "text/dataSmallAlpha.hpp"

#include "math/Vector3.hpp"

#include "macros.hpp"

#include <fstream>
#include <string>
#include <list>

#define FOO(x, y, z) foo(x, &y, &z, MACRO_STRING(y))

struct Point
{
	Vector3 vertex;
	Vector3 normal;

	Point(Vector3 vertex, Vector3 normal) :
		vertex(vertex), normal(normal)
	{
		// empty
	}

	bool operator==(const Point& other) const
	{
		return vertex == other.vertex && normal == other.normal;
	}
};

int gProcessedPoints;
int gKnownPoints;

void foo(int count, const float *vertices, const float *normals,
		const char* name)
{
	std::list<Point> knownPoints;
	std::list<unsigned int> indices;

	int start = 0;
	for (int i = 0; i < count; i++)
	{
		Vector3 vertex(vertices[start], vertices[start + 1], vertices[start + 2]);
		Vector3 normal(normals[start], normals[start + 1], normals[start + 2]);
		Point p(vertex, normal);

		unsigned int index = 0;
		FOREACH(std::list<Point>, knownPoints, iter){
			if (*iter == p)
			{
				break;
			}
			else
			{
				index++;
			}
		}

		if (index >= knownPoints.size())
		{
			knownPoints.push_back(p);
		}

		indices.push_back(index);

		start += 3;
	}

	gProcessedPoints += count;
	gKnownPoints += knownPoints.size();

	fprintf(stderr, "%s: %d -> %d\n", name + 8, count, knownPoints.size());

	std::string basename(name + 8);

	std::fstream fout((std::string("font3d/") + basename + ".inc").c_str(), std::fstream::out);

	fout << "float vertices" << basename << "[" << knownPoints.size() << " * 3] = {" << std::endl;
	FOREACH(std::list<Point>, knownPoints, iter)
	{
		const Point& p = *iter;

		const Vector3& v = p.vertex;

		fout << v.x << ", " << v.y << ", " << v.z << ", " << std::endl;
	}
	fout << "};" << std::endl << std::endl;

	fout << "float normals" << basename << "[" << knownPoints.size() << " * 3] = {" << std::endl;
	FOREACH(std::list<Point>, knownPoints, iter)
	{
		const Point& p = *iter;

		const Vector3& n = p.normal;

		fout << n.x << ", " << n.y << ", " << n.z << ", " << std::endl;
	}
	fout << "};" << std::endl << std::endl;

	fout << "unsigned int indices" << basename << "[" << indices.size() << "] = {";
	int i = 0;
	FOREACH(std::list<unsigned int>, indices, iter)
	{
		const unsigned int& index = *iter;

		if (i % 3 == 0)
		{
			fout << std::endl;
		}
		i++;

		fout << index << ", ";
	}
	fout << std::endl << "};" << std::endl << std::endl;

	fout << "void draw" << basename << "()" << std::endl;
	fout << "{" << std::endl;
	fout << "drawTrianglesVerticesNormalsIndices(" << indices.size() << ", vertices" << basename << ", normals" << basename << ", indices" << basename << ");" << std::endl;
	fout << "}" << std::endl;
}

int main(int argc, char* argv[])
{
	gProcessedPoints = 0;
	gKnownPoints = 0;

	FOO(792, verticesBigAlpha0, normalsBigAlpha0);
	FOO(1008, verticesBigAlpha1, normalsBigAlpha1);
	FOO(1332, verticesBigAlpha2, normalsBigAlpha2);
	FOO(864, verticesBigAlpha3, normalsBigAlpha3);
	FOO(828, verticesBigAlpha4, normalsBigAlpha4);
	FOO(804, verticesBigAlpha5, normalsBigAlpha5);
	FOO(1464, verticesBigAlpha6, normalsBigAlpha6);
	FOO(1116, verticesBigAlpha7, normalsBigAlpha7);
	FOO(660, verticesBigAlpha8, normalsBigAlpha8);
	FOO(792, verticesBigAlpha9, normalsBigAlpha9);
	FOO(1032, verticesBigAlpha10, normalsBigAlpha10);
	FOO(636, verticesBigAlpha11, normalsBigAlpha11);
	FOO(972, verticesBigAlpha12, normalsBigAlpha12);
	FOO(768, verticesBigAlpha13, normalsBigAlpha13);
	FOO(912, verticesBigAlpha14, normalsBigAlpha14);
	FOO(996, verticesBigAlpha15, normalsBigAlpha15);
	FOO(1752, verticesBigAlpha16, normalsBigAlpha16);
	FOO(1068, verticesBigAlpha17, normalsBigAlpha17);
	FOO(1980, verticesBigAlpha18, normalsBigAlpha18);
	FOO(612, verticesBigAlpha19, normalsBigAlpha19);
	FOO(1044, verticesBigAlpha20, normalsBigAlpha20);
	FOO(672, verticesBigAlpha21, normalsBigAlpha21);
	FOO(648, verticesBigAlpha22, normalsBigAlpha22);
	FOO(996, verticesBigAlpha23, normalsBigAlpha23);
	FOO(912, verticesBigAlpha24, normalsBigAlpha24);
	FOO(420, verticesBigAlpha25, normalsBigAlpha25);
	FOO(912, verticesDigits0, normalsDigits0);
	FOO(552, verticesDigits1, normalsDigits1);
	FOO(1044, verticesDigits2, normalsDigits2);
	FOO(1680, verticesDigits3, normalsDigits3);
	FOO(432, verticesDigits4, normalsDigits4);
	FOO(1332, verticesDigits5, normalsDigits5);
	FOO(1392, verticesDigits6, normalsDigits6);
	FOO(396, verticesDigits7, normalsDigits7);
	FOO(1356, verticesDigits8, normalsDigits8);
	FOO(1296, verticesDigits9, normalsDigits9);
	FOO(1176, verticesSmallAlpha0, normalsSmallAlpha0);
	FOO(1044, verticesSmallAlpha1, normalsSmallAlpha1);
	FOO(1248, verticesSmallAlpha2, normalsSmallAlpha2);
	FOO(1044, verticesSmallAlpha3, normalsSmallAlpha3);
	FOO(900, verticesSmallAlpha4, normalsSmallAlpha4);
	FOO(984, verticesSmallAlpha5, normalsSmallAlpha5);
	FOO(1188, verticesSmallAlpha6, normalsSmallAlpha6);
	FOO(1008, verticesSmallAlpha7, normalsSmallAlpha7);
	FOO(492, verticesSmallAlpha8, normalsSmallAlpha8);
	FOO(576, verticesSmallAlpha9, normalsSmallAlpha9);
	FOO(900, verticesSmallAlpha10, normalsSmallAlpha10);
	FOO(456, verticesSmallAlpha11, normalsSmallAlpha11);
	FOO(1224, verticesSmallAlpha12, normalsSmallAlpha12);
	FOO(924, verticesSmallAlpha13, normalsSmallAlpha13);
	FOO(768, verticesSmallAlpha14, normalsSmallAlpha14);
	FOO(1152, verticesSmallAlpha15, normalsSmallAlpha15);
	FOO(1104, verticesSmallAlpha16, normalsSmallAlpha16);
	FOO(900, verticesSmallAlpha17, normalsSmallAlpha17);
	FOO(1500, verticesSmallAlpha18, normalsSmallAlpha18);
	FOO(1140, verticesSmallAlpha19, normalsSmallAlpha19);
	FOO(888, verticesSmallAlpha20, normalsSmallAlpha20);
	FOO(600, verticesSmallAlpha21, normalsSmallAlpha21);
	FOO(636, verticesSmallAlpha22, normalsSmallAlpha22);
	FOO(900, verticesSmallAlpha23, normalsSmallAlpha23);
	FOO(888, verticesSmallAlpha24, normalsSmallAlpha24);
	FOO(360, verticesSmallAlpha25, normalsSmallAlpha25);

	fprintf(stderr, "\n%d -> %d\n", gProcessedPoints, gKnownPoints);

	return 0;
}
