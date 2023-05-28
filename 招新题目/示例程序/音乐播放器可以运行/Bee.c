#include <REGX52.H>
#include "Timer1.h"
#include "Delay.h"
#include "IR.h"
#define P 0
#define L1 1
#define L1_ 2
#define L2 3
#define L2_ 4
#define L3 5
#define L4 6
#define L4_ 7
#define L5 8
#define L5_ 9
#define L6 10
#define L6_ 11
#define L7 12
#define M1 13
#define M1_ 14
#define M2 15
#define M2_ 16
#define M3 17
#define M4 18
#define M4_ 19
#define M5 20
#define M5_ 21
#define M6 22
#define M6_ 23
#define M7 24
#define H1 25
#define H1_ 26
#define H2 27
#define H2_ 28
#define H3 29
#define H4 30
#define H4_ 31
#define H5 32
#define H5_ 33
#define H6 34
#define H6_ 35
#define H7 36

sbit Buzzer = P1 ^ 5;
unsigned char FreqSelect1 = 0, MusicSelect1 = 0;
unsigned char FreqSelect2 = 0, MusicSelect2 = 0;
unsigned char FreqSelect = 0,temp = -1;
unsigned char Command = 0;

unsigned int FreqTable[] = {0, 63556, 63731, 63836, 63929, 64021, 64104, 64185,
                            64261, 64332, 64400, 64464, 64524, 64580, 64634, 64685, 64733, 64778, 64820,
                            64861, 64899, 64935, 64968, 65000, 65030, 65058, 65086, 65111, 65135, 65157,
                            65179, 65199, 65218, 65235, 65252, 65268, 65283};

unsigned char code Music1[] = {M3, 2, M5, 2, M5, 3, M6, 1, M5, 2, M3, 2, M1, 2, M1, 1, M2, 1,
                               M3, 2, M3, 2, M2, 2, M1, 2, M2, 4, P, 4,
                               M3, 2, M5, 2, M5, 3, M6, 1, M5, 2, M3, 2, M1, 2, M1, 1, M2, 1,
                               M3, 2, M3, 2, M2, 2, M2, 2, M1, 4, P, 4,
                               M4, 4, M4, 4, M4, 2, M6, 6,
                               M5, 4, M5, 2, M3, 2, M2, 4, P, 4,
                               M3, 2, M5, 2, M5, 3, M6, 1, M5, 2, M3, 2, M1, 2, M1, 1, M2, 1,
                               M3, 2, M3, 2, M2, 2, M2, 2, M1, 4, P, 4, 0xFF};

unsigned char code Music2[] = {M1, 4, M1, 4, M5, 4, M5, 4, M6, 4, M6, 4, M5, 4, P, 4,
                               M4, 4, M4, 4, M3, 4, M3, 4, M2, 4, M2, 4, M1, 4, P, 4, M5, 4, M5, 4, M4, 4, M4, 4, M3, 4, M3, 4,
                               M2, 4, P, 4, M5, 4, M5, 4, M4, 4, M4, 4, M3, 4, M3, 4,
                               M2, 4, P, 4, M1, 4, M1, 4, M5, 4, M5, 4, M6, 4, M6, 4, M5, 4, P, 4, M4, 4, M4, 4, M3, 4, M3, 4, M2, 4, M2, 4, M1, 4, P, 4, 0xFF};

void Bee_Set(unsigned char Count)
{
    IR_Init();

    while (1)
    {
        TR1 = 0;
        if (IR_GetDataFlag()) //如果收到数据帧
        {
            Command = IR_GetCommand(); //获取遥控器命令码

            if (Command == 0x44)
            {
                TR1 = ~TR1;
                Count = 0;
                break;
            }
        }
    }
    while (1)
    {
        while (Count == 0)
        {

            if (IR_GetDataFlag()) //如果收到数据帧
            {
                Command = IR_GetCommand(); //获取遥控器命令码

                if (Command == 0x44)
                {
                    TR1 = ~TR1;
                    temp = Count;
                    Count = -1;
                    break;
                }
                if (Command == 0x40)
					
                {
					Count--;
                    if (Count < 0)
                    {
                        Count = 1;
                        break;
                    }
                   
                }
                if (Command == 0x43)
                {
					Count++;
                    if (Count > 1)
                    {
                        Count = 0;
                        break;
                    }
                }
            }
            FreqSelect1 = Music1[MusicSelect1];
            FreqSelect = FreqSelect1;
            MusicSelect1++;
            Delay(125 * Music1[MusicSelect1]);
            MusicSelect1++;
            TR1 = 0;
            Delay(5);
            TR1 = 1;
            if (Music1[MusicSelect1] == 0xFF)
            {
                MusicSelect1 = 0;
            }
        }
        while (Count == 1)
        {

            if (IR_GetDataFlag()) //如果收到数据帧
            {
                Command = IR_GetCommand(); //获取遥控器命令码

                if (Command == 0x44)
                {
                    TR1 = ~TR1;
                    temp = Count;
                    Count = -1;
                    break;
                }
               if (Command == 0x40)
					
                {
					Count--;
                    if (Count < 0)
                    {
                        Count = 1;
                        break;
                    }
                   
                }
                if (Command == 0x43)
                {
					Count++;
                    if (Count > 1)
                    {
                        Count = 0;
                        break;
                    }
                }
            }
            TR1 = 1;
            FreqSelect2 = Music2[MusicSelect2];
            FreqSelect = FreqSelect2;
            MusicSelect2++;
            Delay(125 * Music2[MusicSelect2]);
            MusicSelect2++;
            TR1 = 0;
            Delay(5);
            TR1 = 1;
            if (Music2[MusicSelect2] == 0xFF)
            {
                MusicSelect2 = 0;
            }
        }
        if (IR_GetDataFlag()) //如果收到数据帧
        {
            Command = IR_GetCommand();
            //获取遥控器命令码
            if (Command == 0x44)
            {
                TR1 = ~TR1;
                Count = temp;
            }
            if (Command == 0x40)
					
                {
					Count--;
                    if (Count < 0&&Count==-1)
                    {
                        Count = 1;
                        break;
                    }
                   
                }
                if (Command == 0x43)
                {
					Count++;
                    if (Count > 1&&Count==-1)
                    {
                        Count = 0;
                        break;
                    }
                }
        }
    }
}

void Timer1_Routine() interrupt 3
{
    if (FreqTable[FreqSelect])
    {
        TL1 = FreqTable[FreqSelect] % 256; //设置定时初始值
        TH1 = FreqTable[FreqSelect] / 256; //设置定时初始值
        Buzzer = !Buzzer;
    }
}