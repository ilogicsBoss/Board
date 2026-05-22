
/*

  ModbusMaster.cpp - Arduino library for communicating with Modbus slaves
  over RS232/485 (via RTU protocol).

  Library:: ModbusMaster

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

	  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#include "MODBUS_RTU.h"

#ifndef MODBUS_RTU_MASTER
#define MODBUS_RTU_MASTER
ModbusMaster::ModbusMaster(void)
{
	_idle = 0;
	_preTransmission = 0;
	_postTransmission = 0;
}

/**
Initialize class object.

@param slave Modbus slave ID (1..255)
@param &serial reference to serial port object (Serial, Serial1, ... Serial3)
*/

void ModbusMaster::begin(uint8_t slave, HardwareSerial *SerialPort, unsigned long baud, byte serialbit)
{
	//  txBuffer = (uint16_t*) calloc(ku8MaxBufferSize, sizeof(uint16_t));
	_u8MBSlave = slave;
	_serial = SerialPort;
	_u8TransmitBufferIndex = 0;
	u16TransmitBufferLength = 0;
	(*SerialPort).begin(baud, serialbit);
#if __MODBUSMASTER_DEBUG__
	pinMode(__MODBUSMASTER_DEBUG_PIN_A__, OUTPUT);
	pinMode(__MODBUSMASTER_DEBUG_PIN_B__, OUTPUT);
#endif
	
}

void ModbusMaster::beginTransmission(uint16_t u16Address)
{
	_u16WriteAddress = u16Address;
	_u8TransmitBufferIndex = 0;
	u16TransmitBufferLength = 0;
}

// eliminate this function in favor of using existing MB request functions
uint8_t ModbusMaster::requestFrom(uint16_t address, uint16_t quantity)
{
	uint8_t read;
	// clamp to buffer length
	if (quantity > ku8MaxBufferSize)
	{
		quantity = ku8MaxBufferSize;
	}
	// set rx buffer iterator vars
	_u8ResponseBufferIndex = 0;
	_u8ResponseBufferLength = read;

	return read;
}


void ModbusMaster::sendBit(bool data)
{
	uint8_t txBitIndex = u16TransmitBufferLength % 16;
	if ((u16TransmitBufferLength >> 4) < ku8MaxBufferSize)
	{
		if (0 == txBitIndex)
		{
			_u16TransmitBuffer[_u8TransmitBufferIndex] = 0;
		}
		bitWrite(_u16TransmitBuffer[_u8TransmitBufferIndex], txBitIndex, data);
		u16TransmitBufferLength++;
		_u8TransmitBufferIndex = u16TransmitBufferLength >> 4;
	}
}


void ModbusMaster::send(uint16_t data)
{
	if (_u8TransmitBufferIndex < ku8MaxBufferSize)
	{
		_u16TransmitBuffer[_u8TransmitBufferIndex++] = data;
		u16TransmitBufferLength = _u8TransmitBufferIndex << 4;
	}
}

void ModbusMaster::send(uint32_t data)
{
	send(lowWord(data));
	send(highWord(data));
}

void ModbusMaster::send(uint8_t data)
{
	send(word(data));
}


uint8_t ModbusMaster::available(void)
{
	return _u8ResponseBufferLength - _u8ResponseBufferIndex;
}

uint16_t ModbusMaster::RECEIVE(void)
{
	if (_u8ResponseBufferIndex < _u8ResponseBufferLength)
	{
		return _u16ResponseBuffer[_u8ResponseBufferIndex++];
	}
	else
	{
		return 0xFFFF;
	}
}
uint16_t ModbusMaster::receive(void)
{
	if (_u8ResponseBufferIndex < _u8ResponseBufferLength)
	{
		return _u16ResponseBuffer[_u8ResponseBufferIndex++];
	}
	else
	{
		return 0xFFFF;
	}
}

/**
Set idle time callback function (cooperative multitasking).
*/
void ModbusMaster::idle(void (*idle)())
{
	_idle = idle;
}

/**
Set pre-transmission callback function.

*/
void ModbusMaster::preTransmission(void (*preTransmission)())
{
	_preTransmission = preTransmission;
}

/**
Set post-transmission callback function.

*/
void ModbusMaster::postTransmission(void (*postTransmission)())
{
	_postTransmission = postTransmission;
}


/**
Retrieve data from response buffer.
*/
uint16_t ModbusMaster::GET_RESPONSE_BUFFER(uint8_t u8Index)
{
	if (u8Index < ku8MaxBufferSize)
	{
		return _u16ResponseBuffer[u8Index];
	}
	else
	{
		return 0xFFFF;
	}
}
uint16_t ModbusMaster::getResponseBuffer(uint8_t u8Index)
{
	if (u8Index < ku8MaxBufferSize)
	{
		return _u16ResponseBuffer[u8Index];
	}
	else
	{
		return 0xFFFF;
	}
}


