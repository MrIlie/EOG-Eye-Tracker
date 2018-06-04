
// 14-05-2018

#include <Keyboard.h>
#include <CircularBuffer.h>
#include <SimpleTimer.h>


#define SAMPLERATE   200                // Frecventa de esantionare semnal / canal
#define READINGS     20                 // Interval pt calculul mediei semnalului   
#define INACTIVE_CMD 20                 // Interval inactivare comanda noua t = 0.1sec
#define READING_INT  5                  // Durata in ms intre esantionari succesive 

#define XTH_LIMIT_PLUS 60               // Limitele de variatie a pragurilor
#define XTH_LIMIT_MINUS 60
#define YTH_LIMIT_PLUS 25
#define YTH_LIMIT_MINUS 25
 

unsigned int x_threshold_up = 0;        // Praguri detectie comparatoare
unsigned int x_threshold_dn = 0; 
unsigned int y_threshold_up = 0; 
unsigned int y_threshold_dn = 0; 

SimpleTimer read_data;


CircularBuffer<int,SAMPLERATE> x_sampleBuf;
CircularBuffer<int,SAMPLERATE> y_sampleBuf;

 int analog_xValue, analog_yValue;     // Valori analogice de semnal de la convertor 

int read_time = 0;

unsigned long x_mean_filt = 512;        // Valori initiale la mijlocul gamei 
unsigned long y_mean_filt = 512;
unsigned long x_last_mean_filt = 512;
unsigned long y_last_mean_filt = 512;

unsigned long xdc_comp = 512;         
unsigned long ydc_comp = 512;


bool debug_values = false;
bool debug_plotter = true;


bool active_cmd_xAxis = true;
bool active_cmd_yAxis = true;
char x_cmd_cnt = 0;                   // Contor inactivare comanda axa x
char y_cmd_cnt = 0;                   // Contor inactivare comanda axa y

unsigned char x_stare = 0;
unsigned char y_stare = 0;

int _timer = 0;
int reset_duration = 0;

void setup() {

  Serial.begin(9600);
 // Keyboard.begin();
  for(int i = 0; i < SAMPLERATE; i++)
  {
    x_sampleBuf.push(512);
    y_sampleBuf.push(512);
  }
                                  
  x_threshold_up = xdc_comp + (xdc_comp * XTH_LIMIT_PLUS);    // Setare praguri detectie comparatoare
  x_threshold_dn = xdc_comp - (xdc_comp * XTH_LIMIT_MINUS);       
  y_threshold_up = ydc_comp + (ydc_comp * YTH_LIMIT_PLUS);        
  y_threshold_dn = ydc_comp - (ydc_comp * YTH_LIMIT_MINUS);       

  //Serial.println(sample_buffer.size());
  read_data.setInterval(READING_INT,readADC_values);
}


void Xcheck(int xValue)
{
  
   //set starea s1
   if( xValue > x_threshold_up && !x_stare)
   {
      x_stare = 1;
      Serial.println("right");
 //   Keyboard.press(KEY_RIGHT_ARROW);
    //Keyboard.press('d');
//    Keyboard.releaseAll();
      
   }

   //set starea s2
   if( xValue < x_threshold_dn && !x_stare)
   {
      x_stare = 2;
      Serial.println("left");
 //   Keyboard.press(KEY_RIGHT_ARROW);
    //Keyboard.press('d');
//    Keyboard.releaseAll();
      
   }
  
   //reset in starea s0
   if(( xValue <= x_threshold_dn && x_stare == 1) || 
   ( xValue >= x_threshold_up && x_stare == 2))
   {
      x_stare = 0;
   }
  
  
}  
  
  
void Ycheck(int yValue)
{
  
}
 

void readADC_values() {
  
    analog_xValue = analogRead(A5);
    analog_yValue = analogRead(A6);
    
    x_sampleBuf.shift();                          // Eliminam din buffer prima valoare
    x_sampleBuf.push(analog_xValue);              // Adaugam in buffer pe ultima pozitie
    y_sampleBuf.shift();                          // Eliminam din buffer prima valoare
    y_sampleBuf.push(analog_yValue);              // Adaugam in buffer pe ultima pozitie


    xdc_comp = 0;   ydc_comp = 0;         
    for(int i = 0; i < SAMPLERATE; i++)
      xdc_comp += x_sampleBuf[i];
    xdc_comp = xdc_comp / SAMPLERATE;

    x_threshold_up = xdc_comp + XTH_LIMIT_PLUS;   // Recalculare praguri detectie comparatoare
    x_threshold_dn = xdc_comp - XTH_LIMIT_MINUS;       
    y_threshold_up = ydc_comp + YTH_LIMIT_PLUS;        
    y_threshold_dn = ydc_comp - YTH_LIMIT_MINUS;      
    

    x_last_mean_filt = x_mean_filt;               // Salvare ultima valoare filtru medie 
    x_mean_filt = 0;            
    for(int i = SAMPLERATE-1; i > SAMPLERATE-READINGS-1; i--)
      x_mean_filt += x_sampleBuf[i];
    x_mean_filt = x_mean_filt / READINGS;

    y_last_mean_filt = y_mean_filt;               // Salvare ultima valoare filtru medie 
    y_mean_filt = 0;            
    for(int i = SAMPLERATE-1; i > SAMPLERATE-READINGS-1; i--)
      y_mean_filt += y_sampleBuf[i];
    y_mean_filt = y_mean_filt / READINGS;

    Xcheck(x_mean_filt);
    Ycheck(y_mean_filt); 
    
     
    /*Serial.print(0);
    Serial.print(",");
    Serial.print(1023);
    Serial.print(",");
    Serial.print(x_threshold_up);
    Serial.print(",");
    Serial.print(x_threshold_dn);
    Serial.print(",");
    Serial.println(x_mean_filt);*/

   
}



void loop() {
  
  // simulation
  read_data.run();
  
  
}
