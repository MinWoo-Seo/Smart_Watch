#include "stm32f4xx.h"
#include <math.h>
#include "GLCD.h"
#define SW0_PUSH        0xFE00  //PH8
#define SW1_PUSH        0xFD00  //PH9
#define SW2_PUSH        0xFB00  //PH10
#define SW3_PUSH        0xF700  //PH11
#define SW4_PUSH        0xEF00  //PH12
#define SW5_PUSH        0xDF00  //PH13
#define SW6_PUSH        0xBF00  //PH14
#define SW7_PUSH        0x7F00  //PH15

#define NAVI_SW_PUSH       0x0FD0  //PI5
#define NAVI_SW_UP           0x0FB0  //PI6
#define NAVI_SW_DOWN       0x0F70  //PI7
#define NAVI_SW_RIGHT       0x0EF0  //PI8
#define NAVI_SW_LEFT        0x0DF0  //PI9
#define pi  3.141592
void _RCC_Init(void);
void _GPIO_Init(void);
void DisplayTitle(void);

void _ADC_Init(void);
uint16_t KEY_Scan(void);
uint16_t KEY1_Scan(void);
void DelayMS(unsigned short wMS);
void DelayUS(unsigned short wUS);
void BEEP(void);
void TIME7_Init(void);
void TIME7_time(void);
void TIME9_Init(void);
void TIME3_Init(void);
void TIME4_Init(void);
void _EXTI_Init(void);
void ALARM_Setup(void);
void ALARM_Switch(void);
void STOP_WATCH_Setup (void);
void STOP_WATCH (void);
void Calculator_Setup(void);
void Calculator(void);
void USART1_Init(void);
void USART_BRR_Configuration(uint32_t USART_BaudRate);
/////
                          // TX 용  (인터럽트 안 썼음)
void SerialPutChar(uint8_t c);        // 1byte 보냄
void Serial_PutString(char *s);         // 여러 글 보냄
void Thermostat_Setup(void);
void Thermostat(void);
  
uint16_t  q =0;      //플래그
uint8_t ST1=0,ST2=0,ST3=0;   //스탑 워치 변수
uint8_t WT1=0,WT2=0,WT3=0; // 스탑 워치 기록 변수
uint8_t C1=0x30,C2=0x30,C3=0x30, C4=0x30, C5=0x30,C6=0x30; // 계산기 변수
unsigned short ADC_Value, Voltage;
signed short Vo;
uint8_t SW0_Flag=1, i=1;
char str[20];
void _time(uint8_t a,uint8_t b,uint8_t c);
int TIM7_Flag=0;
uint16_t x=29,t=4,y=10,x1=1,xx,tt,yy;  //x : 29 t,:4
uint8_t t1=10, t2=15,t3=1;  //t1 :10,  t2:15
uint8_t AR,AR1=0, AR2=0;
int main(void)
{
  
	_RCC_Init();
	LCD_Init();			// LCD 구동 함수
	DelayMS(10);			// LCD구동 딜레이
        _EXTI_Init();
	DisplayTitle();		//LCD 초기화면구동 함수
	_GPIO_Init();
	//_ADC_Init();
        ALARM_Setup();
       TIME7_Init();   //시계 타이머
       TIME9_Init();   //스탑워치 타이머
       TIME3_Init();   // ADC 구동 타이머
       TIME4_Init();   // LED 타이머
       RCC->APB1ENR &= ~(1<<1);  // timer 3  // 미리 끔
      KEY1_Scan();          // 조이스틱 키스캔
      USART1_Init();         //USART 1
      _ADC_Init();             // ADC 시작
      RCC->APB1ENR &= ~(1<<8);  // timer 14 off   // 처음 상태는 부저 소리 안나게 하기 위해서.    
      RCC->APB2ENR &= ~(1<<16);   //timer9 OFF  // 스탑워치 처음에 끄기
      ST1=0; // timer9가 한번 돌았기 때문에 0 만들어줌
      while(1)
	{
          switch(SW0_Flag){
          case 1:
            if(i==1){ // i 채널 넘어가면서, 한번만 실행되기 위해서
              ALARM_Setup();}   // 알람시계 화면 구동
            ALARM_Switch();   // 시계 코드
            break;
          case 2:
            if(i==1){// i 채널 넘어가면서, 한번만 실행되기 위해서
              STOP_WATCH_Setup ();}// 스탑 워치 화면구동
             STOP_WATCH (); // 스탑워치 코드
            break;
          
          case 3:
            if(i==1){// i 채널 넘어가면서, 한번만 실행되기 위해서
              Calculator_Setup();}  // 계산기 화면 구동
              //Calculator();     // 계산기 코드, 이부분은 Usart rx 인터럽트로 가있음.
            DelayMS(20);   // 다음 화면 넘어가지 않는것을 방지하기 위해서
            break;
            case 4:
            if(i==1){// i 채널 넘어가면서, 한번만 실행되기 위해서
             Thermostat_Setup();  // 히터 화면 구동
              RCC->APB1ENR |= (1<<1);  // timer 3
            }
            DelayMS(20);// 다음 화면 넘어가지 않는것을 방지하기 위해서
           //Thermostat(); ADC인터럽트에서 해결함
               break;
          
              }
} 
}
void Thermostat(void){
                
}
void Thermostat_Setup(void){
                LCD_Clear(RGB_WHITE);// 화면 지우기
                LCD_DisplayText(0,17,":"); // 어디든 화면에 : 표시
                LCD_SetBackColor(RGB_WHITE);// 배경색
                LCD_SetTextColor(RGB_GREEN);	//글자색
                LCD_DisplayText(0,0,"4.Thermostat");   
                LCD_SetTextColor(RGB_BLACK);//글자색
                LCD_DisplayText(1,0,"T:");
                LCD_DisplayText(2,0,"H:0  C:1");
                i=0; // 플래그 지우기
}
char str[20];// 16진수로 값변환을 위한 배열
void Calculator(void){
       LCD_DisplayChar(1,2,C1);  //C1값 그대로 보여줌
       LCD_DisplayChar(1,6,C2);//C2값 그대로 보여줌
       if(C6==0x3D){//3번째값이 '=' 이어야 작동
       if(C1>0x40&C1<0x60){// 소문자 알파벳일 경우
       C3=C1-0x37;    //알파벳은 0x37을 빼줌
       }
       else if(C1>0x60){  // 대문자 알파벳일 경우
       C3=C1-0x57;//알파벳은 0x37을 빼줌
       }
       else{C3=C1-0x30;}   //숫자는 0x370을 빼줌
       if(C2>0x40&C2<0x60){ // 소문자 알파벳일 경우
       C4=C2-0x37;            //알파벳은 0x37을 빼줌
       }
       else if(C2>0x60){// 대문자 알파벳일 경우
       C4=C2-0x57;//대문자 알파벳은 0x57을 빼줌
       }
       else{C4=C2-0x30;} //숫자는 0x370을 빼줌
       C5=C3+C4;             // 값을 계산             0x 자기 숫자 값 만 남도록 하고 덧셈
      sprintf(str,"%2X",C5);   // 16진수 로 표현
        LCD_DisplayText(1,10,str);
       }
       C6=0x30; // 초기화
}


