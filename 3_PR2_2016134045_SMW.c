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
                          // TX ��  (���ͷ�Ʈ �� ����)
void SerialPutChar(uint8_t c);        // 1byte ����
void Serial_PutString(char *s);         // ���� �� ����
void Thermostat_Setup(void);
void Thermostat(void);
  
uint16_t  q =0;      //�÷���
uint8_t ST1=0,ST2=0,ST3=0;   //��ž ��ġ ����
uint8_t WT1=0,WT2=0,WT3=0; // ��ž ��ġ ��� ����
uint8_t C1=0x30,C2=0x30,C3=0x30, C4=0x30, C5=0x30,C6=0x30; // ���� ����
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
	LCD_Init();			// LCD ���� �Լ�
	DelayMS(10);			// LCD���� ������
        _EXTI_Init();
	DisplayTitle();		//LCD �ʱ�ȭ�鱸�� �Լ�
	_GPIO_Init();
	//_ADC_Init();
        ALARM_Setup();
       TIME7_Init();   //�ð� Ÿ�̸�
       TIME9_Init();   //��ž��ġ Ÿ�̸�
       TIME3_Init();   // ADC ���� Ÿ�̸�
       TIME4_Init();   // LED Ÿ�̸�
       RCC->APB1ENR &= ~(1<<1);  // timer 3  // �̸� ��
      KEY1_Scan();          // ���̽�ƽ Ű��ĵ
      USART1_Init();         //USART 1
      _ADC_Init();             // ADC ����
      RCC->APB1ENR &= ~(1<<8);  // timer 14 off   // ó�� ���´� ���� �Ҹ� �ȳ��� �ϱ� ���ؼ�.    
      RCC->APB2ENR &= ~(1<<16);   //timer9 OFF  // ��ž��ġ ó���� ����
      ST1=0; // timer9�� �ѹ� ���ұ� ������ 0 �������
      while(1)
	{
          switch(SW0_Flag){
          case 1:
            if(i==1){ // i ä�� �Ѿ�鼭, �ѹ��� ����Ǳ� ���ؼ�
              ALARM_Setup();}   // �˶��ð� ȭ�� ����
            ALARM_Switch();   // �ð� �ڵ�
            break;
          case 2:
            if(i==1){// i ä�� �Ѿ�鼭, �ѹ��� ����Ǳ� ���ؼ�
              STOP_WATCH_Setup ();}// ��ž ��ġ ȭ�鱸��
             STOP_WATCH (); // ��ž��ġ �ڵ�
            break;
          
          case 3:
            if(i==1){// i ä�� �Ѿ�鼭, �ѹ��� ����Ǳ� ���ؼ�
              Calculator_Setup();}  // ���� ȭ�� ����
              //Calculator();     // ���� �ڵ�, �̺κ��� Usart rx ���ͷ�Ʈ�� ������.
            DelayMS(20);   // ���� ȭ�� �Ѿ�� �ʴ°��� �����ϱ� ���ؼ�
            break;
            case 4:
            if(i==1){// i ä�� �Ѿ�鼭, �ѹ��� ����Ǳ� ���ؼ�
             Thermostat_Setup();  // ���� ȭ�� ����
              RCC->APB1ENR |= (1<<1);  // timer 3
            }
            DelayMS(20);// ���� ȭ�� �Ѿ�� �ʴ°��� �����ϱ� ���ؼ�
           //Thermostat(); ADC���ͷ�Ʈ���� �ذ���
               break;
          
              }
} 
}
void Thermostat(void){
                
}
void Thermostat_Setup(void){
                LCD_Clear(RGB_WHITE);// ȭ�� �����
                LCD_DisplayText(0,17,":"); // ���� ȭ�鿡 : ǥ��
                LCD_SetBackColor(RGB_WHITE);// ����
                LCD_SetTextColor(RGB_GREEN);	//���ڻ�
                LCD_DisplayText(0,0,"4.Thermostat");   
                LCD_SetTextColor(RGB_BLACK);//���ڻ�
                LCD_DisplayText(1,0,"T:");
                LCD_DisplayText(2,0,"H:0  C:1");
                i=0; // �÷��� �����
}
char str[20];// 16������ ����ȯ�� ���� �迭
void Calculator(void){
       LCD_DisplayChar(1,2,C1);  //C1�� �״�� ������
       LCD_DisplayChar(1,6,C2);//C2�� �״�� ������
       if(C6==0x3D){//3��°���� '=' �̾�� �۵�
       if(C1>0x40&C1<0x60){// �ҹ��� ���ĺ��� ���
       C3=C1-0x37;    //���ĺ��� 0x37�� ����
       }
       else if(C1>0x60){  // �빮�� ���ĺ��� ���
       C3=C1-0x57;//���ĺ��� 0x37�� ����
       }
       else{C3=C1-0x30;}   //���ڴ� 0x370�� ����
       if(C2>0x40&C2<0x60){ // �ҹ��� ���ĺ��� ���
       C4=C2-0x37;            //���ĺ��� 0x37�� ����
       }
       else if(C2>0x60){// �빮�� ���ĺ��� ���
       C4=C2-0x57;//�빮�� ���ĺ��� 0x57�� ����
       }
       else{C4=C2-0x30;} //���ڴ� 0x370�� ����
       C5=C3+C4;             // ���� ���             0x �ڱ� ���� �� �� ������ �ϰ� ����
      sprintf(str,"%2X",C5);   // 16���� �� ǥ��
        LCD_DisplayText(1,10,str);
       }
       C6=0x30; // �ʱ�ȭ
}


