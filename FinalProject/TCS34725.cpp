#include "TCS34725.h" 

TCS34725::~TCS34725() {

}

int TCS34725::writeSingleRegister(char address, char data)
{
	char tx[2] = { address | 160, data }; //0d160 = 0b10100000

	stream.beginTransmission(SLAVE_ADDRESS);
	stream.write(tx, 2);
	stream.endTransmission(SLAVE_ADDRESS);

	return 0;
}

int TCS34725::writeMultipleRegisters(char address, char* data, int quantity)
{
	char *tx = new char[quantity + 1];
	tx[0] = address | 160;
	for (int i = 1; i <= quantity; i++) {
		tx[i] = data[i - 1];
	}

	stream.beginTransmission(SLAVE_ADDRESS);
	stream.write(tx, quantity + 1);
	stream.endTransmission(SLAVE_ADDRESS);

	delete[] tx;
	return 0;
}

char TCS34725::readSingleRegister(char address)
{
	char command = address | 160; //0d160 = 0b10100000
	stream.beginTransmission(SLAVE_ADDRESS);
	stream.write(command);
	stream.endTransmission(SLAVE_ADDRESS);
	delay(10);
	stream.requestFrom(SLAVE_ADDRESS, 1);
	return stream.read();
}

int TCS34725::readMultipleRegisters(char address, char* output, int quantity)
{
	char command = address | 160; //0d160 = 0b10100000
	stream.beginTransmission(SLAVE_ADDRESS);
	stream.write(command);
	stream.endTransmission(SLAVE_ADDRESS);
	delay(10);
	stream.requestFrom(SLAVE_ADDRESS, quantity);
	stream.readBytes(output, quantity);
	return quantity;
}

void TCS34725::getAllColors(int* readings)
{
	char buffer[8] = { 0 };

	readMultipleRegisters(CDATA, buffer, 8);

	readings[0] = (int)buffer[1] << 8 | (int)buffer[0];
	readings[1] = (int)buffer[3] << 8 | (int)buffer[2];
	readings[2] = (int)buffer[5] << 8 | (int)buffer[4];
	readings[3] = (int)buffer[7] << 8 | (int)buffer[6];
}

int TCS34725::getClearData()
{
	char buffer[2] = { 0 };
	readMultipleRegisters(CDATA, buffer, 2);
	int reading = (int)buffer[1] << 8 | (int)buffer[0];
	return reading;
}

int TCS34725::getRedData()
{
	char buffer[2] = { 0 };
	readMultipleRegisters(RDATA, buffer, 2);
	int reading = (int)buffer[1] << 8 | (int)buffer[0];
	return reading;
}

int TCS34725::getGreenData()
{
	char buffer[2] = { 0 };
	readMultipleRegisters(GDATA, buffer, 2);
	int reading = (int)buffer[1] << 8 | (int)buffer[0];
	return reading;
}

int TCS34725::getBlueData()
{
	char buffer[2] = { 0 };
	readMultipleRegisters(BDATA, buffer, 2);
	int reading = (int)buffer[1] << 8 | (int)buffer[0];
	return reading;
}

int TCS34725::enablePower()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old | 1; // sets PON (bit 0) to 1
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

int TCS34725::disablePower()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old & 254; // sets PON (bit 0) to 0
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

bool TCS34725::isPowerEnabled()
{
	char enable = readSingleRegister(ENABLE);
	char pon = enable << 7;
	pon = pon >> 7; // gets PON (bit 0) from ENABLE register byte
	return (bool)pon;
}

int TCS34725::enableRGBC()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old | 2; // sets AEN (bit 1) to 1
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

int TCS34725::disableRGBC()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old & 253; // sets AEN (bit 1) to 0
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

bool TCS34725::isRGBCEnabled()
{
	char enable = readSingleRegister(ENABLE);
	char aen = enable << 6;
	aen = aen >> 7; // gets AEN (bit 1) from ENABLE register byte
	return (bool)aen;
}

