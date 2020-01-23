/*
 * CConverter.h
 *
 * Copyright 2015 kon <kon@kononov>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


#ifndef CCONVERTER_H
#define CCONVERTER_H

#include <string>
#include <cstring>


class CConverter
{
    public:
        CConverter(){}
        virtual ~CConverter(){};
        char Hex2Bin( const char *s ){
            int ret=0;
            for( int i=0; i<2; i++ )
            {
                char c = *s++;
                int n=0;
                if( '0'<=c && c<='9' )
                    n = c-'0';
                else if( 'a'<=c && c<='f' )
                    n = 10 + c-'a';
                else if( 'A'<=c && c<='F' )
                    n = 10 + c-'A';
                ret = n + ret*16;
            }
            return ret;
        }
        template <typename T>
        std::string anyToStr( const T any)
        {
            int sz = sizeof(T);
            char* tmp = new char[sz];
            std::string str;
            memcpy(tmp,&any,sz);
            str.append(tmp,sz);
            delete tmp;
            return str;
        };
        template <typename T>
        std::string anyToStr( const T any,int size)
        {
            int sz = size;
            char* tmp = new char[sz];
            std::string str;
            memcpy(tmp,&any,sz);
            str.append(tmp,sz);
            delete tmp;
            return str;
        };
        template <typename T>
        T strToAny( const std::string str ,T &any ) const{
            if(sizeof(T)!=str.size())
                return -1;
            int size = sizeof(T);
            memset(&any,0,size);
            for (int i = 0; i < size; i++)
            {
                any <<=8;
                any |= (unsigned char)(str.data()[(size-1)-i]);
            }
            return any;
        };
    private:
        /* add your private declarations */
};

#endif /* CCONVERTER_H */
