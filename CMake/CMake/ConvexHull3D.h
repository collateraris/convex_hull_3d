#pragma once

#ifndef  __CONVEX_HULL__
#define __CONVEX_HULL__
#include <vector>
#include <list>
#include <iterator>

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

	template <typename T>
	void CleanUp(tVertex<T> &vNext, std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	void CleanEdges(std::list<tEdge<T>>& edges);

	template <typename T>
	void CleanFaces(std::list<tFace<T>>& faces);

	template <typename T>
	void CleanVertices(tVertex<T> &vNext, std::list<tVertex<T>>& vertices, std::list<tEdge<T>>& edges);

	template<typename T>
	bool AddOne(tVertex<T>&v, std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	tFace<T>* MakeConeFace(tEdge<T>& e, tVertex<T>& v, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges);

	template <typename T>
	void MakeCcw(tFace<T>& f, tEdge<T>& e, tVertex<T>& v);

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
		tEdge<T>* duplicate  = nullptr;
		bool onhull = false;
		bool mark = false;
		bool deleted = false;
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
		bool deleted = false;
	};

	template <typename T>
	void DoubleTriangle(std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		tVertex<T> *v0 = nullptr, *v1 = nullptr, *v2 = nullptr, *v3 = nullptr, *t = nullptr;
		tFace<T> *f0 = nullptr, *f1 = nullptr;
		tEdge<T> e0, e1, e2, s;
		int8_t vol;
		std::list<tVertex<T>>::iterator it = vertices.begin();
		for( ; it != vertices.end(); )
		{
			if (!Collinear<T>(*it->v, *(++it)->v, *(++it)->v))
			{
				*v0 = *it;
				break;
			}
			--it;
		}

		*v1 = *(++it);
		*v2 = *(++it);

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
		vol = VolumeSign<T>(f0, v3);

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
		for( std::list<tVertex<T>>::iterator it = vertices.begin(); it != vertices.end(); )
		{
			if (!*it->mark)
			{
				*it->mark = true;
				AddOne<T>(*it, vertices, faces, edges);
				CleanUp<T>(*(++it), vertices, faces, edges);
			}
		}
	}

	template<typename T>
	bool AddOne(tVertex<T>&v, std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		bool visible = false;
		int8_t volSign = 0;

		for (tFace<T>& f : faces)
		{
			volSign = VolumeSign<T>(&f, &v);
			if (volSign < 0)
			{
				f.visible = true;
				visible = true;
			}
		}

		if (!visible)
		{
			return false;
		}

		for (tEdge<T>& e : edges)
		{
			if (e.adjface[0]->visible && e.adjface[1]->visible)
			{
				e.deleted = true;
			}
			else if (e.adjface[0]->visible || e.adjface[1]->visible)
			{
				e.newface = MakeConeFace<T>(e, v, faces, edges);
			}
		}

		return true;
	}

	template <typename T>
	tFace<T>* MakeConeFace(tEdge<T>& e, tVertex<T>& v, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		std::vector<tEdge<T>*> newEdges = {nullptr, nullptr};
		tFace<T>* newFace = nullptr;
		for(std::size_t i = 0; i < newEdges.size(); ++i)
		{
			if (e.endpts[i]->duplicate == nullptr)
			{
				newEdges[i] = MakeNullEdge(edges);
				newEdges[i]->endpts[0] = e.endpts[i];
				newEdges[i]->endpts[1] = &v;
				e.endpts[i]->duplicate = newEdges[i];
			}
		}

		newFace = MakeNullFace<T>(faces);
		newFace->edge[0] = &e;
		newFace->edge[1] = newEdges[0];
		newFace->edge[2] = newEdges[1];
		MakeCcw<T>(*newFace, e, v);

		for (std::size_t i = 0; i < 2; ++i)
		{
			for (std::size_t j = 0; j < 2; ++j)
			{
				if (newEdges[i]->adjface[j] == nullptr)
				{
					newEdges[i]->adjface[j] = newFace;
					break;
				}
			}
		}
		return newFace;
	}

	template <typename T>
	void MakeCcw(tFace<T>& f, tEdge<T>& e, tVertex<T>& v)
	{
		tFace<T> *fv = nullptr;

		fv = e.adjface[0]->visible ? e.adjface[0] : e.adjface[1];

		std::size_t i = 0;
		for ( ; fv->vertex[i] != e.endpts[0]; ++i);

		if (fv->vertex[ (i+1)%3] != e.endpts[1])
		{
			f.vertex[0] = e.endpts[1];
			f.vertex[1] = e.endpts[0];
		}
		else
		{
			f.vertex[0] = e.endpts[0];
			f.vertex[1] = e.endpts[1];
			std::swap(f.edge[1], f.edge[2]);
		}

		f.vertex[2] = &v;
	}

	template <typename T>
	void CleanUp(tVertex<T> &vNext, std::list<tVertex<T>>& vertices, std::list<tFace<T>>& faces, std::list<tEdge<T>>& edges)
	{
		CleanEdges<T>(edges);
		CleanFaces<T>(faces);
		CleanVertices<T>(vNext, vertices);
	}

	template <typename T>
	void CleanEdges(std::list<tEdge<T>>& edges)
	{
		for (tEdge<T>& e: edges)
		{
			if (e.newface != nullptr)
			{
				if ( e.adjface[0]->visible)
				{
					e.adjface[0] = e.newface;
				}
				else
				{
					e.adjface[1] = e.newface;
					e.newface = nullptr;
				}
			}
		}

		for (std::list<tEdge<T>>::iterator it = edges.begin(); it!= edges.end(); ++it)
		{
			if (*it->deleted)
			{
				edges.erase(it);
			}
		}
	}

	template <typename T>
	void CleanFaces(std::list<tFace<T>>& faces)
	{
		for (std::list<tFace<T>>::iterator it = faces.begin(); it != faces.end(); ++it)
		{
			if (*it->visible)
			{
				faces.erase(it);
			}
		}
	}

	template <typename T>
	void CleanVertices(tVertex<T> &vNext, std::list<tVertex<T>>& vertices, std::list<tEdge<T>>& edges)
	{
		for (tEdge<T>& e: edges)
		{
			e.endpts[0]->onhull = e.endpts[1]->onhull = true;
		}

		for (std::list<tVertex<T>>::iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			if (*it->mark && !*it->onhull)
			{
				if (*it == vNext)
				{
					*vNext = *(++it);
					--it;
				}
				vertices.erase(it);
			}
		}

		for (tVertex<T>& v: vertices)
		{
			v.duplicate = nullptr;
			v.onhull = false;
		}

	}
}

#endif
