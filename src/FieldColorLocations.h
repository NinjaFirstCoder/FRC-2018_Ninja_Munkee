/*
 * FieldColorLocations.h
 *
 *  Created on: Feb 20, 2018
 *      Author: frc2395driver
 */

#ifndef SRC_FIELDCOLORLOCATIONS_H_
#define SRC_FIELDCOLORLOCATIONS_H_

class FieldColorLocations {
private:
	bool nearestSwitch = false;
	bool furthestSwitch = false;
	bool scale = false;

public:
	FieldColorLocations();
	void setNearestSwitchLeft();
	void setNearestSwitchRight();
	void setFurthestSwitchLeft();
	void setFurthestSwitchRight();
	void setScaleLeft();
	void setScaleRight();
	bool isNearestSwitchOnLeft();
	bool isFurthestSwitchOnLeft();
	bool isScaleOnLeft();
	virtual ~FieldColorLocations();
};

#endif /* SRC_FIELDCOLORLOCATIONS_H_ */
