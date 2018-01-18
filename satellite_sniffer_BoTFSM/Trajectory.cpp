#include "stdafx.h"
#include "Trajectory.h"
#include "Satellite.h"
#include "Globals.h"
#include "Resources.h"
#include <SDL2_gfxPrimitives.h>

Trajectory::Trajectory(Satellite & sat, Direction direction) :
	sat(sat),
	direction(direction),
	isTextureValid(false),
	texture(nullptr)
{
}

void Trajectory::calculate(std::time_t time)
{
	points.clear();

	int cntr(0);
	double fullDistance(0.0);

	do {
		points.emplace_back(sat.getPositionAtTime(time));
		time += direction * sat.getDelta(time);
		if (points.size() >= 2) fullDistance += getDistance(points[cntr], points[cntr - 1]);
		cntr++;
	} while (points.size() < 2 ||
		(fullDistance < TrajectoryLimits::MAX_DISTANCE &&
		 cntr < TrajectoryLimits::MAX_POINTS));

	isTextureValid = false;
}

void Trajectory::render()
{
	int width = bottomRight.x - topLeft.x + 2*TrajectoryRender::LINE_WEIGHT;
	int height = bottomRight.y - topLeft.y + 2*TrajectoryRender::LINE_WEIGHT;
	if (!isTextureValid) {
		findCorners();

		texture.reset(new Sprite(width, height));
		renderNewTexture();
		isTextureValid = true;
	}
	SDL_Rect rect = { topLeft.x - TrajectoryRender::LINE_WEIGHT, topLeft.y - TrajectoryRender::LINE_WEIGHT, width, height };
	texture->render(&rect);
}

void Trajectory::renderNewTexture()
{
	texture->setAsRenderTarget();
	auto mapSize(Resources::getInstance()->getMapDimensions());
	for (int i = 1; i < points.size(); ++i) {
		Sint16 x1(static_cast<Sint16>(round(points[i - 1].longitude / (MathConstants::PI * 2) * mapSize.w)));
		Sint16 y1(static_cast<Sint16>(round(points[i - 1].latitude / (MathConstants::PI) * mapSize.h)));
		Sint16 x2(static_cast<Sint16>(round(points[i].longitude / (MathConstants::PI * 2) * mapSize.w)));
		Sint16 y2(static_cast<Sint16>(round(points[i].latitude / (MathConstants::PI) * mapSize.h)));
		if (abs(x1 - x2) > TrajectoryLimits::RENDER_DISTANCE_GAP) {
			if (x1 > x2) {
				auto oldX2(x2);
				x2 = mapSize.w;
				renderSegment(x1, y1, x2, y2);
				x2 = oldX2;
				x1 = 0;
			}
			else {
				auto oldX2(x2);
				x2 = 0;
				renderSegment(x1, y1, x2, y2);
				x2 = oldX2;
				x1 = mapSize.w;
			}
		}
		renderSegment(x1, y1, x2, y2);
	}
	Resources::getInstance()->getMap()->setAsRenderTarget();
}

void Trajectory::renderSegment(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2)
{
	x1 -= topLeft.x - TrajectoryRender::LINE_WEIGHT;
	x2 -= topLeft.x - TrajectoryRender::LINE_WEIGHT;
	y1 -= topLeft.y - TrajectoryRender::LINE_WEIGHT;
	y2 -= topLeft.y - TrajectoryRender::LINE_WEIGHT;
	auto color(FORWARD == direction ? TrajectoryRender::FORWARD_COLOR : TrajectoryRender::BACKWARD_COLOR);
	thickLineColor(Resources::getInstance()->getRenderer(), x1, y1, x2, y2,
				   TrajectoryRender::LINE_WEIGHT, color);
}

double Trajectory::getDistance(const CoordGeodetic & a, const CoordGeodetic & b)
{
	double latDelta = a.latitude - b.latitude;
	double longDelta = a.longitude - b.longitude;
	if (abs(longDelta) > TrajectoryLimits::DISTANCE_GAP)
		longDelta = TrajectoryLimits::DISTANCE_GAP;
	return sqrt(pow(latDelta, 2) + pow(longDelta, 2));
}

void Trajectory::findCorners()
{
	auto mapSize(Resources::getInstance()->getMapDimensions());
	int minX{ mapSize.w }, maxX{ 0 }, minY{ mapSize.h }, maxY{0};
	for (int i = 0; i < points.size(); ++i) {
		Sint16 pX(static_cast<Sint16>(round(points[i].longitude / (MathConstants::PI * 2) * mapSize.w)));
		Sint16 pY(static_cast<Sint16>(round(points[i].latitude / (MathConstants::PI) * mapSize.h)));
		if (pX < minX) minX = pX;
		if (pY < minY) minY = pY;
		if (pX > maxX) maxX = pX;
		if (pY > maxY) maxY = pY;
	}
	topLeft.x = minX;
	topLeft.y = minY;
	bottomRight.x = maxX;
	bottomRight.y = maxY;
}
