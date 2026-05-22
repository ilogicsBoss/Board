
#ifndef MODBUS_RTU_h
#define MODBUS_RTU_h

#include <Arduino.h>
#include "util/LinkedList.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////                    MODBUS_M                   ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __MODBUSMASTER_DEBUG__ (0)
#define __MODBUSMASTER_DEBUG_PIN_A__ 4
#define __MODBUSMASTER_DEBUG_PIN_B__ 5
// Define config for Serial.begin(baud, config);
#ifndef SERIAL_5N1
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

#endif // SERIAL_5N1



/* _____PROJECT INCLUDES_____________________________________________________ */
// functions to calculate Modbus Application Data Unit CRC
#include "util/crc16.h"

// functions to manipulate words
#include "util/word.h"


/* _____CLASS DEFINITIONS____________________________________________________ */
/**
Arduino class library for communicating with Modbus slaves over
RS232/485 (via RTU protocol).
*/
class ModbusMaster
{
public:
	ModbusMaster();

	//void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
	//void begin(unsigned long, uint8_t);
	void begin(uint8_t slave, HardwareSerial *SerialPort, unsigned long baud, byte serialbit = SERIAL_8N1);
	void idle(void (*)());
	void preTransmission(void (*)());
	void postTransmission(void (*)());

	// Modbus exception codes
	/**
	Modbus protocol illegal function exception.

	*/
	static const uint8_t ku8MBIllegalFunction = 0x01;

	/**
	Modbus protocol illegal data address exception.

	The data address received in the query is not an allowable address for
	the server (or slave). More specifically, the combination of reference
	number and transfer length is invalid. For a controller with 100
	registers, the ADU addresses the first register as 0, and the last one
	as 99. If a request is submitted with a starting register address of 96
	and a quantity of registers of 4, then this request will successfully
	operate (address-wise at least) on registers 96, 97, 98, 99. If a
	request is submitted with a starting register address of 96 and a
	quantity of registers of 5, then this request will fail with Exception
	Code 0x02 "Illegal Data Address" since it attempts to operate on
	registers 96, 97, 98, 99 and 100, and there is no register with address
	100.

	@ingroup constant
	*/
	static const uint8_t ku8MBIllegalDataAddress = 0x02;

	/**
	Modbus protocol illegal data value exception.
	@ingroup constant
	*/
	static const uint8_t ku8MBIllegalDataValue = 0x03;

	/**
	Modbus protocol slave device failure exception.

	An unrecoverable error occurred while the server (or slave) was
	attempting to perform the requested action.

	@ingroup constant
	*/
	static const uint8_t ku8MBSlaveDeviceFailure = 0x04;

	// Class-defined success/exception codes
	/**
	ModbusMaster success.

	Modbus transaction was successful; the following checks were valid:
	  - slave ID
	  - function code
	  - response code
	  - data
	  - CRC

	@ingroup constant
	*/
	static const uint8_t ku8MBSuccess = 0x00;

	/**
	ModbusMaster invalid response slave ID exception.

	The slave ID in the response does not match that of the request.

	@ingroup constant
	*/
	static const uint8_t ku8MBInvalidSlaveID = 0xE0;

	/**
	ModbusMaster invalid response function exception.

	The function code in the response does not match that of the request.

	@ingroup constant
	*/
	static const uint8_t ku8MBInvalidFunction = 0xE1;

	/**
	ModbusMaster response timed out exception.

	The entire response was not received within the timeout period,
	ModbusMaster::ku8MBResponseTimeout.

	@ingroup constant
	*/
	static const uint8_t ku8MBResponseTimedOut = 0xE2;

	/**
	ModbusMaster invalid response CRC exception.

	The CRC in the response does not match the one calculated.

	@ingroup constant
	*/
	static const uint8_t ku8MBInvalidCRC = 0xE3;

	uint16_t GET_RESPONSE_BUFFER(uint8_t);
	uint16_t getResponseBuffer(uint8_t);
	void     CLAER_RESPONSE_BUFFER();
	void     clearResponseBuffer();
	uint8_t  SET_TRANSMIT_BUFFER(uint8_t, uint16_t);
	uint8_t  setTransmitBuffer(uint8_t, uint16_t);
	void     CLEAR_TRANSMIT_BUFFER();
	void     clearTransmitBuffer();

