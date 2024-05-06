#include <WiFi.h>       // standard library
#include <WebServer.h>  // standard library
#include "SuperMon.h"   // .h file that stores your html page code

#include <Arduino.h>    // standard library
#include <SPI.h>        // standard library

// Coment out to use ESP32 as an access point
#define USE_INTRANET

// replace this with your homes intranet connect parameters
// DEEC , uc2020252382@student.uc.pt , Joao2002
// HUAWEI-89UJQF_HiLink , estudantes
// ESPwlan, 12344321
#define LOCAL_SSID "HUAWEI-89UJQF_HiLink"
#define LOCAL_USER "uc2020252382@student.uc.pt"
#define LOCAL_PASS "estudantes"

// Settings for tne access point
#define AP_SSID "TestWebSite"
#define AP_PASS "023456789"

// start your defines for pins for SPI, outputs, inputs, etc.
#define VSPI_MISO 33
#define VSPI_MOSI 25
#define VSPI_SCLK 26
#define VSPI_SS 27



// initialize functions
void printWifiStatus();     // Print WiFi settings and status
void SendXML();             // Send the XML table to the Web
void SendWebsite();         // Send HTML code to create a Web page
void UpdatePWM();           // Callback when receives new PWM(slider) Value
void ProcessForward_0();    // Callback when receives Forward = 0
void ProcessForward_1();    // Callback when receives Forward = 1
void ProcessBackward_0();   // Callback when receives Backward = 0
void ProcessBackward_1();   // Callback when receives Backward = 1
void transmit_SPI(); //

// initialize tasks
void request_distance_code(void *parameter);
TaskHandle_t request_distance;

// variables to store measure data and sensor states
uint32_t sensor_update_time = 0;
int32_t PWM_slider = 0;

// the XML array size needs to be bigger that your maximum expected size. 2048 is way too big for this example
char XML[2048];
// just some buffer holder for char operations
char buf[32];

// variable for the IP reported when you connect to your homes intranet (during debug mode)
IPAddress Actual_IP;

// definitions of your desired intranet created by the ESP32
IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;

// gotta create a server
WebServer server(80);

//--------------------------------------Start Dylan-------------------------------------

#define bufferSize 10   // SPI Buffer size

// Create a variable(union) to receive int32_t of data over SPI
typedef union _packet_buffer_t
{
  uint8_t buff[4];
  int32_t value;
} packet_buffer_t;

packet_buffer_t motor_encoder;     // Packet to store Motor Counter
packet_buffer_t PWM_toSend;        // Packet to send Motor Speed

char Tx_command;
uint8_t Tx_send[6];
uint8_t distance;

uint8_t RxBuffer[bufferSize]; // Recive Buffer
uint8_t TxBuffer[bufferSize]; // Transmit Buffer

void setup() {

  pinMode(VSPI_MISO, INPUT);    // Master IN Slave OUT -- INPUT
  pinMode(VSPI_MOSI, OUTPUT);   // Master OUT Slave IN -- OUTPUT  
  pinMode(VSPI_SCLK, OUTPUT);   // Master Clock OUT -- OUTPUT  
  pinMode(VSPI_SS, OUTPUT);     // Master Chip Select -- OUTPUT  

  SPI.begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI); // Begin SPI
  SPI.setBitOrder(MSBFIRST);                  // Set Bit Order -- Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);                 // Set Data Transfer Mode -- Mode 0
  SPI.setFrequency(1000000);                  // Set Transfer Speed -- 1 000 000

  Serial.begin(115200);                 // Serial Connection Begin -- 9600
  Serial.setDebugOutput(true);        // Set Debug Output

  PWM_toSend.value = PWM_slider;

    // if your web page or XML are large, you may not get a call back from the web page
    // and the ESP will think something has locked up and reboot the ESP
    // not sure I like this feature, actually I kinda hate it
    // disable watch dog timer 0
    //disableCore0WDT();

    // maybe disable watch dog timer 1 if needed
    //  disableCore1WDT();

  Serial.println("starting server");  // just an update to progress

  // if you have this #define USE_INTRANET,  you will connect to your home intranet, again makes debugging easier
  #ifdef USE_INTRANET
    WiFi.begin(LOCAL_SSID, LOCAL_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
    Actual_IP = WiFi.localIP();
  #endif

  // if you don't have #define USE_INTRANET, here's where you will creat and access point
  #ifndef USE_INTRANET
    WiFi.softAP(AP_SSID, AP_PASS);
    delay(100);
    WiFi.softAPConfig(PageIP, gateway, subnet);
    delay(100);
    Actual_IP = WiFi.softAPIP();
    Serial.print("IP address: "); Serial.println(Actual_IP);
  #endif

  printWifiStatus();  

  //       These calls will handle data coming back from your web page

  // this one is a page request, upon ESP getting / string the web page will be sent
  server.on("/", SendWebsite);

  // upon esp getting /XML string, ESP will build and send the XML, this is how we refresh
  // just parts of the web page
  server.on("/xml", SendXML);

  // upon ESP getting /UPDATE_SLIDER string, ESP will execute the UpdateSlider function
  // same notion for the following .on calls
  // add as many as you need to process incoming strings from your web page
  // as you can imagine you will need to code some javascript in your web page to send such strings
  // this process will be documented in the SuperMon.h web page code
  server.on("/PWM_update", UpdatePWM);
  server.on("/Forward_0", ProcessForward_0);
  server.on("/Forward_1", ProcessForward_1);
  server.on("/Backward_0", ProcessBackward_0);
  server.on("/Backward_1", ProcessBackward_1);

  // finally begin the server
  server.begin();

  xTaskCreatePinnedToCore(
    request_distance_code,
    "request_distance",
    10000,
    NULL,
    0,
    &request_distance,
    0);
}

