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
#define LOCAL_SSID "ESPwlan"
#define LOCAL_USER "uc2020252382@student.uc.pt"
#define LOCAL_PASS "12344321"

// Settings for tne access point
#define AP_SSID "TestWebSite"
#define AP_PASS "12344321"

// start your defines for pins for SPI in ROBOT!!!
#define VSPI_MISO 19
#define VSPI_MOSI 23
#define VSPI_SCLK 18
#define VSPI_SS 5

// start your defines for pins for SPI, outputs, inputs, etc.
/* #define VSPI_MISO 33
#define VSPI_MOSI 25
#define VSPI_SCLK 26
#define VSPI_SS 27 */



// initialize functions
void printWifiStatus();     // Print WiFi settings and status
void SendXML();             // Send the XML table to the Web
void SendWebsite();         // Send HTML code to create a Web page
void SendWebsiteDoc();
void UpdateSpeed();           // Callback when receives new PWM(slider) Value
void ProcessForward();    // Callback when receives Forward = 0
void ProcessBackward();    // Callback when receives Forward = 1
void ProcessLeft();   // Callback when receives Backward = 0
void ProcessRight();   // Callback when receives Backward = 1
void ProcessPreStartMove();   // Callback when receives Backward = 1
void ProcessStartMove();   // Callback when receives Backward = 1
void ProcessStop();   // Callback when receives Backward = 1
void ProcessTurn();   // Callback when receives Backward = 1
void ProcessTrajectory();   // Callback when receives Backward = 1
void transmit_SPI(); //

// initialize tasks
void request_distance_code(void *parameter);
TaskHandle_t request_distance;
void request_position_code(void *parameter);
TaskHandle_t request_position;
void request_velocity_code(void *parameter);
TaskHandle_t request_velocity;

// variables to store measure data and sensor states
uint32_t sensor_update_time = 0;
int32_t PWM_slider = 0;

// the XML array size needs to be bigger that your maximum expected size. 2048 is way too big for this example
char XML[2048];
// just some buffer holder for char operations
char buf[32];
// trajectory part variables
int trajectory_choice = 0;
int path_size[5] = {0,4};
int16_t paths[5][10][2] = {
    {
        {0, 0}, {200, 200}, {0, 0}, {0, 0}
    },
    {
        {100, 0}, {100, 100}, {0, 100}, {0, 0}
    },
    {
        {0, 0}, {200, 200}, {0, 0}, {0, 0}
    },
    {
        {0, 0}, {0, 0}, {0, 0}, {0, 0}
    },
    {
        {0, 0}, {0, 0}, {0, 0}, {0, 0}
    },
};

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

#define bufferSize 18   // SPI Buffer size

// Create a variable(union) to receive int32_t of data over SPI
typedef union _packet_buffer32_t
{
  uint8_t buff[4];
  int32_t value;
} packet_buffer32_t;
typedef union _packet_buffer16_t
{
  uint8_t buff[2];
  int16_t value;
} packet_buffer16_t;

packet_buffer16_t distance_total;     // Packet to store Motor Counter
packet_buffer16_t x_coord;     // Packet to store Motor Counter
packet_buffer16_t y_coord;        // Packet to send Motor Speed
packet_buffer16_t angle_rotation;        // Packet to send Motor Speed
packet_buffer16_t angle_target;        // Packet to send Motor Speed
packet_buffer16_t x_target;        // Packet to send Motor Speed
packet_buffer16_t y_target;        // Packet to send Motor Speed
uint8_t velocity;                   // Packet to send Motor Speed
uint8_t speed;

