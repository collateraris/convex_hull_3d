// CMake.cpp: определяет точку входа для приложения.
//

#include <vector>
#include <list>

#include "CMake.h"
#include "VectorParser.h"
#include "ConvexHull3D.h"

using namespace std;

void convertVectorToverticesList(std::vector<std::vector<float>>& vectorBase, std::list<convex_hull::tVertex<float>>& vertices);

int main()
{
	std::vector<std::vector<float>> vectorBase;
	parser::make_vector_from_text<float>("D:\\object.pts", vectorBase, -1, -1, 32);

	std::list<convex_hull::tVertex<float>> vertices;
	std::list<convex_hull::tEdge<float>> edges;
	std::list<convex_hull::tFace<float>> faces;
	
	vectorBase.pop_back();
	convertVectorToverticesList(vectorBase, vertices);
	convex_hull::DoubleTriangle<float>(vertices, faces, edges);
	cout << "Hello CMake." << endl;
	system("pause");
	return 0;
}

void convertVectorToverticesList(std::vector<std::vector<float>>& vectorBase, std::list<convex_hull::tVertex<float>>& vertices)
{
	int num = 0;
	for (std::vector<float>& vertex: vectorBase)
	{
		convex_hull::tVertex<float>* v = convex_hull::MakeNullVertex<float>(vertices);
		v->v[X] = vertex[X];
		v->v[Y] = vertex[Y];
		v->v[Z] = vertex[Z];
		v->vnum = num++;
	}
}