void loop() {

  // you main loop that measures, processes, runs code, etc.
  // note that handling the "on" strings from the web page are NOT in the loop
  // that processing is in individual functions all managed by the wifi lib

  // in my example here every 50 ms, i measure some analog sensor data (my finger dragging over the pins
  // and process accordingly
  // analog input can be from temperature sensors, light sensors, digital pin sensors, etc.

  // no matter what you must call this handleClient repeatidly--otherwise the web page
  // will not get instructions to do something
  server.handleClient();
}
void transmit_SPI(){
  TxBuffer[4] = Tx_command;
  TxBuffer[5] = Tx_send[0];
  TxBuffer[6] = Tx_send[1];
  TxBuffer[7] = Tx_send[2];
  TxBuffer[8] = Tx_send[3];
  TxBuffer[9] = Tx_send[4];
  digitalWrite(VSPI_SS,LOW);
  SPI.transferBytes(TxBuffer, RxBuffer, bufferSize);
  digitalWrite(VSPI_SS, HIGH);
  distance = RxBuffer[4];
  Serial.printf("Rx: %d \t | \t Tx: %c, %d, %d\n\r", distance, Tx_command, Tx_send[0], Tx_send[1]);
}

// function managed by an .on method to handle slider actions on the web page
void UpdatePWM() {

  String t_state = server.arg("VALUE");

  // conver the string sent from the web page to an int
  PWM_slider = t_state.toInt();
  Serial.print("Update PWM"); Serial.println(PWM_slider);

  // YOU MUST SEND SOMETHING BACK TO THE WEB PAGE--BASICALLY TO KEEP IT LIVE

  // option 1: send no information back, but at least keep the page live
  // just send nothing back
  // server.send(200, "text/plain", ""); //Send web page

  // option 2: send something back immediately, maybe a pass/fail indication, maybe a measured value
  // here is how you send data back immediately and NOT through the general XML page update code
  // my simple example guesses at fan speed--ideally measure it and send back real data
  // i avoid strings at all caost, hence all the code to start with "" in the buffer and build a
  // simple piece of data
  strcpy(buf, "");
  sprintf(buf, "%d", PWM_slider);
  sprintf(buf, buf);

  // now send it back
  server.send(200, "text/plain", buf); //Send web page
}

// processing Forward
void ProcessForward_0() {
  PWM_toSend.value = 0 ;
  sprintf(buf, "%d", PWM_toSend.value);
  server.send(200, "text/plain", buf); //Send web page
  
  Tx_command = 'S';
  Tx_send[0] = 0;
  Tx_send[1] = 0;
  transmit_SPI();
}
void ProcessForward_1() {
  PWM_toSend.value = PWM_slider ;
  sprintf(buf, "%d", PWM_toSend.value);
  server.send(200, "text/plain", buf); //Send web page

  Tx_command = 'F';
  Tx_send[0] = PWM_slider;
  Tx_send[1] = 0;
  transmit_SPI();
}
// processing Backward
void ProcessBackward_0() {
  PWM_toSend.value = 0 ;
  sprintf(buf, "%d", PWM_toSend.value);
  server.send(200, "text/plain", buf); //Send web page

  Tx_command = 'S';
  Tx_send[0] = 0;
  Tx_send[1] = 0;
  transmit_SPI();
}
void ProcessBackward_1() {
  PWM_toSend.value = - PWM_slider ;
  sprintf(buf, "%d", PWM_toSend.value);
  server.send(200, "text/plain", buf); //Send web page

  Tx_command = 'B';
  Tx_send[0] = PWM_slider;
  Tx_send[1] = 0;
  transmit_SPI();
}

// code to send the main web page
// PAGE_MAIN is a large char defined in SuperMon.h
void SendWebsite() {

  Serial.println("sending web page");
  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/html", PAGE_MAIN);

}

// code to send the main web page
// I avoid string data types at all cost hence all the char mainipulation code
void SendXML() {

  // Serial.println("sending xml");

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // send bitsA0
  sprintf(buf, "<E0>%d</E0>\n",distance);
  strcat(XML, buf);
  
  strcat(XML, "</Data>\n");
  // wanna see what the XML code looks like?
  // actually print it to the serial monitor and use some text editor to get the size
  // then pad and adjust char XML[2048]; above
  //Serial.println(XML);

  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/xml", XML);
}

// I think I got this code from the wifi example
void printWifiStatus() {

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Open http://");
  Serial.println(ip);
}

void request_distance_code(void *parameter)
{
  for (;;)
  {
    Tx_command = 'D';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    delay(10);
    Tx_command = 'x';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    delay(1000);
  }
}
// end of code