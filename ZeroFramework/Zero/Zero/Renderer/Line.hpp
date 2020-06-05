#pragma once

#include "Shape.hpp"

namespace zr
{
	class Line : public Shape
	{
	public:
		Line() :
			Shape(RendererAPI::DrawPrimitive::Triangles),
			mThickness(0.f)
		{

		}

		Line(const glm::vec2& point0, const glm::vec2& point1, float thickness = 0.f);
		virtual ~Line();

		void setPoints(const glm::vec2& point0, const glm::vec2& point1);
		void setPoint0(const glm::vec2& point);
		void setPoint1(const glm::vec2& point);

		void setThickness(float thickness)
		{
			if (thickness < 0.f) return;

			mThickness = thickness;
			updatePoints();
		}

	public:
		static std::array<glm::vec3, 4> ComputeEdgesVertices(const glm::vec2& A, const glm::vec2& B, float thickness)
		{
			std::array<glm::vec3, 4> returnValue;
			glm::vec2& diff = B - A;

			if (thickness == 0.f) {
				returnValue[0] = { A, 0.f };
				returnValue[1] = { A, 0.f };
				returnValue[2] = { B, 0.f };
				returnValue[3] = { B, 0.f };
				return returnValue;
			}

			float angleBetweenVectors = Line::ComputeVectorAngle({ 1.f, 0.f }, diff);

			float t = thickness * .5f;
			float angle = 0.f, tCos = 0.f, tSin = 0.f;

			if (diff.x >= 0) { // 1Q || 4Q
				bool is1Q = diff.y >= 0;

				angle = glm::radians(90.f) - angleBetweenVectors;
				tSin = t * glm::sin(angle);
				tCos = t * glm::cos(angle) * (is1Q ? 1 : -1);

				returnValue[0] = glm::vec3(A.x - tCos, A.y + tSin, 0.f);	// 0
				returnValue[1] = glm::vec3(A.x + tCos, A.y - tSin, 0.f);	// 1
				returnValue[2] = glm::vec3(B.x + tCos, B.y - tSin, 0.f);	// 2
				returnValue[3] = glm::vec3(B.x - tCos, B.y + tSin, 0.f);	// 3

				return returnValue;
			}
			else { // 2Q || 3Q
				bool is2Q = diff.y >= 0;

				angle = angleBetweenVectors - glm::radians(90.f);
				tCos = t * glm::cos(angle) * (is2Q ? 1 : -1);
				tSin = t * glm::sin(angle);

				returnValue[0] = glm::vec3(A.x - tCos, A.y - tSin, 0.f);	//	0
				returnValue[1] = glm::vec3(A.x + tCos, A.y + tSin, 0.f);	//	1
				returnValue[2] = glm::vec3(B.x + tCos, B.y + tSin, 0.f);	//	2
				returnValue[3] = glm::vec3(B.x - tCos, B.y - tSin, 0.f);	//	3

				return returnValue;
			}

			returnValue[0] = { A, 0.f };
			returnValue[1] = { A, 0.f };
			returnValue[2] = { B, 0.f };
			returnValue[3] = { B, 0.f };
			return returnValue;
		}

		static float ComputeVectorAngle(const glm::vec2& r1, const glm::vec2& r2, bool inRadians = true)
		{
			const auto& r1Normalized = glm::normalize(r1);
			const auto& r2Normalized = glm::normalize(r2);
			float angleBetweenVectors = glm::acos(glm::clamp(glm::dot(r1Normalized, r2Normalized), -1.f, 1.f));

			if (!inRadians) return glm::degrees(angleBetweenVectors);

			return angleBetweenVectors;
		}

	private:
		void updatePoints();

	private:
		// Inherited via Shape
		virtual void generateOutlineIndices(std::vector<unsigned>& outlineIndices, unsigned shapeVertexCount) override;
		virtual void computeVertexData() override;
		virtual void generateOutlineVertices() override;

	private:
		float mThickness;
		glm::vec2 mA;
		glm::vec2 mB;
	};

	class MultiLine : public Shape
	{
	private:
		struct LineInfo
		{
			LineInfo() :
				V0(0.f, 0.f),
				V1(0.f, 0.f),
				V2(0.f, 0.f),
				V3(0.f, 0.f),
				V4(0.f, 0.f),
				V5(0.f, 0.f)
			{

			}
			glm::vec2 V0;
			glm::vec2 V1;
			glm::vec2 V2;
			glm::vec2 V3;
			glm::vec2 V4;
			glm::vec2 V5;
		};

