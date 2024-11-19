/*
 * @Copyright (c) 2023,  公司名称 : 湖南德米特仪器有限公司
 * @file name:
 * @version: V1.0.1
 * @Date: 2023-09-13
 * @content:
 * @todo:
 * @Author: 陈田力
 */

#ifndef unix
#include <windows.h>
#include <winsock.h>
#else
#define closesocket close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <iostream>
#include <string>
#include <thread>
#include <pthread.h>
#include <vector>
#include "Encode.h"
#include "CommonData.h"
#include <map>
#include "zmq.hpp"
using namespace std;

void program_info()
{
    cout << endl
         << "CBM_Socket_Service for Version V1.0.1 " << endl;
    cout << endl
         << "Copyright (c) 2023-08-31, DMT.  All rights reserved." << endl;
    cout << endl
         << "Welcome to CBM_Socket_Service, type \"CBM_Socket_Service status\" for information." << endl
         << endl;
}

void program_start()
{
    cout << endl
         << "completed starting CBM_Socket_Service!" << endl
         << endl;
    return;
}

void program_stop()
{
}

void program_status()
{
}

int main(int argc, char *argv[])
{
    program_info(); 

    pthread_t sendid, pSocketReceiveId, pMqReceiveId, pMqReceive2PubId;
    string keyboard;
    int ret = 0;
    zmq::context_t context(1);

    extern void *socketReceive(void *);
    ret = pthread_create(&pSocketReceiveId, NULL, socketReceive, &context);

    if (ret != 0)
    {
        printf("Create pthread socket receive error!");
        exit(1);
    }

    
    extern void *mqReceive(void *arg);
    ret = pthread_create(&pMqReceiveId, NULL, mqReceive, &context);    

    if (ret != 0)
    {
        printf("Create pthread MQ receive error!");
        exit(1);
    }

    extern void *mqReceive2Pub(void *);
    ret = pthread_create(&pMqReceive2PubId, nullptr, mqReceive2Pub, &context);
    if (ret != 0)
    {
        printf("Create pthread MQ PUB receive error!");
        exit(1);
    }
    // extern int SendDataMessage(int sockt, void *szbuffer, unsigned long length);
    while (1)
    {
        cin >> keyboard;
        if (!keyboard.compare("quit"))
        {
            break;
        }
        // else
        // {
        //     int sockt = std::stoi(keyboard);
        //     keyboard = "";
        //     std::vector<unsigned char> rcvdata; //= {0xF1, 0x00, 0x0B ,0x01, 0xA1, 0x20, 0x01, 0x01, 0x0D, 0xBB, 0x9F};

        //     char sSndData[100];
        //     string szHead = "F1";
        //     int nLenght = 8;
        //     int nDevId = 1;
        //     string szOpt = "A1";
        //     string szCmd = "2001";
        //     string szRsq = "01";
        //     int nData = 899999;
        //     sprintf(sSndData, "%s %04x %02x %s %s %s %06x", szHead.c_str(), nLenght, nDevId, szOpt.c_str(), szCmd.c_str(), szRsq.c_str(), nData);

        //     String2Hex(sSndData, rcvdata);

        //     SendDataMessage(sockt, rcvdata.data(), rcvdata.size());
        //     cout << rcvdata.size() << endl;
        // }
    }

    pthread_cancel(pSocketReceiveId);
    pthread_cancel(pMqReceiveId);
    context.close();
    Sleep(3);

    return EXIT_SUCCESS;
}