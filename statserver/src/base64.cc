#include "base64.h"

namespace Base64Tool
{

    namespace inner
    {
        static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";
        // static const std::wstring base64_wchars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ" L"abcdefghijklmnopqrstuvwxyz" L"0123456789+/";

        inline bool is_base64( unsigned char c ) {
            return (isalnum(c) || (c == '+') || (c == '/'));
        }

        //inline bool w_is_base64( unsigned short c ) {
        //  return (isalnum(c) || (c == '+') || (c == '/'));
        //}
    }



    std::string convert_to_base64( const std::string& buf )
    {
        std::string ret;
        const unsigned char* bytes_to_encode = reinterpret_cast< const unsigned char* >( buf.data() );
        int in_len = buf.size();
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++)
                    ret += inner::base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i)
        {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += inner::base64_chars[char_array_4[j]];

            while((i++ < 3))
                ret += '=';

        }
        return ret;
    }

    std::string convert_from_base64( const std::string& base64 )
    {
        int in_len = base64.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while (in_len-- && ( base64[in_] != '=') && inner::is_base64(base64[in_])) {
            char_array_4[i++] = base64[in_]; in_++;
            if (i ==4) {
                for (i = 0; i <4; i++)
                    char_array_4[i] = inner::base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;

            for (j = 0; j <4; j++)
                char_array_4[j] = inner::base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }

        return ret;
    }

    char* convert_to_base64(const char* buf,int len,int& outlen)
    {
        std::string outbuf = convert_to_base64(std::string(buf,len));
        outlen = outbuf.size();
        char *pOutBuffer = new char[outbuf.size()+1];
        if(pOutBuffer) {
            memcpy(pOutBuffer,outbuf.data(),outbuf.size());
        }
        return pOutBuffer;
    }

    char*  convert_from_base64(const char* base64,int len,int& outlen)
    {
        std::string outbuf = convert_from_base64(std::string(base64,len));
        outlen = outbuf.size();
        char *pOutBuffer = new char[outbuf.size()+1];
        if(pOutBuffer)
            memcpy(pOutBuffer,outbuf.data(),outbuf.size());
        return pOutBuffer;
    }

}