/**
Clear Modbus response buffer.
*/
void ModbusMaster::CLAER_RESPONSE_BUFFER() {
	uint8_t i;

	for (i = 0; i < ku8MaxBufferSize; i++)
	{
		_u16ResponseBuffer[i] = 0;
	}
}
void ModbusMaster::clearResponseBuffer()
{
	uint8_t i;

	for (i = 0; i < ku8MaxBufferSize; i++)
	{
		_u16ResponseBuffer[i] = 0;
	}
}


/**
Place data in transmit buffer.
*/
uint8_t ModbusMaster::SET_TRANSMIT_BUFFER(uint8_t u8Index, uint16_t u16Value) {
	return setTransmitBuffer( u8Index, u16Value);
}
uint8_t ModbusMaster::setTransmitBuffer(uint8_t u8Index, uint16_t u16Value)
{
	if (u8Index < ku8MaxBufferSize)
	{
		_u16TransmitBuffer[u8Index] = u16Value;
		return ku8MBSuccess;
	}
	else
	{
		return ku8MBIllegalDataAddress;
	}
}


/**
Clear Modbus transmit buffer.
*/
void ModbusMaster::CLEAR_TRANSMIT_BUFFER() {
	uint8_t i;

	for (i = 0; i < ku8MaxBufferSize; i++)
	{
		_u16TransmitBuffer[i] = 0;
	}
}
void ModbusMaster::clearTransmitBuffer()
{
	uint8_t i;

	for (i = 0; i < ku8MaxBufferSize; i++)
	{
		_u16TransmitBuffer[i] = 0;
	}
}


/**
Modbus function 0x01 Read Coils.
*/
uint8_t ModbusMaster::READ_COILS(uint16_t u16ReadAddress, uint16_t u16BitQty) {
	return readCoils(u16ReadAddress, u16BitQty);
}
uint8_t ModbusMaster::readCoils(uint16_t u16ReadAddress, uint16_t u16BitQty)
{
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16BitQty;
	return ModbusMasterTransaction(ku8MBReadCoils);
}


/**
Modbus function 0x02 Read Discrete Inputs.
*/
uint8_t ModbusMaster::READ_DISCRETE_INPUTS(uint16_t u16ReadAddress,
	uint16_t u16BitQty) {
	return readDiscreteInputs(u16ReadAddress, u16BitQty);
}

uint8_t ModbusMaster::readDiscreteInputs(uint16_t u16ReadAddress,
	uint16_t u16BitQty)
{
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16BitQty;
	return ModbusMasterTransaction(ku8MBReadDiscreteInputs);
}


/**
Modbus function 0x03 Read Holding Registers.
*/
uint8_t ModbusMaster::READ_HOLDING_REGISTERS(uint16_t u16ReadAddress,
	uint16_t u16ReadQty) {
	return readHoldingRegisters(u16ReadAddress, u16ReadQty);
}
uint8_t ModbusMaster::readHoldingRegisters(uint16_t u16ReadAddress,
	uint16_t u16ReadQty)
{
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16ReadQty;
	return ModbusMasterTransaction(ku8MBReadHoldingRegisters);
}


/**
Modbus function 0x04 Read Input Registers.
*/
uint8_t ModbusMaster::READ_INPUT_REGISTERS(uint16_t u16ReadAddress,
	uint8_t u16ReadQty) {
	return readInputRegisters( u16ReadAddress,	u16ReadQty);
}
uint8_t ModbusMaster::readInputRegisters(uint16_t u16ReadAddress,
		uint8_t u16ReadQty)
{
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16ReadQty;
	return ModbusMasterTransaction(ku8MBReadInputRegisters);
}

/**
Modbus function 0x05 Write Single Coil.
*/
uint8_t ModbusMaster::WRITE_SINGLE_COIL(uint16_t u16WriteAddress, uint8_t u8State)
{
	return writeSingleCoil(u16WriteAddress, u8State);
}
uint8_t ModbusMaster::writeSingleCoil(uint16_t u16WriteAddress, uint8_t u8State)
{
	_u16WriteAddress = u16WriteAddress;
	_u16WriteQty = (u8State ? 0xFF00 : 0x0000);
	return ModbusMasterTransaction(ku8MBWriteSingleCoil);
}


/**
Modbus function 0x06 Write Single Register.
*/
uint8_t ModbusMaster::WRITE_SINGLE_REGISTER(uint16_t u16WriteAddress,
	uint16_t u16WriteValue) {
	return writeSingleRegister(u16WriteAddress, u16WriteValue);
}
uint8_t ModbusMaster::writeSingleRegister(uint16_t u16WriteAddress,
	uint16_t u16WriteValue)
{
	_u16WriteAddress = u16WriteAddress;
	_u16WriteQty = 0;
	_u16TransmitBuffer[0] = u16WriteValue;
	return ModbusMasterTransaction(ku8MBWriteSingleRegister);
}


/**
Modbus function 0x0F Write Multiple Coils.
*/
uint8_t ModbusMaster::WRITE_MULTIPLE_COILS(uint16_t u16WriteAddress,
	uint16_t u16BitQty) {
	return writeMultipleCoils(u16WriteAddress, u16BitQty);
}
uint8_t ModbusMaster::writeMultipleCoils(uint16_t u16WriteAddress,
	uint16_t u16BitQty)
{
	_u16WriteAddress = u16WriteAddress;
	_u16WriteQty = u16BitQty;
	return ModbusMasterTransaction(ku8MBWriteMultipleCoils);
}

