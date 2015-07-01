#ifndef MODBUS_H
#define MODBUS_H

//Include section
#include <iostream>
#include <vector>
#include <bitset>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>
#include <math.h>
#include <bitset>

//Define section
#define DReadOut 0x01
#define AReadOut 0x03
#define DReadMulOut 0x0F   //15 decimal

#define DWriteOut 0x02
#define AWriteOut 0x06
#define AWriteMulOut 0x10   //16 decimal

//typedef section
typedef unsigned char byte;

using namespace std;


class  ModbusServer {

public:
  byte id;
  clock_t start_milis;
  time_t start_seconds;
  vector<bool> digital_output;
  vector<bool> digital_input;
  vector<int> analog_output;
  vector<int> analog_input;

  ModbusServer( byte id );
  ~ModbusServer();

  vector<byte> peticion( vector<byte> recibido );

private:
  void SetData(void);
  void UpdateData(int bytes_recibidos);

  int BytesToInt(byte byte1, byte byte2);
  int ByteToInt(byte byte1);

  vector<byte> IntToByte(int input);

  vector<byte> CRC16(vector<byte> mensaje, int len);
  vector<byte> CRC16(vector<byte> mensaje);
  bool CheckCRC(vector<byte> input);

  void AddVector(vector<byte> *vector_1, vector<byte> vector_2);

  void PrintDigitalVector(vector<bool> input, string str);
  void PrintAnalogVector(vector<int> input, string str);
  void PrintVectors(void);

  vector<byte> ErrorIllegalFunction_01(vector<byte> input);
  vector<byte> ErrorIllegalDataAddress_02(vector<byte> input);
  vector<byte> ErrorIllegalDataValue_03(vector<byte> input);

  vector<byte> ReadDigitalOutput_01(vector<byte> input);
  vector<byte> ReadDigitalInput_02(vector<byte> input);
  vector<byte> ReadAnalogOutput_03(vector<byte> input);
  vector<byte> WriteAnalogInput_04(vector<byte> input);
  vector<byte> WriteDigitalOutput_05(vector<byte> input);
  vector<byte> WriteAnalogOutput_06(vector<byte> input);
  vector<byte> WriteDigitalOutputMultiple_15(vector<byte> input);
  vector<byte> WriteAnalogOutputMultiple_16(vector<byte> input);
};



#endif