void Calculator_Setup(void){
  
                LCD_Clear(RGB_WHITE);// 화면 지우기
                LCD_DisplayText(0,17,":"); // 어디든 화면에 : 표시
                LCD_SetBackColor(RGB_WHITE);
                LCD_SetTextColor(RGB_GREEN);	//글자색
                LCD_DisplayText(0,0,"3.Calculator");   
                LCD_SetTextColor(RGB_BLACK);//글자색
                LCD_DisplayText(1,0,"  0 + 0 = 00");
                i=0; // 플래그 지우기
}

void USART1_Init(void){
	// USART1 : TX(PA9)
	RCC->AHB1ENR	|= 0x01;	// RCC_AHB1ENR GPIOA Enable
	GPIOA->MODER	|= 0x00080000;	// GPIOA PIN9 Output Alternate function mode                     // 9번 핀 자리					
	GPIOA->OSPEEDR	|= 0x000C0000;	// GPIOA PIN9 Output speed (100MHz Very High speed)
	GPIOA->OTYPER	|= 0x00000000;	// GPIOA PIN9 Output type push-pull (reset state)
	GPIOA->PUPDR	|= 0x00040000;	// GPIOA PIN9 Pull-up
	GPIOA->AFR[1]	|= 0x70;	// Connect GPIOA pin9 to AF7(USART1)
    
	// USART1 : RX(PA10)
	GPIOA->MODER 	|= 0x200000;	// GPIOA PIN10 Output Alternate function mode
	GPIOA->OSPEEDR	|= 0x00300000;	// GPIOA PIN10 Output speed (100MHz Very High speed)
	GPIOA->AFR[1]	|= 0x700;	//Connect GPIOA pin10 to AF7(USART1)

	RCC->APB2ENR	|= 0x0010;	// RCC_APB2ENR USART1 Enable
    
        //////////////////////////////////////////////////////////////////////////////////////////
        
	USART_BRR_Configuration(9600); // USART Baud rate Configuration   //속도 19200       
    
        /////////////////////////////////////////////////////////////////////////////////////////
        
	USART1->CR1	&= ~USART_CR1_M;	// USART_WordLength 8 Data bit                      //M= 12번 ==(1<<12)           8비트 사용 여부   
	USART1->CR1	&= ~USART_CR1_PCE ;	// USART_Parity_No
	USART1->CR1	|= USART_CR1_RE;	// 0x0004, USART_Mode_RX Enable
	USART1->CR1	|= USART_CR1_TE ;	// 0x0008, USART_Mode_Tx Enable
	USART1->CR2	&= ~USART_CR2_STOP;	// USART_StopBits_1
	USART1->CR3	= 0x0000;	// No HardwareFlowControl, No DMA        // flow contron 안함 -> 잘되는지 실험 1번 하는거임 
    
	USART1->CR1 	|= USART_CR1_RXNEIE;	//  0x0020, RXNE interrupt Enable  // 마스크
       // USART1->CR1 	|= USART_CR1_RXNEIE;	//  0x0020, RXNE interrupt Enable  // 마스크
	NVIC->ISER[1]	|= (1 << (37-32)); 	// Enable Interrupt USART1 (NVIC 37번)
	USART1->CR1 	|= USART_CR1_UE;	//  0x2000, USART1 Enable              작동 : ON
}

void USART1_IRQHandler(void)           // 받으면 인터럽트 됨
{       
  if ( (USART1->SR & USART_SR_RXNE) )            // 데이터가 오면   // flag 지울 필요없이  데이터를 읽으면 flag값이 지워짐
	{       
		char ch;
		ch = (uint16_t)(USART1->DR & (uint16_t)0x01FF);	// 수신된 문자 저장
		SerialPutChar(ch); // 시리얼 통신으로 문자 보냄
                 if(q==0){   // q는 순서 1,2,3, 표시임
                  C1=ch;   // 첫번째 값 저장
                  q++;}
                 else if(q==1){
               C2=ch;// 두번째 값 저장
               q++;}
                 else if(q==2){
                   C6=ch;       // 세번째 값 저장
                   q=0;}
                 Calculator();   // 계산기 코드 실행
	} 
                  
        // DR 을 읽으면 SR.RXNE bit(flag bit)는 clear 된다 

 
}
void SerialPutChar(uint8_t Ch) // 1문자 보내기 함수
{
        while((USART1->SR & USART_SR_TXE) == RESET); //  USART_SR_TXE:0x0080, 송신 가능한 상태까지 대기   TX. DR 비웠는지    flag
        // 비워있으면 while문 빠져 나옴 
	USART1->DR = (Ch & 0x01FF);	// 전송                       //0x01FF 쓰레기값 버린거임
}

void Serial_PutString(char *str) // 여러문자 보내기 함수
{
	while (*str != '\0') // 종결문자가 나오기 전까지 구동, 종결문자가 나온후에도 구동시 메모리 오류 발생가능성 있음.
	{
		SerialPutChar(*str);	// 포인터가 가르키는 곳의 데이터를 송신
		str++; 			// 포인터 수치 증가
	}
}
// Baud rate  
void USART_BRR_Configuration(uint32_t USART_BaudRate)
{ 
	uint32_t tmpreg = 0x00;
	uint32_t APB2clock = 84000000;	//PCLK2_Frequency
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;

	// Determine the integer part 
	if ((USART1->CR1 & USART_CR1_OVER8) != 0) // USART_CR1_OVER8 : 0x8000
        {                                                                  // USART1->CR1.OVER8 = 1 (8 oversampling)
		// Integer part computing in case Oversampling mode is 8 Samples 
		integerdivider = ((25 * APB2clock) / (2 * USART_BaudRate));    
	}
	else  // USART1->CR1.OVER8 = 0 (16 oversampling)
	{	// Integer part computing in case Oversampling mode is 16 Samples 
		integerdivider = ((25 * APB2clock) / (4 * USART_BaudRate));    
	}
	tmpreg = (integerdivider / 100) << 4;
  
	// Determine the fractional part 
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

	// Implement the fractional part in the register 
	if ((USART1->CR1 & USART_CR1_OVER8) != 0)
	{
		tmpreg |= (((fractionaldivider * 8) + 50) / 100) & (0x07);
	}
	else // if ((USARTx->CR1 & USART_CR1_OVER8) == 0) 
	{
		tmpreg |= (((fractionaldivider * 16) + 50) / 100) & (0x0F);
	}

	// Write to USART BRR register
	USART1->BRR = (uint16_t)tmpreg;
}


