/*
 * sd_card.c
 *
 *  Created on: Jul 28, 2024
 *      Author: Aly Badawy
 */


#include "vexuf_sd_card.h"
#include "vexuf_helpers.h"
#include "vexuf_indicators.h"
#include "vexuf_timers.h"
#include "vexuf_output.h"

extern OutputConfiguration outputConfig;

FATFS FatFs;
FIL Fil;
FRESULT FR_Status;
FATFS *FS_Ptr;
UINT RWC, WWC; // Read/Write Word Counter
DWORD FreeClusters;


//char TxBuffer[250];
//char RW_Buffer[200];



void SDCard_HandleError(void) {
	if (!outputConfig.error_on_no_sd) return;
	OUTPUT_buzzOnError();
	Indicators_setStatus(IndWarn, IndOFF);

	TIMERS_Stop();

	while (FR_Status != FR_OK) {
		Indicators_setStatus(IndError, IndOFF);
		Indicators_setStatus(IndSdio, IndON);
		HAL_Delay(300);
		Indicators_setStatus(IndError, IndON);
		Indicators_setStatus(IndSdio, IndOFF);
		HAL_Delay(300);
	}
}

FRESULT SDCard_MountFS() {
	char cardLabel[12];
	FR_Status = f_mount(&FatFs, SDPath, 1);
	if (FR_Status == FR_OK) {
		f_getlabel("", cardLabel, NULL);
		uint8_t st = strncmp("VEXUF_HORUS", cardLabel, 12);
		if (st != 0) {
			f_setlabel("VEXUF_HORUS");
		}
	}
	return FR_Status;
}

float SDCard_GetTotalSize(void) {
	uint64_t total_sectors;
	uint64_t total_space;
    FR_Status =  f_getfree("", &FreeClusters, &FS_Ptr);
    if (FR_Status != FR_OK) {
    	SDCard_HandleError();
    	return 0;
    }

    total_sectors = (uint64_t)(FS_Ptr->n_fatent - 2) * FS_Ptr->csize;				// Calculate total sectors
	total_space = total_sectors * 512;										// Convert to bytes
	return (float)total_space / (1024 * 1024 * 1024);						// Convert to gigabytes (use float for GB to handle large values)
}

float SDCard_GetFreeSize(void) {
	uint64_t free_sectors;
	uint64_t free_space;
	FR_Status =  f_getfree("", &FreeClusters, &FS_Ptr);
	if (FR_Status != FR_OK) {
		SDCard_HandleError();
		return 0;
	}
	free_sectors = (uint64_t)FreeClusters * FS_Ptr->csize;					// Calculate free sectors
	free_space = free_sectors * 512;										// Convert to bytes
	return (float)free_space / (1024 * 1024 * 1024);						// Convert to gigabytes (use float for GB to handle large values)
}