uint8_t ModbusMaster::WRITE_MULTIPLE_COILS()
{
	return writeMultipleCoils();
}
uint8_t ModbusMaster::writeMultipleCoils()
{
	_u16WriteQty = u16TransmitBufferLength;
	return ModbusMasterTransaction(ku8MBWriteMultipleCoils);
}


/**
Modbus function 0x10 Write Multiple Registers.
*/
uint8_t ModbusMaster::WRITE_MULTIPLE_REGISTERS(uint16_t u16WriteAddress,
	uint16_t u16WriteQty) {
	return writeMultipleRegisters(u16WriteAddress, u16WriteQty);
}
uint8_t ModbusMaster::writeMultipleRegisters(uint16_t u16WriteAddress,
		uint16_t u16WriteQty)
{
	_u16WriteAddress = u16WriteAddress;
	_u16WriteQty = u16WriteQty;
	return ModbusMasterTransaction(ku8MBWriteMultipleRegisters);
}

// new version based on Wire.h
uint8_t ModbusMaster::WRITE_MULTIPLE_REGISTERS() {
	return writeMultipleRegisters();
}
uint8_t ModbusMaster::writeMultipleRegisters()
{
	_u16WriteQty = _u8TransmitBufferIndex;
	return ModbusMasterTransaction(ku8MBWriteMultipleRegisters);
}


/**
Modbus function 0x16 Mask Write Register.
*/
uint8_t ModbusMaster::MASK_WRITE_REGISTER(uint16_t u16WriteAddress,
	uint16_t u16AndMask, uint16_t u16OrMask)
{
	return maskWriteRegister(u16WriteAddress, u16AndMask, u16OrMask);
}
uint8_t ModbusMaster::maskWriteRegister(uint16_t u16WriteAddress,
	uint16_t u16AndMask, uint16_t u16OrMask)
{
	_u16WriteAddress = u16WriteAddress;
	_u16TransmitBuffer[0] = u16AndMask;
	_u16TransmitBuffer[1] = u16OrMask;
	return ModbusMasterTransaction(ku8MBMaskWriteRegister);
}


/**
Modbus function 0x17 Read Write Multiple Registers.
*/
uint8_t ModbusMaster::READ_WRITE_MULTIPLE_REGISTERS(uint16_t u16ReadAddress,
	uint16_t u16ReadQty, uint16_t u16WriteAddress, uint16_t u16WriteQty)
{
	return readWriteMultipleRegisters(u16ReadAddress, u16ReadQty, u16WriteAddress, u16WriteQty);
}
uint8_t ModbusMaster::readWriteMultipleRegisters(uint16_t u16ReadAddress,
	uint16_t u16ReadQty, uint16_t u16WriteAddress, uint16_t u16WriteQty)
{
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16ReadQty;
	_u16WriteAddress = u16WriteAddress;
	_u16WriteQty = u16WriteQty;
	return ModbusMasterTransaction(ku8MBReadWriteMultipleRegisters);
}

uint8_t ModbusMaster::READ_WRITE_MULTIPLE_REGISTERS(uint16_t u16ReadAddress,
	uint16_t u16ReadQty) {
	return readWriteMultipleRegisters(u16ReadAddress, u16ReadQty);
}
uint8_t ModbusMaster::readWriteMultipleRegisters(uint16_t u16ReadAddress,
	uint16_t u16ReadQty)
{
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16ReadQty;
	_u16WriteQty = _u8TransmitBufferIndex;
	return ModbusMasterTransaction(ku8MBReadWriteMultipleRegisters);
}

