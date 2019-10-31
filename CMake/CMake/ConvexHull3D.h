#pragma once

#ifndef  __CONVEX_HULL__
#define __CONVEX_HULL__
#include <vector>
#include <list>

namespace convex_hull
{
	#define X   0
	#define Y   1
	#define Z   2

	template <typename T>
	struct tVertex;

	template <typename T>
	struct tEdge;

	template <typename T>
	struct tFace;

	template <typename T>
	void DoubleTriangle(std::list<tVertex<T>>& vertices);

	template <typename T>
	bool Collinear(std::vector<T>& va, std::vector<T>& vb, std::vector<T>& vc);

	template <typename T>
	tFace<T>* MakeFace(tVertex<T>& v0, tVertex<T>& v1, tVertex<T>& v2, tFace<T>* fold, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	tEdge<T>* MakeNullEdge(std::list<tEdge<T>>& edges);

	template <typename T>
	tFace<T>* MakeNullFace(std::list<tFace<T>>& faces);

	template <typename T>
	struct tVertex
	{
		std::vector<T> v = { 0.f, 0.f, 0.f };
		int vnum = 0;
		tEdge duplicate;
		bool onhull = false;
		bool mark = false;
		tVertex *next = nullptr;
		tVertex *prev = nullptr;
	};

	template <typename T>
	struct tEdge
	{
		std::vector<tFace<T>*> adjface = { nullptr, nullptr };
		std::vector<tVertex<T>*> endpts = { nullptr, nullptr };
		tFace* newface = nullptr;
		bool deleted = false;
		tEdge *next = nullptr;
		tEdge *prev = nullptr;
	};

	template <typename T>
	struct tFace
	{
		std::vector<tEdge<T>*> edge = { nullptr, nullptr, nullptr };
		std::vector<tVertex<T>*> vertex = { nullptr, nullptr, nullptr };
		bool visible = false;
		tFace *next = nullptr;
		tFace *prev = nullptr;
	};

	template <typename T>
	void DoubleTriangle(std::list<tVertex<T>>& vertices)
	{
		tVertex v0, v1, v2, v3, t;
		tFace f0, f1;
		tEdge e0, e1, e2, s;
		int vol;

		for (tVertex<T> v: vertices)
		{
			if (v.next != nullptr && v.next->next != nullptr)
			{
				if (!Collinear(v.v, v.next->v, v.next->next.v))
				{
					v0 = v;
					break;
				}
			}
		}

		if (v0.next != nullptr || v0.next->next != nullptr)
		{
			return;
		}

		v1 = v0.next;
		v2 = v1.next;

		v0.mark = true;
		v1.mark = true;
		v2.mark = true;


	}

	template <typename T>
	bool Collinear(std::vector<T>& va, std::vector<T>& vb, std::vector<T>& vc)
	{
		return
			(c->v[Z] - a->v[Z]) * (b->v[Y] - a->v[Y]) -
			(b->v[Z] - a->v[Z]) * (c->v[Y] - a->v[Y]) == 0
			&& (b->v[Z] - a->v[Z]) * (c->v[X] - a->v[X]) -
			(b->v[X] - a->v[X]) * (c->v[Z] - a->v[Z]) == 0
			&& (b->v[X] - a->v[X]) * (c->v[Y] - a->v[Y]) -
			(b->v[Y] - a->v[Y]) * (c->v[X] - a->v[X]) == 0;
	}

	template <typename T>
	tEdge<T>* MakeNullEdge(std::list<tEdge<T>>& edges)
	{
		tEdge<T> e;
		edges.push_back(e);
		return edges.back();
	}

	template <typename T>
	tFace<T>* MakeNullFace(std::list<tFace<T>>& faces)
	{
		tFace<T> f;
		faces.push_back(f);
		return faces.back();
	}

	template <typename T>
	tFace<T>* MakeFace(tVertex<T>& v0, tVertex<T>& v1, tVertex<T>& v2, tFace<T>* fold, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		tFace<T>* f;
		tEdge<T>* e0, e1, e2;

		if (fold = nullptr)
		{
			e0 = MakeNullEdge(edges);
			e1 = MakeNullEdge(edges);
			e2 = MakeNullEdge(edges);
		}
		else
		{
			e0 = fold->edge[2];
			e1 = fold->edge[1];
			e2 = fold->edge[0];
		}

		e0->endpts[0] = &v0; e0->endpts[1] = &v1;
		e1->endpts[0] = &v1; e1->endpts[1] = &v2;
		e2->endpts[0] = &v2; e2->endpts[1] = &v0;

		f = MakeNullFace(faces);

		f->edge[0] = e0; 
		f->edge[1] = e1;
		f->edge[2] = e2;

		f->vertex[0] = v0;
		f->vertex[1] = v1;
		f->vertex[2] = v2;

		e0->adjface[0] = e1->adjface[0] = e->adjface[0] = f;

		return f;
	}
}

#endif
