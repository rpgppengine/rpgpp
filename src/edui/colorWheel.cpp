#include "edui/colorWheel.hpp"

#include <cmath>
#include <memory>

#include "edui/helper.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

using namespace edui;

ColorWheel::ColorWheel() { render = std::make_unique<ColorWheelRender>(); }

void ColorWheel::update() {
	float pointScale = (rect.width / 2);
	Vector2 center = {rect.x + (rect.width / 2), rect.y + (rect.height / 2)};
	float value = 1.0f;

	if (holdingMouse) {
		circlePosition = GetMousePosition();

		float distance = Vector2Distance(center, circlePosition) / pointScale;

		float angle =
			((Vector2Angle((Vector2){0.0f, -pointScale}, Vector2Subtract(center, circlePosition)) / PI + 1.0f) / 2.0f);
		if (distance > 1.0f)
			circlePosition = Vector2Add(
				(Vector2){sinf(angle * (PI * 2.0f)) * pointScale, -cosf(angle * (PI * 2.0f)) * pointScale}, center);

		float angle360 = angle * 360.0f;
		float valueActual = Clamp(distance, 0.0f, 1.0f);
		Color color = ColorLerp(
			(Color){static_cast<unsigned char>((value * 255.0f)), static_cast<unsigned char>((value * 255.0f)),
					static_cast<unsigned char>((value * 255.0f)), 255},
			ColorFromHSV(angle360, Clamp(distance, 0.0f, 1.0f), 1.0f), valueActual);

		circlePosition = Vector2Subtract(circlePosition, {rect.x, rect.y});

		posScale = {circlePosition.x / rect.width, circlePosition.y / rect.height};

		onColorChanged.invoke(color);
	}
}

void ColorWheel::draw() {
	auto &rend = render->as<ColorWheelRender>();

	unsigned int renderType = RL_TRIANGLES;
	unsigned int triangleCount = 64;
	float pointScale = (rect.width / 2);
	Vector2 center = {rect.x + (rect.width / 2), rect.y + (rect.height / 2)};

	rlBegin(renderType);
	for (unsigned int i = 0; i < triangleCount; i++) {
		float angleOffset = ((PI * 2.0f) / (float)triangleCount);
		float angle = angleOffset * (float)i;
		float angleOffsetCalculated = ((float)i + 1) * angleOffset;
		Vector2 scale = (Vector2){pointScale, pointScale};

		Vector2 offset = Vector2Multiply((Vector2){sinf(angle), -cosf(angle)}, scale);
		Vector2 offset2 = Vector2Multiply((Vector2){sinf(angleOffsetCalculated), -cosf(angleOffsetCalculated)}, scale);

		Vector2 position = Vector2Add(center, offset);
		Vector2 position2 = Vector2Add(center, offset2);

		float angleNonRadian = (angle / (2.0f * PI)) * 360.0f;
		float angleNonRadianOffset = (angleOffset / (2.0f * PI)) * 360.0f;

		Color currentColor = ColorFromHSV(angleNonRadian, 1.0f, 1.0f);
		Color offsetColor = ColorFromHSV(angleNonRadian + angleNonRadianOffset, 1.0f, 1.0f);

		rlColor4ub(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
		rlVertex2f(position.x, position.y);
		rlColor4f(colorValue, colorValue, colorValue, 1.0f);
		rlVertex2f(center.x, center.y);
		rlColor4ub(offsetColor.r, offsetColor.g, offsetColor.b, offsetColor.a);
		rlVertex2f(position2.x, position2.y);
	}
	rlEnd();

	circlePosition = {rect.x + (rect.width * posScale.x), rect.y + (rect.height * posScale.y)};

	Color handleColor = BLACK;

	if (Vector2Distance(center, circlePosition) / pointScale <= 0.5f && colorValue <= 0.5f) {
		handleColor = DARKGRAY;
	}

	// Draw the color handle
	DrawCircleLinesV(circlePosition, 4.0f, handleColor);

	// draw border
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);
}

void ColorWheel::setCirclePosToCenter() { circlePosition = {rect.x + (rect.width / 2), rect.y + (rect.height / 2)}; }

void ColorWheel::leftMouseClicked() { holdingMouse = true; }

void ColorWheel::leftMouseReleased() { holdingMouse = false; }

void ColorWheel::setColor(Color c) {
	auto hsv = ColorToHSV(c);
	auto hue = hsv.x;
	auto saturation = hsv.y;

	float hueRad = DEG2RAD * hue;
	hueRad -= (PI / 2.0f);

	float sin = sinf(hueRad);
	float cos = cosf(hueRad);

	float maxDist = 0.5f;

	float resX = 0.5f + (maxDist * saturation * cos);
	float resY = 0.5f + (maxDist * saturation * sin);

	posScale = {resX, resY};
}

void ColorWheel::setValue(float newValue) { this->colorValue = constrain(newValue, 0.0f, 1.0f); }

float ColorWheel::getValue() { return colorValue; }