int TCS34725::enablePowerAndRGBC()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old | 3; // sets PON (bit 0) and AEN (bit 1) to 1
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

int TCS34725::disablePowerAndRGBC()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old & 252; // sets PON (bit 0) and AEN (bit 1) to 0
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

int TCS34725::enableWait()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old | 8; // sets WEN (bit 3) to 1
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

int TCS34725::disableWait()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old & 247; // sets WEN (bit 3) to 0
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

bool TCS34725::isWaitEnabled()
{
	char enable = readSingleRegister(ENABLE);
	char wen = enable << 4;
	wen = wen >> 7; // gets WEN (bit 3) from ENABLE register byte
	return (bool)wen;
}

int TCS34725::enableInterrupt()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old | 16; // sets AIEN (bit 4) to 1
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

int TCS34725::disableInterrupt()
{
	char enable_old = readSingleRegister(ENABLE);
	char enable_new = enable_old & 239; // sets AIEN (bit 4) to 0
	int ack = writeSingleRegister(ENABLE, enable_new);
	return ack;
}

bool TCS34725::isInterruptEnabled()
{
	char enable = readSingleRegister(ENABLE);
	char aien = enable << 3;
	aien = aien >> 7; // gets AIEN (bit 4) from ENABLE register byte
	return (bool)aien;
}

int TCS34725::setIntegrationTime(const float itime)
{
	char atime = 256 - roundTowardsZero(itime / 2.4); // rounding ensures nearest value of atime is used
	int ack = writeSingleRegister(ATIME, atime);
	return ack;
}

float TCS34725::readIntegrationTime()
{
	float itime = 0;
	char atime = readSingleRegister(ATIME);
	itime = 2.4 * (256 - atime);
	return itime;
}

int TCS34725::setWaitTime(const float time)
{
	int ack = 1;
	char wtime = 0;
	if (time >= 2.39 && time <= 614.4) { // 2.39 instead of 2.4 to allow for float accuracy errors
		ack = writeSingleRegister(CONFIG, 0); // sets WLONG to 0
		wtime = 256 - roundTowardsZero(time / 2.4);
	}
	else if (time > 614.4 && time <= 7400.1) { // 7400.1 instead of 7400 to allow for float accuracy errors
		ack = writeSingleRegister(CONFIG, 2); // sets WLONG to 1
		wtime = 256 - roundTowardsZero(time / 28.8);
	}
	ack = ack || writeSingleRegister(WTIME, wtime);
	return ack;
}

float TCS34725::readWaitTime()
{
	float time = 0;
	char wtime = readSingleRegister(WTIME);
	char config = readSingleRegister(CONFIG);
	int wlong = (config << 6) >> 7; // gets WLONG (bit 1) from CONFIG register byte
	if (wlong == 0) {
		time = 2.4 * (256 - wtime);
	}
	else if (wlong == 1) {
		time = 28.8 * (256 - wtime); // 28.8 = 2.4 * 12
	}
	return time;
}

char TCS34725::readEnableRegister()
{
	return readSingleRegister(ENABLE);
}

int TCS34725::readLowInterruptThreshold()
{
	char buffer[2] = { 0 };
	readMultipleRegisters(AILTL, buffer, 2);
	int reading = (int)buffer[1] << 8 | (int)buffer[0];
	return reading;
}

int TCS34725::readHighInterruptThreshold()
{
	char buffer[2] = { 0 };
	readMultipleRegisters(AIHTL, buffer, 2);
	int reading = (int)buffer[1] << 8 | (int)buffer[0];
	return reading;
}

int TCS34725::setLowInterruptThreshold(const int threshold)
{
	char threshold_bytes[2];
	threshold_bytes[0] = threshold; // take lowest 8 bits of threshold
	threshold_bytes[1] = threshold >> 8; // take highest 8 bits of threshold
	int ack = writeMultipleRegisters(AILTL, threshold_bytes, 2);
	return ack;
}