uint8_t ModbusMaster::ModbusMasterTransaction(uint8_t u8MBFunction)
{
	uint8_t u8ModbusADU[256];
	uint8_t u8ModbusADUSize = 0;
	uint8_t i, u8Qty;
	uint16_t u16CRC;
	uint32_t u32StartTime;
	uint8_t u8BytesLeft = 8;
	uint8_t u8MBStatus = ku8MBSuccess;

	// assemble Modbus Request Application Data Unit
	u8ModbusADU[u8ModbusADUSize++] = _u8MBSlave;
	u8ModbusADU[u8ModbusADUSize++] = u8MBFunction;

	switch (u8MBFunction)
	{
	case ku8MBReadCoils:
	case ku8MBReadDiscreteInputs:
	case ku8MBReadInputRegisters:
	case ku8MBReadHoldingRegisters:
	case ku8MBReadWriteMultipleRegisters:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16ReadAddress);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16ReadAddress);
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16ReadQty);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16ReadQty);
		break;
	}

	switch (u8MBFunction)
	{
	case ku8MBWriteSingleCoil:
	case ku8MBMaskWriteRegister:
	case ku8MBWriteMultipleCoils:
	case ku8MBWriteSingleRegister:
	case ku8MBWriteMultipleRegisters:
	case ku8MBReadWriteMultipleRegisters:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteAddress);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteAddress);
		break;
	}

	switch (u8MBFunction)
	{
	case ku8MBWriteSingleCoil:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
		break;

	case ku8MBWriteSingleRegister:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[0]);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[0]);
		break;

	case ku8MBWriteMultipleCoils:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
		u8Qty = (_u16WriteQty % 8) ? ((_u16WriteQty >> 3) + 1) : (_u16WriteQty >> 3);
		u8ModbusADU[u8ModbusADUSize++] = u8Qty;
		for (i = 0; i < u8Qty; i++)
		{
			switch (i % 2)
			{
			case 0: // i is even
				u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[i >> 1]);
				break;

			case 1: // i is odd
				u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[i >> 1]);
				break;
			}
		}
		break;

	case ku8MBWriteMultipleRegisters:
	case ku8MBReadWriteMultipleRegisters:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty << 1);

		for (i = 0; i < lowByte(_u16WriteQty); i++)
		{
			u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[i]);
			u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[i]);
		}
		break;

	case ku8MBMaskWriteRegister:
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[0]);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[0]);
		u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[1]);
		u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[1]);
		break;
	}

	// append CRC
	u16CRC = 0xFFFF;
	for (i = 0; i < u8ModbusADUSize; i++)
	{
		u16CRC = crc16_update(u16CRC, u8ModbusADU[i]);
	}
	u8ModbusADU[u8ModbusADUSize++] = lowByte(u16CRC);
	u8ModbusADU[u8ModbusADUSize++] = highByte(u16CRC);
	u8ModbusADU[u8ModbusADUSize] = 0;

	// flush receive buffer before transmitting request
	while (_serial->read() != -1);

	// transmit request
	if (_preTransmission)
	{
		_preTransmission();
	}
	for (i = 0; i < u8ModbusADUSize; i++)
	{
		_serial->write(u8ModbusADU[i]);
	}

	u8ModbusADUSize = 0;
	_serial->flush();    // flush transmit buffer
	if (_postTransmission)
	{
		_postTransmission();
	}

	// loop until we run out of time or bytes, or an error occurs
	u32StartTime = millis();
	while (u8BytesLeft && !u8MBStatus)
	{
		if (_serial->available())
		{
#if __MODBUSMASTER_DEBUG__
			digitalWrite(__MODBUSMASTER_DEBUG_PIN_A__, true);
#endif
			u8ModbusADU[u8ModbusADUSize++] = _serial->read();
			u8BytesLeft--;
#if __MODBUSMASTER_DEBUG__
			digitalWrite(__MODBUSMASTER_DEBUG_PIN_A__, false);
#endif
		}
		else
		{
#if __MODBUSMASTER_DEBUG__
			digitalWrite(__MODBUSMASTER_DEBUG_PIN_B__, true);
#endif
			if (_idle)
			{
				_idle();
			}
#if __MODBUSMASTER_DEBUG__
			digitalWrite(__MODBUSMASTER_DEBUG_PIN_B__, false);
#endif
		}

		// evaluate slave ID, function code once enough bytes have been read
		if (u8ModbusADUSize == 5)
		{
			// verify response is for correct Modbus slave
			if (u8ModbusADU[0] != _u8MBSlave)
			{
				u8MBStatus = ku8MBInvalidSlaveID;
				break;
			}

			// verify response is for correct Modbus function code (mask exception bit 7)
			if ((u8ModbusADU[1] & 0x7F) != u8MBFunction)
			{
				u8MBStatus = ku8MBInvalidFunction;
				break;
			}

			// check whether Modbus exception occurred; return Modbus Exception Code
			if (bitRead(u8ModbusADU[1], 7))
			{
				u8MBStatus = u8ModbusADU[2];
				break;
			}

			// evaluate returned Modbus function code
			switch (u8ModbusADU[1])
			{
			case ku8MBReadCoils:
			case ku8MBReadDiscreteInputs:
			case ku8MBReadInputRegisters:
			case ku8MBReadHoldingRegisters:
			case ku8MBReadWriteMultipleRegisters:
				u8BytesLeft = u8ModbusADU[2];
				break;

			case ku8MBWriteSingleCoil:
			case ku8MBWriteMultipleCoils:
			case ku8MBWriteSingleRegister:
			case ku8MBWriteMultipleRegisters:
				u8BytesLeft = 3;
				break;

			case ku8MBMaskWriteRegister:
				u8BytesLeft = 5;
				break;
			}
		}
		if ((millis() - u32StartTime) > ku16MBResponseTimeout)
		{
			u8MBStatus = ku8MBResponseTimedOut;
		}
	}

	// verify response is large enough to inspect further
	if (!u8MBStatus && u8ModbusADUSize >= 5)
	{
		// calculate CRC
		u16CRC = 0xFFFF;
		for (i = 0; i < (u8ModbusADUSize - 2); i++)
		{
			u16CRC = crc16_update(u16CRC, u8ModbusADU[i]);
		}

		// verify CRC
		if (!u8MBStatus && (lowByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 2] ||
			highByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 1]))
		{
			u8MBStatus = ku8MBInvalidCRC;
		}
	}

	// disassemble ADU into words
	if (!u8MBStatus)
	{
		// evaluate returned Modbus function code
		switch (u8ModbusADU[1])
		{
		case ku8MBReadCoils:
		case ku8MBReadDiscreteInputs:
			// load bytes into word; response bytes are ordered L, H, L, H, ...
			for (i = 0; i < (u8ModbusADU[2] >> 1); i++)
			{
				if (i < ku8MaxBufferSize)
				{
					_u16ResponseBuffer[i] = word(u8ModbusADU[2 * i + 4], u8ModbusADU[2 * i + 3]);
				}

				_u8ResponseBufferLength = i;
			}

			// in the event of an odd number of bytes, load last byte into zero-padded word
			if (u8ModbusADU[2] % 2)
			{
				if (i < ku8MaxBufferSize)
				{
					_u16ResponseBuffer[i] = word(0, u8ModbusADU[2 * i + 3]);
				}

				_u8ResponseBufferLength = i + 1;
			}
			break;

		case ku8MBReadInputRegisters:
		case ku8MBReadHoldingRegisters:
		case ku8MBReadWriteMultipleRegisters:
			// load bytes into word; response bytes are ordered H, L, H, L, ...
			for (i = 0; i < (u8ModbusADU[2] >> 1); i++)
			{
				if (i < ku8MaxBufferSize)
				{
					_u16ResponseBuffer[i] = word(u8ModbusADU[2 * i + 3], u8ModbusADU[2 * i + 4]);
				}

				_u8ResponseBufferLength = i;
			}
			break;
		}
	}

	_u8TransmitBufferIndex = 0;
	u16TransmitBufferLength = 0;
	_u8ResponseBufferIndex = 0;
	return u8MBStatus;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////		MODBUSSLAVE				MODBUSSLAVE		      ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MODBUS_RTU_SLAVE
