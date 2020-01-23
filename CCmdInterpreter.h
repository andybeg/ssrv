/*
 * cmdinterpreter.h
 *
 * Copyright 2014 kon <kon@kononov>
 *
 *
 *
 */
#ifndef CMDINTERPRETER_H
#define CMDINTERPRETER_H

#include <string>
#include "condition.h"
#include "Misc.h"
#include "Thread.h"
#include "./Socket/ServerSocket.h"
    #define cmd(a,b,c,d)   ((uint32)(d<<24)+(uint32)(c<<16)+(uint32)(b<<8)+(uint32)a)


#define ANALOG_INPUTS    16
#define DISCRET_INPUTS   4
#define SAMPLES_IN_INSP_BLOCK   2048

typedef struct{
    std::string fileName;       //4
    SDateTime curTime;  //4
    uint32 fileVer;     //4
    uint32 fileSize;    //4
    uint32 CRC;         //4
    char extID[4];      //4
    int dataReceived;
} FileParams;

class CCmdInterpreter: public CThread,CConverter
{
    public:
        enum CMD_Err{
            CMD_ERR_OK,
            CMD_ERR_UE,
            CMD_ERR_WRONG_CONDITION,
            CMD_ERR_UNKNOWN,
            CMD_HARDWARE_ERR,
            CMD_SOFTWARE_ERR,
            CMD_VALID_ERR,
            CMD_PARSE_ERR,
            CMD_INSUFFICIENTLY_SPACE,
            CMD_ERR_UNKNOWN_CMD,
            CMD_NO_DATA,
            CMD_ERR_COUNT
        };
        CCmdInterpreter();
        virtual ~CCmdInterpreter();
        void    stopThread();
        void    insertBuff(char * buffer);
        char    getCmd(const std::string &buffer);
        char    nextPkgLen(char cmd);
        void    getParams(char cmd, char & buffer);
        char    setDate();
        char    getDate();
        char    createFileData();

        void    mkAnswer(std::string &buffer, const std::string &data);
        char    openFile();
        char    initTimer();
        int     getCondition(void);
        void    setCondition(int);
        void    resumeSession(std::string session);
        void    run();

    private:
        static char const * ShortAnsErr[CMD_ERR_COUNT];
        void            wait();
        virtual void    Thread( void );
        int             globalInit(void);
        void            GTSP(std::string &send);
        void            GetFsState(std::string &send);
        void            CLRT(void);
        void            GRMS(std::string &buf);
        void            GetState(std::string &buf);
        void            RTRE(std::string &buf);
        void            GetSessionsList(std::string &buf);
        void            GetSessionsListSize(std::string &buf);
        void            GetSessionsNum(std::string &buf);
        int             DLSS(const std::string &data);
        void            GetCustomList(std::string &send, const std::string &received);
        void            GetTrendSlow(std::string &send,const std::string &received);
        void            GetTrendSlowSize(std::string &send,const std::string &received);
        //операции с файлами калибровки DSP
        void            GetDspCalibration(std::string &send,const std::string &received);
        void            DelDspCalibrationData(std::string &send,const std::string &received);
        int             SetDspCalibrationData(const std::string &received);


        //получение файла произвольного типа
        void            GetFile(std::string &send,const std::string &received);
        //получение списка медленных трендов
        void            SLNM(std::string &send, const std::string &received);
        void            SLSZ(std::string &send, const std::string &received);
        //получение списка обследований
        void            INNM(std::string &send, const std::string &received);
        void            INSG(std::string &send, const std::string &received);
        //получение списка трендов сессии
        void            GetTrendList(std::string &send, const std::string &received);
        void            TRSZ(std::string &send, const std::string &received);
        //получение обследований
        void            GetInspectionSize(std::string &send,const std::string &received);
        void            GetInspection(std::string &send,const std::string &received);
        //получение конфигурации
        void            GetConfigSize(std::string &send,const std::string &received);
        void            GetConfig(std::string &send,const std::string &received);
        //получение информации о сессии
        void            GetSessionInfo(std::string &send, const std::string &received);
        void            GetSessionInfoSize(std::string &send, const std::string &received);
        //установка новой конфигурации
        int             SetConfig(const std::string &received);
        //получение блока параметров
        void            GetBlockParamSize(std::string &send);
        void            GetBlockParam(std::string &send);
        //получение файла DeviceInfo.xml
        void            GetDeviceInfoSize(std::string &send);
        void            GetDeviceInfo(std::string &send);
        //короткий ответ
        void            ShortAnswere(int err,std::string &send);
        //возможность выполнения произвольных команд среды
        void            BASH(const std::string &data, std::string &buf);
        int             cmdFILE(const std::string &buf);
        int             DATA(const std::string &buf);
        void            postExec();
        FileParams      fileParams;
        char            bytesToSend;
        struct{
            uint32      ID;
            int         numBytes;
        }Data;
        int             calcBlockNum;
        int             fileBlockCounter;
        int             fileBlockNum;
        char            cmdIsFull;
        char            condition;
        char            curCmd;
        size_t          prevCmd;
};
#endif /* CMDINTERPRETER_H */