	public:
		MultiLine() :
			Shape(RendererAPI::DrawPrimitive::Triangles),
			mLineVertices(),
			mLineFragmentInserted(0U),
			mThickness(0.f)
		{

		}

		virtual ~MultiLine()
		{

		}

		void setVertices(const std::vector<glm::vec2>& vertices)
		{
			mLineVertices = vertices;
			computeVertices();
		}

		void setThickness(float thickness)
		{
			if (thickness < 0.f) return;

			mThickness = thickness;
			computeVertices();
		}

	public:
		virtual void generateOutlineIndices(std::vector<unsigned>& outlineIndices, unsigned shapeVertexCount) override
		{

		}

		virtual void computeVertexData() override
		{

		}

		virtual void generateOutlineVertices() override
		{

		}

	public:
		static void ComputeLineEquation(const glm::vec2& point0, const glm::vec2& point1, float& m, float& b)
		{
			float dx = point1.x - point0.x;

			if (dx == 0.f) {
				m = 0.f;
			}
			else {
				m = (point1.y - point0.y) / dx;
			}

			b = m * (-point0.x) + point0.y;
		}

		static void ComputeLinesInterceptionPoint(const glm::vec2& point00, const glm::vec2& point01, const glm::vec2& point10, const glm::vec2& point11, float& x, float& y)
		{
			float m0 = 0.f, b0 = 0.f, m1 = 0.f, b1 = 0.f;
			MultiLine::ComputeLineEquation(point00, point01, m0, b0);
			MultiLine::ComputeLineEquation(point10, point11, m1, b1);

			MultiLine::ComputeLinesInterceptionPoint(m0, b0, m1, b1, x, y);
		}

		static inline void ComputeLinesInterceptionPoint(const glm::vec2& point00, const glm::vec2& point01, const glm::vec2& point10, const glm::vec2& point11, glm::vec2& interceptionPoint)
		{
			MultiLine::ComputeLinesInterceptionPoint(point00, point01, point10, point11, interceptionPoint.x, interceptionPoint.y);
		}

		static void ComputeLinesInterceptionPoint(const float m0, const float b0, const float m1, const float b1, float& x, float& y)
		{
			if (m0 == m1) {
				// parallel lines
				x = std::numeric_limits<float>::infinity();
				y = std::numeric_limits<float>::infinity();
			}
			else {
				x = (b1 - b0) / (m0 - m1);
				y = m0 * x + b0;
			}
		}