void ALARM_Switch(void){
  switch(KEY1_Scan())                      //조이스틱 키스캔
		{
			case  NAVI_SW_PUSH : 	//SW0
				AR=1;   
                                 LCD_SetPenColor(RGB_RED);                        
                                 
                                 //     x값은 시간에 따른 값을 계산 한 것이고, 2초에 1더해짐
                                 //     y값은  360/256 =1.40도 -> tan(0.0244) 이다 x만큼 도가 곱해지기 때문에  tan(0x0244*x) 에 x값을 곱해 x*tan(0.0244*x) 이다 
                          ////////////////////////////////////////////////////////////  빨간색 알람 위치 맞추기                    
                          if(AR2<=3)                                                             // 시간에 따른 x,y 값 
                          {xx=AR1/2+(8*(AR2%4));                                          // 시간에 따른 x값 
                          yy=32-(int)(xx*tan(0.0244*xx));                                   // 시간에 따른 y값
                          tt=1;LCD_DrawLine(120,80, 120+xx, 80-yy); }                // 제 1사분면
                          if(4<=AR2&&AR2<=7)                                                  // 시간에 따른 x,y 값 
                          {xx=AR1/2+(8*(AR2%4));                                           // 시간에 따른 x값
                          yy=32-(int)(xx*tan(0.0244*xx));                                    // 시간에 따른 y값
                          tt=2;LCD_DrawLine(120,80, 120+yy, 80+xx);}                // 제 4사분면
                         if(8<=AR2&&AR2<=11)                                              // 시간에 따른 x,y 값 
                         {xx=AR1/2+(8*(AR2%4));                                           // 시간에 따른 x값
                          yy=32-(int)(xx*tan(0.0244*xx));                                    // 시간에 따른 y값
                          tt=3; LCD_DrawLine(120,80, 120-xx, 80+yy);}                // 제 3사분면
                          if(12<=AR2&&AR2<=15)                                              // 시간에 따른 x,y 값 
                          {xx=AR1/2+(8*(AR2%4));                                           // 시간에 따른 x값
                          yy=32-(int)(xx*tan(0.0244*xx));                                    // 시간에 따른 y값
                          tt=4; LCD_DrawLine(120,80, 120-yy, 80-xx); }                  // 제 2사분면
                         // AR=0;
 			break;

			case  NAVI_SW_UP: 	//SW1
				AR2++; 
			break;

                        case  NAVI_SW_DOWN  : 	//SW2
				if(AR2==0){                // 0-> F 되도록
                                AR2=16;
                                }
                                AR2--;          
			break;

                        case  NAVI_SW_RIGHT  : 	//SW3
				AR1++;   
			break;
                        
                        case  NAVI_SW_LEFT  : 	//SW3
			 if(AR1==0){                // 0-> F 되도록
                                AR1=16;
                                }	
                          AR1--;         
			break;
                }   
  if(AR1<=9){                                                 // 시간 10진수를 16진수로 만드는 코드  AR1은 분,  AR2은 시
               LCD_DisplayChar(1,8,AR1+0x30);}
               else if(AR1>9 && AR1<=15){
                 LCD_DisplayChar(1,8,AR1+0x37); }
               else if(AR1>15){
               AR1=0;
               AR2++;
               LCD_DisplayChar(1,8,AR1+0x30);
               }
               
               if(AR2<=9){
               LCD_DisplayChar(1,6,AR2+0x30);}
               else if(AR2>9 && AR2<=15){
                 LCD_DisplayChar(1,6,AR2+0x37); }
               else if(AR2>15){
               AR1=0;
               AR2=0;
               }
          
}
void ALARM_Setup(void){
  LCD_Clear(RGB_WHITE);// 화면 지우기
  LCD_SetBackColor(RGB_WHITE);
  LCD_DisplayText(0,17,":"); // 어디든 화면에 : 표시
  i=0; // 한번만 하기위한 플래그
  LCD_SetBrushColor(RGB_BLUE);
  y=32-(int)(x*tan(0.0244*x));                      // 시간에따른 x값의 변화에 의한 y 값 (위에서 설명)
  LCD_DrawLine(120,80, 120+x, 80-y); 
  ///////////////////  시계 틀
             LCD_SetBrushColor(RGB_YELLOW);                    // 사각형 만들기
             LCD_DrawFillRect(87,45,68,68); 
             LCD_SetPenColor(RGB_GREEN);                        // 테두리 만들기
             LCD_DrawRectangle(87,45,68,68);

                LCD_SetPenColor(RGB_RED);                         //알람 빨간색 부분 만들기
                          xx=(AR1)+1;  
                          yy=32-(int)(xx*tan(0.0244*xx));
                          if(AR2&&AR2<=3)
                          { tt=1;LCD_DrawLine(120,80, 120+xx, 80-yy); }    //제 1사분면
                          if(4<=AR2&&AR2<=7)
                          {tt=2;LCD_DrawLine(120,80, 120+yy, 80+xx);}    //제 4사분면
                         if(8<=AR2&&AR2<=11)
                         {tt=3; LCD_DrawLine(120,80, 120-xx, 80+yy);}    //제 3사분면
                          if(12<=AR2&&AR2<=15)
                          {tt=4; LCD_DrawLine(120,80, 120-yy, 80-xx); }    //제 2사분면          
		
                LCD_SetBackColor(RGB_WHITE);      // 글자 배경색
                LCD_SetTextColor(RGB_GREEN);	//글자색
                LCD_DisplayText(0,0,"1.ALARM");
                LCD_SetTextColor(RGB_BLACK);//글자색
                LCD_DisplayText(1,0,"ALARM  :");
                 if(AR1<=9){                                       // 분침이 9와 같거나 작으면
               LCD_DisplayChar(1,8,AR1+0x30);}            // 숫자일때, 분침 표현
               else if(AR1>9 && AR1<=15){
                 LCD_DisplayChar(1,8,AR1+0x37); }            // 알파벳일때, 분침 표현
               else if(AR1>15){                                  // 분침이 F를 넘기면 시침으로 +1됨
               AR1=0;
               AR2++;
               LCD_DisplayChar(1,8,AR1+0x30);       // 분침 표현
               }
               
               if(AR2<=9){                                          // 시침이 9와 같거나 작으면
               LCD_DisplayChar(1,6,AR2+0x30);}           // 숫자일때, 분침 표현
               else if(AR2>9 && AR2<=15){
                 LCD_DisplayChar(1,6,AR2+0x37); }           // 알파벳일때, 분침 표현
               else if(AR2>15){                                 // 시침이 F를 넘기면 시침으로 +1됨
               AR1=0;
               AR2=0;
               }
               if(AR==1){
               }
}
void STOP_WATCH_Setup (void){
                LCD_Clear(RGB_WHITE);// 화면 지우기
                LCD_DisplayText(0,17,":"); // 어디든 화면에 : 표시
                LCD_SetBackColor(RGB_WHITE);
                LCD_SetTextColor(RGB_GREEN);	//글자색
                LCD_DisplayText(0,0,"2.Stop Watch");   
                LCD_SetTextColor(RGB_BLACK);//글자색
                LCD_DisplayText(1,0,"S-W   :");
                LCD_DisplayText(2,0,"Record   :");
                i=0; // 플래그 지우기
}
void STOP_WATCH (void){                               // 2번 화면 설정    
               if(ST1<10){ 
                LCD_DisplayChar(1,7,ST1+0x30);}
               else if(ST1>9 && ST1<=15){
               LCD_DisplayChar(1,7,ST1+0x37);
               }else if(ST1>15){
               ST1=0;
               LCD_DisplayChar(1,7,ST1+0x30);
               ST2++; }
                  //////////////////////////////////////////////////////////////////////분 시간 넘어가도록  
               if(ST2<10){
                 LCD_DisplayChar(1,5,ST2+0x30);}
               else if(ST2>9 && ST2<=15){
              LCD_DisplayChar(1,5,ST2+0x37);}
              else if(ST2>15){
               ST2=0;
               LCD_DisplayChar(1,5,ST2+0x30);
               ST3++;}
                 //////////////////////////////////////////////////////////////////////시 시간 넘어가도록  
               if(ST3<10){
                 LCD_DisplayChar(1,4,ST3+0x30);}
               else if(ST3>9 && ST3<=15){
               LCD_DisplayChar(1,4,ST3+0x37);}
              else if(ST3>15){
               ST1=0;
               ST2=0;
               ST3=0;
               LCD_DisplayChar(1,7,ST1+0x30);
              LCD_DisplayChar(1,5,ST2+0x30);
              LCD_DisplayChar(1,4,ST3+0x30);}
              
  if(WT1<10){
   LCD_DisplayChar(2,10,WT1+0x30);
  }
  else if (WT1>9){
  LCD_DisplayChar(2,10,WT1+0x37);
  }
    if(WT2<10){
   LCD_DisplayChar(2,8,WT2+0x30);
  }
  else if (WT2>9){
  LCD_DisplayChar(2,8,WT2+0x37);
  }  if(WT3<10){
   LCD_DisplayChar(2,7,WT3+0x30);
  }
  else if (WT3>9){
  LCD_DisplayChar(2,7,WT3+0x37);
  }
  
              
}
void TIME7_time(void){  
  // 화살표 함수
 //////////////////////////////////////////////////////////////////////초침 만들기   
  
 //////////////////////////////////////긱긱을 t로 1,2,3,4, 를 표현하여 4개의 분면을 표시하고, t3
               if(t==1&t3==1){
                 if(x==0){                                     // 맨 마지막 부분 지우기
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31))); // y값은 알람부분에 표현
               LCD_DrawLine(120,80, 120-y, 80-31);
                 }
               LCD_SetPenColor(RGB_YELLOW);       // 바로 직전 부분 지우기
             y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120+(x-1), 80-y);  
               
            LCD_SetPenColor(RGB_BLUE);               // 화살표 그리기
             y=32-(int)(x*tan(0.0244*x)); //     y값은  360/256 =1.40도 -> tan(0.0244) 이다 x만큼 도가 곱해지기 때문에  tan(0x0244*x) 에 x값을 곱해 x*tan(0.0244*x) 이다 
           LCD_DrawLine(120,80, 120+x, 80-y/1);  
            }
         
            
           if(t==2&t3==1){
             if(x==0){                                     // 맨 마지막 부분 지우기
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31)));
               LCD_DrawLine(120,80, 120+31, 80-y/1);  
                 }
              LCD_SetPenColor(RGB_YELLOW);       // 바로 직전 부분 지우기
             y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120+y, 80+(x-1)); //제 4사 분면
               
             
            LCD_SetPenColor(RGB_BLUE);               // 화살표 그리기
          y=32-(int)(x*tan(0.0244*x)); //     y값은  360/256 =1.40도 -> tan(0.0244) 이다 x만큼 도가 곱해지기 때문에  tan(0x0244*x) 에 x값을 곱해 x*tan(0.0244*x) 이다 
           LCD_DrawLine(120,80, 120+y, 80+x); //제 4사 분면
            }

            if(t==3&t3==1){
                if(x==0){                                    // 맨 마지막 부분 지우기
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31)));
               LCD_DrawLine(120,80, 120+y, 80+31);
                 }
              
               LCD_SetPenColor(RGB_YELLOW);       // 바로 직전 부분 지우기
             y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120-(x-1), 80+y);  //제 3사 분면
               
            LCD_SetPenColor(RGB_BLUE);               // 화살표 그리기
           y=32-(int)(x*tan(0.0244*x)); //     y값은  360/256 =1.40도 -> tan(0.0244) 이다 x만큼 도가 곱해지기 때문에  tan(0x0244*x) 에 x값을 곱해 x*tan(0.0244*x) 이다 
           LCD_DrawLine(120,80, 120-x, 80+y);  //제 3사 분면
            }
            if(t==4&t3==1){
              if(x==0){                                     // 맨 마지막 부분 지우기
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31)));
                LCD_DrawLine(120,80, 120-31, 80+y);
                 }
                 LCD_SetPenColor(RGB_YELLOW);;
              y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120-y, 80-(x-1));  // 제 2사 분면
               
            LCD_SetPenColor(RGB_BLUE);               // 화살표 그리기
          y=32-(int)(x*tan(0.0244*x));
         LCD_DrawLine(120,80, 120-y, 80-x);  // 제 2사 분면
            }


}

