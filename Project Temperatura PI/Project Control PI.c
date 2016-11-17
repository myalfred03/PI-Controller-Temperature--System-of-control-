#include <16f88.h>//pic a utilizar
#device adc=10
#FUSES NOWDT, HS, NOPUT,NOBROWNOUT, NOLVP, NOCPD, NOWRT, NODEBUG, NOPROTECT, FCMEN, IESO
#use delay (internal=4MHZ)
#include <math.h>
#include "flex_lcd.c"  //Llamada a la libreria lcd.c
int16 valor;
float temp,temp1,Ptemp[4];
int contRTCC=0;
int1 val=0;

//T=(4/fosc * PREESCALER )(256-Carga en el timer)   T-> tiempo de desborde del timer.


#int_timer0
void int_tiempo0(void){
++contRTCC;
if(contRTCC==20){
valor=read_adc();
Ptemp[0]=temp;
if (val==1){
val=0;
}else{
val=1;
}
}
if(contRTCC==40){
valor=read_adc();
Ptemp[1]=temp;
delay_ms(1);
}
if(contRTCC==60){
valor=read_adc();
Ptemp[2]=temp;
delay_ms(1);
}
if(contRTCC==80){
valor=read_adc();
Ptemp[3]=temp;
delay_ms(1);
contRTCC=0;
temp1=(Ptemp[0]+Ptemp[1]+Ptemp[2]+Ptemp[3])/4;
}

 
    if(val==1){
   output_high(pin_a0);

   }else{
   output_low(pin_a0);
   }

}





void main(){
float y,rtdv,rtdr,LSB;
setup_comparator(NC_NC_NC_NC);
setup_vref(FALSE);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_8_BIT|RTCC_DIV_128);
   setup_timer_1(T1_DISABLED);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(global);
set_tris_A(0x10);
set_tris_b(0x08);
setup_adc_ports(sAN4|VSS_VDD);
setup_adc(ADC_CLOCK_INTERNAL);
set_adc_channel(4);
valor=read_adc();
delay_ms(30);
lcd_init();                      //Inicializamos el LCD.
lcd_putc("\fTemp. =");      //
LSB=5/1024.0; 
while(1){
rtdv=(valor*LSB)/48.40;                    /* Calculate volt rtdr  */
rtdr = (rtdv*10000.0)/(5.0-rtdv);          /* Calculate RTD resistance rtdr  */ 
y = (0.15274) - ((rtdr-100.0)*(2310.0e-7)); 
y = sqrt(y) ;
temp = (y-0.39083)/(-0.0001155);
lcd_gotoxy(10,1);
printf(lcd_putc,"%4.2f",temp);
delay_ms(300);
lcd_gotoxy(2,2);
printf(lcd_putc,"TEMP %4.2f",temp1);
delay_ms(200);
}
}