#define MODBUS_RTU_SLAVE

ModbusRTUSlave::ModbusRTUSlave(byte slaveAddress, HardwareSerial* serialport, unsigned long baud, byte serialbit)
{
	slave = slaveAddress;
	ser = serialport;
	(*serialport).begin(baud, serialbit);

	words = new LinkedList<ModbusRTUSlaveWordAddress*>();
	bits = new LinkedList<ModbusRTUSlaveBitAddress*>();
	ResCnt = 0;
}


boolean ModbusRTUSlave::addWordArea(u16 Address, u16* values, int cnt)
{
	if (getWordAddress(Address) == NULL)
	{
		words->add(new ModbusRTUSlaveWordAddress(Address, values, cnt));
		return true;
	}
	return false;
}

boolean ModbusRTUSlave::addBitArea(u16 Address, u8* values, int cnt)
{
	if (getBitAddress(Address) == NULL)
	{
		bits->add(new ModbusRTUSlaveBitAddress(Address, values, cnt));
		return true;
	}
	return false;
}

ModbusRTUSlaveWordAddress* ModbusRTUSlave::getWordAddress(u16 Addr)
{
	ModbusRTUSlaveWordAddress* ret = NULL;
	for (int i = 0; i < words->size(); i++)
	{
		ModbusRTUSlaveWordAddress* a = words->get(i);
		if (a != NULL && Addr >= a->addr && Addr < a->addr + a->len) ret = a;
	}
	return ret;
}
ModbusRTUSlaveBitAddress* ModbusRTUSlave::getBitAddress(u16 Addr)
{
	ModbusRTUSlaveBitAddress* ret = NULL;
	for (int i = 0; i < bits->size(); i++)
	{
		ModbusRTUSlaveBitAddress* a = bits->get(i);
		if (a != NULL && Addr >= a->addr && Addr < a->addr + (a->len * 8)) ret = a;
	}
	return ret;
}

ModbusRTUSlaveWordAddress* ModbusRTUSlave::getWordAddress(u16 Addr, u16 Len)
{
	ModbusRTUSlaveWordAddress* ret = NULL;
	for (int i = 0; i < words->size(); i++)
	{
		ModbusRTUSlaveWordAddress* a = words->get(i);
		if (a != NULL && Addr >= a->addr && Addr + Len <= a->addr + a->len) ret = a;
	}
	return ret;
}
ModbusRTUSlaveBitAddress* ModbusRTUSlave::getBitAddress(u16 Addr, u16 Len)
{
	ModbusRTUSlaveBitAddress* ret = NULL;
	for (int i = 0; i < bits->size(); i++)
	{
		ModbusRTUSlaveBitAddress* a = bits->get(i);
		if (a != NULL && Addr >= a->addr && Addr + Len <= a->addr + (a->len * 8)) ret = a;
	}
	return ret;
}
int data123;


