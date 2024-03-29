#include <16f88.h>//pic a utilizar
#device adc=10
#FUSES NOWDT, HS NOPUT,NOBROWNOUT,MCLR, NOLVP, NOCPD, NOPROTECT,INTRC_IO,CCPB0 //si pongo CCPB3
#BYTE OSCCON = 0b01110000
#use delay (internal=8MHZ)
#include <math.h>
#include "flex_lcd.c"  //Llamada a la libreria lcd.c
#use rs232(baud=9600,BITS =8,PARITY=N, xmit=PIN_B5, rcv=PIN_B2)
unsigned int contRTCC=0,set_point=60;
float LSB=0.004882,y,rtdv,rtdr;
float a=9.53,b=0.918/0.9,rkt,ekt,pkt,qkt,ukt,MA=1000.0,MI=0.0,pkt_1=0.0,ekt_1=0.0;
int16 valor,Control,Duty;
float temp1,Ptemp[4],D;
int datoTX[2];
short val=0;

//T=(4/fosc * PREESCALER )(256-Carga en el timer)   T-> tiempo de desborde del timer.
 

float calculos_rtd(int16 ADC) {
                float temp;
rtdv=(ADC*LSB)/48.40;                    /* Calculate volt rtdr  */
rtdr = (rtdv*10000.0)/(5.0-rtdv);          /* Calculate RTD resistance rtdr  */ 
y = (0.15274) - ((rtdr-100.0)*(2310.0e-7)); 
y = sqrt(y) ;
temp = (y-0.39083)/(-0.0001155);
    return temp;
}

float calculos_pid(float temp) {
float duty;
rkt=set_point;

  /* Calculate error */
  ekt=rkt-temp;
  /*end  Calculate error */

  /* Calculate I term */
  pkt=b*ekt+pkt_1;
  /*end  Calculate I term */

  /* Calculate PI output */
   ukt=pkt+a*ekt;
  /*end Calculate PI output */

  if(ukt > MA) {
  pkt=pkt_1;
  ukt =MA;     }
  else if(ukt < MI)
  { pkt=pkt_1;
  ukt=MI;      }

  duty=ukt;

  pkt_1 = pkt ;
  ekt_1= ekt;
                
    return duty;        
}


#int_timer0
void int_tiempo0(void){
++contRTCC;
if(contRTCC==10){
output_high(pin_a0);
delay_ms(10);
valor=read_adc();
Ptemp[0]=calculos_rtd(valor);
 
//output_toggle(PIN_A0);
}
if(contRTCC==20){
valor=read_adc();
Ptemp[1]=calculos_rtd(valor);;
delay_ms(1);
output_toggle(PIN_A0);
delay_ms(10);
}
if(contRTCC==30){
valor=read_adc();
Ptemp[2]=calculos_rtd(valor);;
delay_ms(1);
output_toggle(PIN_A0);
delay_ms(10);
}
if(contRTCC==40){
valor=read_adc();
Ptemp[3]=calculos_rtd(valor);;
delay_ms(1);
output_toggle(PIN_A0);
delay_ms(10);
temp1=(Ptemp[0]+Ptemp[1]+Ptemp[2]+Ptemp[3])/4;
   contRTCC=0;
}


 // output_low(pin_a0);

}

void main(){
char c;
//float y,rtdv,rtdr,LSB;
setup_oscillator(OSC_8MHZ | OSC_INTRC);
setup_comparator(NC_NC_NC_NC);
setup_vref(FALSE);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_8_BIT|RTCC_DIV_128);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DIV_BY_4, 250, 1); 
   setup_ccp1(CCP_PWM); 
   enable_interrupts(INT_TIMER0);
   enable_interrupts(global);
set_tris_A(0x10);
set_tris_b(0x08);
setup_adc_ports(sAN4|VSS_VDD);
setup_adc(ADC_CLOCK_INTERNAL);
set_adc_channel(4);
lcd_init();                      //Inicializamos el LCD.
lcd_putc("\fDuty. =");      //
while(1){

   D=calculos_pid(temp1);
   Duty=(int16)D;
   Control=((1000-Duty));
   set_pwm1_duty(Control);

lcd_gotoxy(10,1);
printf(lcd_putc,"%lu",Control);
delay_ms(300);
lcd_gotoxy(2,2);
printf(lcd_putc,"TEMP %4.2f",temp1);
delay_ms(200);

datoTX[0]=valor >> 8;   //Asignacion de desplazamiento
                             //hacia la derecha
datoTX[1]=valor & 0xFF; //Asignacion AND de Bits  
            //Tiempo de Muestreo Se�al
   putc(datoTX[0]);           
   putc(datoTX[1]);          //Envio de Datos
}
}