void Calculator_Setup(void){
  
                LCD_Clear(RGB_WHITE);// ȭ�� �����
                LCD_DisplayText(0,17,":"); // ���� ȭ�鿡 : ǥ��
                LCD_SetBackColor(RGB_WHITE);
                LCD_SetTextColor(RGB_GREEN);	//���ڻ�
                LCD_DisplayText(0,0,"3.Calculator");   
                LCD_SetTextColor(RGB_BLACK);//���ڻ�
                LCD_DisplayText(1,0,"  0 + 0 = 00");
                i=0; // �÷��� �����
}

void USART1_Init(void){
	// USART1 : TX(PA9)
	RCC->AHB1ENR	|= 0x01;	// RCC_AHB1ENR GPIOA Enable
	GPIOA->MODER	|= 0x00080000;	// GPIOA PIN9 Output Alternate function mode                     // 9�� �� �ڸ�					
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
        
	USART_BRR_Configuration(9600); // USART Baud rate Configuration   //�ӵ� 19200       
    
        /////////////////////////////////////////////////////////////////////////////////////////
        
	USART1->CR1	&= ~USART_CR1_M;	// USART_WordLength 8 Data bit                      //M= 12�� ==(1<<12)           8��Ʈ ��� ����   
	USART1->CR1	&= ~USART_CR1_PCE ;	// USART_Parity_No
	USART1->CR1	|= USART_CR1_RE;	// 0x0004, USART_Mode_RX Enable
	USART1->CR1	|= USART_CR1_TE ;	// 0x0008, USART_Mode_Tx Enable
	USART1->CR2	&= ~USART_CR2_STOP;	// USART_StopBits_1
	USART1->CR3	= 0x0000;	// No HardwareFlowControl, No DMA        // flow contron ���� -> �ߵǴ��� ���� 1�� �ϴ°��� 
    
	USART1->CR1 	|= USART_CR1_RXNEIE;	//  0x0020, RXNE interrupt Enable  // ����ũ
       // USART1->CR1 	|= USART_CR1_RXNEIE;	//  0x0020, RXNE interrupt Enable  // ����ũ
	NVIC->ISER[1]	|= (1 << (37-32)); 	// Enable Interrupt USART1 (NVIC 37��)
	USART1->CR1 	|= USART_CR1_UE;	//  0x2000, USART1 Enable              �۵� : ON
}

void USART1_IRQHandler(void)           // ������ ���ͷ�Ʈ ��
{       
  if ( (USART1->SR & USART_SR_RXNE) )            // �����Ͱ� ����   // flag ���� �ʿ����  �����͸� ������ flag���� ������
	{       
		char ch;
		ch = (uint16_t)(USART1->DR & (uint16_t)0x01FF);	// ���ŵ� ���� ����
		SerialPutChar(ch); // �ø��� ������� ���� ����
                 if(q==0){   // q�� ���� 1,2,3, ǥ����
                  C1=ch;   // ù��° �� ����
                  q++;}
                 else if(q==1){
               C2=ch;// �ι�° �� ����
               q++;}
                 else if(q==2){
                   C6=ch;       // ����° �� ����
                   q=0;}
                 Calculator();   // ���� �ڵ� ����
	} 
                  
        // DR �� ������ SR.RXNE bit(flag bit)�� clear �ȴ� 

 
}
void SerialPutChar(uint8_t Ch) // 1���� ������ �Լ�
{
        while((USART1->SR & USART_SR_TXE) == RESET); //  USART_SR_TXE:0x0080, �۽� ������ ���±��� ���   TX. DR �������    flag
        // ��������� while�� ���� ���� 
	USART1->DR = (Ch & 0x01FF);	// ����                       //0x01FF �����Ⱚ ��������
}

