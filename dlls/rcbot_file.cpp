#include "rcbot_file.h"
#include "rcbot_strings.h"
#include <stdio.h>

RCBotFile::RCBotFile(FILE* _fp)
{
	this->fp = _fp;
}

RCBotFile::~RCBotFile()
{
	if (fp != NULL)
		fclose(fp);
}

void RCBotFile::close()
{
	if (fp != NULL)
		fclose(fp);
	fp = NULL;
}

RCBotFile* RCBotFile::Open(const char* filename, const char* mode)
{
	FILE* fp = fopen(filename, mode);

	if (fp == NULL)
		return NULL;

	return new RCBotFile(fp);
}

void RCBotFile::writeByte(uint8_t b)
{
	fwrite(&b, sizeof(uint8_t), 1, fp);
}

void RCBotFile::writeFloat(float f)
{
	fwrite(&f, sizeof(float), 1, fp);
}

void RCBotFile::writeString(const char* szString)
{
	int len = gRCBotStrings.stringLength(szString);

	fwrite(&len, sizeof(int), 1, fp);
	fwrite(szString, sizeof(char), len, fp);
}

const char *RCBotFile::readLine()
{
	static char line[RCBOT_FILE_MAX_LINE_LENGTH];

	if (fgets(line, RCBOT_FILE_MAX_LINE_LENGTH, fp) == NULL)
		return NULL;

	line[RCBOT_FILE_MAX_LINE_LENGTH - 1] = 0;

	return line;
}

uint8_t RCBotFile::readByte()
{
	uint8_t ret;

	fread(&ret, sizeof(uint8_t), 1, fp);

	return ret;
}
uint16_t RCBotFile::readUInt16()
{
	uint16_t ret;

	fread(&ret, sizeof(uint16_t), 1, fp);

	return ret;
}
uint32_t RCBotFile::readUInt32()
{
	uint32_t ret;

	fread(&ret, sizeof(uint32_t), 1, fp);

	return ret;
}
float RCBotFile::readFloat()
{
	float ret;

	fread(&ret, sizeof(float), 1, fp);

	return ret;
}
char RCBotFile::readChar()
{
	char ret;

	fread(&ret, sizeof(char), 1, fp);

	return ret;
}
/// <summary>
/// RCBotFile::readString
/// </summary>
/// <param name="len">uint32 length</param>
/// <returns></returns>
const char* RCBotFile::readString(uint32_t len)
{
	char* str = new char[len + 1];

	fread(str, sizeof(char), len, fp);
	str[len] = 0;

	return gRCBotStrings.add(str);
}

/// <summary>
/// RCBotFile::readString
/// reads uint32 (length of string) and reads string with length
/// </summary>
/// <returns>new string</returns>
const char* RCBotFile::readString()
{
	// read length
	uint32_t len = readUInt32();

	return readString(len);
}

