#ifndef __RCBOT_FILE_H__
#define __RCBOT_FILE_H__

#include <stdio.h>
#include <stdint.h>
#include <vector>

#define RCBOT_FILE_MAX_LINE_LENGTH 256
#define RCBOT_FILE_MAX_FILENAME_LENGTH 512

#define RCBOT_FILES_FOLDER "rcbot"

class RCBotFile
{
public:

	RCBotFile(FILE* _fp);
	~RCBotFile();
	static RCBotFile* Open(const char* szFolder, const char* szFilename, const char* szExtension, const char* mode, bool bUseModFolder = false);
	void close();
	void writeByte(uint8_t b);
	void writeFloat(float f);
	void writeString(const char* szString);
	void writeUInt32(uint32_t v);
	void writeUInt16(uint16_t v);
	void writeBool(bool v);
	const char* readLine();
	bool readBool();
	uint8_t readByte();
	uint16_t readUInt16();
	uint32_t readUInt32();
	float readFloat();
	char readChar();
	const char* readString();
	const char* readString(uint8_t len);
	uint32_t readBytes(void* pBuffer, uint32_t len);
	
private:
	FILE* fp;
	static char m_szFileName[RCBOT_FILE_MAX_FILENAME_LENGTH];
};

#endif

