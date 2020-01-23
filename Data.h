#ifndef __DATA_H__
#define __DATA_H__

#include "Typedef.h"
#include "CDateTime.h"

#define RECORDS_IN_TREND        600

#define ANALOG_INPUTS    16
#define DISCRET_INPUTS   4
#define PARAM_MAX        31
#define ED_REGS          38
#define MODBUS_REGS      0

#define DIAGNOS_ELEMENTS        16

//Режимы работы
#define MODE_INITIALIZATION     0x00000001
#define MODE_STOPPED            0x00000002
#define MODE_RUNNING            0x00000004
#define MODE_RUN_PREPARATION    0x00000008
#define MODE_STOP_PREPARATION   0x00000010
#define MODE_WAIT_RESTART       0x00000020
#define MODE_ERROR              0x00000040

//признак для компиляции эмулятора взамен целевого устройства
//#define DEBUG
//#define EMUL
#ifndef EMUL
#define ARM
#endif
//путь расположения внутренних конфигурационных файлов
#ifdef EMUL
#define CFG_PATH  "/home/test/dev"
#else
#define CFG_PATH  "/home/root"
#endif
#define CMND(a,b,c,d)   ((uint32)(d<<24)+(uint32)(c<<16)+(uint32)(b<<8)+(uint32)a)

#ifndef M_PI
#define M_PI                    3.1415926535897932384626433832795f
#endif


//======================================================================
// Структура описывающая заголовок тэга
typedef struct {
    uint32   ChunkId;         // Идентификатор тэга
    uint32   ChunkSize;       // Размер тэга в байтах
    uint32   SubChunkOffset;  // Смещение первого вложенного тэга
} SChunk;




typedef struct {
    uint8      NumChan;   // Номер канала (для параметров аналоговых от 0, для параметров дискретных от 0x40, для параметров САУ от 0x80 )
    uint8       NumParam; // Номер параметра в канале
    uint8       NumLevel;   // Порядковый номер уставки для текущего параметра
    uint8       State;
    float       ValueUp;             // Верхняя граница уставки
    float       ValueDown;         // Нижняя граница уставки
}  SLevel;


//=============================================================================
// ПАРАМЕТРЫ
//=============================================================================
typedef struct {
   SChunk    Chunk;          // "PRED"
   uint32    Number;
   SChunk    SubChunk;       // "DATA"
   uint16    Regs[ED_REGS];   // Регистры Экспресс-Диагностики (от 38)
} SParamsED;

#if (MODBUS_REGS > 0)
typedef struct {
   SChunk    Chunk;          // "PRMB"
   uint32    Number;         // Кол-во регистров в таблице Modbus
   SChunk    SubChunk;       // "DATA"
   uint16    Regs[MODBUS_REGS];   // Регистры таблицы Modbus
} SParamsModbus;
#endif

typedef struct {
   uint16    Channel;        // Номер канала
   uint16    Number;         // Кол-во параметров (от 1 до 31)
   uint32    States;         // Состояния (по 4 бита на параметр, старшие 4 бита - состояние канала)
} SHeaderTC;

typedef struct {
   SChunk    Chunk;          // "PRTC"
   SHeaderTC Header;
   SChunk    SubChunk;       // "DATA"
   float     Freq;           // Частоты по каналам
} SParamsTC;

typedef struct {
   uint16    Channel;        // Номер канала
   uint16    Number;         // Кол-во параметров (от 1 до 31)
   uint32    States[4];      // Состояния (по 4 бита на параметр, старшие 4 бита - состояние канала)
} SHeaderVC;

typedef struct {
   SChunk    Chunk;          // "PRVC"
   SHeaderVC Header;
   SChunk    SubChunk;       // "DATA"
   float     Params[PARAM_MAX];  // До 31 параметра
} SParamsVC;

typedef struct {
   SChunk        Chunk;      // "BPRM"
   SDateTime     Time;       // Время блока параметров
   SParamsED     ParamED;    // Экспресс-Диагностика
#if (MODBUS_REGS > 0)
   SParamsModbus ParamMB;    // Параметры Modbus (Опционально)
#endif
   SParamsTC     ParamTC[DISCRET_INPUTS];    // Параметры Тахоканалов (Опционально)
   SParamsVC     ParamVC[ANALOG_INPUTS];  // Параметры Виброканалов ( от 1 до ... )
} SBlockParams;

typedef struct {
    uint32 Mode;
    uint32 State;
    uint32 Progress;
} SState;

