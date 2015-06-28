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
  vector<bool> digital_output;
  vector<bool> digital_input;
  vector<int> analog_output;
  vector<int> analog_input;

  ModbusServer( byte id );
  ~ModbusServer();

  vector<byte> peticion( vector<byte> recibido );

private:
  void PrintDigitalVector(vector<bool> input, string str);
  void PrintAnalogVector(vector<int> input, string str);
  void PrintQueryVector(vector<byte> input);

  int ByteToInt(byte byte1, byte byte2);
  vector<byte> IntToByte(int input);
  void AddVector(vector<byte> *vector_1, vector<byte> vector_2);
  vector<byte> CRC16( vector<byte> mensaje, int len);
  vector<byte> CRC16( vector<byte> mensaje);
  void PrintVectors(void);

  vector<byte> ReadAnalogOutput_03(vector<byte> input);
};



#endif
