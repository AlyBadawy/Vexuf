/*
 * config.h
 *
 *  Created on: Jul 31, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "main.h"

/*
	    0	           1	          2	             3	            4	           5	          6	             7	            8	           9	          A	             B	            C	           D	          E	             F
0x0000	CONF Flag      CONF Ver       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED
0x0010	Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      Serial No      REG no.        REG no.        RESERVED
0x0020	RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED
0x0030	TTL Baud	   RESERVED       TNC Baud	     RESERVED       SPI Type	   RESERVED       RESERVED       LCD Add.	    LCD Type	   RESERVED       RESERVED       I2C Add.	    I2C Type	   RESERVED       RESERVED       RESERVED
0x0040	RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED
0x0050	Buzzer En	   RESERVED       RESERVED       RESERVED       Glob Ind       En Status En	  SD Ind En	     TTL Ind En     Act Ind En     RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED
0x0060	AV1 ENABLE	   Av1 Ind En.    RESERVED	     Av1OFF En	    Av1OFF Min	   Av1OFF Max     Av1SLOW En     Av1SLOW Min    Av1SLOW Max    Av1FAST En	  Av1FAST Min    AV1FAST Max    Av1ON En   	   Av1ON Min	  Av1ON Max 	 RESERVED
0x0070	AV2 ENABLE	   AV2 Ind En.    RESERVED	     AV2OFF En	    AV2OFF Min	   AV2OFF Max     AV2SLOW En     AV2SLOW Min    AV2SLOW Max    AV2FAST En	  AV2FAST Min    AV2FAST Max    AV2ON En   	   AV2ON Min	  AV2ON Max 	 RESERVED
0x0080	AV3 ENABLE	   AV3 Ind En.    RESERVED	     AV3OFF En	    AV3OFF Min	   AV3OFF Max     AV3SLOW En     AV3SLOW Min    AV3SLOW Max    AV3FAST En	  AV3FAST Min    AV3FAST Max    AV3ON En   	   AV3ON Min	  AV3ON Max 	 RESERVED
0x0090	RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED       RESERVED
0x00A0	TRIG00 En      TRIG00 SRc	  TRIG00 Min	 TRIG00Act1	    TRIG00Act1	   TRIG00Act2	  TRIG00Act3	 TRIG00Act4	    TRIG00Act5	   TRIG00Act6	  TRIG00Act7	 TRIG00Act8	    TRIG00Buzz	   TRIG00PWM1	  TRIG00PWM2	 TRIG00Log
0x00B0	TRIG01 En      TRIG01 SRc	  TRIG01 Min	 TRIG01 Max	    TRIG01Act1	   TRIG01Act2	  TRIG01Act3	 TRIG01Act4	    TRIG01Act5	   TRIG01Act6	  TRIG01Act7	 TRIG01Act8	    TRIG01Buzz	   TRIG01PWM1	  TRIG01PWM2	 TRIG01Log
0x00C0	TRIG02 En      TRIG02 SRc	  TRIG02 Min	 TRIG02 Max	    TRIG02Act1	   TRIG02Act2	  TRIG02Act3	 TRIG02Act4	    TRIG02Act5	   TRIG02Act6	  TRIG02Act7	 TRIG02Act8	    TRIG02Buzz	   TRIG02PWM1	  TRIG02PWM2	 TRIG02Log
0x00D0	TRIG03 En      TRIG03 SRc	  TRIG03 Min	 TRIG03 Max	    TRIG03Act1	   TRIG03Act2	  TRIG03Act3	 TRIG03Act4	    TRIG03Act5	   TRIG03Act6	  TRIG03Act7	 TRIG03Act8	    TRIG03Buzz	   TRIG03PWM1	  TRIG03PWM2	 TRIG03Log
0x00E0	TRIG04 En      TRIG04 SRc	  TRIG04 Min	 TRIG04 Max	    TRIG04Act1	   TRIG04Act2	  TRIG04Act3	 TRIG04Act4	    TRIG04Act5	   TRIG04Act6	  TRIG04Act7	 TRIG04Act8	    TRIG04Buzz	   TRIG04PWM1	  TRIG04PWM2	 TRIG04Log
0x00F0	TRIG05 En      TRIG05 SRc	  TRIG05 Min	 TRIG05 Max	    TRIG05Act1	   TRIG05Act2	  TRIG05Act3	 TRIG05Act4	    TRIG05Act5	   TRIG05Act6	  TRIG05Act7	 TRIG05Act8	    TRIG05Buzz	   TRIG05PWM1	  TRIG05PWM2	 TRIG05Log
0x0100	TRIG06 En      TRIG06 SRc	  TRIG06 Min	 TRIG06 Max	    TRIG06Act1	   TRIG06Act2	  TRIG06Act3	 TRIG06Act4	    TRIG06Act5	   TRIG06Act6	  TRIG06Act7	 TRIG06Act8	    TRIG06Buzz	   TRIG06PWM1	  TRIG06PWM2	 TRIG06Log
0x0110	TRIG07 En      TRIG07 SRc	  TRIG07 Min	 TRIG07 Max	    TRIG07Act1	   TRIG07Act2	  TRIG07Act3	 TRIG07Act4	    TRIG07Act5	   TRIG07Act6	  TRIG07Act7	 TRIG07Act8	    TRIG07Buzz	   TRIG07PWM1	  TRIG07PWM2	 TRIG07Log
0x0120	TRIG08 En      TRIG08 SRc	  TRIG08 Min	 TRIG08 Max	    TRIG08Act1	   TRIG08Act2	  TRIG08Act3	 TRIG08Act4	    TRIG08Act5	   TRIG08Act6	  TRIG08Act7	 TRIG08Act8	    TRIG08Buzz	   TRIG08PWM1	  TRIG08PWM2	 TRIG08Log
0x0130	TRIG09 En      TRIG09 SRc	  TRIG09 Min	 TRIG09 Max	    TRIG09Act1	   TRIG09Act2	  TRIG09Act3	 TRIG09Act4	    TRIG09Act5	   TRIG09Act6	  TRIG09Act7	 TRIG09Act8	    TRIG09Buzz	   TRIG09PWM1	  TRIG09PWM2	 TRIG09Log
0x0140	TRIG10 En      TRIG10 SRc	  TRIG10 Min	 TRIG10 Max	    TRIG10Act1	   TRIG10Act2	  TRIG10Act3	 TRIG10Act4	    TRIG10Act5	   TRIG10Act6	  TRIG10Act7	 TRIG10Act8	    TRIG10Buzz	   TRIG10PWM1	  TRIG10PWM2	 TRIG10Log
0x0150	TRIG11 En      TRIG11 SRc	  TRIG11 Min	 TRIG11 Max	    TRIG11Act1	   TRIG11Act2	  TRIG11Act3	 TRIG11Act4	    TRIG11Act5	   TRIG11Act6	  TRIG11Act7	 TRIG11Act8	    TRIG11Buzz	   TRIG11PWM1	  TRIG11PWM2	 TRIG11Log
0x0160	TRIG12 En      TRIG12 SRc	  TRIG12 Min	 TRIG12 Max	    TRIG12Act1	   TRIG12Act2	  TRIG12Act3	 TRIG12Act4	    TRIG12Act5	   TRIG12Act6	  TRIG12Act7	 TRIG12Act8	    TRIG12Buzz	   TRIG12PWM1	  TRIG12PWM2	 TRIG12Log
0x0170	TRIG13 En      TRIG13 SRc	  TRIG13 Min	 TRIG13 Max	    TRIG13Act1	   TRIG13Act2	  TRIG13Act3	 TRIG13Act4	    TRIG13Act5	   TRIG13Act6	  TRIG13Act7	 TRIG13Act8	    TRIG13Buzz	   TRIG13PWM1	  TRIG13PWM2	 TRIG13Log
0x0180	TRIG14 En      TRIG14 SRc	  TRIG14 Min	 TRIG14 Max	    TRIG14Act1	   TRIG14Act2	  TRIG14Act3	 TRIG14Act4	    TRIG14Act5	   TRIG14Act6	  TRIG14Act7	 TRIG14Act8	    TRIG14Buzz	   TRIG14PWM1	  TRIG14PWM2	 TRIG14Log
0x0190	TRIG15 En      TRIG15 SRc	  TRIG15 Min	 TRIG15 Max	    TRIG15Act1	   TRIG15Act2	  TRIG15Act3	 TRIG15Act4	    TRIG15Act5	   TRIG15Act6	  TRIG15Act7	 TRIG15Act8	    TRIG15Buzz	   TRIG15PWM1	  TRIG15PWM2	 TRIG15Log
0x01A0	AVComp1	       AVcomp1 REF	  AVcomp1 Tgt    AVcomp1 Tst    AVComp1Act1    AVComp1Act2    AVComp1Act3    AVComp1Act4    AVComp1Act5    AVComp1Act6    AVComp1Act7    AVComp1Act8    AVComp1Buzz    AVComp1PWM1    AVComp1PWM2    AvComp1Log
0x01B0	AVComp2	       AVComp2 REF	  AVComp2 Tgt    AVComp2 Tst    AVComp2Act1    AVComp2Act2    AVComp2Act3    AVComp2Act4    AVComp2Act5    AVComp2Act6    AVComp2Act7    AVComp2Act8    AVComp2Buzz    AVComp2PWM1    AVComp2PWM2    AVComp2Log
0x01C0	AVComp3	       AVComp3 REF	  AVComp3 Tgt    AVComp3 Tst    AVComp3Act1    AVComp3Act2    AVComp3Act3    AVComp3Act4    AVComp3Act5    AVComp3Act6    AVComp3Act7    AVComp3Act8    AVComp3Buzz    AVComp3PWM1    AVComp3PWM2    AVComp3Log
0x01D0	TEMPComp	   RESERVED	      RESERVED	     TEMP-TEST	    TEMP-Act1      TEMP-Act2      TEMP-Act3	     TEMP-Act4	    TEMP-Act5	   TEMP-Act6      TEMP-Act7	     TEMP-Act8	    TEMP-Buzz	   TEMP-PWM1      TEMPPWM2       TEMP-Log
0x01E0	ALRM1 EN	   RESERVED	      RESERVED	     RESERVED	    Alarm1Act1	   Alarm1Act2 	  Alarm1Act3   	 Alarm1Act4	    Alarm1Act5	   Alarm1Act6 	  Alarm1Act7	 Alarm1Act8	    Alarm1Buzz	   Alarm1PWM1     Alarm1PWM2	 Alarm1LOG
0x01F0	ALRM2 EN	   RESERVED	      RESERVED	     RESERVED	    Alarm2Act1	   Alarm2Act2	  Alarm2Act3	 Alarm2Act4	    Alarm2Act5	   Alarm2Act6	  Alarm2Act7	 Alarm2Act8	    Alarm2Buzz	   Alarm2PWM1	  Alarm1PWM2	 Alarm2LOG

*/