/*
typedef struct {
   SChunk    Chunk;          // "PRED"
   uint32    Number;
   SChunk    SubChunk;       // "DATA"
   uint16    Regs[Number];   // Регистры Экспресс-Диагностики (от 38)
} SParamsED;

typedef struct {
   SChunk    Chunk;          // "PRMB"
   uint32    Number;         // Кол-во регистров в таблице Modbus
   SChunk    SubChunk;       // "DATA"
   uint16    Regs[Number];   // Регистры таблицы Modbus
} SParamsModbus;

typedef struct {
   uint16    Channel;        // Номер канала
   uint16    Number;         // Кол-во параметров (от 1 до 31)
   uint16    States[1..8];   // Состояния (по 4 бита на параметр, старшие 4 бита - состояние канала)
} SParamHeader;              // Размер структуры определяется полем SParams.Chunk.SubChunkOffset

typedef struct {
   SChunk    Chunk;          // "PRVC" или "PRTC"
   SParamHeader Header;
   SChunk    SubChunk;       // "DATA"
   float     Params[PARAM_MAX];  // До 31 параметра
} SParams;

typedef struct {
   SChunk        Chunk;      // "BPRM"
   SDateTime     Time;       // Время блока параметров
   SParamsED     ParamED;    // Экспресс-Диагностика
   SParamsModbus ParamMB;    // Параметры Modbus (Опционально)
   SParams       ParamTC[DISCRET_INPUTS]; // Параметры Тахоканалов
   SParams       ParamVC[ANALOG_INPUTS];  // Параметры Виброканалов
} SBlockParams;
*/


//=============================================================================
// ТРЕНД
//=============================================================================

// Структура заголовока тренда
typedef struct {
    SId       DeviceID;      // Идентификатор прибора
    SId       SessionID;     // Идентификатор сессии
    SId       ConfigID;      // Идентификатор сессии
    SId       AggregateID;   // Идентификатор агрегата
    SDateTime FirstTime;     // Время первого блока параметров в тренде
    SDateTime LastTime;      // Время последнего блока параметров в тренде
    uint32    Number;        // Кол-во блоков параметров в тренде
} STrendHeader;

/*
// Структура файла тренда
typedef struct {
    SChunk       Chunk;         // "TRND"
    STrendHeader Header;
    SBlockParams BlockParams[Header.Number];
} STrend;
*/

//=============================================================================
// ОБСЛЕДОВАНИЕ
//=============================================================================

// Структура заголовка обследования
typedef struct {
    SId       DeviceID;       // Идентификатор прибора
    SId       SessionID;      // Идентификатор сессии
    SId       ConfigID;       // Идентификатор сессии
    SId       AggregateID;    // Идентификатор агрегата
    SDateTime DateTime;       // Время окончания сбора обследования
    uint32    MaskVibro;      // Битовые Флаги виброканалов в обследовании
    uint32    MaskTacho;      // Битовые флаги тахоканалов в обследовании
} SInspectionHeader;

// Структура заголовка данных виброволн
typedef struct {
    uint32 Channel;           // Номер канала
    uint32 SampleFrequency;   // Частота дискретизации канала (сбора)
    float  FreqLowerCutoff;   // Нижняя частота среза волны (ФВЧ)
    float  FreqUpperCutoff;   // Верхняя частота среза волны (ФНЧ)
    uint32 Dimension;         // Размерность (1, 101 - м/с2, 102 - мм/с, 103 - мкм и т.д. и т.п.)
    uint32 Number;            // Кол-во отсчётов
} SVibroHeader;

// Структура заголовка данных тахометок
typedef struct {
    uint32 Channel;           // Номер канала
    uint32 SampleFrequency;   // Частота дискретизации канала (сбора)
    uint32 Dimension;         // Размерность (1, 101 - м/с2, 102 - мм/с, 103 - мкм и т.д. и т.п.)
    uint32 Number;            // Кол-во отсчётов
} STachoHeader;

//------------------------------------------------
//------------------------------------------------
/*
// Структура данных виброканала
typedef struct {
    SChunk        Chunk;      // "WAVE"
    SVibroHeader  WaveHeader; // Информация о виброданных
    SChunk        SubChunk;   // "DATA"
    float         Wave[WaveHeader.Number];   // Отсчёты виброволны
} SWaveVibro;

// Структура данных тахоканала
typedef struct {
    SChunk        Chunk;       // "TAXO"
    STachoHeader  TachoHeader; // Информация о таходанных
    SChunk        SubChunk;    // "DATA"
    uint32        Tacho[TachoHeader.Number]; // Тахометки
} SWaveTacho;

// Структура файла обследования
typedef struct {
    SChunk            Chunk;      // "INSP"
    SInspectionHeader Header;     // Параметры обследования

    SBlockParams      Parameters; // Режимные параметры - значения параметров в момент события (для обследований по событиям)
                                  // или в момент окончания сбора обследования (для обследований пользователя)
    SWaveVibro        Waves[];    // Данные виброканалов участвующих в обследовании

    SWaveTacho        Tachos[];   // Данные тахоканалов участвующих в обследовании
} SInspection;
*/

char hex2bin( const char *s );
const char *DateTime2Str( SDateTime *pDT );
void GetCurDateTime( SDateTime* pDT );
void printfd(const char * format, ...);
int SetRTCDateTime( struct tm* udt );
#endif