void _ADC_Init(void)  //시작을 타이머 200ms으로
{   
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	// 0x00000001  // ENABLE GPIOA CLK
	GPIOA->MODER |= GPIO_MODER_MODER1;       // 0x0000000C	// CONFIG GPIOA PIN1(PA1) TO ANALOG IN MODE
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	// 0x00000100  // ENABLE ADC1 CLK

        NVIC->ISER[0] = (1<<18);  //   ADC 인터럽트          
        
	ADC->CCR &= ~0X0000001F;	// MULTI[4:0]: ADC_Mode_Independent
	ADC->CCR |= 0x00010000;		// ADCPRE: ADC_Prescaler_Div4 (ADC MAX Clock 36MHz, 84Mhz(APB2)/4 = 21MHz)
	ADC->CCR &= ~0x0000C000;	// DMA: Disable

	ADC1->CR1 &= ~(3<<24);		// RES[1:0]: 12bit Resolution
	ADC1->CR1 &= ~(1<<8);		// SCAN: ADC_ScanCovMode Disable
      
        ADC1->CR1 |= (1<<5);                // 인터럽트 마스크
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
	ADC1->CR2 &= ~(1<<1);		// CONT: ADC_ContinuousConvMode Disable

        ADC1->CR2 |= (1<<28);   // EXTEN RISING EDGE
        ADC1->CR2 &= ~(1<<29);   // EXTEN RISING EDGE          01

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
     //   ADC1->CR2 &=~(3<<24);  // TIMER3 CC1 Event      
        ADC1->CR2 |=(7<<24);  // TIMER3 CC1 Event                     0111
	///////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        ADC1->CR2 &= ~(1<<11);		// ALIGN: ADC_DataAlign_Right
	ADC1->CR2 |= 1<<10;		// EOCS: The EOC bit is set at the end of each regular conversion
	ADC1->CR2 &=~(1<<1);       // Continuous conversion mode disable
        ADC1->CR2 |= 1<<0;		// ADON: ADC ON

	ADC1->SQR1 &= ~0x00F00000;	// L[3:0]: ADC Regular channel sequece length = 1 conversion
        //
     ADC1->SMPR2	|= 0x00 << (3*1);	// ADC1_CH1 Sample TIme_480Cycles (3*Channel_1)
	//
        //Channel selection, The Conversion Sequence of PIN1(ADC1_CH1) is first, Config sequence Range is possible from 0 to 17
	ADC1->SQR3 |= 1<<0;             //첫 순위 1번 채널 PA1
     //   NVIC->ISER[0] |= (1<<18);	// Enable ADC global Interrupt
        
}
UINT16 n=0;
void ADC_IRQHandler(void)
{
            ADC1->SR &= ~(1<<1);	// EOC flag clear
            
             ADC_Value = ADC1->DR;   // Read ADC result value from ADC Data Reg(ADC1->DR) 
        
        	sprintf(str,"%4d",ADC_Value);
     
               
               Voltage = ADC_Value * (3.3 * 100) / 4095;   // 5: 4095 =  Volatge : ADC_Value 
                                                                // 100:  소수점아래 두자리까지 표시하기 위한 값  

             static UINT16 Cnt = 0;
               Vo=ADC_Value*50/4095-10;
                
               // 각 온도에 맞는 히터 및, 쿨러 표현
               if(Vo<1){                                          // C=2
                  TIM4->CCR1 =2000;                       // duty = 90%
                  LCD_DisplayText(2,0,"H:0  C:2");
               GPIOG->ODR =0x01; }// LED1 ON
               else if(Vo>0&Vo<11){                          // C=1
                  TIM4->CCR1 =18000;                       // duty = 10%
                  LCD_DisplayText(2,0,"H:0  C:1");
                GPIOG->ODR =0x01; }// LED1 ON
               else if(Vo>10&Vo<21){                         // C=0, H=0
                 LCD_DisplayText(2,0,"H:0  C:0");}         // duty = 50%
               else if(Vo>20&Vo<31){                        // H=1
                  TIM4->CCR1 =2000;                        // duty = 10%
                  LCD_DisplayText(2,0,"H:1  C:0");
                GPIOG->ODR =0x02;} // LED2 ON
               else if(Vo>30&Vo<41){                          // H=2
                   TIM4->CCR1 =18000;                       // duty = 90%
                   LCD_DisplayText(2,0,"H:2  C:0"); 
                 GPIOG->ODR =0x02; }  // LED2 ON
               if(Vo<0){                                      // 온도가 0보다 낮아진다면,
                n=1;
                 Vo=(-1)*Vo;                               // 값은 -로
                 LCD_DisplayText(1,2,"-");
                 
                 for(  Cnt=0; Cnt<4; Cnt++){
                   LCD_SetPenColor(RGB_WHITE);         //하얀색 주고 초기화
                  LCD_DrawHorLine(50, 25+Cnt, 60);

                  
                  LCD_SetPenColor(RGB_BLUE);         //파란색 주고 값 표현
                  LCD_DrawHorLine(50, 25+Cnt, 10- Vo);

             
}

               }
               else{LCD_DisplayText(1,2," ");           //- 표현 없애기
               n=0;
                for(  Cnt=0; Cnt<4; Cnt++){
                   LCD_SetPenColor(RGB_WHITE);         //하얀색 주고
                   LCD_DrawHorLine(50, 25+Cnt, 60);     // 초기화

                  if(Vo<11)
                  LCD_SetPenColor(RGB_BLUE);         //파란색 주고
                  else if(Vo>10&Vo<21)
                    LCD_SetPenColor(RGB_GREEN);    //초록색 주고
                  else if(Vo>20&Vo<31)
                  LCD_SetPenColor(RGB_RED);    //빨간색 주고
                  else if(Vo>30&Vo<41)
                  LCD_SetPenColor(RGB_RED);    //빨간색 주고
                  
                  // 그래프 표현
                  LCD_DrawHorLine(50, 25+Cnt, 10+Vo);
}
               
               }
               
               
               LCD_DisplayChar(1,3,(Vo/10)+0x30);             //  가변저항에 따른 온도 표현
               LCD_DisplayChar(1,4,(Vo%10/1) +0x30);       //  가변저항에 따른 온도 표현
               
                
                     
}
void TIME4_Init(void)
{   
	RCC->AHB1ENR	|= 0x00000002;	// RCC_AHB1ENR GPIOB Enable
	RCC->APB1ENR 	|= 0x00000004;	// RCC_APB1ENR TIMER4 Enable 
    						
	// PB6을 출력설정하고 Alternate function(TIM4_CH3)으로 사용 선언
	GPIOB->MODER 	|= 0x00002000;	// GPIOB PIN6 Output Alternate function mode					
	GPIOB->OSPEEDR 	|= 0x00003000;	// GPIOB PIN8 Output speed (100MHz High speed)
	GPIOB->OTYPER	= 0x00000000;	// GPIOB PIN6 Output type push-pull (reset state)
	GPIOB->PUPDR	|= 0x00001000;	// GPIOB PIN6 Pull-up
    
	GPIOB->AFR[0]	|= 0x02000000;	// AFRH: Connect TIM4 pins(PB8) to AF2(TIM3..5)
					// PB6 ==> TIM4_CH1
    
	// TIM4 Channel 3 : PWM 1 mode
	TIM4->PSC	= 8400-1;	// Prescaler 84,000,000Hz/8400 = 10,000 Hz(0.1ms)  (1~65536)
	TIM4->ARR	= 20000-1;	// Auto reload  (0.1ms * 20,000 = 2s : PWM Period)
	TIM4->CR1 	&= ~(1<<4);	// DIR: Countermode = Upcounter (reset state)
	TIM4->CR1 	&= ~(3<<8);	// CKD: Clock division = 1 (reset state)
	TIM4->CR1 	&= ~(3<<5); 	// CMS(Center-aligned mode Sel): No(reset state)
    
	TIM4->CCER	|= (1<<0);	// CC1E: OC3 Active(Capture/Compare 3 output enable)
	TIM4->CCER	&= ~(1<<1);	// CC1P: Capture/Compare 1 output Polarity High
	TIM4->CCR1	= 10000;		// CCR1 value

	TIM4->CCMR1 	&= ~(3<<0); 	// CC1S(CC1 channel): Output 
	TIM4->CCMR1	&= ~(1<<3); 	// OC1PE: Output Compare 1 preload disable
	TIM4->CCMR1	|= (6<<4);	// OC1M: Output compare 31mode: PWM 1 mode
	TIM4->CCMR1	|= (1<<7);	// OC1CE: Output compare 1 Clear enable

	TIM4->CR1	|= (1<<7);	// ARPE: Auto-reload preload enable
	TIM4->CR1	|= (1<<0);	// CEN: Counter TIM4 enable
}
void TIME7_Init(void)  //타이머 200ms 마다 adc값을 읽도록 한다. 그 값은 가변저항 값이다.
{
  RCC->APB1ENR |= (1<<5);   //timer7 ON
  NVIC->ISER[1] |= (1<<(55-32)); // tim7 global interupt
  TIM7->PSC =  8400-1;  // 84,000,000 / 8400 =10000Hz = 0.1ms
  TIM7->ARR =  10000-1;    // 0.1msec * 1000 =1s  // 그러나 토글이기 때문에 1/2 해준것이다. 그래서 주기는 200ms
 
  TIM7->CR1 &= ~(1<<4);      // upcount
  TIM7->CR1 &= ~(3<<5);      // Edge-aligned mode
  TIM7->CR1 &=~ (1<<7);  // buffer 넣음   ========잘모르겠음
  TIM7->CR1 &=~(3<<8);       // clock division =1
  
   TIM7->EGR |=(1<<0);    //update
   TIM7->DIER |=(1<<0);   //update
 
  TIM7->CR1 |= (1<<0);  //enable
}
void TIM7_IRQHandler(void)
{
  if((TIM7->SR & 0x01) != RESET)	// Capture/compare 1 interrupt flag
	{
		TIM7->SR &= ~0x01;	// Capture Compare1 Interrupt Claer
               //////////////////////////////////////////////////////////////////////초 시간 넘어가도록 
                ////// 분침이 10진수를 16진수로 변환하는 과정////////////////////////////////
               LCD_SetBackColor(RGB_WHITE);	//글자배경색
               if(t1<10){                                    // 분이 10보다 작으면
               LCD_SetTextColor(RGB_BLUE);
               LCD_DisplayChar(0,18,t1+0x30);} // 숫자는 0x30 더함
               else if(t1>9 && t1<=15){           // 분이 9보다 크고,15와 같거나 작으면
                 LCD_SetTextColor(RGB_BLUE);
                 LCD_DisplayChar(0,18,t1+0x37); }// 알파벳은 0x37 더함
               else if(t1>15){
               t1=0;          
               t2++;                                      //분침 0으로 초기화 시침을 +1 추가
               LCD_SetTextColor(RGB_BLUE);
               LCD_DisplayChar(0,18,t1+0x30);
               }
                 ////// 시침이 10진수를 16진수로 변환하는 과정////////////////////////////////
               if(t2<10){                                    // 시침이 10보다 작으면
                 LCD_SetTextColor(RGB_BLACK);
               LCD_DisplayChar(0,16,t2+0x30);}
               else if(t2>9 && t2<=15){              // 시침이 9보다 크고,15와 같거나 작으면
                 LCD_SetTextColor(RGB_BLACK);
                 LCD_DisplayChar(0,16,t2+0x37); }
               else if(t2>15){                                 // 시침이 10보다 작으면
               t1=0;
               t2=0;                                           // 초기화
               LCD_SetTextColor(RGB_BLUE);
               LCD_DisplayChar(0,18,t1+0x30);
               LCD_SetTextColor(RGB_BLACK);
               LCD_DisplayChar(0,16,t2+0x30);
               }
               
                  if(AR==1&AR1==(t1)&AR2==t2){  // 알람 부분;
              BEEP();  DelayMS(30);  BEEP();  DelayMS(30);  BEEP(); AR=0;   }
                
              t1++;  // 초 증가
              
               if(SW0_Flag==1){   
                 TIME7_time();}    // 화살표 함수
          if(x==32){ x=0;  t++;} // 시간에 따른 화살표 움직임
          if(t>4){ t=1; }          // 시간에 따른 화살표 움직임
           t3^=1;   x1^=1;       // 시간에 따른 화살표 움직임
         if(x1==0){ x++; }       // 시간에 따른 화살표 움직임
  }	
}
void TIME9_Init(void){
  RCC->APB2ENR |= (1<<16);   //timer9 ON
  NVIC->ISER[0] |= (1<<(24)); // tim9 global interupt
  TIM9->PSC =  16800-1;  // 168,000,000 / 16800 =10000Hz = 0.1ms
  TIM9->ARR =  1000-1;    // 0.1msec * 500 =0.05s  // 그러나 토글이기 때문에 1/2 해준것이다. 그래서 주기는 0.05s
 
  TIM9->CR1 &= ~(1<<4);      // upcount
  TIM9->CR1 &= ~(3<<5);      // Edge-aligned mode
  TIM9->CR1 &=~ (1<<7);  // buffer 넣음   ========잘모르겠음
  TIM9->CR1 &=~(3<<8);       // clock division =1
           
  TIM9->CCER |= (1<<0);  //CC enable
  TIM9->CCER &=~(1<<1);  // 비반전
  TIM9->CCMR1 &= ~(3<<0); // CC1S(CC1 channel): Output 
  TIM9->CCMR1 &= ~(1<<3); // OC1PE: Output Compare 1 preload disable
  TIM9->CCMR1 |= (3<<4);	// OC1M: Output Compare 1 Mode : toggle
        
   TIM9->EGR |=(1<<1);    //update
   TIM9->DIER |=(1<<1);   //update
 
   TIM9->CR1 |= (1<<0);  //enable
}
void TIM1_BRK_TIM9_IRQHandler(void){
  if((TIM9->SR &&0x02)!=0){
  TIM9->SR &=~0x02;
  ST1++;// 스탑워치 0.1초씩 증가
  }
  
}
void TIME3_Init(void)
{
      RCC->APB1ENR |= (1<<1);  // timer 3
      NVIC->ISER[0]|= (1<<(29));

          TIM3->PSC = 8400-1;  // 84,000,000 / 8400 =10,000Hz = 0.1ms
          TIM3->ARR =  2500-1;    // 0.1msec * 5000=500ms   
          TIM3->CCR1 =1250;  // 반  =250Hz
  
  
          TIM3->CR1 &= ~(1<<4);      // upcount
          TIM3->CR1 &= ~(3<<5);      // Edge-aligned mode
          TIM3->CR1 &=~ (1<<7);  // buffer 넣음   ========잘모르겠음
          TIM3->CR1 &=~(3<<8);       // clock division =1
  
          TIM3->CCER |= (1<<0);  //CC enable
          TIM3->CCER &=~(1<<1);  // 비반전
          
          TIM3->CCMR1 &= ~(3<<0); // CC1S(CC1 channel): Output 
          TIM3->CCMR1 &= ~(1<<3); // OC1PE: Output Compare 1 preload disable
          TIM3->CCMR1 |= (3<<4);	// OC1M: Output Compare 1 Mode : toggle
        
          TIM3->EGR |=(1<<1);    //CC1 이벤트
          TIM3->DIER |=(1<<1);   //mask cc1
          TIM3->CR1 |= (1<<0);  //enable
}
void TIM3_IRQHandler(void)
{  
  if((TIM3->SR & 0x02) != RESET)	// Capture/compare 1 interrupt flag
	{
		TIM3->SR &= ~0x02;	// Capture Compare1 Interrupt Claer
                 
                
        }
                   
}
void _GPIO_Init(void)
{
	// LED GPIO(PORT G) 설정
    	RCC->AHB1ENR    |= 0x00000040; 	// RCC_AHB1ENR(bit8~0) GPIOG(bit#6) Enable							
	GPIOG->MODER    = 0x00005555;	// GPIOG PIN0~PIN7 Output mode (0b01)						
	GPIOG->OTYPER   = 0x0000;	// GPIOG PIN0~PIN7 : Push-pull  (PIN8~PIN15) (reset state)	
 	GPIOG->OSPEEDR  = 0x00005555;	// GPIOG PIN0~PIN7 Output speed (25MHZ Medium speed) 
    
	// SW GPIO(PORT H) 설정 
	RCC->AHB1ENR    |= 0x00000080;	// RCC_AHB1ENR(bit8~0) GPIOH(bit#7) Enable							
	GPIOH->MODER    = 0x00000000;	// GPIOH PIN8~PIN15 Input mode (reset state)				
	GPIOH->PUPDR    = 0x00000000;	// GPIOH PIN8~PIN15 Floating input (No Pull-up, pull-down) (reset state)

	//Buzzer GPIO(PORT F) 설정 
    	RCC->AHB1ENR    |= 0x00000020; 	// RCC_AHB1ENR(bit8~0) GPIOF(bit#5) Enable							
	GPIOF->MODER    |= 0x00040000;	// GPIOF PIN9 Output mode (0b01)						
	GPIOF->OTYPER   &= 0xFDFF;	// GPIOF PIN9 : Push-pull  	
 	GPIOF->OSPEEDR  |= 0x00040000;	// GPIOF PIN9 Output speed (25MHZ Medium speed) 
	
	//NAVI_SW(PORT I) 설정
	RCC->AHB1ENR 	|= 0x00000100;	// RCC_AHB1ENR GPIOI Enable
	GPIOI->MODER 	= 0x00000000;	// GPIOI PIN8~PIN15 Input mode (reset state)
	GPIOI->PUPDR    = 0x00000000;	// GPIOI PIN8~PIN15 Floating input (No Pull-up, pull-down) (reset state)
	
	// DC_MOTOR DIR
	RCC->AHB1ENR	|= 0x00000002; 	// RCC_AHB1ENR GPIOB Enable							
	GPIOB->MODER 	= 0x00040000;	// GPIOB PIN9 Output mode						
	GPIOB->OSPEEDR 	= 0x00040000;	// GPIOB PIN9 Output speed (25MHZ Medium speed) 
}	

