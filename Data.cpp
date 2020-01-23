#ifndef __linux__
#include <windows.h>
#else
#include <time.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include "Data.h"
#include <cstring>
#include <cstdarg>
#include <linux/rtc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>

void GetCurDateTime( SDateTime* pDT ) {
#ifndef __linux__
    SYSTEMTIME time;
    GetLocalTime(&time);

    pDT->Milliseconds = (uint16)time.wMilliseconds;
    pDT->Second       = (uint8)time.wSecond;
    pDT->Minute       = (uint8)time.wMinute;
    pDT->Hour         = (uint8)time.wHour;
    pDT->Day          = (uint8)time.wDay;
    pDT->Month        = (uint8)time.wMonth;
    pDT->Year         = (uint8)(time.wYear - 2000);
#else
    char buffer[16];
    char tmp[3];
    int milli;
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
#endif
}
char hex2bin( const char *s )
{
    int ret=0;
    int i;
    for( i=0; i<2; i++ )
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

const char *DateTime2Str( SDateTime *pDT ) {
    static char str[32];

    sprintf( str, "%02u%02u%02u.%02u%02u%02u%03u", pDT->Year, pDT->Month, pDT->Day,
        pDT->Hour, pDT->Minute, pDT->Second, pDT->Milliseconds );
    str[16] = 0;

    return str;
}


//============================================================================
//
//============================================================================
int SetRTCDateTime( struct tm* udt ) {
    struct rtc_time rtc_tm;
    int rtc_fd = open( "/dev/rtc0", O_RDWR);
    if ( rtc_fd < 0 ) {
        printf( "/dev/rtc open error\n" );
        return -1;
    }

    // Set the RTC time/date
    rtc_tm.tm_mday = udt->tm_mday;
    rtc_tm.tm_mon  = udt->tm_mon;
    rtc_tm.tm_year = udt->tm_year;
    rtc_tm.tm_hour = udt->tm_hour;
    rtc_tm.tm_min  = udt->tm_min;
    rtc_tm.tm_sec  = udt->tm_sec;

    int ret = ioctl( rtc_fd, RTC_SET_TIME, &rtc_tm );
    if ( ret < 0 ) {
        printf( "GetDateTime RTC_SET_TIME error (%d) %s\n", ret , strerror(errno));
        return ret;
    }
    close(rtc_fd);
    return 0;
}
