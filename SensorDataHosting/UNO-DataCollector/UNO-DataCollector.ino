#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

// Data Transfer setup
SoftwareSerial espSerial(5, 6);

// Distance sonar setup
NewPing sonar (3, 2, 450);// create ultrasonic object with the following parameters (Trig=3, Echo=2, Max distance=400cm)
// Variables declaration
int sensorValue;
float voltage, temp, dst;
String strTemp, strDst;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize serial communication on 5/6
  espSerial.begin(115200);
  // Setup screen and display logo to check if it is working
  displaySetup();
  delay(2000);
}



// the loop routine runs over and over again forever:
void loop() {
  // Get data from sensors
  temp = readTemperature();
  dst = readDistance();
  // Convert to string
  strTemp = String(temp);
  strDst = String(dst);
  // Send data to ESP8266
  espSerial.println(strTemp + " C");
  espSerial.println(strDst + " cm");

  // Display gathered data on the SSD1306
  displayData(temp, dst);
  delay(1000);
}

float readTemperature() {
  sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = sensorValue * (5.0 / 1023.0);
  temp = voltage * 100;

  Serial.print("Temperature= ");
  Serial.print(temp);
  Serial.println(" C");
  return temp;
}

float readDistance() {
  float distance = sonar.ping_cm();// putting the measured value in distance
  Serial.print("Distance= ");// print "Distance"
  Serial.print(distance);// print the value of the distance
  Serial.println(" cm");// print "cm"
  return distance;
}

void displaySetup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
}

void displayData(float temperature, float distance){
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.write("Temperature: ");
  display.print(temperature);
  display.write("*C");
  display.setCursor(0, 16);
  display.write("Distance: ");
  display.print(distance);
  display.write(" cm");
  display.display();
}
