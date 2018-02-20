/*
 * FieldColorLocations.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: frc2395driver
 */

#include <FieldColorLocations.h>

FieldColorLocations::FieldColorLocations() {
	nearestSwitch = true;
	furthestSwitch = true;
	scale = true;
}

void FieldColorLocations::setNearestSwitchLeft() {
	nearestSwitch = true;
}

void FieldColorLocations::setNearestSwitchRight() {
	nearestSwitch = false;
}

void FieldColorLocations::setFurthestSwitchLeft() {
	furthestSwitch = true;
}

void FieldColorLocations::setFurthestSwitchRight() {
	furthestSwitch = false;
}

void FieldColorLocations::setScaleLeft() {
	scale = true;
}

void FieldColorLocations::setScaleRight() {
	scale = false;
}

bool FieldColorLocations::isNearestSwitchOnLeft() {
	return nearestSwitch;
}

bool FieldColorLocations::isFurthestSwitchOnLeft() {
	return furthestSwitch;
}

bool FieldColorLocations::isScaleOnLeft() {
	return scale;
}

FieldColorLocations::~FieldColorLocations() { }

