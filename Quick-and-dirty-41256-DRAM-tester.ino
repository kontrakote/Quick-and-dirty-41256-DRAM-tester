
// DRAM tester
// Copyright 2014 - Len Bayes
// Works with 41265 DRAMS - Should work with 4146 (Same pinout minus the A8 address line
// Green LED goes on untill an error is detected.
// Red LED will blink after all test have been run
// A solid RED means the tests failed
// Blue LED goes on after all test have completed

 
 #define  DIN  2
 #define  DOUT 3
 #define  CAS  5
 #define  RAS  6
 #define  WE   7
 
 #define  GREEN    4
 #define  RED      19
 #define  BLUE     18

void  fillZero();
void  fillOne();
void  fillZeroAlternateOne();
void  fillOneAlternateZero();

void setup() 
  {
  pinMode(DIN, OUTPUT);      // Data to DRAM
  pinMode(DOUT, INPUT);      // Data from DRAM

  pinMode(CAS, OUTPUT);
  pinMode(RAS, OUTPUT);
  pinMode(WE, OUTPUT);

  DDRB = B00111111;          // DRAM A0 through A5  
  pinMode(14, OUTPUT);       // DRAM A6
  pinMode(15, OUTPUT);       // DRAM A7
  pinMode(16, OUTPUT);       // DRAM A8
  
  digitalWrite(CAS, HIGH);
  digitalWrite(RAS, HIGH);  
  digitalWrite(WE, HIGH);
  
  pinMode(GREEN, OUTPUT);     // Green LED
  pinMode(RED, OUTPUT);       // Red Led
  pinMode(BLUE, OUTPUT);       // Blue Led
  
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  }

int  blue = 0;

void loop() 
  {
  int  i;
  
  
  fillZero();
  fillOne();
  fillZeroAlternateOne();
  fillOneAlternateZero();
  
  for(i = 10 ; i ; i--)
    {
    digitalWrite(RED, HIGH);
    delay(500);
    digitalWrite(RED, LOW);
    delay(500);
    }
    
  blue = 1 << 4;
  }
  

// TEST ONE  
void  fillZero()
    {
    int  row, col, miss = 0;  

  
  // First write all Zeros to DRAM
  
  digitalWrite(DIN, LOW);
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue;  
      digitalWrite(RAS, LOW);         // Set RAS LOW
      digitalWrite(WE, LOW);         // Set WE low for write
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(CAS, LOW);        // Set CAS LOW
      
      digitalWrite(WE, HIGH);         // Set WE high to complete write
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }


  // Read back and check for Zero
  digitalWrite(DIN, HIGH);
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue;  
      digitalWrite(CAS, LOW);        // Set CAS LOW
      
      if(digitalRead(DOUT) == HIGH)
        miss = 1;
        
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }

    if(miss)
      {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      
      while(1)
        ;
      }
   }
   

// TEST TWO 
void  fillOne()
    {
    int  row, col, miss = 0;  

  
  // First write all Ones to DRAM
  
  digitalWrite(DIN, HIGH);
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      digitalWrite(WE, LOW);         // Set WE low for write
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(CAS, LOW);        // Set CAS LOW
      
      digitalWrite(WE, HIGH);         // Set WE high to complete write
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }
    

  // Read back and check for One
  digitalWrite(DIN, LOW);
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(CAS, LOW);        // Set CAS LOW
      
      if(digitalRead(DOUT) == LOW)
        miss = 1;
        
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }

    if(miss)
      {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      
      while(1)
        ;
      }

   }
   
   
 // TEST THREE
 void  fillZeroAlternateOne()
    {
    int  row, col, i = 0, miss = 0;  
    
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      digitalWrite(DIN, i);
      if(i)
        i = 0;
      else
        i = 1;
        
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      digitalWrite(WE, LOW);         // Set WE low for write
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(CAS, LOW);        // Set CAS LOW
      
      digitalWrite(WE, HIGH);         // Set WE high to complete write
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }
    
  i = 0;
  
  // Read back and check for One
  digitalWrite(DIN, LOW);
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(CAS, LOW);        // Set CAS LOW
      

     
      if(i)
        {
        i = 0;
         
        if(digitalRead(DOUT) == LOW)
          miss = 1;
        }
      else
      {
        i = 1;

        if(digitalRead(DOUT) == HIGH)
          miss = 1;       
       }
        
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }

    if(miss)
      {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      
      while(1)
        ;
      }
   }
   

// TEST FOUR 
void  fillOneAlternateZero()
    {
    int  row, col, i = 1, miss = 0;  
    
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      digitalWrite(DIN, i);
      if(i)
        i = 0;
      else
        i = 1;
        
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      digitalWrite(WE, LOW);         // Set WE low for write
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(CAS, LOW);        // Set CAS LOW
      
      digitalWrite(WE, HIGH);         // Set WE high to complete write
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }
    
  i = 1;
  
  // Read back and check for One
  digitalWrite(DIN, LOW);
  
  for(col = 0 ; col < 512 ; col++)
    {
    for(row = 0 ; row < 512 ; row++)
      {
      PORTB = row & 0x3f;            // Write out ROW address
      PORTC = ((col & 0x1c0) >> 6) | blue; 
      digitalWrite(RAS, LOW);         // Set RAS LOW
      
      PORTB = col & 0x3f;            // Write out COL address
      PORTC = ((col & 0x1c0) >> 6) | blue;  
      digitalWrite(CAS, LOW);        // Set CAS LOW
      

     
      if(i)
        {
        i = 0;
         
        if(digitalRead(DOUT) == LOW)
          miss = 1;
        }
      else
      {
        i = 1;

        if(digitalRead(DOUT) == HIGH)
          miss = 1;       
       }
        
      digitalWrite(CAS, HIGH);        // Set CAS HIGH
      digitalWrite(RAS, HIGH);         // Set RAS HIGH
      } 
    }

    if(miss)
      {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      
      while(1)
        ;
      }
   }