char Tx_command, Rx_command;
int32_t target_distance;
uint8_t Tx_send[bufferSize - 4];

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

  speed = 0;

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
  server.on("/document", SendWebsiteDoc);


  // upon esp getting /XML string, ESP will build and send the XML, this is how we refresh
  // just parts of the web page
  server.on("/xml", SendXML);

  // upon ESP getting /UPDATE_SLIDER string, ESP will execute the UpdateSlider function
  // same notion for the following .on calls
  // add as many as you need to process incoming strings from your web page
  // as you can imagine you will need to code some javascript in your web page to send such strings
  // this process will be documented in the SuperMon.h web page code
  server.on("/Speed", UpdateSpeed);
  server.on("/BUTTON_Forward", ProcessForward);
  server.on("/BUTTON_Backward", ProcessBackward);
  server.on("/BUTTON_Left", ProcessLeft);
  server.on("/BUTTON_Right", ProcessRight);
  server.on("/XTARGET", ProcessPreStartMove);
  server.on("/BUTTON_START", ProcessStartMove);
  server.on("/BUTTON_STOP", ProcessStop);
  server.on("/BUTTON_TURN", ProcessTurn);
  server.on("/BUTTON_TRAJECTORY", ProcessTrajectory);

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

  xTaskCreatePinnedToCore(
    request_position_code,
    "request_position",
    10000,
    NULL,
    0,
    &request_position,
    0);

    xTaskCreatePinnedToCore(
    request_velocity_code,
    "request_velocity",
    10000,
    NULL,
    0,
    &request_velocity,
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
  TxBuffer[10] = Tx_send[5];
  TxBuffer[11] = Tx_send[6];
  TxBuffer[12] = Tx_send[7];
  TxBuffer[13] = Tx_send[8];
  TxBuffer[14] = Tx_send[9];
  TxBuffer[15] = Tx_send[10];
  TxBuffer[16] = Tx_send[11];
  TxBuffer[17] = Tx_send[12];
  digitalWrite(VSPI_SS,LOW);
  SPI.transferBytes(TxBuffer, RxBuffer, bufferSize);
  digitalWrite(VSPI_SS, HIGH);
  RxBuffer[4] = Rx_command;
  Serial.printf("Rx: %c | %d, %d, %d, %d, %d\n\rTx: %c | %d, %d, %d, %d, %d\n\r", Rx_command, RxBuffer[5], RxBuffer[6], RxBuffer[7], RxBuffer[8], RxBuffer[9], Tx_command, TxBuffer[5], TxBuffer[6], TxBuffer[7], TxBuffer[8], TxBuffer[9]);
}

// function managed by an .on method to handle slider actions on the web page
void UpdateSpeed() {

  String t_state = server.arg("VALUE");

  // conver the string sent from the web page to an int
  speed = t_state.toInt();
  Serial.print("Update Speed"); Serial.println(speed);

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
  sprintf(buf, "%d", speed);
  sprintf(buf, buf);

  // now send it back
  server.send(200, "text/plain", buf); //Send web page
}

