#ifndef _LCD12864BU_H_
#define _LCD12864BU_H_

#define LCDLCDDisp_Off   0x3e
#define Page_Add     0xb8//页地址
#define Start_Line     0xC0//行地址
#define LCDCol_Add     0x40//列地址
#define LCDLCDDisp_On    0x3f
//	write_com(LCDLCDDisp_Off);	 //写初始化命令
//	write_com(Page_Add+0);
//	write_com(Start_Line+0);
//	write_com(LCDCol_Add+0);
//	write_com(LCDLCDDisp_On);

/*****液晶显示器的端口定义*****/
#define data_ora P0 /*液晶数据总线*/
sbit LCDDi=P2^0 ;     /*数据/指令 选择*/
sbit LCDRW=P2^1 ;     /*读/写 选择*/
sbit LCDEnable=P2^2 ;     /*读/写 使能*/
sbit cs1=P2^3 ;    /*片选1*/
sbit cs2=P2^4 ;    /*片选2*/
sbit RESET=P2^5 ;  /*复位信号*/

uchar menu_1;
uchar flag_clock_en;	//闹钟标志位
uchar flag_clock_kai;	//闹钟开标志位
uchar flag_c_en;
bit flag_100ms;



#include "ds1302.h"


/****************************************************************************
函数功能:LCD延时程序
入口参数:t
出口参数:
****************************************************************************/
void LCDdelay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++);
	for(j=0;j<10;j++);
}
/****************************************************************************
状态检查，LCD是否忙


*****************************************************************************/
void CheckState()		
{
   unsigned char dat,DATA;//状态信息（判断是否忙）
   LCDDi=0; // 数据\指令选择，D/I（RS）="L" ，表示 DB7∽DB0 为显示指令数据 
   LCDRW=1; //R/W="H" ，E="H"数据被读到DB7∽DB0 
   do
   {
      DATA=0x00;
      LCDEnable=1;	//EN下降源
			LCDdelay(2);//延时
			dat=DATA;
      LCDEnable=0;
      dat=0x80 & dat; //仅当第7位为0时才可操作(判别busy信号)
    }
    while(!(dat==0x00));
}
/*****************************************************************************
函数功能:写命令到LCD程序，RS(DI)=L,RW=L,EN=H，即来一个脉冲写一次
入口参数:cmdcode
出口参数:
*****************************************************************************/
void write_com(unsigned char cmdcode)
{
    CheckState();//检测LCD是否忙
	LCDDi=0;
	LCDRW=0;
	P0=cmdcode;		
	LCDdelay(2);
	LCDEnable=1;
	LCDdelay(2);
	LCDEnable=0;
}
/*****************************************************************************
函数功能:LCD初始化程序
入口参数:
出口参数:
*****************************************************************************/
void init_lcd()
{
	LCDdelay(100);	
	cs1=1;//刚开始关闭两屏
	cs2=1;
	LCDdelay(100);
	write_com(LCDLCDDisp_Off);	 //写初始化命令
	write_com(Page_Add+0);
	write_com(Start_Line+0);
	write_com(LCDCol_Add+0);
	write_com(LCDLCDDisp_On);
}
/*****************************************************************************
函数功能:写数据到LCD程序，RS(DI)=H,RW=L,EN=H，即来一个脉冲写一次
入口参数:LCDDispdata
出口参数:
*****************************************************************************/
void write_data(unsigned char LCDDispdata)
{
    CheckState();//检测LCD是否忙
	LCDDi=1;
	LCDRW=0;
	P0=LCDDispdata;
	LCDdelay(2);
	LCDEnable=1;
	LCDdelay(2);
	LCDEnable=0;
}

/***********************写汉字函数**********************/
void write_yhanzi(uchar n,uchar x,uchar y,uchar  * p)
{
   uchar i,j,nm,m; 
   m=0xb8+x;

   for(j=0;j<n;j++)
   {
	   
	   if((16*j+y)<64)
	       {
		   		cs1=1;cs2=0;nm=0x40+y+16*j;  
		   }
	   else
	       {
		   		cs1=0;cs2=1;nm=0x40+y+16*(j-4);  
		   }
       write_com(m);
       write_com(nm);
	   for(i=0;i<16;i++)
	    { 
			write_data(*p);p++; 
		}
	   write_com(m+1);
	   write_com(nm);
	   for(i=16;i<32;i++)
	    { 
			write_data(*p);p++;
	    }
   }
}

void write_yhanzi_fanbai(uchar n,uchar x,uchar y,uchar  * p,uchar fanbai)
{
   uchar i,j,nm,m; 
   m=0xb8+x;

   for(j=0;j<n;j++)
   {
	   
	   if((16*j+y)<64)
	       {
		   		cs1=1;cs2=0;nm=0x40+y+16*j;  
		   }
	   else
	       {
		   		cs1=0;cs2=1;nm=0x40+y+16*(j-4);  
		   }
       write_com(m);
       write_com(nm);
	   for(i=0;i<16;i++)
	    { 
			if(fanbai == 0)
				write_data(*p);
			else 	
				write_data(~(*p));
				p++; 
		}
	   write_com(m+1);
	   write_com(nm);
	   for(i=16;i<32;i++)
	    { 
			if(fanbai == 0)
				write_data(*p);
			else 	
				write_data(~(*p));
				p++; 
	    }
   }
}

