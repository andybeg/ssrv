#include <cstring>
#include <string>

using namespace std;

char getChar(char Hi,char Lo)
{
    char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    char res=0;
    for(int i=0;i<16;i++)
    {
        if(Hi==hex[i])
            res+=(i<<4);
        if(Lo==hex[i])
            res+=i;
    }
    return res;
}

void strHex2str(const char* strHex, char* str,char length)
{
    for( int i = 0; i < length; ++i )
        str[i] = getChar(strHex[2*i+1],strHex[2*i]);
}
static bool isValidFloat(const std::string& string)
{
    std::string::const_iterator it = string.begin();
    bool decimalPoint = false;
    int minSize = 0;
    if(string.size()>0 && (string[0] == '-' || string[0] == '+')){
        it++;
        minSize++;
    }
    while(it != string.end())
    {
        if(*it == '.')
        {
            if(!decimalPoint) decimalPoint = true;
            else break;
        }else
            if(!std::isdigit(*it) && ((*it!='f') || it+1 != string.end() || !decimalPoint))
            {
                break;
            }
        ++it;
    }
    return string.size()>minSize && it == string.end();
}
bool isValidInt(const std::string& string)
{
    bool Checked = true;

    int i = 0;
    for(int i=0;i<string.size();i++)
    {
        //valid digit?
        if (isdigit(string.data()[i]))
        {
            //to the next character
            i++;
            Checked = true;
        }
        else
        {
            //to the next character
            i++;
            Checked = false;
            break;
        }
    } while (string.data()[i] != '\0');

    return Checked;
}
void memcpyExt( void* dest, void*src , int size, int* offset )
{
    memcpy(((char*)dest)+ *offset,src, size);
    * offset+=size;
}