// processing Forward
void ProcessForward() {
  Tx_command = 'F';
  Tx_send[0] = speed;
  transmit_SPI();
}
void ProcessBackward() {
  Tx_command = 'B';
  Tx_send[0] = speed;
  transmit_SPI();
}
// processing Backward
void ProcessLeft() {
  Tx_command = 'L';
  Tx_send[0] = speed;
  transmit_SPI();
}
void ProcessRight() {
  Tx_command = 'R';
  Tx_send[0] = speed;
  transmit_SPI();
}
void ProcessPreStartMove() {
  String t_state = server.arg("VALUE");
  x_target.value = t_state.toInt();
}
void ProcessStartMove() {
  Tx_command = 'M';
  Tx_send[0] = speed;

  String t_state = server.arg("VALUE");
  y_target.value = t_state.toInt();

  Tx_send[1] = x_target.buff[0];
  Tx_send[2] = x_target.buff[1];
  Tx_send[3] = y_target.buff[0];
  Tx_send[4] = y_target.buff[1];
  transmit_SPI();
}
void ProcessStop() {
  Tx_command = 'S';
  Tx_send[0] = 0;
  Tx_send[1] = 0;
  Tx_send[2] = 0;
  Tx_send[3] = 0;
  Tx_send[4] = 0;
  transmit_SPI();
}
void ProcessTurn(){ 
  Tx_command = 'T';
  Tx_send[0] = speed;

  String t_state = server.arg("VALUE");
  angle_target.value = t_state.toInt();

  Tx_send[1] = angle_target.buff[0];
  Tx_send[2] = angle_target.buff[1];

  Serial.printf("target_angle: %d\r\n", angle_target.value);
  transmit_SPI();
}
void ProcessTrajectory(){
  int8_t x_high_byte, x_low_byte;
  int8_t y_high_byte, y_low_byte;

  String t_state = server.arg("VALUE");
  trajectory_choice = t_state.toInt();

  for(int i = 0; i < path_size[trajectory_choice]; i++){
    x_high_byte = (paths[trajectory_choice][i][0] >> 8) & 0xFF;  
    x_low_byte = paths[trajectory_choice][i][0] & 0xFF;   
    y_high_byte = (paths[trajectory_choice][i][1] >> 8) & 0xFF;  
    y_low_byte = paths[trajectory_choice][i][1] & 0xFF;   


    Tx_command = 'M';
    Tx_send[0] = speed;
    Tx_send[1] = x_high_byte;
    Tx_send[2] = x_low_byte;
    Tx_send[3] = y_high_byte;
    Tx_send[4] = y_low_byte;
    transmit_SPI();
    while (!((x_coord.value > paths[trajectory_choice][i][0] -10) && (x_coord.value < paths[trajectory_choice][i][0] +10)) && !((y_coord.value > paths[trajectory_choice][i][1] -10) && (y_coord.value < paths[trajectory_choice][i][1] +10)) )
    {
      /* code */
    }
    
  }
}
// code to send the main web page
// PAGE_MAIN is a large char defined in SuperMon.h
void SendWebsite() {

  Serial.println("sending web page");
  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/html", PAGE_MAIN);

}
void SendWebsiteDoc() {

  Serial.println("sending web page");
  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/html", PAGE_DOC);

}

// code to send the main web page
// I avoid string data types at all cost hence all the char mainipulation code
void SendXML() {

  // Serial.println("sending xml");

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // send bitsA0
  sprintf(buf, "<TOTALDIST>%d</TOTALDIST>\n",distance_total.value);
  strcat(XML, buf);

  sprintf(buf, "<INSTCOORX>%d</INSTCOORX>\n",x_coord.value);
  strcat(XML, buf);

  sprintf(buf, "<INSTCOORY>%d</INSTCOORY>\n",y_coord.value);
  strcat(XML, buf);

  sprintf(buf, "<INSTROT>%d</INSTROT>\n",angle_rotation.value);
  strcat(XML, buf);

  sprintf(buf, "<INSTVELO>%d</INSTVELO>\n",velocity);
  strcat(XML, buf);

  sprintf(buf, "<VICSTATUS>%d</VICSTATUS>\n",0);
  strcat(XML, buf);
  
  strcat(XML, "</Data>\n");

  // Serial.println(XML);
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
    Tx_command = 'x';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    distance_total.buff[0] = RxBuffer[5];
    distance_total.buff[1] = RxBuffer[6];
    //Serial.printf("total_distance: %d\r\n", total_distance.value);
   
    delay(100);
  }
}
void request_position_code(void *parameter)
{ 
  delay(20);
  for (;;)
  {
    
    Tx_command = 'P';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    Tx_command = 'x';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    x_coord.buff[0] = RxBuffer[5];
    x_coord.buff[1] = RxBuffer[6];
    y_coord.buff[0] = RxBuffer[7];
    y_coord.buff[1] = RxBuffer[8];
    angle_rotation.buff[0] = RxBuffer[9];
    angle_rotation.buff[1] = RxBuffer[10];
    //Serial.printf("x_instant: %d, y_instant: %d, rotation_angle: %d\r\n", x_instant.value, y_instant.value, rotation_angle.value);
   
    delay(100);
  }
}
void request_velocity_code(void *parameter)
{
  for (;;)
  {
    
    delay(40);
    Tx_command = 'V';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    Tx_command = 'x';
    Tx_send[0] = 0;
    Tx_send[1] = 0; 
    transmit_SPI();
    velocity = RxBuffer[5];
    //Serial.printf("total_distance: %d\r\n", total_distance.value);
   
    delay(100);
  }
}
// end of code