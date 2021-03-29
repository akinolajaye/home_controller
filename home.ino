#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <EEPROM.h>
#include <avr/eeprom.h>
#define DEBUG
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
int HOUSE_LEN=10;

void writeEE(){//function to write house description into eeprom


  char floors[5]={'F','G','O','-'};
  char first[6]={'1','2','B','P','-'};
  char ground[7]={'K','H','L','3','4','-'};
  char outside[4]={'G','R','-'};
  char type[6]={'L','A','H','W','-'};
  char names[8]={'M','C','D','B','P','W','-'};
  char qualifier[7]={'1','2','3','L','R','-'};
  char action[5]={'1','0','L','-'};
  char  val[4]={'T','R','x'};
  char* house[HOUSE_LEN]={floors,first,ground,outside,type,names,qualifier,action,val};//stores pointers to the different char arrays
  int totalLen=0;

  EEPROM.write(totalLen,1); //writes a 1 at the first memory location to signify data has been written to
  for(int i=0;i<HOUSE_LEN-1;i++){
    for(int j=0;j<strlen(house[i]);j++){
      totalLen++;
      EEPROM.write(totalLen,house[i][j]);
    }
  }

    

  



}

void readEE(char** house){//reads the house desc from the eeprom

     
    for(int i=0;i<HOUSE_LEN-1;i++){
      int j=0;

      while((char) EEPROM.read(count)!='-'){
        
        if((char) EEPROM.read(count)=='x'){break;}
   
        house[i][j]=(char) EEPROM.read(count);
        j++;
        count++;
        
      }
      count++;
       
    }
       
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);

  char floors[4]="";
  char first[5]="";
  char ground[6]="";
  char outside[3]="";
  char type[5]="";
  char names[7]="";
  char qualifier[6]="";
  char action[4]="";
  char  val[2]="";
  char* house[HOUSE_LEN]={floors,first,ground,outside,type,names,qualifier,action,val};
  int written=EEPROM.read(0);// reads the first value in the eeprom 
  

  if (written ==0){
    writeEE();
    Serial.println("done");
  }else{
    Serial.println("Already exits");
  }
  
  readEE(house);
  Serial.print(floors);

  

  


}

void loop() {
  // put your main code here, to run repeatedly:

}
