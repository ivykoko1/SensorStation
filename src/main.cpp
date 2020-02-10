#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SensorsManager.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <TemperatureGraph.h>

#define DHT_PIN 22

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
SensorsManager sensors(22, 23);
TemperatureGraph* graph;


void setup() {
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  sensors.begin();
  graph = new TemperatureGraph();
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(300);
  
  float temp = sensors.getTemperature();
  graph->addTemperature(temp);
  graph->drawGraph(&display);

}
  