#ifndef TEMPGRAPH_H
#define TEMPGRAPH_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define LOWER_BOUND 1850
#define UPPER_BOUND 2175

//#define DEBUG /*enables debug*/
class TemperatureGraph
{
private:
    uint8_t points[128];
    uint8_t lastIndex = 0;
    uint16_t lowerBound = LOWER_BOUND;
    uint16_t upperBound = UPPER_BOUND;

    float lastTemperature;
    float minTemperature;
    float maxTemperature;

    bool isGraphTempUpdated = false;

    void shiftArray();
    void displayInfo(Adafruit_SSD1306 *display, bool force = false);
    typedef struct
    {
        uint16_t lower_bound;
        uint16_t upper_bound;
    } boundsTuple;

public:
    TemperatureGraph(uint16_t lower_bound = LOWER_BOUND, uint16_t upper_bound = UPPER_BOUND);
    ~TemperatureGraph();
    void drawGraph(Adafruit_SSD1306 *display);
    float addTemperature(float temp);
    void reset();
    boundsTuple getGraphBounds();
    boundsTuple setGraphBounds();
};

#endif // !#TEMPGRAPH_H
