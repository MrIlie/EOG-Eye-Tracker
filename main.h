
#define SAMPLERATE   200                // Frecventa de esantionare semnal / canal
#define READINGS     20                 // Interval pt calculul mediei semnalului   
#define INACTIVE_CMD 20                 // Interval inactivare comanda noua t = 0.1sec
#define READING_INT  5                  // Durata in ms intre esantionari succesive 

#define XTH_LIMIT_PLUS 60               // Limitele de variatie a pragurilor
#define XTH_LIMIT_MINUS 60
#define YTH_LIMIT_PLUS 60
#define YTH_LIMIT_MINUS 60
 




#ifndef HEADER_EYEBLINK
  #define HEADER_EYEBLINK
  
  void EyeBlink( int );
   
#endif