/*
void SDCard_Test(void) {
  	printf("\r\n");
  	printf("Testing SD Card Functionalities...\r\n");


    do
    {
      //------------------[ Mount The SD Card ]--------------------
      FR_Status = f_mount(&FatFs, SDPath, 1);
      if (FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Mounting SD Card, Error Code: (%i)\r\n", FR_Status);
        printf(TxBuffer);
        break;
      }
      sprintf(TxBuffer, "  SD Card Mounted Successfully! \r\n");
      printf(TxBuffer);
      //------------------[ Get & Print The SD Card Size & Free Space ]--------------------
      f_getfree("", &FreeClusters, &FS_Ptr);
      TotalSize = (uint32_t)((FS_Ptr->n_fatent - 2) * FS_Ptr->csize * 0.5);
      FreeSpace = (uint32_t)(FreeClusters * FS_Ptr->csize * 0.5);
      sprintf(TxBuffer, "  Total SD Card Size: %lu Bytes\r\n", TotalSize);
      printf(TxBuffer);
      sprintf(TxBuffer, "  Free SD Card Space: %lu Bytes\r\n", FreeSpace);
      printf(TxBuffer);
      //------------------[ Open A Text File For Write & Write Data ]--------------------
      //Open the file
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
      if(FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Creating/Opening A New Text File, Error Code: (%i)\r\n", FR_Status);
        printf(TxBuffer);
        break;
      }
      sprintf(TxBuffer, "  Text File Created & Opened! Writing Data To The Text File..\r\n");
      printf(TxBuffer);
      // (1) Write Data To The Text File [ Using f_puts() Function ]
      f_puts("Hello! From STM32 To SD Card Over SDIO, Using f_puts()\n", &Fil);
      // (2) Write Data To The Text File [ Using f_write() Function ]
      strcpy(RW_Buffer, "Hello! From STM32 To SD Card Over SDIO, Using f_write().\r\n");
      f_write(&Fil, RW_Buffer, strlen(RW_Buffer), &WWC);
      // Close The File
      f_close(&Fil);
      //------------------[ Open A Text File For Read & Read Its Data ]--------------------
      // Open The File
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_READ);
      if(FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Opening (MyTextFile.txt) File For Read.. \r\n");
        printf(TxBuffer);
        break;
      }
      // (1) Read The Text File's Data [ Using f_gets() Function ]
      f_gets(RW_Buffer, sizeof(RW_Buffer), &Fil);
      sprintf(TxBuffer, "  Data Read From (MyTextFile.txt) Using f_gets().\r\n");
      printf(TxBuffer);
      // (2) Read The Text File's Data [ Using f_read() Function ]
      f_read(&Fil, RW_Buffer, f_size(&Fil), &RWC);
      sprintf(TxBuffer, "  Data Read From (MyTextFile.txt) Using f_read().\r\n");
      printf(TxBuffer);
      // Close The File
      f_close(&Fil);
      sprintf(TxBuffer, "  File Closed! \r\n");
      printf(TxBuffer);
      //------------------[ Open An Existing Text File, Update Its Content, Read It Back ]--------------------
      // (1) Open The Existing File For Write (Update)
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_OPEN_EXISTING | FA_WRITE);
      FR_Status = f_lseek(&Fil, f_size(&Fil)); // Move The File Pointer To The EOF (End-Of-File)
      if(FR_Status != FR_OK)
      {
        sprintf(TxBuffer, "  Error! While Opening (MyTextFile.txt) File For Update.. \r\n");
        printf(TxBuffer);
        break;
      }
      // (2) Write New Line of Text Data To The File
      FR_Status = f_puts("  This New Line Was Added During File Update!\r\n", &Fil);
      f_close(&Fil);
      memset(RW_Buffer,'\0',sizeof(RW_Buffer)); // Clear The Buffer
      // (3) Read The Contents of The Text File After The Update
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_READ); // Open The File For Read
      f_read(&Fil, RW_Buffer, f_size(&Fil), &RWC);
      sprintf(TxBuffer, "  Data Read From (MyTextFile.txt) After Update.\r\n");
      printf(TxBuffer);
      f_close(&Fil);
      //------------------[ Delete The Text File ]--------------------
      // Delete The File
      FR_Status = f_unlink(MyTextFile.txt);
      if (FR_Status != FR_OK){
          sprintf(TxBuffer, "Error! While Deleting The (MyTextFile.txt) File.. \r\n");
          USC_CDC_Print(TxBuffer);
      }
    } while(0);
    //------------------[ Test Complete! Unmount The SD Card ]--------------------
    FR_Status = f_mount(NULL, "", 0);
    if (FR_Status != FR_OK)
    {
        sprintf(TxBuffer, "  Error! While Un-mounting SD Card, Error Code: (%i)\r\n", FR_Status);
        printf(TxBuffer);
    } else{
        sprintf(TxBuffer, "  SD Card Un-mounted Successfully! \r\n");
        printf(TxBuffer);
    }
}

*/