	void beginTransmission(uint16_t);
	uint8_t requestFrom(uint16_t, uint16_t);
	void sendBit(bool);
	void send(uint8_t);
	void send(uint16_t);
	void send(uint32_t);
	uint8_t available(void);
	uint16_t RECEIVE(void);
	uint16_t receive(void);


	uint8_t  READ_COILS(uint16_t, uint16_t);
	uint8_t  readCoils(uint16_t, uint16_t);
	uint8_t  READ_DISCRETE_INPUTS(uint16_t, uint16_t);
	uint8_t  readDiscreteInputs(uint16_t, uint16_t);
	uint8_t  READ_HOLDING_REGISTERS(uint16_t, uint16_t);
	uint8_t  readHoldingRegisters(uint16_t, uint16_t);
	uint8_t  READ_INPUT_REGISTERS(uint16_t, uint8_t);
	uint8_t  readInputRegisters(uint16_t, uint8_t);
	uint8_t  WRITE_SINGLE_COIL(uint16_t, uint8_t);
	uint8_t  writeSingleCoil(uint16_t, uint8_t);
	uint8_t  WRITE_SINGLE_REGISTER(uint16_t, uint16_t);
	uint8_t  writeSingleRegister(uint16_t, uint16_t);
	uint8_t  WRITE_MULTIPLE_COILS(uint16_t, uint16_t);
	uint8_t  writeMultipleCoils(uint16_t, uint16_t);
	uint8_t  WRITE_MULTIPLE_COILS();
	uint8_t  writeMultipleCoils();
	uint8_t  WRITE_MULTIPLE_REGISTERS(uint16_t, uint16_t);
	uint8_t  writeMultipleRegisters(uint16_t, uint16_t);
	uint8_t  WRITE_MULTIPLE_REGISTERS();
	uint8_t  writeMultipleRegisters();
	uint8_t  MASK_WRITE_REGISTER(uint16_t, uint16_t, uint16_t);
	uint8_t  maskWriteRegister(uint16_t, uint16_t, uint16_t);
	uint8_t  READ_WRITE_MULTIPLE_REGISTERS(uint16_t, uint16_t, uint16_t, uint16_t);
	uint8_t  readWriteMultipleRegisters(uint16_t, uint16_t, uint16_t, uint16_t);
	uint8_t  READ_WRITE_MULTIPLE_REGISTERS(uint16_t, uint16_t);
	uint8_t  readWriteMultipleRegisters(uint16_t, uint16_t);

	// Modbus timeout [milliseconds]
	uint16_t ku16MBResponseTimeout = 100; ///< Modbus timeout [milliseconds]

private:
	Stream* _serial;                                             ///< reference to serial port object
	uint8_t  _u8MBSlave;                                         ///< Modbus slave (1..255) initialized in begin()
	static const uint8_t ku8MaxBufferSize = 64;   ///< size of response/transmit buffers    
	uint16_t _u16ReadAddress;                                    ///< slave register from which to read
	uint16_t _u16ReadQty;                                        ///< quantity of words to read
	uint16_t _u16ResponseBuffer[ku8MaxBufferSize];               ///< buffer to store Modbus slave response; read via GetResponseBuffer()
	uint16_t _u16WriteAddress;                                   ///< slave register to which to write
	uint16_t _u16WriteQty;                                       ///< quantity of words to write
	uint16_t _u16TransmitBuffer[ku8MaxBufferSize];               ///< buffer containing data to transmit to Modbus slave; set via SetTransmitBuffer()
	uint16_t* txBuffer; // from Wire.h -- need to clean this up Rx
	uint8_t _u8TransmitBufferIndex;
	uint16_t u16TransmitBufferLength;
	uint16_t* rxBuffer; // from Wire.h -- need to clean this up Rx
	uint8_t _u8ResponseBufferIndex;
	uint8_t _u8ResponseBufferLength;

	// Modbus function codes for bit access
	static const uint8_t ku8MBReadCoils = 0x01; ///< Modbus function 0x01 Read Coils
	static const uint8_t ku8MBReadDiscreteInputs = 0x02; ///< Modbus function 0x02 Read Discrete Inputs
	static const uint8_t ku8MBWriteSingleCoil = 0x05; ///< Modbus function 0x05 Write Single Coil
	static const uint8_t ku8MBWriteMultipleCoils = 0x0F; ///< Modbus function 0x0F Write Multiple Coils