void _EXTI_Init(void)
{
    	RCC->AHB1ENR 	|= 0x80;	// RCC_AHB1ENR GPIOH Enable
	RCC->APB2ENR 	|= 0x4000;	//Enable System Configuration Controller Clock
	
	GPIOH->MODER 	&=  0x00FF;	// GPIOH PIN8~PIN15 Input mode (reset state)				 
	
	SYSCFG->EXTICR[ 2] |= 0x7777; 	// EXTI  8~11에 대한 소스 입력은 GPIOH로 설정 (EXTICR2) (reset value: 0x0000)	
	SYSCFG->EXTICR[ 3] |= 0x7777; 	// EXTI 12~15에 대한 소스 입력은 GPIOH로 설정 (EXTICR3) (reset value: 0x0000)	
        
	EXTI->FTSR |= 0x00FF00;		// Falling Trigger Enable  (EXTI8~15:PH8~15)
	//EXTI->RTSR |= 0x000200;		// Rising Trigger  Enable  (EXTI9:PH9) 
    	EXTI->IMR |= 0x00FF00;  	// EXTI8~15인터럽트 mask (Interrupt Enable)
		
	NVIC->ISER[0] |= ( 1 << (23) );      // Enable Interrupt EXTI5_9 Vector table Position 참조
        NVIC->ISER[1] |= ( 1 << (40-32) ); // Enable Interrupt EXTI10_15 Vector table Position 참조
}
void EXTI9_5_IRQHandler(void)		// EXTI 5~9 인터럽트 핸들러
{
	if(EXTI->PR & 0x0100) 		// EXTI9 nterrupt Pending?
	{
		EXTI->PR |= 0x0100; 	// Pending bit Clear
               SW0_Flag++;   // SW0 플래그
               if(SW0_Flag==5){   // 메뉴 4번 끝나면 다시 1번
               SW0_Flag=1;
               RCC->APB1ENR &= ~(1<<1);  // timer 3 off
               }               BEEP();// 소리 1번
               i=1;
	}
        
	
}
void EXTI15_10_IRQHandler(void)		// EXTI 15~10 인터럽트 핸들러
{
	if(EXTI->PR & 0x1000) 		// EXTI12 nterrupt Pending?
	{
		EXTI->PR |= 0x1000; 	// Pending bit Clear
                GPIOG->ODR |= 0x10;
                RCC->APB2ENR |= (1<<16);   //timer9 ON
	}
	if(EXTI->PR & 0x2000) 		// EXTI13 nterrupt Pending?
	{
		EXTI->PR |= 0x2000; 	// Pending bit Clear
                GPIOG->ODR |= 0x20;
                WT1=ST1; // 스탑워치 기록값1
                WT2=ST2; // 스탑워치 기록값2
                WT3=ST3; // 스탑워치 기록값3
        }
        if(EXTI->PR & 0x4000) 		// EXTI14 nterrupt Pending?
	{
		EXTI->PR |= 0x4000; 	// Pending bit Clear
                GPIOG->ODR &= ~0x30;
                GPIOG->ODR |= 0x40;
                 RCC->APB2ENR &= ~(1<<16);   //timer9 ON
        }
	 if(EXTI->PR & 0x8000) 		// EXTI15 nterrupt Pending?
	{
		EXTI->PR |= 0x8000; 	// Pending bit Clear
                GPIOG->ODR &= ~0x30;
                GPIOG->ODR &= ~0x40;
                 RCC->APB2ENR &= ~(1<<16);   //timer9 ON
                 WT1=0;                     // 기록값 , 스탑워치 초기화
                   ST1=0;
                WT2=0;
                  ST2=0;
                WT3=0;
                  ST3=0;
        }
}

