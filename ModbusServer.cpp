#include "ModbusServer.hpp"

#define DIGITAL_INPUT_CAP 14
#define ANALOG_OUTPUT_FACTOR 4

using namespace std;

ModbusServer::ModbusServer(byte id) {
  const clock_t begin_time = clock();

  this->digital_output = vector<bool> (20, false);
  this->digital_input = vector<bool> (20, false);
  this->analog_output = vector<int> (10, 0);
  this->analog_input = vector<int> (20, 0);

  unsigned int i;
  //Digital Output
  for (i = 1; i < this->digital_output.size(); i+=2)
    this->digital_output[i] = true;

  //Analog Output
  for (i = 0; i < this->analog_output.size(); i++)
    this->analog_output[i] = i * ANALOG_OUTPUT_FACTOR;

  //Analog Input
  for (i = 15; i < this->analog_input.size(); i++)
    if (i % 2 == 0)
      this->analog_input[i] = 1111;
    else
      this->analog_input[i] = 0;

  time_t timer = time(NULL);
  tm* y2k = localtime(&timer);

  //Los 6 [0-5] primeros: año, mes, dia, hora, minuto, segundo
  this->analog_input[0] = y2k->tm_year + 1900;
  this->analog_input[1] = y2k->tm_mon + 1;
  this->analog_input[2] = y2k->tm_mday;
  this->analog_input[3] = y2k->tm_hour;
  this->analog_input[4] = y2k->tm_min;
  this->analog_input[5] = y2k->tm_sec;
  //Los 4 [6-9] siguientes: UID, GID, PID, PPID
  this->analog_input[6] = getuid();
  this->analog_input[7] = getgid();
  this->analog_input[8] = getpid();
  this->analog_input[9] = getppid();
  //Los 2 [10-11] siguientes: segundos y milisegundos de computo del proceso
  float end_time = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
  double seconds;
  float milisec = modf(end_time, &seconds);
  this->analog_input[10] = (int) seconds;
  this->analog_input[11] = (int) (milisec * 10000);
  //Los 3 [12-14] siguientes: contador peticiones recibidas, numero de bytes recibido, numero de bytes enviado
  this->analog_input[12] = 0;
  this->analog_input[13] = 0;
  this->analog_input[14] = 0;

  //Digital Input
  for (i = 0; i < DIGITAL_INPUT_CAP; i++)
    if(this->analog_input[i] % 2 == 0)
      this->digital_input[i] = true;

  //Printing Vector
  PrintDigitalVector(this->digital_output, "Output");
  PrintDigitalVector(this->digital_input, "Input");
  PrintAnalogVector(this->analog_output, "Output");
  PrintAnalogVector(this->analog_input, "Input");
}

ModbusServer::~ModbusServer() { }

vector<byte> ModbusServer::peticion(vector<byte> recibido)
{
  vector<byte> output;

  return output;
}

vector<byte> ModbusServer::ReadAnalogOutput()
{
  vector<byte> output;

  return output;
}

void ModbusServer::PrintDigitalVector(vector<bool> input, string str)
{
  unsigned int i;

  cout << "Digital " << str << " -> [ ";
  for (i = 0; i < input.size(); i++)
    cout << input[i] << " ";
  cout << "]\n";
}

void ModbusServer::PrintAnalogVector(vector<int> input, string str)
{
  unsigned int i;

  cout << "Analog " << str << " -> [ ";
  for (i = 0; i < input.size(); i++)
    cout << input[i] << " ";
  cout << "]\n";
}