	private:
		static LineInfo ComputeInterceptionPoints(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float thickness)
		{
			LineInfo returnValue;

			float m1 = 0.f, _ = 0.f, m2 = 0.f;
			MultiLine::ComputeLineEquation(A, B, m1, _);
			MultiLine::ComputeLineEquation(B, C, m2, _);

			if (glm::epsilonEqual(m1, m2, glm::epsilon<float>())) {
				// Lines are parallel
				const glm::vec2& newA = glm::length(B - C) >= glm::length(B - A) ? C : A;
				auto& vertices = Line::ComputeEdgesVertices(newA, B, thickness);

				returnValue.V0 = vertices[0];
				returnValue.V1 = vertices[1];
				returnValue.V2 = vertices[2];
				returnValue.V3 = vertices[3];
				returnValue.V4 = vertices[0];
				returnValue.V5 = vertices[1];
				return returnValue;
			}

			auto& r1Vertices = Line::ComputeEdgesVertices(A, B, thickness);
			auto& r2Vertices = Line::ComputeEdgesVertices(B, C, thickness);

			auto& r1 = A - B;
			auto& r2 = C - B;
			auto& diff = -r1;

			glm::vec2 r1Top[2] = { {0.f, 0.f}, {0.f, 0.f} };
			glm::vec2 r1Bottom[2] = { {0.f, 0.f}, {0.f, 0.f} };
			glm::vec2 r2Top[2] = { {0.f, 0.f}, {0.f, 0.f} };
			glm::vec2 r2Bottom[2] = { {0.f, 0.f}, {0.f, 0.f} };

			glm::vec2 externalInterceptionPoint(0.f, 0.f);
			glm::vec2 internalInterceptionPoint(0.f, 0.f);

			if (diff.x >= 0.f) {
				// r1Top line has r1Vertices[0] and r1Vertices[3]
				r1Top[0] = r1Vertices[0];		r1Top[1] = r1Vertices[3];
				// r1Bottom line has r1Vertices[1] and r1Vertices[2]
				r1Bottom[0] = r1Vertices[1];	r1Bottom[1] = r1Vertices[2];
			}
			else {
				// r1Top line has r1Vertices[1] and r1Vertices[2]
				r1Top[0] = r1Vertices[1];		r1Top[1] = r1Vertices[2];
				// r1Bottom line has r1Vertices[0] and r1Vertices[3]
				r1Bottom[0] = r1Vertices[0];	r1Bottom[1] = r1Vertices[3];
			}

			if (r2.x <= 0.f) {
				// r2Top line has r2Vertices[1] and r2Vertices[2]
				r2Top[0] = r2Vertices[1];		r2Top[1] = r2Vertices[2];
				// r2Bottom line has r2Vertices[0] and r2Vertices[3]
				r2Bottom[0] = r2Vertices[0];	r2Bottom[1] = r2Vertices[3];

				if (r2.y <= 0.f) {
					if (diff.x >= 0.f) {	// N:4
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Bottom[0], r2Bottom[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Top[0], r2Top[1], internalInterceptionPoint);

						returnValue.V0 = r1Top[0];
						returnValue.V1 = r1Bottom[0];
						returnValue.V2 = internalInterceptionPoint;
						returnValue.V3 = r2Top[1];
						returnValue.V4 = r2Bottom[1];
						returnValue.V5 = externalInterceptionPoint;
					}
					else {	// N:8
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Top[0], r2Top[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Bottom[0], r2Bottom[1], internalInterceptionPoint);

						returnValue.V0 = r1Bottom[0];
						returnValue.V1 = r1Top[0];
						returnValue.V2 = externalInterceptionPoint;
						returnValue.V3 = r2Top[1];
						returnValue.V4 = r2Bottom[1];
						returnValue.V5 = internalInterceptionPoint;
					}
				}
				else {
					if (diff.x >= 0.f) {	// N:1
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Top[0], r2Top[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Bottom[0], r2Bottom[1], internalInterceptionPoint);

						returnValue.V0 = r1Top[0];
						returnValue.V1 = r1Bottom[0];
						returnValue.V2 = externalInterceptionPoint;
						returnValue.V3 = r2Top[1];		//
						returnValue.V4 = r2Bottom[1];	//
						returnValue.V5 = internalInterceptionPoint;
					}
					else {	// N:5
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Bottom[0], r2Bottom[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Top[0], r2Top[1], internalInterceptionPoint);

						returnValue.V0 = r1Bottom[0];
						returnValue.V1 = r1Top[0];
						returnValue.V2 = internalInterceptionPoint;
						returnValue.V3 = r2Top[1];
						returnValue.V4 = r2Bottom[1];
						returnValue.V5 = externalInterceptionPoint;
					}
				}
			}
			else {
				// r2Top line has r2Vertices[0] and r2Vertices[3]
				r2Top[0] = r2Vertices[0];		r2Top[1] = r2Vertices[3];
				// r2Bottom line has r2Vertices[1] and r2Vertices[2]
				r2Bottom[0] = r2Vertices[1];	r2Bottom[1] = r2Vertices[2];

				if (r2.y <= 0.f) {
					if (diff.x >= 0.f) {	// N:3
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Top[0], r2Top[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Bottom[0], r2Bottom[1], internalInterceptionPoint);

						returnValue.V0 = r1Top[0];
						returnValue.V1 = r1Bottom[0];
						returnValue.V2 = internalInterceptionPoint;
						returnValue.V3 = r2Bottom[1];
						returnValue.V4 = r2Top[1];
						returnValue.V5 = externalInterceptionPoint;
					}
					else {	// N:7
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Bottom[0], r2Bottom[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Top[0], r2Top[1], internalInterceptionPoint);

						returnValue.V0 = r1Bottom[0];
						returnValue.V1 = r1Top[0];
						returnValue.V2 = externalInterceptionPoint;
						returnValue.V3 = r2Bottom[1];
						returnValue.V4 = r2Top[1];
						returnValue.V5 = internalInterceptionPoint;
					}
				}
				else {
					if (diff.x >= 0.f) { //	N:2
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Bottom[0], r2Bottom[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Top[0], r2Top[1], internalInterceptionPoint);

						returnValue.V0 = r1Top[0];
						returnValue.V1 = r1Bottom[0];
						returnValue.V2 = externalInterceptionPoint;
						returnValue.V3 = r2Bottom[1];
						returnValue.V4 = r2Top[1];
						returnValue.V5 = internalInterceptionPoint;
					}
					else { // N:6
						MultiLine::ComputeLinesInterceptionPoint(r1Bottom[0], r1Bottom[1], r2Top[0], r2Top[1], externalInterceptionPoint);
						MultiLine::ComputeLinesInterceptionPoint(r1Top[0], r1Top[1], r2Bottom[0], r2Bottom[1], internalInterceptionPoint);

						returnValue.V0 = r1Bottom[0];
						returnValue.V1 = r1Top[0];
						returnValue.V2 = internalInterceptionPoint;
						returnValue.V3 = r2Bottom[1];
						returnValue.V4 = r2Top[1];
						returnValue.V5 = externalInterceptionPoint;
					}
				}
			}

			return returnValue;
		}

