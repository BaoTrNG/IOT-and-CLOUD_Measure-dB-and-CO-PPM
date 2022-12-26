#include "arduino_secrets.h"
#include "thingProperties.h"
#include "MQ135.h"
#include <ESP_Mail_Client.h>
//#include <Wire.h>

const int sampleWindow = 50;                              
unsigned int sample;

#define PINMQ135 35
#define PINKY037 34
#define ledrgb 2
int EmailStart =0;
bool IsSent = false;
MQ135 mq135_sensor(PINMQ135);

SMTPSession smtp;

void smtpCallback(SMTP_Status status)
{
  Serial.println(status.info());
  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;
    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
  }
}

void setup() {
  pinMode(ledrgb,OUTPUT);

 analogReadResolution(10);
  Serial.begin(115200);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
    delay(8000);
}
void ReadPPM()
{
  ppm = mq135_sensor.getPPM();
  Serial.print(ppm);
  Serial.println("ppm");
}
void ReadSoundDB()
{
  unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
  
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(PINKY037);                    //get reading from microphone
      Serial.println(sample);
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   sound = map(peakToPeak,20,1023,49,120);             //calibrate for deciBels
  Serial.print(sound);
  Serial.println(" db");

}
void DB_EmailSender(float value)
{
  smtp.debug(1);
  smtp.callback(smtpCallback);
  ESP_Mail_Session session;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;

  message.sender.name = "WARNING";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "High Decibel Detected";
  message.addRecipient("", RECIPIENT_EMAIL);

  String htmlMsg;
  
  /*Send HTML message */
  if(value ==79)
  {
   htmlMsg = "<div style=\"color:#2f4468;\"><h1>DECIBEL IS HIGHER THAN 80!</h1><p></p></div>";
  }
  else if(value ==120)
  {
   htmlMsg = "<div style=\"color:#2f4468;\"><h1>DECIBEL IS HIGHER THAN 120!</h1><p>RUN</p></div>";
  }
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session))
    return;
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void PPM_EmailSender(float value)
{
  smtp.debug(1);
  smtp.callback(smtpCallback);
  ESP_Mail_Session session;

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;

  message.sender.name = "WARNING";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "High PPM Detected";
  message.addRecipient("", RECIPIENT_EMAIL);
  String htmlMsg;
  
  /*Send HTML message */
  if(value ==1000)
  {
   htmlMsg = "<div style=\"color:#2f4468;\"><h1>PPM LEVEL IS HIGHER THAN 1000!</h1><p></p></div>";
  }
  else if(value ==2000)
  {
   htmlMsg = "<div style=\"color:#2f4468;\"><h1>PPM LEVEL IS HIGHER THAN 2000!</h1><p>RUN</p></div>";
  }
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&session))
    return;
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void SendEmail()
{
  if(IsSent ==false)
  {
    CheckValue();
  } 
  EmailStart++;
  Serial.println(EmailStart);
  if(EmailStart >= 11)
  {
    IsSent = false;
    EmailStart = 0;
  }
  Serial.println(IsSent);
}
void LedControl()
{
  if(ppm >1000 || sound >78)
  {
  digitalWrite(ledrgb,HIGH);
  Serial.write("high");
  }
  else if(ppm < 1000 || sound <75)
  {
     digitalWrite(ledrgb,LOW);
       Serial.write("low");

  }
}
void CheckValue()
{
 if(ppm > 2000)
 {
  PPM_EmailSender(2000);
      IsSent = true;

 }
 else if(ppm >1000 && ppm <2000)
 {
  PPM_EmailSender(1000);
      IsSent = true;

 }


 if(sound > 120)
 {
  DB_EmailSender(120);
      IsSent = true;

 }
 else if(sound < 120 && sound >79)
 {
  DB_EmailSender(79);
      IsSent = true;

 }
}
void loop() 
{

  ReadPPM();
  ReadSoundDB();
  LedControl();
  SendEmail();
  ArduinoCloud.update();
  delay(1000);
}


void onGasChange()  
{
}
void onSoundChange()
{
}
void onPpmChange() 
{
}