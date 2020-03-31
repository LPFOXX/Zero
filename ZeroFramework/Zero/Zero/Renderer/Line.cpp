#include <zr_pch.h>

#include "Line.hpp"

namespace zr
{
	Line::Line(const glm::vec2& point0, const glm::vec2& point1, float thickness) :
		Shape(RendererAPI::DrawPrimitive::Triangles),
		mThickness(0.f),
		mA(point0),
		mB(point1)
	{
		updatePoints();
	}

	Line::~Line()
	{

	}

	void Line::setPoints(const glm::vec2& point0, const glm::vec2& point1)
	{
		mA = point0;
		mB = point1;

		updatePoints();
	}

	void Line::setPoint0(const glm::vec2& point)
	{
		mA = point;
		updatePoints();
	}

	void Line::setPoint1(const glm::vec2& point)
	{
		mB = point;
		updatePoints();
	}

	void Line::updatePoints()
	{
		mVertices.clear();
		mIndices.clear();

		auto& vertices = Line::ComputeEdgesVertices(mA, mB, mThickness);

		mVertices.insert(mVertices.end(), vertices.begin(), vertices.end());
		mIndices.insert(mIndices.end(), { 0, 1, 2, 2, 3, 0 });

		//glm::vec2& diff = mB - mA;

		//const auto& normalized = glm::normalize(mB - mA);
		//float angleBetweenVectors = glm::acos(glm::clamp(glm::dot(glm::vec2(1.f, 0.f), normalized), -1.f, 1.f));

		//float t = mThickness * .5f;
		//float angle = 0.f, tCos = 0.f, tSin = 0.f;

		//if (diff.x >= 0) { // 1Q || 4Q
		//	bool is1Q = diff.y >= 0;

		//	angle = glm::radians(90.f) - angleBetweenVectors;
		//	tSin = t * glm::sin(angle);
		//	tCos = t * glm::cos(angle) * (is1Q ? 1 : -1);

		//	mVertices.emplace_back(mA.x - tCos, mA.y + tSin, 0.f);	// 0
		//	mVertices.emplace_back(mA.x + tCos, mA.y - tSin, 0.f);	// 1
		//	mVertices.emplace_back(mB.x + tCos, mB.y - tSin, 0.f);	// 2
		//	mVertices.emplace_back(mB.x - tCos, mB.y + tSin, 0.f);	// 3

		//	mIndices.insert(mIndices.end(), { 0, 1, 2, 2, 3, 0 });
		//}
		//else { // 2Q || 3Q
		//	bool is2Q = diff.y >= 0;

		//	angle = angleBetweenVectors - glm::radians(90.f);
		//	tCos = t * glm::cos(angle) * (is2Q ? 1 : -1);
		//	tSin = t * glm::sin(angle);

		//	mVertices.emplace_back(mA.x - tCos, mA.y - tSin, 0.f);	//	0
		//	mVertices.emplace_back(mA.x + tCos, mA.y + tSin, 0.f);	//	1
		//	mVertices.emplace_back(mB.x + tCos, mB.y + tSin, 0.f);	//	2
		//	mVertices.emplace_back(mB.x - tCos, mB.y - tSin, 0.f);	//	3

		//	mIndices.insert(mIndices.end(), { 0, 1, 2, 2, 3, 0 });
		//}
	}

	void Line::generateOutlineIndices(std::vector<unsigned>& outlineIndices, unsigned shapeVertexCount)
	{
	}

	void Line::computeVertexData()
	{
	}

	void Line::generateOutlineVertices()
	{
	}
}