	// Modbus function codes for 16 bit access
	static const uint8_t ku8MBReadHoldingRegisters = 0x03; ///< Modbus function 0x03 Read Holding Registers
	static const uint8_t ku8MBReadInputRegisters = 0x04; ///< Modbus function 0x04 Read Input Registers
	static const uint8_t ku8MBWriteSingleRegister = 0x06; ///< Modbus function 0x06 Write Single Register
	static const uint8_t ku8MBWriteMultipleRegisters = 0x10; ///< Modbus function 0x10 Write Multiple Registers
	static const uint8_t ku8MBMaskWriteRegister = 0x16; ///< Modbus function 0x16 Mask Write Register
	static const uint8_t ku8MBReadWriteMultipleRegisters = 0x17; ///< Modbus function 0x17 Read Write Multiple Registers

	// master function that conducts Modbus transactions
	uint8_t ModbusMasterTransaction(uint8_t u8MBFunction);

	// idle callback function; gets called during idle time between TX and RX
	void (*_idle)();
	// preTransmission callback function; gets called before writing a Modbus message
	void (*_preTransmission)();
	// postTransmission callback function; gets called after a Modbus message has been sent
	void (*_postTransmission)();
};

/**
@example examples/Basic/Basic.pde
@example examples/PhoenixContact_nanoLC/PhoenixContact_nanoLC.pde
@example examples/RS485_HalfDuplex/RS485_HalfDuplex.ino
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////                    MODBUS_S                   ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ModbusRTUSlaveWordAddress
{
public:
	u16 addr;
	u16 len;
	u16* values;
	ModbusRTUSlaveWordAddress(u16 Address, u16* value, int cnt);
};

class ModbusRTUSlaveBitAddress
{
public:
	u16 addr;
	u16 len;
	u8* values;
	ModbusRTUSlaveBitAddress(u16 Address, u8* value, int cnt);
};

class ModbusRTUSlave
{
public:
	ModbusRTUSlave(byte Slave, HardwareSerial *serialport, unsigned long baud, byte serialbit = SERIAL_8N1);
	boolean addWordArea(u16 Address, u16* values, int cnt);
	boolean addBitArea(u16 Address, u8* values, int cnt);
	void process();

private:
	byte slave;
	HardwareSerial* ser;
	LinkedList<ModbusRTUSlaveWordAddress*>* words;
	LinkedList<ModbusRTUSlaveBitAddress*>* bits;
	ModbusRTUSlaveWordAddress* getWordAddress(u16 Addr);
	ModbusRTUSlaveBitAddress* getBitAddress(u16 Addr);
	ModbusRTUSlaveWordAddress* getWordAddress(u16 Addr, u16 Len);
	ModbusRTUSlaveBitAddress* getBitAddress(u16 Addr, u16 Len);
	byte lstResponse[300];
	int ResCnt = 0;
	unsigned long lastrecv;
	void getCRC(byte* pby, int arsize, int startindex, int nSize, byte* byFirstReturn, byte* bySecondReturn);
};

const byte auchCRCHi1[] = {
						0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
						0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
						0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
						0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
						0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
						0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
						0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
						0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
						0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
						0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
						0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
						0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
						0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
						0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
						0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
						0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
						0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
						0x40 };

const byte auchCRCLo1[] = {
						0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
						0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
						0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
						0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
						0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
						0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
						0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
						0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
						0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
						0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
						0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
						0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
						0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
						0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
						0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
						0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
						0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
						0x40 };

boolean getBit(u8* area, int index);
void setBit(u8* area, int index, bool value);

extern bool Timer1On_m;

namespace MODTIMER {
	extern unsigned long msecs1_m;
	extern void (*func1_m)();
	extern volatile unsigned long count1_m;
	extern volatile char overflowing1_m;
	extern volatile unsigned int tcnt1_m;
	

	void SET(void (*f)());
	void START(uint8_t Numstart = 10);
	void STOP(uint8_t Numstop = 10);
	void _overflow1_m();
};

#endif 