/*
 * LightSensor.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

void ALSensorConfigure(); // This function configures the Ambiental Light Sensor for 0~5162 lux (resolution 0.0788 lux/count)
uint16_t ALSensorReadData(); // This function reads the data from the i2c AL Sensor

#endif /* LIGHTSENSOR_H_ */
