#ifndef SENSORLOG_H
#define SENSORLOG_H

#include <list>
#include <istream>
#include "sensorreading.h"
#include "odometrysensor.h"
#include "rangesensor.h"
#include "odometryreading.h"
#include "rangereading.h"
#include "configuration.h"

namespace GMapping {

class SensorLog : public std::list<SensorReading*>{
	public:
		SensorLog(const SensorMap&);
		~SensorLog();
		std::istream& load(std::istream& is);
		OrientedPoint boundingBox(double& xmin, double& ymin, double& xmax, double& ymax) const;
	protected:
		const SensorMap& m_sensorMap;
		OdometryReading* parseOdometry(std::istream& is, const OdometrySensor* ) const;
		RangeReading* parseRange(std::istream& is, const RangeSensor* ) const;
};

};

#endif
