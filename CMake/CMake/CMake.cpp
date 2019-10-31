// CMake.cpp: определяет точку входа для приложения.
//

#include <vector>

#include "CMake.h"
#include "VectorParser.h"
#include "ConvexHull3D.h"

using namespace std;

int main()
{
	std::vector<std::vector<float>> vectorBase;
	parser::make_vector_from_text<float>("D:\\object.pts", vectorBase, -1, -1, 32);
	cout << "Hello CMake." << endl;
	system("pause");
	return 0;
}
