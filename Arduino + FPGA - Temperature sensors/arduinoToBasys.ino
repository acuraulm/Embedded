#define analogTemperaturePin A0
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
#define binaryArray9 49

#define binaryArray8 48
#define binaryArray7 47
#define binaryArray6 46
#define binaryArray5 45
#define binaryArray4 44
#define binaryArray3 43
#define binaryArray2 42
#define binaryArray1 41
#define binaryArray0 40
#define recievedArray0 30
#define recievedArray1 31
#define recievedArray2 32
#define recievedArray3 33
#define recievedArray4 34
#define recievedArray5 35
#define recievedArray6 36
#define recievedArray7 37
#define recievedArray8 38
#define recievedArray9 39
#define redPin 8
#define greenPin 9
#define bluePin 10

int binaryArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int recievedArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int recievedDecimal = 0 ;

void setup() {
 pinMode(recievedArray0, INPUT);
 pinMode(recievedArray1, INPUT);
 pinMode(recievedArray2, INPUT);
 pinMode(recievedArray3, INPUT);
 pinMode(recievedArray4, INPUT);
 pinMode(recievedArray5, INPUT);
 pinMode(recievedArray6, INPUT);
 pinMode(recievedArray7, INPUT);
 pinMode(recievedArray8, INPUT);
 pinMode(recievedArray9, INPUT);
 pinMode(binaryArray0, OUTPUT);
 pinMode(binaryArray1, OUTPUT);
 pinMode(binaryArray2, OUTPUT);
 pinMode(binaryArray3, OUTPUT);
 pinMode(binaryArray4, OUTPUT);
 pinMode(binaryArray5, OUTPUT);
 pinMode(binaryArray6, OUTPUT);
 pinMode(binaryArray7, OUTPUT);
 pinMode(binaryArray8, OUTPUT);
 pinMode(binaryArray9, OUTPUT);
 pinMode(redPin, OUTPUT);
 pinMode(greenPin, OUTPUT);
 pinMode(bluePin, OUTPUT);

 recievedDecimal = 0;
 Serial.begin(9600);
 
}

void loop() {
  // Reads temperature sensor, converts the analog value into binary and sends it to the basys
  long analogTemperature = 1023 - analogRead(analogTemperaturePin)+ 220;
  Serial.print("Analog read and sent to basys: ");
  Serial.print(analogTemperature);
  Serial.println();
  long a = analogTemperature;
  //the calculating formula of temperature
  float tempC = (analogTemperature - 500) /10;
 // float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
 // Serial.print("Temp in C* from Arduino: ");
 // Serial.print(tempC);
 //s Serial.println();
  decimalToBinaryArray(analogTemperature);
  sendDataToBasys();
  //Reads the value sent from basys, converts it from binary to decimal
  //int recievedArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  readDataFromBasys();
  binaryArrayToDecimal(recievedArray);
  
  //Logic for Light-Switching LED
  lightLedDependingOnTemperature(recievedDecimal);

  for(int i = 0; i <= 10; i++){
    if(i==0){
      delay(100);
      Serial.print("Temp in C* from Basys: ");
      Serial.print(recievedDecimal);
      Serial.println();
    }
    if(i<10){
      delay(100);
      Serial.print(recievedArray[i]);
    }
    else
      Serial.println(); 
   }
  
}
void binaryArrayToDecimal(int binary[10]){
  recievedDecimal = 0;
  for(int i = 0; i < 10; i++)
    recievedDecimal = recievedDecimal*2 + binary[i];
}
void decimalToBinaryArray(long number){
  long i = 9;
  while(number != 0){
    binaryArray[i--] = number%2;
    number /= 2;
  }
}

void sendDataToBasys(){
      digitalWrite(binaryArray0 , binaryArray[0]);
      digitalWrite(binaryArray1 , binaryArray[1]);
      digitalWrite(binaryArray2 , binaryArray[2]);
      digitalWrite(binaryArray3 , binaryArray[3]);
      digitalWrite(binaryArray4 , binaryArray[4]);
      digitalWrite(binaryArray5 , binaryArray[5]);
      digitalWrite(binaryArray6 , binaryArray[6]);
      digitalWrite(binaryArray7 , binaryArray[7]);
      digitalWrite(binaryArray8 , binaryArray[8]);
      digitalWrite(binaryArray9 , binaryArray[9]);
}


void readDataFromBasys(){
    recievedArray[0] = digitalRead(recievedArray0);
    recievedArray[1] = digitalRead(recievedArray1);
    recievedArray[2] = digitalRead(recievedArray2);
    recievedArray[3] = digitalRead(recievedArray3);
    recievedArray[4] = digitalRead(recievedArray4);
    recievedArray[5] = digitalRead(recievedArray5);
    recievedArray[6] = digitalRead(recievedArray6);
    recievedArray[7] = digitalRead(recievedArray7);
    recievedArray[8] = digitalRead(recievedArray8);
    recievedArray[9] = digitalRead(recievedArray9);
}

void color(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void lightLedDependingOnTemperature(int temperature){
  if(temperature < 28){
   color(12,120,165);
  }
  else if(temperature >= 28 && temperature <= 30){
    
     color(255,88,0);
  }
  else if(temperature > 30){
    color(255,0 ,10);
  }
}

