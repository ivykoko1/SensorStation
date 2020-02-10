#ifndef TEMPGRAPH_H
#define TEMPGRAPH_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define LOWER_BOUND 1875
#define UPPER_BOUND 2150
/* Percentage for discarding next reading i.e.:
 if new registered temperature / previous temperature  * 100 > DISCARD_THRESOLD then
 the newly listed temperature won't be registered and instead the previous will be used  */
#define DISCARD_THRESOLD_RATIO 120
#define MAX_BOUNDS_CHANGE 105

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
    void updateBounds(uint16_t newLower, uint16_t newUpper);
    
public:
    TemperatureGraph(uint16_t lower_bound = LOWER_BOUND, uint16_t upper_bound = UPPER_BOUND);
    ~TemperatureGraph();
    void drawGraph(Adafruit_SSD1306 *display);
    float addTemperature(float temp);
    void reset();
    //struct boundsTuple getGraphBounds();
    //struct boundsTuple setGraphBounds();
};

#endif // !#TEMPGRAPH_H