void ModbusRTUSlave::process()
{
	/*if (Serial2.available() > 0) {
		data123 = Serial2.read();

		Serial.println(data123, HEX);
		//    Serial.println(data);
	}
	if (Serial.available() > 0) {
		Serial2.write(Serial.read());
	}*/
	bool bvalid = true;
	while (ser->available())
	{
		byte d = ser->read();
		lstResponse[ResCnt++] = d;


		if (ResCnt >= 1) {
			if (ResCnt >= 4)
			{
				byte Slave = lstResponse[0];
				if (Slave == slave)
				{
					byte Function = lstResponse[1];
					u16 Address = (lstResponse[2] << 8) | lstResponse[3];
					switch (Function)
					{
						//Serial.println();
					case 1:		//BitRead
					case 2:
						if (ResCnt >= 8)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse, 300, 0, 6, &hi, &lo);
							ModbusRTUSlaveBitAddress* a = getBitAddress(Address, Length);
							if (Length > 0 && a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = (Address - a->addr) / 8;
								u16 nlen = ((Length - 1) / 8) + 1;

								byte dat[nlen];
								memset(dat, 0, nlen);

								int ng = (Address - a->addr) % 8;
								int ns = stidx;
								for (int i = 0; i < nlen; i++)
								{
									byte val = 0;
									for (int j = 0; j < 8; j++)
									{
										if (bitRead(a->values[ns], ng++)) bitSet(val, j);
										if (ng == 8) { ns++; ng = 0; }
									}
									dat[i] = val;
								}

								byte ret[3 + nlen + 2];
								ret[0] = Slave;	ret[1] = Function;	ret[2] = nlen;
								for (int i = 0; i < nlen; i++) ret[3 + i] = dat[i];
								byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 3 + nlen + 2, 0, 3 + nlen, &hi, &lo);
								ret[3 + nlen] = hi;
								ret[3 + nlen + 1] = lo;
								ser->write(ret, 3 + nlen + 2);

								ResCnt = 0;
							}
							else bvalid = false;
						}
						break;
					case 3:		//WordRead	
					case 4:
						if (ResCnt >= 8)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse, 300, 0, 6, &hi, &lo);
							ModbusRTUSlaveWordAddress* a = getWordAddress(Address, Length);
							if (Length > 0 && a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = Address - a->addr;
								u16 nlen = Length * 2;

								byte ret[3 + nlen + 2];
								ret[0] = Slave;	ret[1] = Function;	ret[2] = nlen;
								for (int i = stidx; i < stidx + Length; i++)
								{
									ret[3 + ((i - stidx) * 2) + 0] = ((a->values[i] & 0xFF00) >> 8);
									ret[3 + ((i - stidx) * 2) + 1] = ((a->values[i] & 0xFF));
								}
								byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 3 + nlen + 2, 0, 3 + nlen, &hi, &lo);
								ret[3 + nlen] = hi;
								ret[3 + nlen + 1] = lo;
								ser->write(ret, 3 + nlen + 2);
								ResCnt = 0;
							}
							else bvalid = false;
						}
						break;
					case 5:		//BitWrite
						if (ResCnt >= 8)
						{
							u16 Data = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse, 300, 0, 6, &hi, &lo);
							ModbusRTUSlaveBitAddress* a = getBitAddress(Address);
							if (a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = (Address - a->addr) / 8;

								bitWrite(a->values[stidx], (Address - a->addr) % 8, Data == 0xFF00);

								byte ret[8];
								ret[0] = Slave;
								ret[1] = Function;
								ret[2] = ((Address & 0xFF00) >> 8);
								ret[3] = ((Address & 0x00FF));
								ret[4] = ((Data & 0xFF00) >> 8);
								ret[5] = ((Data & 0x00FF));
								byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 8, 0, 6, &hi, &lo);
								ret[6] = hi;
								ret[7] = lo;
								ser->write(ret, 8);

								ResCnt = 0;
							}
							else bvalid = false;
						}
						break;
					case 6:		//WordWrite
						if (ResCnt >= 8)
						{
							u16 Data = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse, 300, 0, 6, &hi, &lo);
							ModbusRTUSlaveWordAddress* a = getWordAddress(Address);
							if (a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = Address - a->addr;

								a->values[stidx] = Data;

								byte ret[8];
								ret[0] = Slave;
								ret[1] = Function;
								ret[2] = ((Address & 0xFF00) >> 8);
								ret[3] = ((Address & 0x00FF));
								ret[4] = ((Data & 0xFF00) >> 8);
								ret[5] = ((Data & 0x00FF));
								byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 8, 0, 6, &hi, &lo);
								ret[6] = hi;
								ret[7] = lo;
								ser->write(ret, 8);

								ResCnt = 0;
							}
							else bvalid = false;
						}
						break;
					case 15:	//MultiBitWrite
						if (ResCnt >= 7)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							u8 ByteCount = lstResponse[6];
							if (ResCnt >= 9 + ByteCount)
							{
								byte hi = 0xFF, lo = 0xFF;
								getCRC(lstResponse, 300, 0, 7 + ByteCount, &hi, &lo);
								if (lstResponse[(9 + ByteCount - 2)] == hi && lstResponse[(9 + ByteCount - 1)] == lo)
								{
									ModbusRTUSlaveBitAddress* a = getBitAddress(Address, Length);
									if (a != NULL)
									{
										u16 stidx = (Address - a->addr) / 8;
										int ng = (Address - a->addr) % 8;
										int ns = stidx;

										for (int i = 7; i < 7 + ByteCount; i++)
										{
											byte val = lstResponse[i];
											for (int j = 0; j < 8; j++)
											{
												bitWrite(a->values[ns], ng++, bitRead(val, j));
												if (ng == 8) { ns++; ng = 0; }
											}
										}

										if (bvalid)
										{
											byte ret[8];
											ret[0] = Slave;
											ret[1] = Function;
											ret[2] = ((Address & 0xFF00) >> 8);
											ret[3] = ((Address & 0x00FF));
											ret[4] = ((Length & 0xFF00) >> 8);
											ret[5] = ((Length & 0x00FF));
											byte hi = 0xFF, lo = 0xFF;
											getCRC(ret, 8, 0, 6, &hi, &lo);
											ret[6] = hi;
											ret[7] = lo;
											ser->write(ret, 8);

											ResCnt = 0;
										}
									}
								}
								else bvalid = false;
							}
						}
						break;
					case 16:	//MultiWordWrite
						if (ResCnt >= 7)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							u8 ByteCount = lstResponse[6];
							if (ResCnt >= 9 + ByteCount)
							{
								byte hi = 0xFF, lo = 0xFF;
								getCRC(lstResponse, 300, 0, 7 + ByteCount, &hi, &lo);
								if (lstResponse[(9 + ByteCount - 2)] == hi && lstResponse[(9 + ByteCount - 1)] == lo)
								{
									for (int i = 7; i < 7 + ByteCount; i += 2)
									{
										u16 data = lstResponse[i] << 8 | lstResponse[i + 1];
										ModbusRTUSlaveWordAddress* a = getWordAddress(Address + ((i - 7) / 2));
										if (a != NULL) { a->values[(Address + ((i - 7) / 2)) - a->addr] = data; }
										else { bvalid = false; break; }
									}
									if (bvalid)
									{
										byte ret[8];
										ret[0] = Slave;
										ret[1] = Function;
										ret[2] = ((Address & 0xFF00) >> 8);
										ret[3] = ((Address & 0x00FF));
										ret[4] = ((Length & 0xFF00) >> 8);
										ret[5] = ((Length & 0x00FF));
										byte hi = 0xFF, lo = 0xFF;
										getCRC(ret, 8, 0, 6, &hi, &lo);
										ret[6] = hi;
										ret[7] = lo;
										ser->write(ret, 8);

										ResCnt = 0;
									}
								}
								else bvalid = false;
							}
						}
						break;
					}
				}
				else bvalid = false;
			}
			lastrecv = micros();
		}
	}

	if (ResCnt > 0)
	{
		unsigned long aaa = micros();
		if (aaa > lastrecv)
		{
			if (aaa - lastrecv > 1500) {
				ResCnt = 0;
				while (ser->available() > 0) { ser->read(); }
			}

		}
		else if (aaa < lastrecv)
		{
			if ((lastrecv - aaa) < (4294967295 - 1500))
			{
				ResCnt = 0;
				while (ser->available() > 0) { ser->read(); }
			}
		}
	}
	if (!bvalid && ResCnt > 0) {
		ResCnt = 0;
		while (ser->available() > 0) { ser->read(); }
	}
}

