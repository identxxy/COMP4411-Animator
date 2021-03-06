#include "BsplineCurveEvaluator.h"
#include <assert.h>
#include "vec.h"
#include "mat.h"

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const 
{
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	float interval = 0.001;
	Mat4<float> BtoV = Mat4<float>(1, 4, 1, 0, 0, 4, 2, 0, 0, 2, 4, 0, 0, 1, 4, 1) / 6;
	Mat4<float> M(-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0);
	std::vector<Point> ControlPoints;
	if (bWrap) {
		if (ptvCtrlPts.size() > 2) {
			Point p1(ptvCtrlPts[iCtrlPtCount - 3].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 3].y);
			ControlPoints.push_back(p1);
		}
		Point p2(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 2].y);
		Point p3(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
		Point p4(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		Point p5(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
		ControlPoints.push_back(p2);
		ControlPoints.push_back(p3);
		ControlPoints.insert(ControlPoints.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
		ControlPoints.push_back(p4);
		ControlPoints.push_back(p5);
		if (ptvCtrlPts.size() > 2) {
			Point p6(ptvCtrlPts[2].x + fAniLength, ptvCtrlPts[2].y);
			ControlPoints.push_back(p6);
		}
	}
	else {
		Point p1(ptvCtrlPts[0].x, ptvCtrlPts[0].y);
		Point p2(ptvCtrlPts[0].x, ptvCtrlPts[0].y);
		Point p3(ptvCtrlPts[0].x, ptvCtrlPts[0].y);
		Point p4(ptvCtrlPts[iCtrlPtCount - 1].x, ptvCtrlPts[iCtrlPtCount - 1].y);
		Point p5(ptvCtrlPts[iCtrlPtCount - 1].x, ptvCtrlPts[iCtrlPtCount - 1].y);
		Point p6(ptvCtrlPts[iCtrlPtCount - 1].x, ptvCtrlPts[iCtrlPtCount - 1].y);
		ControlPoints.push_back(p1);
		ControlPoints.push_back(p2);
		ControlPoints.push_back(p3);
		ControlPoints.insert(ControlPoints.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
		if (ptvCtrlPts.size() != 2) {
			ControlPoints.push_back(p4);
			ControlPoints.push_back(p5);
			ControlPoints.push_back(p6);
		}
	}
	
	for (vector<Point>::const_iterator i = ControlPoints.begin(); i != ControlPoints.end(); i++) {
		if (i < (ControlPoints.end() - 3)) {
			Vec4<float> Bx((*i).x, (*(i + 1)).x, (*(i + 2)).x, (*(i + 3)).x);
			Vec4<float> By((*i).y, (*(i + 1)).y, (*(i + 2)).y, (*(i + 3)).y);
			Vec4<float> Vx = BtoV * Bx;
			Vec4<float> Vy = BtoV * By;
			for (float t = 0; t <= 1; t += interval) {
				Vec4<float> T(t * t * t, t * t, t, 1);
				float Qx = T * (M * Vx);
				float Qy = T * (M * Vy);
				ptvEvaluatedCurvePts.push_back(Point(Qx, Qy));
			}
		}
	}
	if (!bWrap) {
		float x1 = 0.0;
		float y1 = ptvCtrlPts[0].y;
		ptvEvaluatedCurvePts.push_back(Point(x1, y1));

		float x2 = fAniLength;
		float y2 = ptvCtrlPts[iCtrlPtCount - 1].y;
		ptvEvaluatedCurvePts.push_back(Point(x2, y2));
	}
}