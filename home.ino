#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#define DEBUG
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

typedef struct {
  char names[10];
  int value;
  
} person;





// a variable of type datatype* can only equal and address of an existing variable. On its own its just a null address unless you use malloc
// the arrow means using the address of the struct to retrieve a struct variable eg x ->name

// instead of pointer use the word address
//a pointer holds an adress or holds nothing

person* hash_table[10];// creates 10 pointers for an instance of a person struct hashtable* is a pointer of type struct

// pointer like this => int *p has a null value (i do not store a memory location) *p=&num (this gets the memory location of num and gives it to *p
//you cant define char* to anyting unless it has been created already hence why its called dereferencing

unsigned int hash(char* nameChar){ //parameter is a pointer to a char/ this function creates an index number

  int len =strlen(nameChar);

  unsigned int hash_value =0;
  
  for (int i=0;i<len;i++){
    hash_value += nameChar[i];//the values of nameChar will be different based on their ascii values
    hash_value = (hash_value * nameChar[i]) % 10; // multiplies the hash value with the ascii value then returns the mod
  }

  return hash_value; 
}



void init_hash(){
  for(int i=0;i< 10; i++){
    hash_table[i]=NULL;//hashtable is an address and all values in the address [i]... are set to 0
  }
  
}

void table(){
  for(int i=0;i< 10; i++){
    if (hash_table[i]==NULL){//checks if address is empty
      Serial.println("ok");
    }else{
      Serial.println(hash_table[i]->names);//hast_table[i] will be the address of a person struct and so if not empty then retreive the variable of the struct at that address called name
    }
  }
}


bool hashTableInsert(person* p){ //expecting the address of a person variable
  if (p== NULL) return false; //if address is empty return false

  int index = hash (p->names); //uses the address to get the struct value
  
  
  if (hash_table[index] != NULL){//checks if hash_table is holding an adress
    return false;
  }
  
  hash_table[index]=p; //sets the adress of hashtble[i] to p
  return true;
}

person* hashLookUp (char* names){
  int index = hash(names);
 
  if (hash_table[index] !=NULL && strcmp(hash_table[index]->names, names)==0){
    return hash_table[index];
  }else{
    return NULL;
  }
}



char* toChar(String str){
  char s[10];
  str.trim();
  str.toCharArray(s,10);
  char* text=(char*) malloc(strlen(s)*sizeof(char));
  
  for(int i=0;i<strlen(s);i++){


    *text=s[i];//*text means the head of the value at the address and so will be the first letter in the address 
    text++;//increments the address number so the new head is the next letter
  }
  
  text-=strlen(s);//minuses the length from the address to return to the original head.
  

  return text;

 
}


void setup() {

  Serial.begin(9600);
  lcd.begin(16,2);

  person jacob={"Jacob",45};
  
  person kate={"Kate",27};
  person mpho={"Mpho",14};

  hashTableInsert(&jacob);
  hashTableInsert(&kate);
  hashTableInsert(&mpho);
  table();

  char* t=toChar("Jacob");
  person* h = hashLookUp(t);
  free(t);

  
  if (h==NULL){
    Serial.println("No");
  }else{
    Serial.println(h->names);
  }
 


  
























  


}


void loop() {
  // put your main code here, to run repeatedly:

}
