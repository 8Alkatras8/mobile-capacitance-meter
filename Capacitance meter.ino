#define analogPin       0          
#define chargePin1      14
#define chargePin2      15
#define chargePin3      16              
#define resistorValue1  118.0F          //118
#define resistorValue2  9600.0F         //9500 
#define resistorValue3  1500000.0F      //1500000

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;
float picoFarads;

int DI = 12;
int RW = 11;
int DB[] = {3, 4, 5, 6, 7, 8, 9, 10};// массив для выбора портов для шины
int Enable = 2;
// *************************************ДИСПЛЕЙ ИНИЦИАЛИЗАЦИЯ****************************
void LcdCommandWrite(int value) {
// define all pins
int i = 0;
for (i=DB[0]; i <= DI; i++) // assign value for bus
  {
    digitalWrite(i,value & 01);// for 1602 LCD, it uses D7-D0( not D0-D7) for signal identification; here, it’s used for signal inversion. 
    value >>= 1;
  }
digitalWrite(Enable,LOW);
delayMicroseconds(1);
digitalWrite(Enable,HIGH);
delayMicroseconds(1);  // задержка 1ms
digitalWrite(Enable,LOW);
delayMicroseconds(1);  // задержка 1ms
}

void LcdDataWrite(int value) {  
int i = 0;
digitalWrite(DI, HIGH);
digitalWrite(RW, LOW);
for (i=DB[0]; i <= DB[7]; i++) {
   digitalWrite(i,value & 01);
   value >>= 1;
}
digitalWrite(Enable,LOW);
delayMicroseconds(1);
digitalWrite(Enable,HIGH);
delayMicroseconds(1);
digitalWrite(Enable,LOW);
delayMicroseconds(1);  // wait for 1ms
}
// *************************************ДИСПЛЕЙ НАСТРОЙКИ ВЫВОДА****************************
void LcdFloatWrite(float value)
{
long i=0;        //переменная для расчета
int k=0;         //одна цифра разряда для вывода на дисплей
int l=0;         // определение ведущих нулей
float ii=0;      // дробная часть
long z=100000;   //делитель
i=long(value);
ii=10*(value-i);
ii=int(ii);
while(z>=1)
  {
   k=int(i/z);
   if ((l==0)&(k==0)&(z!=1)) 
    {
      LcdDataWrite(32);  //вывод пробела вместо ведущих нулей
    }
   else
    {
      LcdDataWrite(48+k); //вывод значащих цифр
      l=1;
    }
   if(i>=k*z){i=i-k*z;};
   z=z/10; 
  }
if (ii!=0)  // отображение дробной части
  {
    LcdDataWrite(',');
    LcdDataWrite(48+ii);
  }
}
// *************************************ДИСПЛЕЙ НАСТРОЙКА****************************
void setup (void) {
int i = 0;
for (i=Enable; i <= DI; i++) {
   pinMode(i,OUTPUT);
}
delay(100);
// инициализация LCD

LcdCommandWrite(0x38);  // выбор 8-битного интерфейса, 2-строчног дисплея, 5x7 размер символа 
delay(64);                                      
LcdCommandWrite(0x06);  // установка режима вывода с автоинкрементом и без сдвига
delay(20);                      
LcdCommandWrite(0x0E);  // установка дисплея, монитор включен курсор не мигает
delay(20);                      
LcdCommandWrite(0x01);  // очистка дисплея, возврат курсора в начало;
delay(100);                      
LcdCommandWrite(0x80);  // установка дисплея, монитор включен курсор не мигает

delay(20); 
  pinMode(chargePin1, INPUT); 
  pinMode(chargePin2, INPUT); 
  pinMode(chargePin3, INPUT); 
// *************************************ЛОГО********************************************
               LcdCommandWrite(0x01);  // clear the screen, cursor position returns to 0 
               delay(2);     
               LcdCommandWrite(0x80+2); 
               delay(2);        
               LcdDataWrite('D');
               LcdDataWrite('e');
               LcdDataWrite('v');
               LcdDataWrite('e');
               LcdDataWrite('l');
               LcdDataWrite('o');
               LcdDataWrite('p');
               LcdDataWrite('e');
               LcdDataWrite('d');
               LcdDataWrite(' ');
               
               LcdDataWrite('b');
               LcdDataWrite('y');
               LcdCommandWrite(0xc0+2);  // set cursor position at second line, second position
               LcdDataWrite('P');
               LcdDataWrite('i');
               LcdDataWrite('s');
               LcdDataWrite('a');
               LcdDataWrite('r');
               LcdDataWrite(' ');
               LcdDataWrite('A');
               LcdDataWrite('l');
               LcdDataWrite('e');
               LcdDataWrite('x');
               LcdDataWrite('i');
               LcdDataWrite('s');
               
delay(3000);
// *************************************ТЕЛО********************************************              
}
void loop (void) {
  pinMode(chargePin1, OUTPUT); 
  pinMode(chargePin2, INPUT); 
  pinMode(chargePin3, INPUT); 
  digitalWrite(chargePin1, HIGH);  
  startTime = micros();
  while(analogRead(analogPin) < 648){}
  elapsedTime= micros() - startTime-112;
  if (elapsedTime > 500)
     {
       microFarads = ((float)elapsedTime / resistorValue1);   //измеряем конденсаторы от 5 до 10000 микрофарад
       LcdCommandWrite(0x01);  // очистка дисплея, возврат курсора в начало;
       delay(2);     
       LcdCommandWrite(0x80+1); 
       delay(2); 
       if (microFarads>1000)   //обнуление мало значащих разрядов
         {
         microFarads=(10*long(microFarads/10)); 
         }
       else
         {
          if (microFarads>100)
           {
             microFarads=long(microFarads); 
           }
         }
       LcdFloatWrite(microFarads);
       LcdCommandWrite(0x80+10); 
       delay(2);               
       LcdDataWrite('u');
       LcdDataWrite('F');
     }         
  else     // Подключаем резистор2
     {
       digitalWrite(chargePin1, LOW);     //Разрядка конденсатора
       while(analogRead(analogPin) > 0){};
       pinMode(chargePin1, INPUT); 
       pinMode(chargePin2, OUTPUT); 
       pinMode(chargePin3, INPUT); 
       digitalWrite(chargePin2, HIGH);  
       startTime = micros();
       while(analogRead(analogPin) < 648){}
       elapsedTime= micros() - startTime-112;
       if (elapsedTime > 500)
         {
           nanoFarads = ((float)elapsedTime / resistorValue2*1000); 
           if (nanoFarads<1000)
             {  
               LcdCommandWrite(0x01);  // очистка дисплея, возврат курсора в начало;
               delay(2);     
               LcdCommandWrite(0x80+1); 
               delay(2);                
               LcdFloatWrite( (long)nanoFarads);
               LcdCommandWrite(0x80+10); 
               delay(2);               
               LcdDataWrite('n');
               LcdDataWrite('F');
             }
           else
             {
               LcdCommandWrite(0x01);  // очистка дисплея, возврат курсора в начало;
               delay(2);     
               LcdCommandWrite(0x80+1); 
               delay(2);                
               LcdFloatWrite(nanoFarads/1000);     
               LcdCommandWrite(0x80+10); 
               delay(2);               
               LcdDataWrite('u');
               LcdDataWrite('F');
             }   
         }
       else   //Подключаем резистор3
         {
           pinMode(chargePin1, OUTPUT);       //Разрядка конденсатора
           pinMode(chargePin2, INPUT); 
           pinMode(chargePin3, INPUT); 
           digitalWrite(chargePin1, LOW);     
           while(analogRead(analogPin) > 0){};
           pinMode(chargePin1, INPUT); 
           pinMode(chargePin3, OUTPUT); 
           digitalWrite(chargePin3, HIGH);  
           startTime = micros();
           while(analogRead(analogPin) < 648){}
           elapsedTime= micros() - startTime;
           if (elapsedTime > 250)
             {
               picoFarads = ((float)(elapsedTime-235) / resistorValue3*1000000); 
               if (picoFarads<1000)
                 {  
                   LcdCommandWrite(0x01);  // очистка дисплея, возврат курсора в начало;
                   delay(2);     
                   LcdCommandWrite(0x80+1); 
                   delay(2);                
                   LcdFloatWrite( (long)picoFarads);     
                   LcdCommandWrite(0x80+10); 
                   delay(2);               
                   LcdDataWrite('p');
                   LcdDataWrite('F');
                 }
               else
                 {
                   LcdCommandWrite(0x01);  // clear the screen, cursor position returns to 0;
                   delay(2);     
                   LcdCommandWrite(0x80+1); 
                   delay(2);                
                   LcdFloatWrite(picoFarads/1000);     
                   LcdCommandWrite(0x80+10); 
                   delay(2);               
                   LcdDataWrite('n');
                   LcdDataWrite('F');
                 }     
             }
           else   //подключенная ёмкость меньше предела измерения
             {
               LcdCommandWrite(0x01);  // clear the screen, cursor position returns to 0 
               delay(2);     
               LcdCommandWrite(0x80+5); 
               delay(2);        
               LcdDataWrite('R');
               LcdDataWrite('e');
               LcdDataWrite('a');
               LcdDataWrite('d');
               LcdDataWrite('y');
             }
         }
     }
 pinMode(chargePin1, OUTPUT);       //Разрядка конденсатора
 pinMode(chargePin2, INPUT); 
 pinMode(chargePin3, INPUT); 
 digitalWrite(chargePin1, LOW);     
 while(analogRead(analogPin) > 0){};
 pinMode(chargePin1, INPUT); 
 delay(500); 
}