// Memory MAp addresses
#define EEPROM_CONFIG_FLAG                           0X0000
#define EEPROM_CONFIG_VERSION                        0X0001

#define EEPROM_SERIAL_NUMBER                         0X0010      // 13 WORDS - 25 BYTES
#define EEPROM_REGISTERATION_NUMBER                  0X001D      //  2 WORDS
#define EEPROM_
#define EEPROM_TTL_BAUD                              0X0030
#define EEPROM_TNC_BAUD                              0X0032
#define EEPROM_SPI_TYPE                              0X0034
#define EEPROM_LCD_ADDRESS                           0X0037
#define EEPROM_LCD_TYPE                              0X0038

#define EEPROM_BUZZER_ENABLED                        0X0050
#define EEPROM_GLOBAL_INDICATOR_ENABLED              0X0054
#define EEPROM_STATUSES_INDICATORS_ENABLED           0X0055
#define EEPROM_SD_CARD_INDICATOR_ENABLED             0X0056
#define EEPROM_TTL_INDICATOR_ENABLED                 0X0057
#define EEPROM_ACTUATORS_INDICATORS_ENABLED          0X0058


#define EEPROM_AV_ENABLED                            0X0060
#define EEPROM_AV_INDICATOR_ENABLED                  0X0061
#define EEPROM_AV_OFF_RULE_ENABLED                   0X0063
#define EEPROM_AV_OFF_RULE_MIN                       0X0064
#define EEPROM_AV_OFF_RULE_MAX                       0X0065
#define EEPROM_AV_SLOW_RULE_ENABLED                  0X0066
#define EEPROM_AV_SLOW_RULE_MIN                      0X0067
#define EEPROM_AV_SLOW_RULE_MAX                      0X0068
#define EEPROM_AV_FAST_RULE_ENABLED                  0X0069
#define EEPROM_AV_FAST_RULE_MIN                      0X006A
#define EEPROM_AV_FAST_RULE_MAX                      0X006B
#define EEPROM_AV_ON_RULE_ENABLED                    0X006C
#define EEPROM_AV_ON_RULE_MIN                        0X006D
#define EEPROM_AV_ON_RULE_MAX                        0X006E
#define EEPROM_AV_SHIFT                              16
#define EEPROM_NUMBER_OF_AVS                         3

