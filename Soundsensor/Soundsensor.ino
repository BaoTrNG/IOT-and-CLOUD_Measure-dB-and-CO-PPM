int digital_Sound_pin = D7;
int analog_Sound_pin = A0;
int digital_value;
int analog_value;
void setup() 
{
  Serial.begin(115200);
   pinMode(digital_Sound_pin,INPUT);
   pinMode(analog_Sound_pin,INPUT);
}
void loop() 
    {
 digital_value = digitalRead(digital_Sound_pin);
 analog_value = analogRead(analog_Sound_pin);
 Serial.print("Digital_Output :");
 Serial.println(digital_value);
 Serial.print("Analog_Output :");
 Serial.println(analog_value);
delay(1000);
} 