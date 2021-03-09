#include "rcbot_strings.h"

RCBotStrings gRCBotStrings;

const char* RCBotStrings::NullString = "(nullptr)";

void RCBotStrings::stringLower(char* str)
// lower a string to make it lower case.
{
    while (*str)
    {
        *str = RCBotStrings::charLower(*str);
        str++;
    }
}

char RCBotStrings::charLower(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return 'a' + (ch - 'A');
    }

    return ch;
}