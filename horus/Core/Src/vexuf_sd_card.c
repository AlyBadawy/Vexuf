/*
 * sd_card.c
 *
 *  Created on: Jul 28, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_sd_card.h"

OutputConfiguration outputConfig;

char SdTxBuffer[250];
char SdRW_Buffer[200];
FATFS FatFs;
FIL Fil;
FRESULT FR_Status;
FATFS *FS_Ptr;
UINT SdRWC, SdWWC; // Read/Write Word Counter
DWORD SdFreeClusters;
uint32_t SdTotalSize, SdFreeSpace;

void SDCard_HandleError(void) {
//	if (!outputConfig.error_on_no_sd) return; TODO: Enable this line

	while (FR_Status != FR_OK) {
			HAL_GPIO_WritePin(ErrorInd_GPIO_Port, ErrorInd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(SdioInd_GPIO_Port, SdioInd_Pin, GPIO_PIN_RESET);
			HAL_Delay(300);
			HAL_GPIO_WritePin(ErrorInd_GPIO_Port, ErrorInd_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SdioInd_GPIO_Port, SdioInd_Pin, GPIO_PIN_SET);
			HAL_Delay(300);
		}
}

FRESULT SDCard_MountFS() {
	FR_Status = f_mount(&FatFs, SDPath, 1);
	return FR_Status;
}

uint32_t SDCard_GetTotalSize(void) {
    FR_Status =  f_getfree("", &SdFreeClusters, &FS_Ptr);
    if (FR_Status != FR_OK) {
    	SDCard_HandleError();
    	return 0;
    }
    return (uint32_t)((FS_Ptr->n_fatent - 2) * FS_Ptr->csize * 0.5);
}

uint32_t SDCard_GetFreeSize(void) {
	FR_Status =  f_getfree("", &SdFreeClusters, &FS_Ptr);
	if (FR_Status != FR_OK) {
		SDCard_HandleError();
		return 0;
	}
	return (uint32_t)(SdFreeClusters * FS_Ptr->csize * 0.5);
}

void SDCard_Test(void) {
  	printf("\r\n");
  	printf("Testing SD Card Functionalities...\r\n");


    do
    {
      //------------------[ Mount The SD Card ]--------------------
      FR_Status = f_mount(&FatFs, SDPath, 1);
      if (FR_Status != FR_OK)
      {
        sprintf(SdTxBuffer, "  Error! While Mounting SD Card, Error Code: (%i)\r\n", FR_Status);
        printf(SdTxBuffer);
        break;
      }
      sprintf(SdTxBuffer, "  SD Card Mounted Successfully! \r\n");
      printf(SdTxBuffer);

      //------------------[ Open A Text File For Write & Write Data ]--------------------
      //Open the file
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
      if(FR_Status != FR_OK)
      {
        sprintf(SdTxBuffer, "  Error! While Creating/Opening A New Text File, Error Code: (%i)\r\n", FR_Status);
        printf(SdTxBuffer);
        break;
      }
      sprintf(SdTxBuffer, "  Text File Created & Opened! Writing Data To The Text File..\r\n");
      printf(SdTxBuffer);
      // (1) Write Data To The Text File [ Using f_puts() Function ]
      f_puts("Hello! From STM32 To SD Card Over SDIO, Using f_puts()\n", &Fil);
      // (2) Write Data To The Text File [ Using f_write() Function ]
      strcpy(SdRW_Buffer, "Hello! From STM32 To SD Card Over SDIO, Using f_write().\r\n");
      f_write(&Fil, SdRW_Buffer, strlen(SdRW_Buffer), &SdWWC);
      // Close The File
      f_close(&Fil);
      //------------------[ Open A Text File For Read & Read Its Data ]--------------------
      // Open The File
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_READ);
      if(FR_Status != FR_OK)
      {
        sprintf(SdTxBuffer, "  Error! While Opening (MyTextFile.txt) File For Read.. \r\n");
        printf(SdTxBuffer);
        break;
      }
      // (1) Read The Text File's Data [ Using f_gets() Function ]
      f_gets(SdRW_Buffer, sizeof(SdRW_Buffer), &Fil);
      sprintf(SdTxBuffer, "  Data Read From (MyTextFile.txt) Using f_gets().\r\n");
      printf(SdTxBuffer);
      // (2) Read The Text File's Data [ Using f_read() Function ]
      f_read(&Fil, SdRW_Buffer, f_size(&Fil), &SdRWC);
      sprintf(SdTxBuffer, "  Data Read From (MyTextFile.txt) Using f_read().\r\n");
      printf(SdTxBuffer);
      // Close The File
      f_close(&Fil);
      sprintf(SdTxBuffer, "  File Closed! \r\n");
      printf(SdTxBuffer);
      //------------------[ Open An Existing Text File, Update Its Content, Read It Back ]--------------------
      // (1) Open The Existing File For Write (Update)
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_OPEN_EXISTING | FA_WRITE);
      FR_Status = f_lseek(&Fil, f_size(&Fil)); // Move The File Pointer To The EOF (End-Of-File)
      if(FR_Status != FR_OK)
      {
        sprintf(SdTxBuffer, "  Error! While Opening (MyTextFile.txt) File For Update.. \r\n");
        printf(SdTxBuffer);
        break;
      }
      // (2) Write New Line of Text Data To The File
      FR_Status = f_puts("  This New Line Was Added During File Update!\r\n", &Fil);
      f_close(&Fil);
      memset(SdRW_Buffer,'\0',sizeof(SdRW_Buffer)); // Clear The Buffer
      // (3) Read The Contents of The Text File After The Update
      FR_Status = f_open(&Fil, "MyTextFile.txt", FA_READ); // Open The File For Read
      f_read(&Fil, SdRW_Buffer, f_size(&Fil), &SdRWC);
      sprintf(SdTxBuffer, "  Data Read From (MyTextFile.txt) After Update.\r\n");
      printf(SdTxBuffer);
      f_close(&Fil);
      //------------------[ Delete The Text File ]--------------------
      // Delete The File
      /*
      FR_Status = f_unlink(MyTextFile.txt);
      if (FR_Status != FR_OK){
          sprintf(TxBuffer, "Error! While Deleting The (MyTextFile.txt) File.. \r\n");
          USC_CDC_Print(TxBuffer);
      }
      */
    } while(0);
    //------------------[ Test Complete! Unmount The SD Card ]--------------------
    FR_Status = f_mount(NULL, "", 0);
    if (FR_Status != FR_OK)
    {
        sprintf(SdTxBuffer, "  Error! While Un-mounting SD Card, Error Code: (%i)\r\n", FR_Status);
        printf(SdTxBuffer);
    } else{
        sprintf(SdTxBuffer, "  SD Card Un-mounted Successfully! \r\n");
        printf(SdTxBuffer);
    }
}
