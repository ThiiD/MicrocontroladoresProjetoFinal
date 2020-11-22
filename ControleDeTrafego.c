/*******************************************************
This program was created by the CodeWizardAVR V3.42 
Automatic Program Generator
© Copyright 1998-2020 Pavel Haiduc, HP InfoTech S.R.L.
http://www.hpinfotech.ro

Project : Controle de Trafego
Version : 1
Date    : 21/11/2020
Author  : MRR & TDSL
Company : Microcontroladores
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 14,745600 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>
#include <delay.h>
// Alphanumeric LCD functions
#include <alcd.h>
#include <stdio.h>

// Declare your global variables here
char flagPedestre=0;
int tempo = -1, dezena, unidade;
char flag0=0;
char flag1=0;

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
// Reinitialize Timer1 value
TCNT1H=0xC7C0 >> 8;
TCNT1L=0xC7C0 & 0xff;
// Place your code here

dezena = tempo/10;
unidade = tempo - dezena*10;

if (tempo >= 0)
   {
    tempo = tempo - 1;
   }



PORTA.0 = unidade & 0b0001;
PORTA.1 = unidade & 0b0010;
PORTA.2 = unidade & 0b0100;
PORTA.3 = unidade & 0b1000;
PORTA.4 = dezena & 0b0001;
PORTA.5 = dezena & 0b0010;

}

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here
// Interrupção de pedestre, ou seja, um pedestre deseja atravessar a rua.
// Essa interrupção fecha o semaforo da avenida principal e abre o da via secundaria.
// Sendo assim, o pedestre consegue passar seguramente.
    delay_ms(15000);
    PORTB.0 = 0;
    PORTB.1 = 1;
    delay_ms(2000);
    PORTB.1 = 0;
    PORTB.2 = 1;
    delay_ms(1500);
    PORTB.5=0;
    PORTB.3 = 1;
    delay_ms(15000);
    PORTB.3 = 0;
    PORTB.4 = 1;
    delay_ms(2000);
    PORTB.2 = 1;
    PORTB.4 = 0;
    PORTB.5 = 1;
    delay_ms(2000);
    PORTB.2 = 0;
    PORTB.0 = 1;
    PORTB.5 = 1;
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
// Place your code here

}

// External Interrupt 2 service routine
interrupt [EXT_INT2] void ext_int2_isr(void)
{
// Place your code here

}

void main(void)
{
// Declare your local variables here
char paginaInicial[33];
int contador1=0, contador2=0, contador3=0, contador4=0, contadorMenu = 0;
int senha[4];
// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRA=(0<<DDA7) | (0<<DDA6) | (1<<DDA5) | (1<<DDA4) | (1<<DDA3) | (1<<DDA2) | (1<<DDA1) | (1<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (1<<PORTB6) | (1<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (1<<PORTB0);;

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=0xff;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 14,400 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 1 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (1<<CS10);
TCNT1H=0xC7;
TCNT1L=0xC0;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Rising Edge
// INT1: On
// INT1 Mode: Rising Edge
// INT2: On
// INT2 Mode: Rising Edge
GICR|=(1<<INT1) | (1<<INT0) | (1<<INT2);
MCUCR=(1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (1<<ISC00);
MCUCSR=(1<<ISC2);
GIFR=(1<<INTF1) | (1<<INTF0) | (1<<INTF2);

// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
SFIOR=(0<<ACME);

// ADC initialization
// ADC disabled
ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 4
// D5 - PORTC Bit 5
// D6 - PORTC Bit 6
// D7 - PORTC Bit 7
// Characters/line: 8
lcd_init(16);

// Globally enable interrupts
#asm("sei")
senha[0] = 0;
senha[1] = 0;
senha[2] = 0;
senha[3] = 0;
while (1)
      {
      // Place your code here
      // Printa a tela inicial no display
      sprintf(paginaInicial,"C. de Trafego");
      lcd_puts(paginaInicial);
      lcd_gotoxy(0,1);
      sprintf(paginaInicial,"Senha: ");
      lcd_puts(paginaInicial);
      lcd_clear();
      
      while(PIND.5){
          if(PIND.3 == 0x0){
            contador1 = contador1 + 1;
            delay_ms(200);
          }
          if(PIND.4 == 0x0){
            contador1--;
            delay_ms(200);
          }
      sprintf(paginaInicial,"C. de Trafego");
      lcd_puts(paginaInicial);
      lcd_gotoxy(0,1);
      sprintf(paginaInicial,"Senha: %d",contador1);
      lcd_puts(paginaInicial);
      delay_ms(200);
      lcd_clear();
              }
      delay_ms(1000);
      while(PIND.5){
          if(PIND.3 == 0x0){
            contador2++;
            delay_ms(200);
          }
          if(PIND.4 == 0x0){
            contador2--;
            delay_ms(200);
          }
      sprintf(paginaInicial,"C. de Trafego");
      lcd_puts(paginaInicial);
      lcd_gotoxy(0,1);
      sprintf(paginaInicial,"Senha: %d%d",contador1,contador2);
      lcd_puts(paginaInicial);
      delay_ms(200);
      lcd_clear();
              }
      delay_ms(1000);
      while(PIND.5){
          if(PIND.3 == 0x0){
            contador3++;
            delay_ms(200);
          }
          if(PIND.4 == 0x0){
            contador3--;
            delay_ms(200);
          }
      sprintf(paginaInicial,"C. de Trafego");
      lcd_puts(paginaInicial);
      lcd_gotoxy(0,1);
      sprintf(paginaInicial,"Senha: %d%d%d",contador1,contador2,contador3);
      lcd_puts(paginaInicial);
      delay_ms(200);
      lcd_clear();
              }
      delay_ms(1000);
      while(PIND.5){
          if(PIND.3 == 0x0){
            contador4++;
            delay_ms(200);
          }
          if(PIND.4 == 0x0){
            contador4--;
            delay_ms(200);
          }
      sprintf(paginaInicial,"C. de Trafego");
      lcd_puts(paginaInicial);
      lcd_gotoxy(0,1);
      sprintf(paginaInicial,"Senha: %d%d%d%d",contador1,contador2,contador3,contador4);
      lcd_puts(paginaInicial);
      delay_ms(200);
      lcd_clear();
              }
      delay_ms(1000);
      if(contador1 == senha[0] && contador2 == senha[1] && contador3 == senha[2] && contador4 == senha[3]){
        sprintf(paginaInicial,"Bem Vindo");
        contador1=0;
        contador2=0;
        contador3=0;
        contador4=0;
        lcd_puts(paginaInicial);
        delay_ms(2000);
        lcd_clear();
        while(1){
            // Desabilitar os semaforos
            if(contadorMenu == 0){
                sprintf(paginaInicial,"Desabilitar semaforos");
                lcd_puts(paginaInicial);
                delay_ms(200);
                lcd_clear();
                if(PIND.5 == 0x0){
                    PORTB.0 = 0;
                    PORTB.1 = 1;
                    PORTB.5 = 0;
                    PORTB.4 = 1;
                }
                if(PIND.3 == 0x0){
                    contadorMenu++;
                    delay_ms(200);
                    }
                if(PIND.4 == 0x0){
                    contadorMenu--;
                    delay_ms(200);
                }
            }
            
            // Habilita semaforos
            if(contadorMenu == 1){
                sprintf(paginaInicial,"Habilita semaforos");
                lcd_puts(paginaInicial);
                delay_ms(200);
                lcd_clear();
                if(PIND.5 == 0x0){
                    PORTB.0 = 1;
                    PORTB.1 = 0;
                    PORTB.5 = 1;
                    PORTB.4 = 0;
                }
                if(PIND.3 == 0x0){
                    contadorMenu++;
                    delay_ms(200);
                    }
                if(PIND.4 == 0x0){
                    contadorMenu--;
                    delay_ms(200);
                }
            }
            
            // Alterar o valor de tempo do TIMER
            if(contadorMenu == 2){
                sprintf(paginaInicial,"Alterar timer");
                lcd_puts(paginaInicial);
                delay_ms(200);
                lcd_clear();
                if(PIND.5 == 0x0){
                    sprintf(paginaInicial,"Novo valor para timer");
                    lcd_puts(paginaInicial);
                    delay_ms(1000);
                    lcd_clear();
                    sprintf(paginaInicial,"Pedestre - 1");
                    lcd_puts(paginaInicial);
                    lcd_gotoxy(0,1);
                    sprintf(paginaInicial,"Carro - 2");
                    lcd_puts(paginaInicial);
                    delay_ms(1500);
                    lcd_clear();
                    contador1 = 1;
                    
                    //Faz a escolha de pedestre ou carro
                    while(PIND.5 == 0x1){
                        sprintf(paginaInicial,"Digite o numero: %d",contador1);
                        lcd_puts(paginaInicial);
                        delay_ms(200);
                        lcd_clear();
                        if (PIND.3 == 0x0){
                            contador1++;
                            delay_ms(200);
                        }
                        if (PIND.4 == 0x0){
                            contador1--;
                            delay_ms(200);
                        }
                        // Delimita os limites do contador
                        if(contador1 == 3){
                            contador1 = 1;
                        }
                        if(contador1 == 0){
                            contador1 = 2;
                        }
                    }
                    delay_ms(200);
                    
                    // Altera timer para pedestres
                    if(contador1 == 1){
                        contador1 = 0;
                        //Altera o valor das dezenas
                        while(PIND.5 == 0x1){
                            sprintf(paginaInicial,"Valor dezena: %d",contador1);
                            lcd_puts(paginaInicial);
                            delay_ms(200);
                            lcd_clear();
                            if(PIND.6 == 0x0){
                                break;
                            }
                            if(PIND.3 == 0x0){
                                contador1++;
                                delay_ms(200);
                            }
                            if(PIND.4 == 0x0){
                                contador1--;
                                delay_ms(200);
                            }
                        
                            // Delimita os valores da nossa dezena de acordo com o projeto do nosso circuito.
                            if(contador1 == 4){
                                contador1 = 0;
                            }
                            if(contador1 == -1){
                                contador1 = 3;
                            }
                        
                        }
                        delay_ms(200);
                        // Altera o valor das unidades
                        while(PIND.5 == 0x1){
                            sprintf(paginaInicial,"Valor unidades: %d%d",contador1,contador2);
                            lcd_puts(paginaInicial);
                            delay_ms(200);
                            lcd_clear();
                            if(PIND.6 == 0x0){
                                break;
                            }
                            if(PIND.3 == 0x0){
                                contador2++;
                                delay_ms(200);
                            }
                            if(PIND.4 == 0x0){
                                contador2--;
                                delay_ms(200);
                            }
                        
                            // Delimita os valores da nossa dezena de acordo com o projeto do nosso circuito.
                            if(contador2 == 10){
                                contador2 = 0;
                            }
                            if(contador2 == -1){
                                contador2 = 9;
                            }
                        
                        }
                        delay_ms(200);
                        // Novo valor de timer
                        tempoPedestre = 10*contador1 + contador2;
                        contador1 = 0;
                        contador2 = 0;
                        sprintf(paginaInicial,"Timer = %d s",tempoPedestre);
                        lcd_puts(paginaInicial);
                        delay_ms(1500);
                        sprintf(paginaInicial,"Timer alterado com sucesso!");
                        delay_ms(2000); 
                }
                
                    // Altera timer para Carros
                    if(contador1 == 2){
                        contador1 = 0;
                        //Altera o valor das dezenas
                        while(PIND.5 == 0x1){
                            sprintf(paginaInicial,"Valor dezena: %d",contador1);
                            lcd_puts(paginaInicial);
                            delay_ms(200);
                            lcd_clear();
                            if(PIND.6 == 0x0){
                                break;
                            }
                            if(PIND.3 == 0x0){
                                contador1++;
                                delay_ms(200);
                            }
                            if(PIND.4 == 0x0){
                                contador1--;
                                delay_ms(200);
                            }
                        
                            // Delimita os valores da nossa dezena de acordo com o projeto do nosso circuito.
                            if(contador1 == 4){
                                contador1 = 0;
                            }
                            if(contador1 == -1){
                                contador1 = 3;
                            }
                        
                        }
                        delay_ms(200);
                        // Altera o valor das unidades
                        while(PIND.5 == 0x1){
                            sprintf(paginaInicial,"Valor unidades: %d%d",contador1,contador2);
                            lcd_puts(paginaInicial);
                            delay_ms(200);
                            lcd_clear();
                            if(PIND.6 == 0x0){
                                break;
                            }
                            if(PIND.3 == 0x0){
                                contador2++;
                                delay_ms(200);
                            }
                            if(PIND.4 == 0x0){
                                contador2--;
                                delay_ms(200);
                            }
                        
                            // Delimita os valores da nossa dezena de acordo com o projeto do nosso circuito.
                            if(contador2 == 10){
                                contador2 = 0;
                            }
                            if(contador2 == -1){
                                contador2 = 9;
                            }
                        
                        }
                        delay_ms(200);
                        // Novo valor de timer
                        tempoCarro = 10*contador1 + contador2;
                        contador1 = 0;
                        contador2 = 0;
                        sprintf(paginaInicial,"Timer = %d s",tempoCarro);
                        lcd_puts(paginaInicial);
                        delay_ms(1500);
                        sprintf(paginaInicial,"Timer alterado com sucesso!");
                        delay_ms(2000); 
                }
                    }                    

                if(PIND.3 == 0x0){
                    contadorMenu++;
                    delay_ms(200);
                    }
                if(PIND.4 == 0x0){
                    contadorMenu--;
                    delay_ms(200);
                } 
            }
            
            // Alterar senha
            if(contadorMenu == 3){
                sprintf(paginaInicial,"Alterar senha");
                lcd_puts(paginaInicial);
                delay_ms(200);
                lcd_clear();
                
                // Confirma o desejo de mudar a senha
                if(PIND.5 == 0x0){
                    delay_ms(500);
                    
                    // Altera o Primeiro Digito da Senha 
                    while(PIND.5 == 0x1){
                        sprintf(paginaInicial,"Primeiro digito: ");
                        lcd_puts(paginaInicial);
                        lcd_gotoxy(0,1);
                        sprintf(paginaInicial,"%d%",contador1);
                        lcd_puts(paginaInicial);    
                        delay_ms(200);
                        lcd_clear();
                        if(PIND.6 == 0x0){
                            break;
                        }
                        if(PIND.3 == 0x0){
                            contador1++;
                            delay_ms(200);
                        }
                        if(PIND.4 == 0x0){
                            contador1--;
                            delay_ms(200);
                        }
                    }
                    senha[0] = contador1;
                    delay_ms(200);
                    
                    // Altera o Segundo Digito da Senha
                    while(PIND.5 == 0x1){
                        sprintf(paginaInicial,"Segundo digito:");
                        lcd_puts(paginaInicial);
                        lcd_gotoxy(0,1);
                        sprintf(paginaInicial,"%d%d",contador1,contador2);
                        lcd_puts(paginaInicial);
                        delay_ms(200);
                        lcd_clear();
                        if(PIND.6 == 0x0){
                            break;
                        }
                        if(PIND.3 == 0x0){
                            contador2++;
                            delay_ms(200);
                        }
                        if(PIND.4 == 0x0){
                            contador2--;
                            delay_ms(200);
                        }
                    }
                    senha[1] = contador2;
                    delay_ms(200);
                    
                    // Altera o Terceiro Digito da Senha
                    while(PIND.5 == 0x1){
                        sprintf(paginaInicial,"Terceiro digito:");
                        lcd_puts(paginaInicial);
                        lcd_gotoxy(0,1);
                        sprintf(paginaInicial,"%d%d%d",contador1,contador2,contador3);
                        lcd_puts(paginaInicial);
                        delay_ms(200);
                        lcd_clear();
                        if(PIND.6 == 0x0){
                            break;
                        }
                        if(PIND.3 == 0x0){
                            contador3++;
                            delay_ms(200);
                        }
                        if(PIND.4 == 0x0){
                            contador3--;
                            delay_ms(200);
                        }
                    }
                    senha[2] = contador3;
                    delay_ms(200);
                    
                    // Altera o Quarto Digito da Senha
                    while(PIND.5 == 0x1){
                        sprintf(paginaInicial,"Quarto digito:");
                        lcd_puts(paginaInicial);
                        lcd_gotoxy(0,1);
                        sprintf(paginaInicial,"%d%d%d%d",contador1,contador2,contador3,contador4);
                        lcd_puts(paginaInicial);
                        delay_ms(200);
                        lcd_clear();
                        if(PIND.6 == 0x0){
                            break;
                        }
                        if(PIND.3 == 0x0){
                            contador4++;
                            delay_ms(200);
                        }
                        if(PIND.4 == 0x0){
                            contador4--;
                            delay_ms(200);
                        }
                    }
                    senha[3] = contador4;
                    contador1 = 0;
                    contador2 = 0;
                    contador3 = 0;
                    contador4 = 0;
                    sprintf(paginaInicial,"Nova senha: ");
                    lcd_puts(paginaInicial);
                    sprintf(paginaInicial,"%d%d%d%d",senha[0],senha[1],senha[2],senha[3]);
                    lcd_gotoxy(0,1);
                    lcd_puts(paginaInicial);
                    delay_ms(1500);
                    lcd_clear();
                    sprintf(paginaInicial,"Senha alterada com sucesso!");
                    lcd_puts(paginaInicial);
                    delay_ms(2000);
                    lcd_clear();                                    
                }
                if(PIND.3 == 0x0){
                    contadorMenu++;
                    delay_ms(200);
                }
                if(PIND.4 == 0x0){
                    contadorMenu--;
                    delay_ms(200);
                }
            }
            
            if(contadorMenu == 4){
                contadorMenu = 0;
            }
            
            if(contadorMenu == -1){
                contadorMenu = 3;
            }    
            
        }
      }
      
      
      
      
      
      
      
      else{
        sprintf(paginaInicial,"Senha Incorreta");
        lcd_puts(paginaInicial);
        contador1 = 0;
        contador2 = 0;
        contador3 = 0;
        contador4 = 0;
        delay_ms(1000);
        lcd_clear();
      }
      }  
       
}
