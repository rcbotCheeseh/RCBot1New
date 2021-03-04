#include "rcbot_file.h"
#include "rcbot_strings.h"
#include "rcbot_utils.h"
#include "rcbot_mod.h"
#include <stdio.h>

char RCBotFile::m_szFileName[RCBOT_FILE_MAX_FILENAME_LENGTH] = "";

RCBotFile::RCBotFile(FILE* _fp)
{
	this->fp = _fp;
}

RCBotFile::~RCBotFile()
{
	if (fp != nullptr)
		fclose(fp);
}

void RCBotFile::close()
{
	if (fp != nullptr)
		fclose(fp);

	fp = nullptr;
}

RCBotFile* RCBotFile::Open(const char *szFolder, const char* szFilename, const char *szExtension, const char* mode, bool bUseModFolder)
{
	FILE* fp; 

	RCBotModification *pMod = gRCBotModifications.getCurrentMod();

	if (bUseModFolder && (pMod != nullptr))
		snprintf(m_szFileName, RCBOT_FILE_MAX_FILENAME_LENGTH, "%s/%s/%s/%s.%s", RCBOT_FILES_FOLDER, szFolder, pMod->getFolder(), szFilename, szExtension);
	else 
		snprintf(m_szFileName, RCBOT_FILE_MAX_FILENAME_LENGTH, "%s/%s/%s.%s", RCBOT_FILES_FOLDER, szFolder, szFilename, szExtension);

	fp = fopen(m_szFileName, mode);

	if (fp == nullptr)
	{ 
		RCBotUtils::Message(NULL, MessageErrorLevel::Information, "WARNING: Attempt to open file \"%s\" mode \"%s\" failed", szFilename, mode);
		return nullptr;
	}

	return new RCBotFile(fp);
}

void RCBotFile::writeByte(uint8_t b)
{
	fwrite(&b, sizeof(uint8_t), 1, fp);
}

uint32_t RCBotFile::readBytes(void* pBuffer, uint32_t len)
{
	return fread(pBuffer, 1, len, fp);
}


void RCBotFile::writeUInt16(uint16_t v)
{
	fwrite(&v, sizeof(uint16_t), 1, fp);
}

void RCBotFile::writeBool(bool v)
{
	fwrite(&v, sizeof(bool), 1, fp);
}

void RCBotFile::writeUInt32(uint32_t v)
{
	fwrite(&v, sizeof(uint32_t), 1, fp);
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

	if (fgets(line, RCBOT_FILE_MAX_LINE_LENGTH, fp) == nullptr)
		return nullptr;

	line[RCBOT_FILE_MAX_LINE_LENGTH - 1] = 0;

	return line;
}

bool RCBotFile::readBool()
{
	bool ret;

	fread(&ret, sizeof(bool), 1, fp);

	return ret;
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
const char* RCBotFile::readString(uint8_t len)
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
	uint8_t len = readByte();

	return readString(len);
}