void BEEP(void)			// Beep for 20 ms 
{ 	
        GPIOF->ODR |= (1<<9);	// PF9 'H' Buzzer on
	DelayMS(20);		// Delay 20 ms
	GPIOF->ODR &= ~(1<<9);	// PF9 'L' Buzzer off
}

void DelayMS(unsigned short wMS)
{
	register unsigned short i;
	for (i=0; i<wMS; i++)
		DelayUS(1000);   // 1000us => 1ms
}

void DelayUS(unsigned short wUS)
{
	volatile int Dly = (int)wUS*17;
		for(; Dly; Dly--);
}

void DisplayTitle(void)
{
	LCD_Clear(RGB_WHITE);
	LCD_SetFont(&Gulim10);		//폰트 
	LCD_SetBackColor(RGB_WHITE);	//글자배경색
	LCD_SetTextColor(RGB_BLACK);	//글자색
        LCD_DisplayText(0,17,":");
      //	LCD_DisplayText(1,0,"Temp:00.0C");
      //  LCD_DisplayText(2,0,"Freq:000Hz");
	LCD_SetBackColor(RGB_YELLOW);	//글자배경색
}

uint8_t key_flag = 0;
uint8_t key1_flag = 0;
uint16_t KEY_Scan(void)	// input key SW0 - SW7 
{ 
	uint16_t key;
	key = GPIOH->IDR & 0xFF00;	// any key pressed ?
	if(key == 0xFF00)		// if no key, check key off
	{  	if(key_flag == 0)
        		return key;
      		else
		{	DelayMS(10);
        		key_flag = 0;
        		return key;
        	}
    	}
  	else				// if key input, check continuous key
	{	if(key_flag != 0)	// if continuous key, treat as no key input
        		return 0xFF00;
      		else			// if new key,delay for debounce
		{	key_flag = 1;
			DelayMS(10);
 			return key;
        	}
	}
        
      
}
uint16_t KEY1_Scan(void){
  uint16_t key1;
	key1 = GPIOI->IDR & 0x0FF0;	// any key pressed ?
	if(key1 == 0x0FF0)		// if no key, check key off
	{  	if(key1_flag == 0)
        		return key1;
      		else
		{	DelayMS(10);
        		key1_flag = 0;
        		return key1;
        	}
    	}
  	else				// if key input, check continuous key
	{	if(key1_flag != 0)	// if continuous key, treat as no key input
        		return 0x0FF0;
      		else			// if new key,delay for debounce
		{	key1_flag = 1;
			DelayMS(10);
 			return key1;
        	}
	}}
