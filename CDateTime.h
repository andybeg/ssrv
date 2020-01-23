/*
 * CDateTime.h
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


#ifndef CDATETIME_H
#define CDATETIME_H

#include <string>
#include <time.h>
#include <cstdio>
#include <cstdlib>


// Структура описывающая дату и время
typedef struct {
   uint16 Milliseconds;      // Миллисекунды (0..999)
   uint8  Second;            // Секунды (0..59)
   uint8  Minute;            // Минуты (0..59)
   uint8  Hour;              // Час (0..23)
   uint8  Day;               // День (1..31)
   uint8  Month;             // Месяц (1..12)
   uint8  Year;              // Год (0..99)
} SDateTime;

class CDateTime
{
    public:
        CDateTime(){};
        virtual         ~CDateTime(){};
        const char *DateTime2Str( void ) {
            static char str[28];
            struct timeval tv;
            struct timespec ts;
            time_t t = time(NULL);
            struct tm *t_m;
            clock_gettime (CLOCK_REALTIME, &ts);
            t_m = localtime(&ts.tv_sec);
            int Milliseconds = ts.tv_nsec/1000000UL;
            sprintf( str, "%02u/%02u/%04u %02u:%02u:%02u.%03u ", (uint8)t_m->tm_mday, (uint8)t_m->tm_mon+1, t_m->tm_year+1900,(uint8)t_m->tm_hour, (uint8)t_m->tm_min, (uint8)t_m->tm_sec, Milliseconds );
            return str;
        }
        const char *DateTime2Str( SDateTime *pDT ) {
            static char str[32];

            sprintf( str, "%02u%02u%02u.%02u%02u%02u%03u", pDT->Year, pDT->Month, pDT->Day,
                pDT->Hour, pDT->Minute, pDT->Second, pDT->Milliseconds );
            str[16] = 0;

            return str;
        }

        const char *    ToStr(SDateTime* pDT){
            sprintf( str, "%02u%02u%02u.%02u%02u%02u%03u", pDT->Year, pDT->Month, pDT->Day,
                pDT->Hour, pDT->Minute, pDT->Second, pDT->Milliseconds );
            str[16] = 0;
            return str;
        }
        const char *    ToStr(void){
            sprintf( str, "%02u%02u%02u.%02u%02u%02u%03u", m_Time.Year, m_Time.Month, m_Time.Day,m_Time.Hour, m_Time.Minute, m_Time.Second, m_Time.Milliseconds );
            str[16] = 0;
            return str;
        }
        const char *    TimeToStr(){
            sprintf( str, "%02u%02u%02u%03u", m_Time.Hour, m_Time.Minute, \
                m_Time.Second, m_Time.Milliseconds );
            str[16] = 0;
            return str;
        }
        void    FromStr(std::string str)
        {

        };
        bool DateTimeIsUp(SDateTime *pDT){
            time_t now;
            struct tm t_m;
            time(&now);

            t_m.tm_sec       = pDT->Second;
            t_m.tm_min       = pDT->Minute;
            t_m.tm_hour      = pDT->Hour;
            t_m.tm_mday      = pDT->Day ;
            t_m.tm_mon       = pDT->Month-1;
            t_m.tm_year      = pDT->Year+100;
            if(difftime(now,mktime(&t_m))>0){
                return true;
            }else{
                return false;
            }
        }
        static SDateTime ToTime(uint32 tm){
            SDateTime Time;
            return Time;
        }
        void GetCurDateTime( SDateTime* pDT ) {
            struct timeval tv;
            struct timespec ts;
            time_t t = time(NULL);
            struct tm *t_m;
            clock_gettime (CLOCK_REALTIME, &ts);
            t_m = localtime(&ts.tv_sec);
            pDT->Milliseconds = ts.tv_nsec/1000000UL;
            pDT->Second       = (uint8)t_m->tm_sec;
            pDT->Minute       = (uint8)t_m->tm_min;
            pDT->Hour         = (uint8)t_m->tm_hour;
            pDT->Day          = (uint8)t_m->tm_mday;
            pDT->Month        = (uint8)t_m->tm_mon+1;
            pDT->Year         = (uint8)(t_m->tm_year - 100);
        }
        SDateTime GetTime(std::string str){
            m_Time.Second       = atoi(str.substr(6,2).c_str());
            m_Time.Minute       = atoi(str.substr(3,2).c_str());
            m_Time.Hour         = atoi(str.substr(0,2).c_str());
            m_Time.Milliseconds = atoi(str.substr(9,3).c_str());
            return m_Time;
        }
        SDateTime GetDateTime(std::string str){
            m_Time.Day          = atoi(str.substr(0,2).c_str());
            m_Time.Month        = atoi(str.substr(3,2).c_str());
            m_Time.Year         = atoi(str.substr(6,4).c_str());
            m_Time.Hour         = atoi(str.substr(11,2).c_str());
            m_Time.Minute       = atoi(str.substr(14,2).c_str());
            m_Time.Second       = atoi(str.substr(17,2).c_str());
            m_Time.Milliseconds = atoi(str.substr(20,3).c_str());
        }
        uint64 TimeVal(SDateTime tm)
        {
            return ((tm.Hour*60+tm.Minute)*60+tm.Second)*1000+tm.Milliseconds;
        }
        SDateTime GetCurDateTime( void ) {
            struct timeval tv;
            struct timespec ts;
            time_t t = time(NULL);
            struct tm *t_m;
            clock_gettime (CLOCK_REALTIME, &ts);
            t_m = localtime(&ts.tv_sec);
            m_Time.Milliseconds = ts.tv_nsec/1000000UL;
            m_Time.Second       = (uint8)t_m->tm_sec;
            m_Time.Minute       = (uint8)t_m->tm_min;
            m_Time.Hour         = (uint8)t_m->tm_hour;
            m_Time.Day          = (uint8)t_m->tm_mday;
            m_Time.Month        = (uint8)t_m->tm_mon+1;
            m_Time.Year         = (uint8)(t_m->tm_year - 100);
            return m_Time;
        }
    private:
        SDateTime   m_Time;
        static char str[32];
};

#endif /* CDATETIME_H */
