#ifndef __BASE64_H__
#define __BASE64_H__

#include <string>

namespace Base64Tool
{
    char*   convert_to_base64(const char* buf,int len,int& outlen);
    char*   convert_from_base64(const char* base64,int len,int& outlen);

    inline std::string easy_convert_to_base64(const char* buf, int len) {
        int outlen = 0;
        char* szBase64 = convert_to_base64(buf, len, outlen);
        std::string strBase64(szBase64,len);
        delete [] szBase64;
        return strBase64;
    }

    inline std::string easy_convert_to_base64(const std::string& str)
    {
        char *szBase64 = NULL;
        int  len = 0;
        szBase64 = convert_to_base64(str.c_str(),str.size(),len);
        std::string strBase64(szBase64,len);
        //ReleaseBuffer(szBase64);
        delete szBase64;
        return strBase64;
    }

    inline std::string easy_convert_from_base64(const std::string& str)
    {
        char *szData = NULL;
        int  len = 0;
        szData = convert_from_base64(str.c_str(),str.size(),len);
        std::string strData(szData,len);
        //ReleaseBuffer(szData);
        delete [] szData;
        return strData;
    }
}

#endif
