#ifndef TEMPGRAPH_H
#define TEMPGRAPH_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define LOWER_BOUND 1925
#define UPPER_BOUND 2075

#define MAX_AMPLITUDE 200
#define GRAPH_HEIGHT 32

#define MAX_BOUNDS_CHANGE 0.5

//#define DEBUG /*enables debug*/
class TemperatureGraph
{
private:
    uint8_t points[128];
    uint16_t pointVals[128];
    uint8_t lastIndex = 0;
    uint16_t lowerBound = LOWER_BOUND;
    uint16_t upperBound = UPPER_BOUND;

    float lastTemperature = 0;
    float minTemperature;
    float maxTemperature;

    bool isGraphTempUpdated = false;
    bool remapped = false;

    void shiftArray();
    void displayInfo(Adafruit_SSD1306 *display, bool force = false);
    void updateBounds(float diff);
    void remap();
    
public:
    TemperatureGraph(uint16_t lower_bound = LOWER_BOUND, uint16_t upper_bound = UPPER_BOUND);
    ~TemperatureGraph();
    void drawGraph(Adafruit_SSD1306 *display);
    float addTemperature(float temp);
    void reset();
};

#endif // !#TEMPGRAPH_H