/***********************写数字函数**********************/
void write_shuzi(uchar n,uchar x,uchar y,uchar  * p,uchar fanbai)
{
   uchar i,j,nm,m; 
   m=0xb8+x;
   for(j=0;j<n;j++)
   {
	   
	   if((8*j+y)<64)
	       {cs1=1;cs2=0;nm=0x40+y+8*j;  }
	   else
	       {cs1=0;cs2=1;nm=0x40+y+8*(j-8);  }
       write_com(m);
       write_com(nm);
	   for(i=0;i<8;i++)
	    { 
		if(fanbai == 0)
		write_data((*p));
		else 
		write_data(~(*p));
		p++; 
		}
	   write_com(m+1);
	   write_com(nm);
	   for(i=8;i<16;i++)
	    { 
		if(fanbai == 0)
		write_data((*p));
		else 
		write_data(~(*p));
		p++; 
		}
   }
}

/**********************************************************
*函数名: write_shu16
*功能:在12864上显示16进制数据
*说明:
*输入:无
*返回:无
**********************************************************/
void write_shu16(uchar x,uchar y,uchar dat,uchar fanbai)
{
	write_shuzi(1,x,y,shuzi[dat / 16],fanbai);
	write_shuzi(1,x,y+8,shuzi[dat % 16],fanbai);
}


void write_shu18b20(uchar x,uchar y,uint dat,uchar fanbai)
{
	write_shuzi(1,x,y,shuzi[dat / 100 % 10],fanbai);
	write_shuzi(1,x,y+8,shuzi[dat / 10 % 10],fanbai);
	write_shuzi(1,x,y+24,shuzi[dat % 10],fanbai);
}


void write_shu123(uchar x,uchar y,uchar dat,uchar fanbai)
{
	write_shuzi(1,x,y,shuzi[dat / 16],fanbai);
	write_shuzi(1,x,y+8,shuzi[dat % 16],fanbai);
}


/*****************************************************************************
函数功能:清除LCD内存程序
入口参数:pag,col,hzk
出口参数:
*****************************************************************************/
void Clr_Scr()
{
	unsigned char j,k;
	cs1=0; //左、右屏均开显示
	cs2=0;
 	write_com(Page_Add+0);
 	write_com(LCDCol_Add+0);
 	for(k=0;k<8;k++)//控制页数0-7，共8页
 	{
		write_com(Page_Add+k); //每页每页进行写
  		for(j=0;j<64;j++)  //每页最多可写32个中文文字或64个ASCII字符
		{
      		write_com(LCDCol_Add+j);
			write_data(0x00);//控制列数0-63，共64列，写点内容，列地址自动加1
		}
	}
}
/*****************************************************************************
函数功能:左屏位置显示
入口参数:page,column,hzk
出口参数:
*****************************************************************************/
void Bmp_Left_Disp(unsigned char page,unsigned char column, unsigned char code *Bmp)
{
	unsigned char j=0,i=0;
	for(j=0;j<2;j++)
	{
		write_com(Page_Add+page+j);
		write_com(LCDCol_Add+column);
		for(i=0;i<16;i++) 
		{
			write_data(Bmp[128*j+i]);
			delay_1ms(50);
		}
	}
}

void Bmp_Left_Disp_hanzi(unsigned char page,unsigned char hang, unsigned char code *Bmp)
{
	unsigned char j=0,i=0;
	for(j=0;j<2;j++)
	{
		write_com(Page_Add+page+j);
		write_com(LCDCol_Add+hang);
		for(i=0;i<16;i++) 
//			write_data(Bmp[128*j+i]);
			write_data(Bmp[0x00]);
	}
}


void clear_12864()
{	 	
	uchar x,y;
	cs1=1;
	cs2=0;
	for(x=0;x<8;x++)
	{
		write_com(0xb8+x);
		for(y=0;y<64;y++)
		{		 
			write_data(0X00);
		}
	}		
}
/*****************************************************************************
函数功能:右屏位置显示
入口参数:page,column,hzk
出口参数:
*****************************************************************************/
void Bmp_Right_Disp(unsigned char page,unsigned char column, unsigned char code *Bmp)
{
	unsigned char j=0,i=0;
	for(j=0;j<2;j++)
	{
		write_com(Page_Add+page+j);
		write_com(LCDCol_Add+column);
		for(i=64;i<128;i++) 
		{
			write_data(Bmp[128*j+i]);
			delay_1ms(50);

		}

	}
}

void qingling()
{
   uchar x,y ;
   cs1=1;
   cs2=1;
   write_com(0x3f);
   write_com(0xc0);
   for(x=0;x<8;x++)
    {  
	   write_com(0xb8+x);
	   write_com(0xc0);
     for(y=0;y<64;y++)
	   {
	      write_data(0x00);
	   }
	   write_com(0xb8+x+1);
	   write_com(0xc0);
     for(y=0;y<64;y++)
	   {
	      write_data(0x00);
	   }
	}
}
#endif