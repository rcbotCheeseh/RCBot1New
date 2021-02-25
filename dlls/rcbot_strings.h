#ifndef __RCBOT_STRINGS_H__
#define __RCBOT_STRINGS_H__

#include <vector>

#define RCBOT_STRINGS_MAX_LENGTH 1024
#define RCBOT_STRINGS_HASHES 26

class RCBotStrings
{
public:

    RCBotStrings()
    {

    }

    ~RCBotStrings()
    {
        for (uint32_t i = 0; i < RCBOT_STRINGS_HASHES; i++)
        {
            for (uint32_t j = 0; j < m_Strings[i].size(); j++)
            {
                const char* str = m_Strings[i][j];

                delete str;
            }
        }
    }

    int stringLength(const char* str)
    {
        int ret = 0;

        while (*str++)
            ret++;

        return ret;
    }

    static void stringCopy(const char* source, char* dest)
    {
        while (*dest++ = *source++);
    }

    bool stringMatch(const char* a, const char* b)
    {
        while (*a && *b && *a == *b)
        {
            a++;
            b++;
        }

        return (*a == *b);
    }

    const char* find(const char* str)
    {
        if (str != NULL)
        {
            int hash = *str % RCBOT_STRINGS_HASHES;

            for (uint32_t i = 0; i < m_Strings[hash].size(); i++)
            {
                if (m_Strings[hash][i] == str)
                    return m_Strings[hash][i];
                if (stringMatch(m_Strings[hash][i], str))
                    return m_Strings[hash][i];
            }
        }

        return NULL;
    }

    const char* add(const char* str)
    {
        if (str != NULL)
        {
            const char* found = find(str);
            int hash = *str % RCBOT_STRINGS_HASHES;

            if (found != NULL)
                return found;

            int len = stringLength(str);
            char* newString = new char[len + 1];
            stringCopy(str, newString);

            m_Strings[hash].push_back(newString);

            return m_Strings[hash][m_Strings[hash].size() - 1];

        }

        return NullString;
    }

    int split(const char* szString, std::vector<const char*>* list, const char split_character)
    {
        char str[RCBOT_STRINGS_MAX_LENGTH];
        const char* pC = szString;
        bool bQuote = false;
        int len = 0;

        list->clear();

        while (*pC)
        {
            if (*pC == '\"')
                bQuote = !bQuote;

            if (!bQuote && *pC == split_character)
            {
                if (len < RCBOT_STRINGS_HASHES)
                    str[len] = 0;

                str[RCBOT_STRINGS_HASHES - 1] = 0;

                list->push_back(add(str));

                len = 0;
                str[0] = 0;
            }
            else if (len < RCBOT_STRINGS_HASHES)
                str[len++] = *pC;

            pC++;
        }

        return list->size();
    }

private:
    std::vector<const char*> m_Strings[RCBOT_STRINGS_HASHES];
    static const char* NullString;
};

extern RCBotStrings gRCBotStrings;

#endif 
