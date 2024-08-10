/*
 * sd_card.h
 *
 *  Created on: Jul 28, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_SD_CARD_H_
#define INC_VEXUF_SD_CARD_H_

#include "main.h"
#include "fatfs.h"



FRESULT SDCard_MountFS();
float SDCard_GetTotalSize(void);
float SDCard_GetFreeSize(void);


void SDCard_HandleError(void);

void SDCard_Test(void);

#endif /* INC_VEXUF_SD_CARD_H_ */
