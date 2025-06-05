#include "globals.h"
#include "serialParser.h"
//#include <Arduino_JSON.h>
#include <ArduinoJson.h>

uint16_t tempRPM = 0;
uint8_t serialECURequestQueueSize = 0;
boolean hasConnectionToECU = 0; // Have we spoken to the ECU succesfully at some point??

// #define DEBUG_SERIAL_PARSER

void requestSerialData() 
{
  #ifdef DEBUG_SERIAL_PARSER
    Serial.println("Requesting Serial Data");
  #endif
  Serial_ECU.write('n');
  serialECURequestQueueSize++;
}

void parseFixedSerialData() 
{
  
  #ifdef DEBUG_SERIAL_PARSER
    Serial.print("ECU Serial Data received. Received: ");
    Serial.print(Serial_ECU.available());
    Serial.println(" bytes");
  #endif
  

  if( (Serial_ECU.available() >= 75) && (Serial_ECU.read() == 'n') )
  {


    #ifdef DEBUG_SERIAL_PARSER
      Serial.println("Got Serial Data");
    #endif

    serialECURequestQueueSize--;
    hasConnectionToECU = 1; // We got a response

    //Serial.println("Valid Data found");
    //readings_JSON = undefined;

    Serial_ECU.read(); // Command type response - 0x32. Skip - Also 50 in dec
    Serial_ECU.read(); // Response size - I think its minus the header bytes





    
    
    
    
    
    
    
    
    
    
    


    readings_JSON["secl"] = Serial_ECU.read();

    uint8_t status1 = Serial_ECU.read();
    readings_JSON["inj1_status"] = BIT_CHECK(status1, 0);
    readings_JSON["inj2_status"] = BIT_CHECK(status1, 1);
    readings_JSON["inj3_status"] = BIT_CHECK(status1, 2);
    readings_JSON["inj4_status"] = BIT_CHECK(status1, 3);
    readings_JSON["dfco_active"] = BIT_CHECK(status1, 4);
    readings_JSON["boost_cut_fuel"] = BIT_CHECK(status1, 5);
    
    uint8_t engine = Serial_ECU.read();
    readings_JSON["running"] = BIT_CHECK(engine, 0);
    readings_JSON["cranking"] = BIT_CHECK(engine, 1);
    readings_JSON["warmup"] = BIT_CHECK(engine, 2);

    readings_JSON["dwell"] = Serial_ECU.read() / 10.0;
    readings_JSON["MAP"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    readings_JSON["IAT"] = Serial_ECU.read() - CALIBRATION_TEMPERATURE_OFFSET;
    readings_JSON["CLT"] = Serial_ECU.read() - CALIBRATION_TEMPERATURE_OFFSET;
    readings_JSON["correction_voltage"] = Serial_ECU.read();
    readings_JSON["Battery_Voltage"] = Serial_ECU.read() / 10.0;
    readings_JSON["AFR1"] = Serial_ECU.read() / 10.0;
    readings_JSON["correction_o2"] = Serial_ECU.read();
    readings_JSON["correction_iat"] = Serial_ECU.read();  
    readings_JSON["correction_wue"] = Serial_ECU.read();
    readings_JSON["rpm"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    #ifdef FAKE_RPM
      tempRPM += 16;
      readings_JSON["rpm"] = tempRPM;
      if(tempRPM >= 9000) tempRPM = 0;
    #endif

    readings_JSON["correction_ae"] = Serial_ECU.read();
    readings_JSON["correction_total"] = Serial_ECU.read();
    readings_JSON["VE"] = Serial_ECU.read();
    readings_JSON["afr_target"] = Serial_ECU.read() / 10.0;
    readings_JSON["PW1"] = (Serial_ECU.read() | (Serial_ECU.read() << 8)) / 10;0;
    readings_JSON["tps_DOT"] = Serial_ECU.read() * 10;
    readings_JSON["advance"] = Serial_ECU.read();
    readings_JSON["TPS"] = Serial_ECU.read();

    readings_JSON["loops_per_second"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    readings_JSON["free_ram"] = Serial_ECU.read() | (Serial_ECU.read() << 8);

    readings_JSON["boost_target"] = Serial_ECU.read() * 2;
    readings_JSON["boost_duty"] = Serial_ECU.read();

    uint8_t spark_bits = Serial_ECU.read();
    readings_JSON["spark_bits"] = spark_bits;

    readings_JSON["launch_hard"] = BIT_CHECK(spark_bits, 0);
    readings_JSON["launch_soft"] = BIT_CHECK(spark_bits, 1);
    readings_JSON["hard_limit_on"] = BIT_CHECK(spark_bits, 2);
    readings_JSON["soft_limit_on"] = BIT_CHECK(spark_bits, 3);
    // Boost cut spark
    readings_JSON["spark_error"] = BIT_CHECK(spark_bits, 5);
    readings_JSON["idle_control_on"] = BIT_CHECK(spark_bits, 6);
    readings_JSON["sync"] = BIT_CHECK(spark_bits, 7);



    readings_JSON["rpmDOT"] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    readings_JSON["ethanol%"] = Serial_ECU.read();
    readings_JSON["flex_correction"] = Serial_ECU.read();
    readings_JSON["flex_ign_correction"] = Serial_ECU.read();
    readings_JSON["idle_load"] = Serial_ECU.read();
    readings_JSON["test_outputs"] = Serial_ECU.read();
    readings_JSON["AFR2"] = Serial_ECU.read();
    readings_JSON["baro"] = Serial_ECU.read();

    //CAN Status variables 1 to 16
    for(uint8_t i = 1; i <= 16; i++)
    {
      readings_JSON["CAN_Status_" + String(i)] = Serial_ECU.read() | (Serial_ECU.read() << 8);
    }

    Serial_ECU.read(); // TPS RAW

    readings_JSON["error_codes"] = Serial_ECU.read(); // getNextError() - 74
    readings_JSON["launch_correction"] = Serial_ECU.read(); // currentStatus.launchCorrection - 75

    // A command only sends first 75 values
    // Use n Command:

    Serial_ECU.read(); // 76 lowByte(currentStatus.PW2); break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    Serial_ECU.read(); // 77 highByte(currentStatus.PW2); break; //Pulsewidth 2 multiplied by 10 in ms. Have to convert from uS to mS.
    Serial_ECU.read(); // 78 lowByte(currentStatus.PW3); break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    Serial_ECU.read(); // 79 highByte(currentStatus.PW3); break; //Pulsewidth 3 multiplied by 10 in ms. Have to convert from uS to mS.
    Serial_ECU.read(); // 80 lowByte(currentStatus.PW4); break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.
    Serial_ECU.read(); // 81 highByte(currentStatus.PW4); break; //Pulsewidth 4 multiplied by 10 in ms. Have to convert from uS to mS.
    Serial_ECU.read(); // 82 currentStatus.status3; break; // resentLockOn(0), nitrousOn(1), fuel2Active(2), vssRefresh(3), halfSync(4), nSquirts(6:7)
    readings_JSON["engine_protect_status"] = Serial_ECU.read(); // 83 currentStatus.engineProtectStatus; break; //RPM(0), MAP(1), OIL(2), AFR(3), Unused(4:7)
    Serial_ECU.read(); // 84 lowByte(currentStatus.fuelLoad); break;
    Serial_ECU.read(); // 85 highByte(currentStatus.fuelLoad); break;
    Serial_ECU.read(); // 86 lowByte(currentStatus.ignLoad); break;
    Serial_ECU.read(); // 87 highByte(currentStatus.ignLoad); break;
    Serial_ECU.read(); // 88 lowByte(currentStatus.injAngle); break; 
    Serial_ECU.read(); // 89 highByte(currentStatus.injAngle); break; 
    Serial_ECU.read(); // 90 currentStatus.idleLoad; break; -- NOTE: this is literally the same function as the earlier idle load??
    Serial_ECU.read(); // 91 currentStatus.CLIdleTarget; break; //closed loop idle target
    Serial_ECU.read(); // 92 (uint8_t)(currentStatus.mapDOT / 10); break; //rate of change of the map 
    readings_JSON["vvt1_angle"] = Serial_ECU.read(); // 93 (int8_t)currentStatus.vvt1Angle; break;
    Serial_ECU.read(); // 94 currentStatus.vvt1TargetAngle; break;
    Serial_ECU.read(); // 95 currentStatus.vvt1Duty; break;
    Serial_ECU.read(); // 96 lowByte(currentStatus.flexBoostCorrection); break;
    Serial_ECU.read(); // 97 highByte(currentStatus.flexBoostCorrection); break;
    Serial_ECU.read(); // 98 currentStatus.baroCorrection; break;
    Serial_ECU.read(); // 99 currentStatus.ASEValue; break; //Current ASE (%)

    // Serial_ECU.read(); // 100 lowByte(currentStatus.vss); break; //speed reading from the speed sensor
    // Serial_ECU.read(); // 101 highByte(currentStatus.vss); break;
    readings_JSON["vss"] = Serial_ECU.read() | (Serial_ECU.read() << 8);


    readings_JSON["current_gear"] = Serial_ECU.read(); // 102 currentStatus.gear; break; 
    Serial_ECU.read(); // 103 currentStatus.fuelPressure; break;
    Serial_ECU.read(); // 104 currentStatus.oilPressure; break;
    Serial_ECU.read(); // 105 currentStatus.wmiPW; break;
    Serial_ECU.read(); // 106 currentStatus.status4; break; // wmiEmptyBit(0), vvt1Error(1), vvt2Error(2), fanStatus(3), UnusedBits(4:7)
    Serial_ECU.read(); // 107 (int8_t)currentStatus.vvt2Angle; break;
    Serial_ECU.read(); // 108 currentStatus.vvt2TargetAngle; break;
    Serial_ECU.read(); // 109 currentStatus.vvt2Duty; break;
    Serial_ECU.read(); // 110 currentStatus.outputsStatus; break;
    Serial_ECU.read(); // 111 (byte)(currentStatus.fuelTemp + CALIBRATION_TEMPERATURE_OFFSET); break; //Fuel temperature from flex sensor
    Serial_ECU.read(); // 112 currentStatus.fuelTempCorrection; break; //Fuel temperature Correction (%)
    Serial_ECU.read(); // 113 currentStatus.VE1; break; //VE 1 (%)
    Serial_ECU.read(); // 114 currentStatus.VE2; break; //VE 2 (%)
    Serial_ECU.read(); // 115 currentStatus.advance1; break; //advance 1 
    Serial_ECU.read(); // 116 currentStatus.advance2; break; //advance 2 
    Serial_ECU.read(); // 117 currentStatus.nitrous_status; break;
    Serial_ECU.read(); // 118 currentStatus.TS_SD_Status; break; //SD card status
    Serial_ECU.read(); // 119 lowByte(currentStatus.EMAP); break; //2 bytes for EMAP
    Serial_ECU.read(); // 120 highByte(currentStatus.EMAP); break;
    Serial_ECU.read(); // 121 currentStatus.fanDuty; break;
    Serial_ECU.read(); // 122 currentStatus.airConStatus; break;



    // Read whatever else there may be - Clear buffer

    while(Serial_ECU.available())
    {
      Serial_ECU.read();
    }
  }
  else
  {
    return;
  }
}

void initSerialData()
{
    readings_JSON["secl"] = 0;

    uint8_t status1 = 0;
    readings_JSON["inj1_status"] = BIT_CHECK(status1, 0);
    readings_JSON["inj2_status"] = BIT_CHECK(status1, 1);
    readings_JSON["inj3_status"] = BIT_CHECK(status1, 2);
    readings_JSON["inj4_status"] = BIT_CHECK(status1, 3);
    readings_JSON["dfco_active"] = BIT_CHECK(status1, 4);
    readings_JSON["boost_cut_fuel"] = BIT_CHECK(status1, 5);
    
    uint8_t engine =0;
    readings_JSON["running"] = BIT_CHECK(engine, 0);
    readings_JSON["cranking"] = BIT_CHECK(engine, 1);
    readings_JSON["warmup"] = BIT_CHECK(engine, 2);

    readings_JSON["dwell"] = 0;
    readings_JSON["MAP"] = 0;
    readings_JSON["IAT"] = 0;
    readings_JSON["CLT"] = 0;
    readings_JSON["TPS"] = 0;
    readings_JSON["correction_voltage"] = 0;
    readings_JSON["Battery_Voltage"] = 0;
    readings_JSON["AFR1"] = 0;
    readings_JSON["PW1"] = 0;
    readings_JSON["correction_o2"] = 0;
    readings_JSON["correction_iat"] = 0;
    readings_JSON["correction_clt"] =0;
    readings_JSON["correction_wue"] = 0;
  
    readings_JSON["rpm"] = 0;

    uint8_t spark_bits = 0;
    readings_JSON["spark_bits"] = spark_bits;

    readings_JSON["launch_hard"] = BIT_CHECK(spark_bits, 0);
    readings_JSON["launch_soft"] = BIT_CHECK(spark_bits, 1);
    readings_JSON["hard_limit_on"] = BIT_CHECK(spark_bits, 2);
    readings_JSON["soft_limit_on"] = BIT_CHECK(spark_bits, 3);
    // Boost cut spark
    readings_JSON["spark_error"] = BIT_CHECK(spark_bits, 5);
    readings_JSON["idle_control_on"] = BIT_CHECK(spark_bits, 6);
    readings_JSON["sync"] = BIT_CHECK(spark_bits, 7);

    
    readings_JSON["current_gear"] = 0;
    readings_JSON["vss"] = 0;
    readings_JSON["vvt1_angle"] = 0;

    readings_JSON["error_codes"] = 0;
    readings_JSON["launch_correction"] = 0;
    readings_JSON["engine_protect_status"] = 0;

}