void Serial_PutString(char *str) // �������� ������ �Լ�
{
	while (*str != '\0') // ���Ṯ�ڰ� ������ ������ ����, ���Ṯ�ڰ� �����Ŀ��� ������ �޸� ���� �߻����ɼ� ����.
	{
		SerialPutChar(*str);	// �����Ͱ� ����Ű�� ���� �����͸� �۽�
		str++; 			// ������ ��ġ ����
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
  switch(KEY1_Scan())                      //���̽�ƽ Ű��ĵ
		{
			case  NAVI_SW_PUSH : 	//SW0
				AR=1;   
                                 LCD_SetPenColor(RGB_RED);                        
                                 
                                 //     x���� �ð��� ���� ���� ��� �� ���̰�, 2�ʿ� 1������
                                 //     y����  360/256 =1.40�� -> tan(0.0244) �̴� x��ŭ ���� �������� ������  tan(0x0244*x) �� x���� ���� x*tan(0.0244*x) �̴� 
                          ////////////////////////////////////////////////////////////  ������ �˶� ��ġ ���߱�                    
                          if(AR2<=3)                                                             // �ð��� ���� x,y �� 
                          {xx=AR1/2+(8*(AR2%4));                                          // �ð��� ���� x�� 
                          yy=32-(int)(xx*tan(0.0244*xx));                                   // �ð��� ���� y��
                          tt=1;LCD_DrawLine(120,80, 120+xx, 80-yy); }                // �� 1��и�
                          if(4<=AR2&&AR2<=7)                                                  // �ð��� ���� x,y �� 
                          {xx=AR1/2+(8*(AR2%4));                                           // �ð��� ���� x��
                          yy=32-(int)(xx*tan(0.0244*xx));                                    // �ð��� ���� y��
                          tt=2;LCD_DrawLine(120,80, 120+yy, 80+xx);}                // �� 4��и�
                         if(8<=AR2&&AR2<=11)                                              // �ð��� ���� x,y �� 
                         {xx=AR1/2+(8*(AR2%4));                                           // �ð��� ���� x��
                          yy=32-(int)(xx*tan(0.0244*xx));                                    // �ð��� ���� y��
                          tt=3; LCD_DrawLine(120,80, 120-xx, 80+yy);}                // �� 3��и�
                          if(12<=AR2&&AR2<=15)                                              // �ð��� ���� x,y �� 
                          {xx=AR1/2+(8*(AR2%4));                                           // �ð��� ���� x��
                          yy=32-(int)(xx*tan(0.0244*xx));                                    // �ð��� ���� y��
                          tt=4; LCD_DrawLine(120,80, 120-yy, 80-xx); }                  // �� 2��и�
                         // AR=0;
 			break;

			case  NAVI_SW_UP: 	//SW1
				AR2++; 
			break;

                        case  NAVI_SW_DOWN  : 	//SW2
				if(AR2==0){                // 0-> F �ǵ���
                                AR2=16;
                                }
                                AR2--;          
			break;

                        case  NAVI_SW_RIGHT  : 	//SW3
				AR1++;   
			break;
                        
                        case  NAVI_SW_LEFT  : 	//SW3
			 if(AR1==0){                // 0-> F �ǵ���
                                AR1=16;
                                }	
                          AR1--;         
			break;
                }   
  if(AR1<=9){                                                 // �ð� 10������ 16������ ����� �ڵ�  AR1�� ��,  AR2�� ��
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
  LCD_Clear(RGB_WHITE);// ȭ�� �����
  LCD_SetBackColor(RGB_WHITE);
  LCD_DisplayText(0,17,":"); // ���� ȭ�鿡 : ǥ��
  i=0; // �ѹ��� �ϱ����� �÷���
  LCD_SetBrushColor(RGB_BLUE);
  y=32-(int)(x*tan(0.0244*x));                      // �ð������� x���� ��ȭ�� ���� y �� (������ ����)
  LCD_DrawLine(120,80, 120+x, 80-y); 
  ///////////////////  �ð� Ʋ
             LCD_SetBrushColor(RGB_YELLOW);                    // �簢�� �����
             LCD_DrawFillRect(87,45,68,68); 
             LCD_SetPenColor(RGB_GREEN);                        // �׵θ� �����
             LCD_DrawRectangle(87,45,68,68);

                LCD_SetPenColor(RGB_RED);                         //�˶� ������ �κ� �����
                          xx=(AR1)+1;  
                          yy=32-(int)(xx*tan(0.0244*xx));
                          if(AR2&&AR2<=3)
                          { tt=1;LCD_DrawLine(120,80, 120+xx, 80-yy); }    //�� 1��и�
                          if(4<=AR2&&AR2<=7)
                          {tt=2;LCD_DrawLine(120,80, 120+yy, 80+xx);}    //�� 4��и�
                         if(8<=AR2&&AR2<=11)
                         {tt=3; LCD_DrawLine(120,80, 120-xx, 80+yy);}    //�� 3��и�
                          if(12<=AR2&&AR2<=15)
                          {tt=4; LCD_DrawLine(120,80, 120-yy, 80-xx); }    //�� 2��и�          
		
                LCD_SetBackColor(RGB_WHITE);      // ���� ����
                LCD_SetTextColor(RGB_GREEN);	//���ڻ�
                LCD_DisplayText(0,0,"1.ALARM");
                LCD_SetTextColor(RGB_BLACK);//���ڻ�
                LCD_DisplayText(1,0,"ALARM  :");
                 if(AR1<=9){                                       // ��ħ�� 9�� ���ų� ������
               LCD_DisplayChar(1,8,AR1+0x30);}            // �����϶�, ��ħ ǥ��
               else if(AR1>9 && AR1<=15){
                 LCD_DisplayChar(1,8,AR1+0x37); }            // ���ĺ��϶�, ��ħ ǥ��
               else if(AR1>15){                                  // ��ħ�� F�� �ѱ�� ��ħ���� +1��
               AR1=0;
               AR2++;
               LCD_DisplayChar(1,8,AR1+0x30);       // ��ħ ǥ��
               }
               
               if(AR2<=9){                                          // ��ħ�� 9�� ���ų� ������
               LCD_DisplayChar(1,6,AR2+0x30);}           // �����϶�, ��ħ ǥ��
               else if(AR2>9 && AR2<=15){
                 LCD_DisplayChar(1,6,AR2+0x37); }           // ���ĺ��϶�, ��ħ ǥ��
               else if(AR2>15){                                 // ��ħ�� F�� �ѱ�� ��ħ���� +1��
               AR1=0;
               AR2=0;
               }
               if(AR==1){
               }
}
void STOP_WATCH_Setup (void){
                LCD_Clear(RGB_WHITE);// ȭ�� �����
                LCD_DisplayText(0,17,":"); // ���� ȭ�鿡 : ǥ��
                LCD_SetBackColor(RGB_WHITE);
                LCD_SetTextColor(RGB_GREEN);	//���ڻ�
                LCD_DisplayText(0,0,"2.Stop Watch");   
                LCD_SetTextColor(RGB_BLACK);//���ڻ�
                LCD_DisplayText(1,0,"S-W   :");
                LCD_DisplayText(2,0,"Record   :");
                i=0; // �÷��� �����
}
void STOP_WATCH (void){                               // 2�� ȭ�� ����    
               if(ST1<10){ 
                LCD_DisplayChar(1,7,ST1+0x30);}
               else if(ST1>9 && ST1<=15){
               LCD_DisplayChar(1,7,ST1+0x37);
               }else if(ST1>15){
               ST1=0;
               LCD_DisplayChar(1,7,ST1+0x30);
               ST2++; }
                  //////////////////////////////////////////////////////////////////////�� �ð� �Ѿ����  
               if(ST2<10){
                 LCD_DisplayChar(1,5,ST2+0x30);}
               else if(ST2>9 && ST2<=15){
              LCD_DisplayChar(1,5,ST2+0x37);}
              else if(ST2>15){
               ST2=0;
               LCD_DisplayChar(1,5,ST2+0x30);
               ST3++;}
                 //////////////////////////////////////////////////////////////////////�� �ð� �Ѿ����  
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
  // ȭ��ǥ �Լ�
 //////////////////////////////////////////////////////////////////////��ħ �����   
  
 //////////////////////////////////////����� t�� 1,2,3,4, �� ǥ���Ͽ� 4���� �и��� ǥ���ϰ�, t3
               if(t==1&t3==1){
                 if(x==0){                                     // �� ������ �κ� �����
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31))); // y���� �˶��κп� ǥ��
               LCD_DrawLine(120,80, 120-y, 80-31);
                 }
               LCD_SetPenColor(RGB_YELLOW);       // �ٷ� ���� �κ� �����
             y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120+(x-1), 80-y);  
               
            LCD_SetPenColor(RGB_BLUE);               // ȭ��ǥ �׸���
             y=32-(int)(x*tan(0.0244*x)); //     y����  360/256 =1.40�� -> tan(0.0244) �̴� x��ŭ ���� �������� ������  tan(0x0244*x) �� x���� ���� x*tan(0.0244*x) �̴� 
           LCD_DrawLine(120,80, 120+x, 80-y/1);  
            }
         
            
           if(t==2&t3==1){
             if(x==0){                                     // �� ������ �κ� �����
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31)));
               LCD_DrawLine(120,80, 120+31, 80-y/1);  
                 }
              LCD_SetPenColor(RGB_YELLOW);       // �ٷ� ���� �κ� �����
             y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120+y, 80+(x-1)); //�� 4�� �и�
               
             
            LCD_SetPenColor(RGB_BLUE);               // ȭ��ǥ �׸���
          y=32-(int)(x*tan(0.0244*x)); //     y����  360/256 =1.40�� -> tan(0.0244) �̴� x��ŭ ���� �������� ������  tan(0x0244*x) �� x���� ���� x*tan(0.0244*x) �̴� 
           LCD_DrawLine(120,80, 120+y, 80+x); //�� 4�� �и�
            }

            if(t==3&t3==1){
                if(x==0){                                    // �� ������ �κ� �����
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31)));
               LCD_DrawLine(120,80, 120+y, 80+31);
                 }
              
               LCD_SetPenColor(RGB_YELLOW);       // �ٷ� ���� �κ� �����
             y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120-(x-1), 80+y);  //�� 3�� �и�
               
            LCD_SetPenColor(RGB_BLUE);               // ȭ��ǥ �׸���
           y=32-(int)(x*tan(0.0244*x)); //     y����  360/256 =1.40�� -> tan(0.0244) �̴� x��ŭ ���� �������� ������  tan(0x0244*x) �� x���� ���� x*tan(0.0244*x) �̴� 
           LCD_DrawLine(120,80, 120-x, 80+y);  //�� 3�� �и�
            }
            if(t==4&t3==1){
              if(x==0){                                     // �� ������ �κ� �����
                  LCD_SetPenColor(RGB_YELLOW);
             y=32-(int)((31)*tan(0.0244*(31)));
                LCD_DrawLine(120,80, 120-31, 80+y);
                 }
                 LCD_SetPenColor(RGB_YELLOW);;
              y=32-(int)((x-1)*tan(0.0244*(x-1)));
               LCD_DrawLine(120,80, 120-y, 80-(x-1));  // �� 2�� �и�
               
            LCD_SetPenColor(RGB_BLUE);               // ȭ��ǥ �׸���
          y=32-(int)(x*tan(0.0244*x));
         LCD_DrawLine(120,80, 120-y, 80-x);  // �� 2�� �и�
            }


}

