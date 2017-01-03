/*
    qgz - gzip extension for kdb+/q
    Copyright (C) 2016  Lucas Martin-King

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cstdint>
#include <arpa/inet.h> // for ntohl, etc

#include "zlib.h"

#define KXVER 3
#include "k.h"


extern "C" K gunzip(K x)
{
    if (x->t != KC)
    {
        return krr("expecting a string");
    }

    const int len = x->n;
    const char* data = (char*)&kC(x)[0];
    
    if (len < (10 + 8))
    {
        return krr("too small to be correct");
    }

    uint32_t uncomp_len = *(uint32_t*)((uint8_t*)data + (len - 4));
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        // Value is encoded in byte stream order in the gzip file
        uncomp_len = __builtin_bswap32(uncomp_len);
    #endif

    #ifndef NDEBUG
    std::cerr << "Compressed len: " << len << std::endl;
    std::cerr << "Uncompressed len: " << uncomp_len << std::endl;
    #endif

    K uc = ktn(KC, uncomp_len);
    if (! uc)
    {
        return krr("ktn uc");
    }

    z_stream strm;
    strm.next_in = (Bytef*)data;
    strm.avail_in = len;
    strm.next_out = (Bytef*)&kC(uc)[0];
    strm.avail_out = uncomp_len;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (inflateInit2(&strm, (16+MAX_WBITS)) != Z_OK) {
        r0(uc);
        return krr("inflateInit2"); 
    }

    int ret = inflate(&strm, Z_SYNC_FLUSH);

    if (ret != Z_STREAM_END)
    {
        inflateEnd(&strm);
        r0(uc);
        if (strm.msg)
        {
            std::cerr << "zlib error: " << strm.msg << std::endl;
        }
        return krr("inflate");
    }

    inflateEnd(&strm);

    #ifndef NDEBUG
    std::cerr << "Ret: " << ret << std::endl;
    std::cerr << "Total out: " << strm.total_out << std::endl;
    #endif

    return uc;
}