int TCS34725::setHighInterruptThreshold(const int threshold)
{
	char threshold_bytes[2];
	threshold_bytes[0] = threshold;
	threshold_bytes[1] = threshold >> 8;
	int ack = writeMultipleRegisters(AIHTL, threshold_bytes, 2);
	return ack;
}

int TCS34725::readInterruptPersistence()
{
	char pers = readSingleRegister(PERS);
	char persistence_bits = (pers << 4) >> 4; // discard bits 4 to 7, keep only bits 0 to 3
	int persistence = -1;
	switch (persistence_bits) {
	case 0:
		persistence = 0;
		break;
	case 1:
		persistence = 1;
		break;
	case 2:
		persistence = 2;
		break;
	case 3:
		persistence = 3;
		break;
	case 4:
		persistence = 5;
		break;
	case 5:
		persistence = 10;
		break;
	case 6:
		persistence = 15;
		break;
	case 7:
		persistence = 20;
		break;
	case 8:
		persistence = 25;
		break;
	case 9:
		persistence = 30;
		break;
	case 10:
		persistence = 35;
		break;
	case 11:
		persistence = 40;
		break;
	case 12:
		persistence = 45;
		break;
	case 13:
		persistence = 50;
		break;
	case 14:
		persistence = 55;
		break;
	case 15:
		persistence = 60;
		break;
	default:
		break;
	}
	return persistence;
}

int TCS34725::setInterruptPersistence(const int persistence) {
	char pers_byte;
	int ack = 0;
	switch (persistence) {
	case 0:
		pers_byte = 0;
		break;
	case 1:
		pers_byte = 1;
		break;
	case 2:
		pers_byte = 2;
		break;
	case 3:
		pers_byte = 3;
		break;
	case 5:
		pers_byte = 4;
		break;
	case 10:
		pers_byte = 5;
		break;
	case 15:
		pers_byte = 6;
		break;
	case 20:
		pers_byte = 7;
		break;
	case 25:
		pers_byte = 8;
		break;
	case 30:
		pers_byte = 9;
		break;
	case 35:
		pers_byte = 10;
		break;
	case 40:
		pers_byte = 11;
		break;
	case 45:
		pers_byte = 12;
		break;
	case 50:
		pers_byte = 13;
		break;
	case 55:
		pers_byte = 14;
		break;
	case 60:
		pers_byte = 15;
		break;
	default:
		ack = 2; // 2 used to indicate invalid entry
		break;
	}
	if (ack != 2) {
		ack = writeSingleRegister(PERS, pers_byte);
	}
	return ack;
}

int TCS34725::clearInterrupt() {
	char tx = 230;
	stream.write(tx);
	return 0;
}

int TCS34725::readRGBCGain() {
	char control = readSingleRegister(CONTROL);
	char gain_bits = (control << 6) >> 6; // discard  bits 2 to 7, keep only bits 0 & 1
	int gain;
	switch (gain_bits) {
	case 0:
		gain = 1;
		break;
	case 1:
		gain = 4;
		break;
	case 2:
		gain = 16;
		break;
	case 3:
		gain = 60;
		break;
	default:
		gain = 0;
		break;
	}
	return gain;
}

int TCS34725::setRGBCGain(const int gain) {
	char control;
	int ack = 0;
	switch (gain) {
	case 1:
		control = 0;
		break;
	case 4:
		control = 1;
		break;
	case 16:
		control = 2;
		break;
	case 60:
		control = 3;
		break;
	default:
		ack = 2; // 2 used to indicate invalid entry
		break;
	}
	if (ack != 2) {
		ack = writeSingleRegister(CONTROL, control);
	}
	return ack;
}

char TCS34725::getDeviceID() {
	return readSingleRegister(ID);
}

char TCS34725::readStatusRegister() {
	return readSingleRegister(STATUS);
}

float TCS34725::roundTowardsZero(const float value) {
	float result = 0;
	if ((value >= 0 && (value - (int)value) < 0.5) || (value < 0 && (abs(value) - (int)abs(value)) >= 0.5)) {
		result = floor(value);
	}
	else {
		result = ceil(value);
	}
	return result;
}
