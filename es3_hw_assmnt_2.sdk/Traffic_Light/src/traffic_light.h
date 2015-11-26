/*
 * traffic_light.h
 *
 *  Created on: 24 Nov 2015
 *      Author: Will
 */

#ifndef TRAFFIC_LIGHT_H_
#define TRAFFIC_LIGHT_H_

#define RED 0xF00
#define GREEN 0x0F0
#define YELLOW  0xFF0
#define	OFF  0xFFF

void next_state();
void flash_ped();

#endif /* TRAFFIC_LIGHT_H_ */
