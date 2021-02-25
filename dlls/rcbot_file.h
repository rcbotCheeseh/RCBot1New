#ifndef __RCBOT_FILE_H__
#define __RCBOT_FILE_H__

#include <stdio.h>
#include <stdint.h>
#include <vector>

#define RCBOT_FILE_MAX_LINE_LENGTH 256

class RCBotFile
{
public:

	RCBotFile(FILE* _fp);
	~RCBotFile();
	static RCBotFile* Open(const char* filename, const char* mode);
	void close();
	void writeByte(uint8_t b);
	void writeFloat(float f);
	void writeString(const char* szString);
	const char* RCBotFile::readLine();

	uint8_t readByte();
	uint16_t readUInt16();
	uint32_t readUInt32();
	float readFloat();
	char readChar();
	const char* readString();
	const char* readString(uint32_t len);

private:
	FILE* fp;
};

#endif