/******************************************************************************/
/*     RCC Set up                                                             */
/******************************************************************************/
void _RCC_Init(void)
{
    // PLL (clocked by HSE) used as System clock source                    

    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    // Enable HSE : 외부 입력 클락주파수: 5MHz(회로도 참조)
    RCC->CR |= 0x00010000;	// Set bit#16 of CR
 
    // Wait till HSE is ready and if Time out is reached exit 
    do
    {
	HSEStatus = RCC->CR & 0x00020000;	// CHECK bit#17 of CR (HSE RDY?) 
	StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & 0x00020000) != RESET)	// CHECK bit#17 of CR (HSE RDY?) // RESET is 0
    {
	HSEStatus = 0x01;	// HSE is Ready!
    }
    else
    {
	HSEStatus = 0x00;	// HSE is NOT Ready!
    }

    if (HSEStatus == 0x01)	// HSE clock Enable
    {
	// HCLK = SYSCLK / 1 (HCLK = 168MHz)
	RCC->CFGR |= 0x00000000;
 
	// PCLK2 = HCLK / 2  (PCLK2 = 84MHz)
	RCC->CFGR |= 0x00008000;	// PPRE2: APB(Advanced Peripheral Bus)(APB2) High-speed prescaler
					// 100: AHB clock divided by 2 

	// PCLK1 = HCLK / 4  (PCLK1 = 42MHz)
	RCC->CFGR |= 0x00001400;	// PPRE1: APB(Advanced Peripheral Bus)(APB1) Low-speed prescaler
					// 101: AHB clock divided by 4 

    	// Configure the main PLL 
	// Reset vlaue: 0x2400 3010 (PPLQ:4, PLLSR:0, PLL_M:16, PLL_N:192, PLL_P: 2(00))
        RCC->PLLCFGR = 0;
	RCC->PLLCFGR |= 8;		// PLL_M(6bits): 8(0b001000): /8
	RCC->PLLCFGR |= (336 << 6);	// PLL_N(9bits): 336 : *336
	RCC->PLLCFGR |= (0<<16);	// PLL_P(2bits): (2 >> 1) -1=0b00 : 2 : /2 
	RCC->PLLCFGR |= 0x00400000; 	// PLL_SR(1bit): 1 : HSE oscillator clock selected as PLL and PLLI2S clock
//      RCC->PLLCFGR = 0x24405408;
	// SYSCLK 계산 (HSE 입력클럭주파수: 8MHz)
	// SYSCLK = 8M * 336(N) /8(M) /2(P) = 168MHz	
    
	// Enable the main PLL 
	RCC->CR |= 0x01000000;	// Set bit#24 of CR : PLL ON

	// Wait till the main PLL is ready 
	while((RCC->CR & 0x02000000) == 0)	// CHECK bit#25 : PLL RDY?  
	{}
   
	// Configure Flash prefetch, Instruction cache, Data cache and wait state 
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	// Select the main PLL as system clock source 
	// Reset value of RCC->CFGR: 0x0000 0000
	RCC->CFGR &= ~0x00000003;	// clear, (Reset value: HSI) 
	RCC->CFGR |= 0x00000002;	// PLL

	// Wait till the main PLL is used as system clock source 
	while ((RCC->CFGR & 0x0000000C ) != 0x00000008);	// CHECK bit#2~3 : PLL as system clock is RDY?  
	{}
    }
    else
    { // If HSE fails to start-up, the application will have wrong clock
        // configuration. User can add here some code to deal with this error 
    }
}

