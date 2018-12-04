#ifndef CONSTANTS_H
#define CONSTANTS_H

const int TrainThreshold = 7;		// "days" for Train mode become Repeat
const double OldRepeatCoeff = 0.8;	// "forgetting" of repeats and errors num in Repeat mode
const double TrainErrorRatio = 0.3;	// drop mode to Train if error rate is more
const double TrainErrorCost = 1.0;	// age reducing coeff for error rate
const double SecToAgeCoeff = 1.0/86400;	// second/"day"
const double MaxAgeIncrease = 1.0;	// max increase of age in "days"
const double BadSpeedRatio = 2.0;	// if new speed if this times more, it's considered bad
const double BadSpeedCost = 1.0;	// age reducing coeff for bad speed
const int TrainElapsedRound = 3600;	// time from last repeat rounded
const int TrainElapsedCoeff = 7;	// time from last repeat coeff
const int DefaultSeriesSize = 20;

#endif // CONSTANTS_H