void _ADC_Init(void)  //������ Ÿ�̸� 200ms����
{   
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	// 0x00000001  // ENABLE GPIOA CLK
	GPIOA->MODER |= GPIO_MODER_MODER1;       // 0x0000000C	// CONFIG GPIOA PIN1(PA1) TO ANALOG IN MODE
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	// 0x00000100  // ENABLE ADC1 CLK

        NVIC->ISER[0] = (1<<18);  //   ADC ���ͷ�Ʈ          
        
	ADC->CCR &= ~0X0000001F;	// MULTI[4:0]: ADC_Mode_Independent
	ADC->CCR |= 0x00010000;		// ADCPRE: ADC_Prescaler_Div4 (ADC MAX Clock 36MHz, 84Mhz(APB2)/4 = 21MHz)
	ADC->CCR &= ~0x0000C000;	// DMA: Disable

	ADC1->CR1 &= ~(3<<24);		// RES[1:0]: 12bit Resolution
	ADC1->CR1 &= ~(1<<8);		// SCAN: ADC_ScanCovMode Disable
      
        ADC1->CR1 |= (1<<5);                // ���ͷ�Ʈ ����ũ
        
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
	ADC1->SQR3 |= 1<<0;             //ù ���� 1�� ä�� PA1
     //   NVIC->ISER[0] |= (1<<18);	// Enable ADC global Interrupt
        
}
UINT16 n=0;
void ADC_IRQHandler(void)
{
            ADC1->SR &= ~(1<<1);	// EOC flag clear
            
             ADC_Value = ADC1->DR;   // Read ADC result value from ADC Data Reg(ADC1->DR) 
        
        	sprintf(str,"%4d",ADC_Value);
     
               
               Voltage = ADC_Value * (3.3 * 100) / 4095;   // 5: 4095 =  Volatge : ADC_Value 
                                                                // 100:  �Ҽ����Ʒ� ���ڸ����� ǥ���ϱ� ���� ��  

             static UINT16 Cnt = 0;
               Vo=ADC_Value*50/4095-10;
                
               // �� �µ��� �´� ���� ��, �� ǥ��
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
               if(Vo<0){                                      // �µ��� 0���� �������ٸ�,
                n=1;
                 Vo=(-1)*Vo;                               // ���� -��
                 LCD_DisplayText(1,2,"-");
                 
                 for(  Cnt=0; Cnt<4; Cnt++){
                   LCD_SetPenColor(RGB_WHITE);         //�Ͼ�� �ְ� �ʱ�ȭ
                  LCD_DrawHorLine(50, 25+Cnt, 60);

                  
                  LCD_SetPenColor(RGB_BLUE);         //�Ķ��� �ְ� �� ǥ��
                  LCD_DrawHorLine(50, 25+Cnt, 10- Vo);

             
}

               }
               else{LCD_DisplayText(1,2," ");           //- ǥ�� ���ֱ�
               n=0;
                for(  Cnt=0; Cnt<4; Cnt++){
                   LCD_SetPenColor(RGB_WHITE);         //�Ͼ�� �ְ�
                   LCD_DrawHorLine(50, 25+Cnt, 60);     // �ʱ�ȭ

                  if(Vo<11)
                  LCD_SetPenColor(RGB_BLUE);         //�Ķ��� �ְ�
                  else if(Vo>10&Vo<21)
                    LCD_SetPenColor(RGB_GREEN);    //�ʷϻ� �ְ�
                  else if(Vo>20&Vo<31)
                  LCD_SetPenColor(RGB_RED);    //������ �ְ�
                  else if(Vo>30&Vo<41)
                  LCD_SetPenColor(RGB_RED);    //������ �ְ�
                  
                  // �׷��� ǥ��
                  LCD_DrawHorLine(50, 25+Cnt, 10+Vo);
}
               
               }
               
               
               LCD_DisplayChar(1,3,(Vo/10)+0x30);             //  �������׿� ���� �µ� ǥ��
               LCD_DisplayChar(1,4,(Vo%10/1) +0x30);       //  �������׿� ���� �µ� ǥ��
               
                
                     
}
void TIME4_Init(void)
{   
	RCC->AHB1ENR	|= 0x00000002;	// RCC_AHB1ENR GPIOB Enable
	RCC->APB1ENR 	|= 0x00000004;	// RCC_APB1ENR TIMER4 Enable 
    						
	// PB6�� ��¼����ϰ� Alternate function(TIM4_CH3)���� ��� ����
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
void TIME7_Init(void)  //Ÿ�̸� 200ms ���� adc���� �е��� �Ѵ�. �� ���� �������� ���̴�.
{
  RCC->APB1ENR |= (1<<5);   //timer7 ON
  NVIC->ISER[1] |= (1<<(55-32)); // tim7 global interupt
  TIM7->PSC =  8400-1;  // 84,000,000 / 8400 =10000Hz = 0.1ms
  TIM7->ARR =  10000-1;    // 0.1msec * 1000 =1s  // �׷��� ����̱� ������ 1/2 ���ذ��̴�. �׷��� �ֱ�� 200ms
 
  TIM7->CR1 &= ~(1<<4);      // upcount
  TIM7->CR1 &= ~(3<<5);      // Edge-aligned mode
  TIM7->CR1 &=~ (1<<7);  // buffer ����   ========�߸𸣰���
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
               //////////////////////////////////////////////////////////////////////�� �ð� �Ѿ���� 
                ////// ��ħ�� 10������ 16������ ��ȯ�ϴ� ����////////////////////////////////
               LCD_SetBackColor(RGB_WHITE);	//���ڹ���
               if(t1<10){                                    // ���� 10���� ������
               LCD_SetTextColor(RGB_BLUE);
               LCD_DisplayChar(0,18,t1+0x30);} // ���ڴ� 0x30 ����
               else if(t1>9 && t1<=15){           // ���� 9���� ũ��,15�� ���ų� ������
                 LCD_SetTextColor(RGB_BLUE);
                 LCD_DisplayChar(0,18,t1+0x37); }// ���ĺ��� 0x37 ����
               else if(t1>15){
               t1=0;          
               t2++;                                      //��ħ 0���� �ʱ�ȭ ��ħ�� +1 �߰�
               LCD_SetTextColor(RGB_BLUE);
               LCD_DisplayChar(0,18,t1+0x30);
               }
                 ////// ��ħ�� 10������ 16������ ��ȯ�ϴ� ����////////////////////////////////
               if(t2<10){                                    // ��ħ�� 10���� ������
                 LCD_SetTextColor(RGB_BLACK);
               LCD_DisplayChar(0,16,t2+0x30);}
               else if(t2>9 && t2<=15){              // ��ħ�� 9���� ũ��,15�� ���ų� ������
                 LCD_SetTextColor(RGB_BLACK);
                 LCD_DisplayChar(0,16,t2+0x37); }
               else if(t2>15){                                 // ��ħ�� 10���� ������
               t1=0;
               t2=0;                                           // �ʱ�ȭ
               LCD_SetTextColor(RGB_BLUE);
               LCD_DisplayChar(0,18,t1+0x30);
               LCD_SetTextColor(RGB_BLACK);
               LCD_DisplayChar(0,16,t2+0x30);
               }
               
                  if(AR==1&AR1==(t1)&AR2==t2){  // �˶� �κ�;
              BEEP();  DelayMS(30);  BEEP();  DelayMS(30);  BEEP(); AR=0;   }
                
              t1++;  // �� ����
              
               if(SW0_Flag==1){   
                 TIME7_time();}    // ȭ��ǥ �Լ�
          if(x==32){ x=0;  t++;} // �ð��� ���� ȭ��ǥ ������
          if(t>4){ t=1; }          // �ð��� ���� ȭ��ǥ ������
           t3^=1;   x1^=1;       // �ð��� ���� ȭ��ǥ ������
         if(x1==0){ x++; }       // �ð��� ���� ȭ��ǥ ������
  }	
}
void TIME9_Init(void){
  RCC->APB2ENR |= (1<<16);   //timer9 ON
  NVIC->ISER[0] |= (1<<(24)); // tim9 global interupt
  TIM9->PSC =  16800-1;  // 168,000,000 / 16800 =10000Hz = 0.1ms
  TIM9->ARR =  1000-1;    // 0.1msec * 500 =0.05s  // �׷��� ����̱� ������ 1/2 ���ذ��̴�. �׷��� �ֱ�� 0.05s
 
  TIM9->CR1 &= ~(1<<4);      // upcount
  TIM9->CR1 &= ~(3<<5);      // Edge-aligned mode
  TIM9->CR1 &=~ (1<<7);  // buffer ����   ========�߸𸣰���
  TIM9->CR1 &=~(3<<8);       // clock division =1
           
  TIM9->CCER |= (1<<0);  //CC enable
  TIM9->CCER &=~(1<<1);  // �����
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
  ST1++;// ��ž��ġ 0.1�ʾ� ����
  }
  
}
void TIME3_Init(void)
{
      RCC->APB1ENR |= (1<<1);  // timer 3
      NVIC->ISER[0]|= (1<<(29));

          TIM3->PSC = 8400-1;  // 84,000,000 / 8400 =10,000Hz = 0.1ms
          TIM3->ARR =  2500-1;    // 0.1msec * 5000=500ms   
          TIM3->CCR1 =1250;  // ��  =250Hz
  
  
          TIM3->CR1 &= ~(1<<4);      // upcount
          TIM3->CR1 &= ~(3<<5);      // Edge-aligned mode
          TIM3->CR1 &=~ (1<<7);  // buffer ����   ========�߸𸣰���
          TIM3->CR1 &=~(3<<8);       // clock division =1
  
          TIM3->CCER |= (1<<0);  //CC enable
          TIM3->CCER &=~(1<<1);  // �����
          
          TIM3->CCMR1 &= ~(3<<0); // CC1S(CC1 channel): Output 
          TIM3->CCMR1 &= ~(1<<3); // OC1PE: Output Compare 1 preload disable
          TIM3->CCMR1 |= (3<<4);	// OC1M: Output Compare 1 Mode : toggle
        
          TIM3->EGR |=(1<<1);    //CC1 �̺�Ʈ
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
	// LED GPIO(PORT G) ����
    	RCC->AHB1ENR    |= 0x00000040; 	// RCC_AHB1ENR(bit8~0) GPIOG(bit#6) Enable							
	GPIOG->MODER    = 0x00005555;	// GPIOG PIN0~PIN7 Output mode (0b01)						
	GPIOG->OTYPER   = 0x0000;	// GPIOG PIN0~PIN7 : Push-pull  (PIN8~PIN15) (reset state)	
 	GPIOG->OSPEEDR  = 0x00005555;	// GPIOG PIN0~PIN7 Output speed (25MHZ Medium speed) 
    
	// SW GPIO(PORT H) ���� 
	RCC->AHB1ENR    |= 0x00000080;	// RCC_AHB1ENR(bit8~0) GPIOH(bit#7) Enable							
	GPIOH->MODER    = 0x00000000;	// GPIOH PIN8~PIN15 Input mode (reset state)				
	GPIOH->PUPDR    = 0x00000000;	// GPIOH PIN8~PIN15 Floating input (No Pull-up, pull-down) (reset state)

	//Buzzer GPIO(PORT F) ���� 
    	RCC->AHB1ENR    |= 0x00000020; 	// RCC_AHB1ENR(bit8~0) GPIOF(bit#5) Enable							
	GPIOF->MODER    |= 0x00040000;	// GPIOF PIN9 Output mode (0b01)						
	GPIOF->OTYPER   &= 0xFDFF;	// GPIOF PIN9 : Push-pull  	
 	GPIOF->OSPEEDR  |= 0x00040000;	// GPIOF PIN9 Output speed (25MHZ Medium speed) 
	
	//NAVI_SW(PORT I) ����
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
	
	SYSCFG->EXTICR[ 2] |= 0x7777; 	// EXTI  8~11�� ���� �ҽ� �Է��� GPIOH�� ���� (EXTICR2) (reset value: 0x0000)	
	SYSCFG->EXTICR[ 3] |= 0x7777; 	// EXTI 12~15�� ���� �ҽ� �Է��� GPIOH�� ���� (EXTICR3) (reset value: 0x0000)	
        
	EXTI->FTSR |= 0x00FF00;		// Falling Trigger Enable  (EXTI8~15:PH8~15)
	//EXTI->RTSR |= 0x000200;		// Rising Trigger  Enable  (EXTI9:PH9) 
    	EXTI->IMR |= 0x00FF00;  	// EXTI8~15���ͷ�Ʈ mask (Interrupt Enable)
		
	NVIC->ISER[0] |= ( 1 << (23) );      // Enable Interrupt EXTI5_9 Vector table Position ����
        NVIC->ISER[1] |= ( 1 << (40-32) ); // Enable Interrupt EXTI10_15 Vector table Position ����
}
void EXTI9_5_IRQHandler(void)		// EXTI 5~9 ���ͷ�Ʈ �ڵ鷯
{
	if(EXTI->PR & 0x0100) 		// EXTI9 nterrupt Pending?
	{
		EXTI->PR |= 0x0100; 	// Pending bit Clear
               SW0_Flag++;   // SW0 �÷���
               if(SW0_Flag==5){   // �޴� 4�� ������ �ٽ� 1��
               SW0_Flag=1;
               RCC->APB1ENR &= ~(1<<1);  // timer 3 off
               }               BEEP();// �Ҹ� 1��
               i=1;
	}
        
	
}
void EXTI15_10_IRQHandler(void)		// EXTI 15~10 ���ͷ�Ʈ �ڵ鷯
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
                WT1=ST1; // ��ž��ġ ��ϰ�1
                WT2=ST2; // ��ž��ġ ��ϰ�2
                WT3=ST3; // ��ž��ġ ��ϰ�3
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
                 WT1=0;                     // ��ϰ� , ��ž��ġ �ʱ�ȭ
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
	LCD_SetFont(&Gulim10);		//��Ʈ 
	LCD_SetBackColor(RGB_WHITE);	//���ڹ���
	LCD_SetTextColor(RGB_BLACK);	//���ڻ�
        LCD_DisplayText(0,17,":");
      //	LCD_DisplayText(1,0,"Temp:00.0C");
      //  LCD_DisplayText(2,0,"Freq:000Hz");
	LCD_SetBackColor(RGB_YELLOW);	//���ڹ���
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

    // Enable HSE : �ܺ� �Է� Ŭ�����ļ�: 5MHz(ȸ�ε� ����)
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
	// SYSCLK ��� (HSE �Է�Ŭ�����ļ�: 8MHz)
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