	private:
		void computeVertices()
		{
			mVertices.clear();
			mIndices.clear();
			mLineFragmentInserted = 0U;

			if (mLineVertices.size() > 2) {
				if (mLineVertices[0] == mLineVertices[mLineVertices.size() - 1]) {	// loop
					std::vector<LineInfo> linesInfo;
					linesInfo.reserve(mLineVertices.size() - 1);

					for (unsigned i = 0; i < mLineVertices.size() - 2; ++i) {
						linesInfo.emplace_back(MultiLine::ComputeInterceptionPoints(mLineVertices[i], mLineVertices[i + 1], mLineVertices[i + 2], mThickness));
					}

					unsigned size = (unsigned)mLineVertices.size();
					linesInfo.emplace_back(MultiLine::ComputeInterceptionPoints(mLineVertices[size - 2], mLineVertices[size - 1], mLineVertices[1], mThickness));
					linesInfo.emplace_back(MultiLine::ComputeInterceptionPoints(mLineVertices[size - 1], mLineVertices[1], mLineVertices[2], mThickness));

					for (unsigned i = 0; i < linesInfo.size() - 1; ++i) {
						const LineInfo& line1 = linesInfo[i + 0];
						const LineInfo& line2 = linesInfo[i + 1];

						insertLineVertices(line1.V5, line1.V2, line1.V3, line1.V4);
						insertLineVertices(line2.V0, line2.V1, line2.V2, line2.V5);
					}
				}
				else {
					std::vector<LineInfo> linesInfo;
					linesInfo.reserve(mLineVertices.size() - 2);

					for (unsigned i = 0; i < mLineVertices.size() - 2; ++i) {
						linesInfo.emplace_back(MultiLine::ComputeInterceptionPoints(mLineVertices[i], mLineVertices[i + 1], mLineVertices[i + 2], mThickness));
					}

					// first and last lines info
					const auto& firstLine = linesInfo[0];
					const auto& lastLine = linesInfo[linesInfo.size() - 1];

					// insert first line vertices
					insertLineVertices(firstLine.V0, firstLine.V1, firstLine.V2, firstLine.V5);

					for (unsigned i = 0; i < linesInfo.size() - 1; ++i) {
						const LineInfo& line1 = linesInfo[i + 0];
						const LineInfo& line2 = linesInfo[i + 1];

						insertLineVertices(line1.V5, line1.V2, line1.V3, line1.V4);
						insertLineVertices(line2.V0, line2.V1, line2.V2, line2.V5);
					}

					// insert last line vetices
					insertLineVertices(lastLine.V5, lastLine.V2, lastLine.V3, lastLine.V4);
				}
			}
			else if (mLineVertices.size() == 2) {
				const auto& lineVertices = Line::ComputeEdgesVertices(mLineVertices[0], mLineVertices[1], mThickness);
				insertLineVertices(lineVertices[0], lineVertices[1], lineVertices[2], lineVertices[3]);
			}
		}

		void insertLineVertices(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3)
		{
			mVertices.emplace_back(v0, 0.f);
			mVertices.emplace_back(v1, 0.f);
			mVertices.emplace_back(v2, 0.f);
			mVertices.emplace_back(v3, 0.f);

			std::vector<uint32_t> indices{ 0, 1, 2, 2, 3, 0 };
			std::transform(indices.begin(), indices.end(), std::back_inserter(mIndices), [&](unsigned index) {
				return mLineFragmentInserted + index;
			});

			mLineFragmentInserted += 4U;
		}

	private:
		std::vector<glm::vec2> mLineVertices;	// vertices
		unsigned mLineFragmentInserted;
		float mThickness;
	};
}