/*
void ModbusRTUSlave::getCRC(LinkedList<byte>* pby, int startindex, int nSize, byte* byFirstReturn, byte* bySecondReturn)
{
	int uIndex;
	byte uchCRCHi = 0xff;
	byte uchCRCLo = 0xff;
	for (int i = startindex; i < startindex + nSize && i<pby->size(); i++)
	{
		uIndex = uchCRCHi ^ pby->get(i);
		uchCRCHi = uchCRCLo ^ auchCRCHi1[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	(*byFirstReturn) = uchCRCHi;
	(*bySecondReturn) = uchCRCLo;
}
*/
void ModbusRTUSlave::getCRC(byte* pby, int arsize, int startindex, int nSize, byte* byFirstReturn, byte* bySecondReturn)
{
	int uIndex;
	byte uchCRCHi = 0xff;
	byte uchCRCLo = 0xff;
	for (int i = startindex; i < startindex + nSize && i < arsize; i++)
	{
		uIndex = uchCRCHi ^ pby[i];
		uchCRCHi = uchCRCLo ^ auchCRCHi1[uIndex];
		uchCRCLo = auchCRCLo1[uIndex];
	}
	(*byFirstReturn) = uchCRCHi;
	(*bySecondReturn) = uchCRCLo;
}

boolean getBit(u8* area, int index)
{
	u16 stidx = index / 8;
	return bitRead(area[stidx], index % 8);
}

void setBit(u8* area, int index, bool value)
{
	u16 stidx = index / 8;
	bitWrite(area[stidx], index % 8, value);
}

ModbusRTUSlaveBitAddress::ModbusRTUSlaveBitAddress(u16 Address, u8* value, int cnt)
{
	addr = Address;
	values = value;
	len = cnt;
}

