/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace util
{
	constexpr double PI = 3.14159265358979323846;

	template<class T>
	struct Point2D
	{
		T x;
		T y;

		T getDistanceFromSq(Point2D<T> p) const
		{
			return ((p.x - x) * (p.x - x)) + ((p.y - y) * (p.y - y));
		}

		float getDistanceFrom(Point2D<T> p) const
		{
			return std::sqrt(getDistanceFromSq(p));
		}
	};

	template<class T>
	struct Line2D
	{
		Point2D<T> start;
		Point2D<T> end;

		T getLengthSq() const
		{
			return start.getDistanceFromSq(end);
		}

		float getLength() const
		{
			return start.getDistanceFrom(end);
		}

		bool isRight(const Point2D<T>& point) const
		{
			Point2D<T> direction{ end.x - start.x, end.y - start.y };
			T v = (direction.y * (point.x - start.x)) + (direction.x * (start.y - point.y));
			return (v <= 0);
		}

		bool insersects(const Line2D<T>& line, Point2D<T>* intersection = nullptr, bool segment_only = true) const
		{
			const float commonDenominator = 
				(float)(line.end.y - line.start.y)*(end.x - start.x) - (line.end.x - line.start.x)*(end.y - start.y);

			const float numeratorA = 
				(float)(line.end.x - line.start.x)*(start.y - line.start.y) - (line.end.y - line.start.y)*(start.x - line.start.x);

			const float numeratorB = 
				(float)(end.x - start.x)*(start.y - line.start.y) - (end.y - start.y)*(start.x - line.start.x);

			//if (equals(commonDenominator, 0.f))
			if (commonDenominator > -0.000001f && commonDenominator < 0.000001f)
				return false; // parallel or coincident (let's just not handle the latter now)

			const float uA = numeratorA / commonDenominator;
			if (segment_only && (uA < 0.f || uA > 1.f))
				return false; // Outside the line segment

			const float uB = numeratorB / commonDenominator;
			if (segment_only && (uB < 0.f || uB > 1.f))
				return false; // Outside the line segment

			if (intersection)
			{
				// Calculate the intersection point.
				intersection->x = (T)(start.x + uA * (end.x - start.x));
				intersection->y = (T)(start.y + uA * (end.y - start.y));
			}

			return true;
		}
	};

	template<class T>
	struct BoundingBox2D;

	template<class T>
	struct Polygon2D
	{
		struct IntersectionData
		{
			size_t edge_n;
			Point2D<T> point;
		};

		std::vector<Point2D<T>> points;

		void initNgon(T radius, unsigned edges, float angle_deg, Point2D<T> position)
		{
			float angle_rad = static_cast<float>((angle_deg * PI) / 180.f);
			float angle_step = static_cast<float>(PI * 2 / edges);

			for (unsigned i = 0; i < edges; ++i)
			{
				float x = std::sin(angle_rad) * radius;
				float y = -std::cos(angle_rad) * radius;
				points.push_back(Point2D<T>{ (T)x + position.x, (T)y + position.y });
				angle_rad += angle_step;
			}
		}

		T getArea() const
		{
			T area = 0;

			for (size_t i = 0, len = points.size(); i < len; ++i)
			{
				auto edge = getEdge(i);
				area += (edge.start.x + edge.end.x) * (edge.start.y - edge.end.y);
			}

			return (area / -2);
		}

		Line2D<T> getEdge(size_t n) const
		{
			return { points[n], points[(n + 1) % points.size()] };
		}

		Point2D<T> getCenter() const
		{
			Point2D<T> center = { (T)0, (T)0 };

			for (auto& p : points)
			{
				center.x += p.x;
				center.y += p.y;
			}

			center.x /= points.size();
			center.y /= points.size();

			return center;
		}

		bool isCW() const
		{
			return getArea() > (T)0;
		}

		bool isCCW() const
		{
			return getArea() < (T)0;
		}

		bool contains_fast(const Point2D<T>& point) const // only works for convex polygons!
		{
			for (size_t i = 0, len = points.size(); i != len; ++i)
			{
				Line2D<T> edge = { points[i], points[(i + 1) % len] };
				if (!edge.isRight(point))
					return false;
			}
			return true;
		}

		bool contains(const Point2D<T>& point) const
		{
			std::vector<util::Point2D<T>> intersections;

			BoundingBox2D<T> bbox;
			bbox.update(*this);
			util::Line2D<T> test_line { { bbox.min_x, point.y },{ bbox.max_x, point.y } };

			for (size_t i = 0, len = points.size(); i < len; ++i)
			{
				util::Point2D<T> intersection;
				if (getEdge(i).insersects(test_line, &intersection))
					intersections.push_back(intersection);
			}

			if (intersections.size() % 2)
				return false;

			std::sort(intersections.begin(), intersections.end(),
				[](const util::Point2D<T>& p1, const util::Point2D<T>& p2) -> bool { return p1.x < p2.x; });

			for (size_t i = 0, len = intersections.size(); i < len; ++i)
			{
				if (point.x < intersections[i].x)
				{
					if (i % 2)
						return true;
					else
						return false;
				}
			}

			return false;
		}

		void getIntersections(const util::Line2D<float>& line, std::vector<IntersectionData>& out) const
		{
			// getting intersection points
			for (size_t i = 0, len = points.size(); i < len; ++i)
			{
				util::Line2D<T> edge = getEdge(i);
				util::Point2D<T> intersection;

				if (edge.insersects(line, &intersection))
					out.push_back({ i, intersection });
			}

			if (out.size() < 2)
				return;

			// sorting points
			std::sort(out.begin(), out.end(),
				[line](const IntersectionData& p1, const IntersectionData& p2) -> bool
			{
				return p1.point.getDistanceFromSq(line.start) < p2.point.getDistanceFromSq(line.start);
			});
		}

		static void intersect(const util::Polygon2D<T>& nonconvex, const util::Polygon2D<T>& convex, std::vector<util::Point2D<T>>& out)
		{
			std::vector<IntersectionData> intersections;
			int first_ingoing_intersection = -1;
			int last_outgoing_intersection = -1;
			bool inside = nonconvex.contains(convex.points.front());

			for (size_t i = 0, len = convex.points.size(); i < len; ++i)
			{
				util::Line2D<T> line = convex.getEdge(i);

				intersections.clear();
				nonconvex.getIntersections(line, intersections);

				if (inside)
					out.push_back(line.start);

				for (auto& intersection : intersections)
				{
					if (inside) // we are going out, let's save the currently intersected edge
					{
						last_outgoing_intersection = intersection.edge_n;
					}
					else // when going back in, let's check if we left out some edges
					{
						if (first_ingoing_intersection == -1)
						{
							first_ingoing_intersection = intersection.edge_n;
						}

						if (last_outgoing_intersection != -1)
						{
							for (size_t j = last_outgoing_intersection;
							j != intersection.edge_n;
								j = ((j + 1) % nonconvex.points.size()))
							{
								out.push_back(nonconvex.getEdge(j).end);
							}
						}
					}

					inside = !inside;

					out.push_back(intersection.point);
				}
			}

			if (first_ingoing_intersection != -1 && !inside)
			{
				for (size_t j = last_outgoing_intersection;
				j != first_ingoing_intersection;
					j = ((j + 1) % nonconvex.points.size()))
				{
					out.push_back(nonconvex.getEdge(j).end);
				}
			}
		}
	};

	template<class T>
	struct BoundingBox2D
	{
		T min_x = 1;
		T min_y = 1;
		T max_x = 0;
		T max_y = 0;

		void update(const Point2D<T>& point)
		{
			if (min_x > max_x) // first point
			{
				min_x = max_x = point.x;
				min_y = max_y = point.y;
			}
			else
			{
				if (point.x < min_x) min_x = point.x;
				else if (point.x > max_x) max_x = point.x;

				if (point.y < min_y) min_y = point.y;
				else if (point.y > max_y) max_y = point.y;
			}
		}

		void update(const Line2D<T>& line)
		{
			update(line.start);
			update(line.end);
		}

		void update(const Polygon2D<T>& polygon)
		{
			for (auto p : polygon.points)
				update(p);
		}

		T getWidth() const
		{
			return max_x - min_x;
		}

		T getHeight() const
		{
			return max_y - min_y;
		}

		Point2D<T> getUpperLeftCorner() const
		{
			return { min_x, min_y };
		}

		Point2D<T> getLowerRightCorner() const
		{
			return { max_x, max_y };
		}
	};
};
