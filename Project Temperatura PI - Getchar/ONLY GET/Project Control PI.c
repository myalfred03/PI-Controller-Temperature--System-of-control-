#include <16f88.h>//pic a utilizar
#device adc=10
#FUSES NOWDT, HS NOPUT,NOBROWNOUT,MCLR, NOLVP, NOCPD, NOPROTECT,INTRC_IO,CCPB0,NOPUT //si pongo CCPB3
#BYTE OSCCON = 0b01110000
#use delay (internal=8MHZ)
#include "flex_lcd.c"  //Llamada a la libreria lcd.c
#use rs232(baud=9600,BITS =8,PARITY=N, xmit=PIN_B5, rcv=PIN_B2, stream=HOSTPC,errors)
INT contRTCC,datoTX[2];
int16 valor;
char c;

//T=(4/fosc * PREESCALER )(256-Carga en el timer)   T-> tiempo de desborde del timer.

//INTERRUPCION RDA RECEPCION DE DATOS SERIE
#int_RDA
VOID serial_ISR()
{
c=fgetc(hostpc);
Printf("\rjaja%d",C);
}



//ENVIO DE DATOS SERIE
void envioUART(int16 ADC){
   datoTX[0]=ADC >> 8;     //Asignacion de desplazamiento hacia la derecha                                            
   datoTX[1]=ADC & 0xFF;   //Asignacion AND de Bits  
   putc(datoTX[0]);           
   putc(datoTX[1]);          //Envio de Datos 
}

//!#int_timer0
//!void int_tiempo0(void){
//!++contRTCC;
//!if(contRTCC==10){
//!output_high(pin_a0);
//!delay_ms(10);
//!valor=read_adc();
//!
//!}
//!if(contRTCC==20){
//!valor=read_adc();
//!output_toggle(PIN_A0);
//!delay_ms(10);
//!}
//!if(contRTCC==30){
//!valor=read_adc();
//!output_toggle(PIN_A0);
//!delay_ms(10);
//!}
//!if(contRTCC==40){
//!valor=read_adc();
//!   contRTCC=0;
//!}
//!
//!
//! // output_low(pin_a0);
//!
//!}

void main(void){
char a;
//float y,rtdv,rtdr,LSB;
setup_oscillator(OSC_8MHZ | OSC_INTRC);
setup_comparator(NC_NC_NC_NC);
   enable_interrupts(INT_RDA);
   enable_interrupts(global);
set_tris_A(0x10);
set_tris_b(0x04);
setup_adc(ADC_CLOCK_INTERNAL);
set_adc_channel(4);
lcd_init();                      //Inicializamos el LCD.
lcd_putc("\fControl =");      //

    //  do {
    while(1)
    {
          
     //c=fgetc(HOSTPC);
     a=c;
     delay_us(100);
          //} while(a!='1' && a!='2' && a!='3');
 switch (a)
           {
            case '1': 
            {
            
            printf(lcd_putc,"PI1 %c",a);
            envioUART(valor);
            }
   
          }  //end Switch
}
}