ModbusRTUSlaveWordAddress::ModbusRTUSlaveWordAddress(u16 Address, u16* value, int cnt)
{
	addr = Address;
	values = value;
	len = cnt;
}


#ifdef __AVR_ATmega2560__

unsigned long MODTIMER::msecs1_m;
void (*MODTIMER::func1_m)();
volatile unsigned long MODTIMER::count1_m;
volatile char MODTIMER::overflowing1_m;
volatile unsigned int MODTIMER::tcnt1_m;

uint8_t Ntimer = 1;
unsigned long ms = 1;
bool Timer1On_m = 0;

void MODTIMER::SET(void (*f)()) {
	float prescaler = 0.0;
	if (Ntimer == 1) {
		Timer1On_m = true;
		TIMSK1 &= ~(1 << TOIE1);
		TCCR1A = 0x00;
		TCCR1B = 0x03;
		TIMSK1 &= ~(1 << OCIE1A);

		//		tcnt1_m = 64286;    //5ms
		//		tcnt1_m = 64536;	//4ms
		//		tcnt1_m = 64786;	//3ms
		//		tcnt1_m = 65036;	//2ms     
		//		tcnt1_m = 65286;    //1ms
//		tcnt1_m = 65411;    //0.5ms
		tcnt1_m = 65436;    //0.4ms
//		tcnt1_m = 65461;    //0.3ms
//		tcnt1_m = 65486;    //0.2ms
//		tcnt1_m = 65511;    //0.1ms
		if (ms == 0)
			msecs1_m = 1;
		else
			msecs1_m = ms;

		func1_m = f;
	}
	count1_m = 0;
	overflowing1_m = 0;
	TCNT1 = tcnt1_m;
	TIMSK1 |= (1 << TOIE1);
	
	MODTIMER::START();	
}
void MODTIMER::START(uint8_t Numstart) {
	if (((Timer1On_m == true) && (Numstart == 10)) || ((Timer1On_m == true) && (Numstart == 1))) {
		count1_m = 0;
		overflowing1_m = 0;
		TCNT1 = tcnt1_m;
		TIMSK1 |= (1 << TOIE1);
	}
}

void MODTIMER::STOP(uint8_t Numstop) {
	if (((Timer1On_m == true) && (Numstop == 1)) || ((Timer1On_m == true) && (Numstop == 10))) {
		TIMSK1 &= ~(1 << TOIE1);
	}
}
void MODTIMER::_overflow1_m() {
	count1_m += 1;
	if (count1_m >= msecs1_m && !overflowing1_m) {
		overflowing1_m = 1;
		count1_m = 0;
		(*func1_m)();
		overflowing1_m = 0;
	}	
}
#endif

#ifdef __AVR_ATmega128__
unsigned long MODTIMER::msecs1_m;
void (*MODTIMER::func1_m)();
volatile unsigned long MODTIMER::count1_m;
volatile char MODTIMER::overflowing1_m;
volatile unsigned int MODTIMER::tcnt1_m;

uint8_t Ntimer = 1;
unsigned long ms = 1;
bool Timer1On_m = 0;

void MODTIMER::SET(void (*f)()) {
	float prescaler = 0.0;
	if (Ntimer == 1) {
		Timer1On_m = true;
		TIMSK &= ~(1 << TOIE1);
		TCCR1A = 0x00;
		TCCR1B = 0x03;
		TIMSK &= ~(1 << OCIE1A);

		//		tcnt1_m = 64286;    //5ms
		//		tcnt1_m = 64536;	//4ms
		//		tcnt1_m = 64786;	//3ms
		//		tcnt1_m = 65036;	//2ms     
		//		tcnt1_m = 65286;    //1ms
//		tcnt1_m = 65411;    //0.5ms
		tcnt1_m = 65436;    //0.4ms
//		tcnt1_m = 65461;    //0.3ms
//		tcnt1_m = 65486;    //0.2ms
//		tcnt1_m = 65511;    //0.1ms
		if (ms == 0)
			msecs1_m = 1;
		else
			msecs1_m = ms;

		func1_m = f;
	}
	count1_m = 0;
	overflowing1_m = 0;
	TCNT1 = tcnt1_m;
	TIMSK |= (1 << TOIE1);
	MODTIMER::START();
}
void MODTIMER::START(uint8_t Numstart) {
	if (((Timer1On_m == true) && (Numstart == 10)) || ((Timer1On_m == true) && (Numstart == 1))) {
		count1_m = 0;
		overflowing1_m = 0;
		TCNT1 = tcnt1_m;
		TIMSK |= (1 << TOIE1);
	}
}

void MODTIMER::STOP(uint8_t Numstop) {
	if (((Timer1On_m == true) && (Numstop == 1)) || ((Timer1On_m == true) && (Numstop == 10))) {
		TIMSK &= ~(1 << TOIE1);
	}
}
void MODTIMER::_overflow1_m() {
	count1_m += 1;
	if (count1_m >= msecs1_m && !overflowing1_m) {
		overflowing1_m = 1;
		count1_m = 0;
		(*func1_m)();
		overflowing1_m = 0;
	}
}
#endif




#endif //MODBUS_RTU_SLAVE