#define EEPROM_TRIG_ENABLE                           0X00A0
#define EEPROM_TRIG_SOURCE                           0X00A1
#define EEPROM_TRIG_MIN                              0X00A2
#define EEPROM_TRIG_MAX                              0X00A3
#define EEPROM_TRIG_ACTUATOR                         0X00A4
#define EEPROM_TRIG_BUZZER                           0X00AD
#define EEPROM_TRIG_SERVO                            0X00AD
#define EEPROM_TRIG_LOG                              0X00AF
#define EEPROM_TRIG_SHIFT                            16
#define EEPROM_TRIGS_COUNT                           16

#define EEPROM_AV_COMP_ENABLE_SHIFT                  0X01A0
#define EEPROM_AV_COMP_REF                           0X01A1
#define EEPROM_AV_COMP_TARGET                        0X01A2
#define EEPROM_AV_COMP_COMPARATOR                    0X01A3
#define EEPROM_AV_COMP_ACTUATOR                      0X01A4
#define EEPROM_AV_COMP_BUZZER                        0X01AC
#define EEPROM_AV_COMP_SERVO                         0X01AD
#define EEPROM_AV_COMP_LOG                           0X01AF
#define EEPROM_AV_COMP_SHIFT                         16
#define EEPROM_AV_COMP_COUNT                         3

#define EEPROM_TEMP_COMP_ENABLE_SHIFT                0X01D0
#define EEPROM_TEMP_COMP_COMPARATOR                  0X01D3
#define EEPROM_TEMP_COMP_ACTUATOR                    0X01D4
#define EEPROM_TEMP_COMP_BUZZER                      0X01DC
#define EEPROM_TEMP_COMP_SERVO                       0X01DD
#define EEPROM_TEMP_COMP_LOG                         0X01DF

#define EEPROM_ALARM_ENABLE		                     0X01E0
#define EEPROM_ALARM_ACTUATOR                        0X01E4
#define EEPROM_ALARM_BUZZER                          0X01EC
#define EEPROM_ALARM_SERVO                           0X01ED
#define EEPROM_ALARM_LOG                             0X01EF
#define EEPROM_ALARM_SHIFT                    	     16
#define EEPROM_ALARM_COUNT                           2



#endif /* INC_CONFIG_H_ */
