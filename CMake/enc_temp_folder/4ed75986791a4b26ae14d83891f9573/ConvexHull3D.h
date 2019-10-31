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
	void DoubleTriangle(std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	bool Collinear(std::vector<T>& a, std::vector<T>& b, std::vector<T>& c);

	template <typename T>
	void ConstructHull(std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template<typename T>
	bool AddOne(tVertex<T>&v, std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	tFace<T>* MakeFace(tVertex<T>& v0, tVertex<T>& v1, tVertex<T>& v2, tFace<T>* fold, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	tEdge<T>* MakeNullEdge(std::list<tEdge<T>>& edges);

	template <typename T>
	tFace<T>* MakeNullFace(std::list<tFace<T>>& faces);

	template <typename T>
	int8_t VolumeSign(tFace<T>* f, tVertex<T>* p);

	template <typename T>
	tVertex<T>* MakeNullVertex(std::list<tVertex<T>>& vertices);

	template <typename T>
	struct tVertex
	{
		std::vector<T> v = { 0.f, 0.f, 0.f };
		int vnum = 0;
		tEdge<T> duplicate;
		bool onhull = false;
		bool mark = false;
		tVertex<T> *next = nullptr;
		tVertex<T> *prev = nullptr;
	};

	template <typename T>
	struct tEdge
	{
		std::vector<tFace<T>*> adjface = { nullptr, nullptr };
		std::vector<tVertex<T>*> endpts = { nullptr, nullptr };
		tFace<T>* newface = nullptr;
		bool deleted = false;
		tEdge<T> *next = nullptr;
		tEdge<T> *prev = nullptr;
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
	void DoubleTriangle(std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		tVertex<T> *v0 = nullptr, *v1 = nullptr, *v2 = nullptr, *v3 = nullptr, *t = nullptr;
		tFace<T> *f0 = nullptr, *f1 = nullptr;
		tEdge<T> e0, e1, e2, s;
		int8_t vol;

		for (tVertex<T>& ver: vertices)
		{
			if (ver.next != nullptr && ver.next->next != nullptr)
			{
				if (!Collinear<T>(ver.v, ver.next->v, ver.next->next->v))
				{
					v0 = &ver;
					break;
				}
			}
		}

		if (v0->next == nullptr || v0->next->next == nullptr)
		{
			return;
		}

		v1 = v0->next;
		v2 = v1->next;

		v0->mark = true;
		v1->mark = true;
		v2->mark = true;

		f0 = MakeFace<T>(*v0, *v1, *v2, f1, faces, edges);
		f1 = MakeFace<T>(*v2, *v1, *v0, f0, faces, edges);

		f0->edge[0]->adjface[1] = f1;
		f0->edge[1]->adjface[1] = f1;
		f0->edge[2]->adjface[1] = f1;
		f1->edge[0]->adjface[1] = f0;
		f1->edge[1]->adjface[1] = f0;
		f1->edge[2]->adjface[1] = f0;

		v3 = v2->next;
		vol = VolumeSign<T>( f0, v3);

		while (vol < 1)
		{
			v3 = v3->next;
			vol = VolumeSign<T>(f0, v3);
		}

		vertices.front() = *v3;
	}

	template <typename T>
	bool Collinear(std::vector<T>& a, std::vector<T>& b, std::vector<T>& c)
	{
		return
			static_cast<int>((c[Z] - a[Z]) * (b[Y] - a[Y]) -
			(b[Z] - a[Z]) * (c[Y] - a[Y])) == 0
			&& static_cast<int>((b[Z] - a[Z]) * (c[X] - a[X]) -
			(b[X] - a[X]) * (c[Z] - a[Z])) == 0
			&& static_cast<int>((b[X] - a[X]) * (c[Y] - a[Y]) -
			(b[Y] - a[Y]) * (c[X] - a[X])) == 0;
	}

	template <typename T>
	tEdge<T>* MakeNullEdge(std::list<tEdge<T>>& edges)
	{
		tEdge<T> e;
		if (edges.empty())
		{
			edges.push_back(e);
			edges.back().next = edges.back().prev = &edges.back();
		}
		else
		{
			tEdge<T>& old_back = edges.back();
			edges.push_back(e);
			old_back.next = &edges.back();
		}
		return &edges.back();
	}

	template <typename T>
	tFace<T>* MakeNullFace(std::list<tFace<T>>& faces)
	{
		tFace<T> f;
		if (faces.empty())
		{
			faces.push_back(f);
			faces.back().next = faces.back().prev = &faces.back();
		}
		else
		{
			tFace<T>& old_back = faces.back();
			faces.push_back(f);
			old_back.next = &faces.back();
		}
		return &faces.back();
	}

	template <typename T>
	tVertex<T>* MakeNullVertex(std::list<tVertex<T>>& vertices)
	{
		tVertex<T> v;
		if (vertices.empty())
		{
			vertices.push_back(v);
			vertices.back().next = vertices.back().prev = &vertices.back();
		}
		else
		{
			tVertex<T>& old_back = vertices.back();
			vertices.push_back(v);
			old_back.next = &vertices.back();
		}
		return &vertices.back();
	}

	template <typename T>
	tFace<T>* MakeFace(tVertex<T>& v0, tVertex<T>& v1, tVertex<T>& v2, tFace<T>* fold, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		tFace<T>* f;
		tEdge<T> *e0, *e1, *e2;

		if (fold == nullptr)
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

		f->vertex[0] = &v0;
		f->vertex[1] = &v1;
		f->vertex[2] = &v2;

		e0->adjface[0] = e1->adjface[0] = e2->adjface[0] = f;

		return f;
	}

	template <typename T>
	int8_t VolumeSign(tFace<T>* f, tVertex<T>* p)
	{
		double vol;
		double ax, ay, az, bx, by, bz, cx, cy, cz;

		ax = f->vertex[0]->v[X] - p->v[X];
		ay = f->vertex[0]->v[Y] - p->v[Y];
		az = f->vertex[0]->v[Z] - p->v[Z];

		bx = f->vertex[1]->v[X] - p->v[X];
		by = f->vertex[1]->v[Y] - p->v[Y];
		bz = f->vertex[1]->v[Z] - p->v[Z];

		cx = f->vertex[2]->v[X] - p->v[X];
		cy = f->vertex[2]->v[Y] - p->v[Y];
		cz = f->vertex[2]->v[Z] - p->v[Z];

		vol = ax * (by * cz - bz * cy)
			+ ay * (bz * cx - bx * cz)
			+ az * (bx * cy - by * cx);

		if (vol > 0.5) return 1;
		if (vol < -0.5) return -1;
		return 0;
	}

	template <typename T>
	void ConstructHull(std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{

	}

	template<typename T>
	bool AddOne(tVertex<T>&v, std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		return false;
	}
}

#endif
