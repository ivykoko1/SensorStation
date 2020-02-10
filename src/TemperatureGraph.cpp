#include <TemperatureGraph.h>

TemperatureGraph::TemperatureGraph(uint16_t lower_bound, uint16_t upper_bound)
{
    for (uint8_t x = 0; x < 128; x++)
    {
        points[x] = -1;
    }
}

void TemperatureGraph::shiftArray()
{
    uint8_t temp[128];

    //clone array

    for (uint8_t i = 0; i < 128; i++)
    {
        temp[i] = points[i];
    }

    for (uint8_t i = 0; i < 128; i++)
    {
        if (i < 127)
        {
            points[i] = temp[i + 1];
        }
    }

    points[127] = -1;
}

//TODO fix overflow, add dynamic bounds, display bounds (maybe on button press)

void TemperatureGraph::drawGraph(Adafruit_SSD1306 *display)
{
    if (lastIndex < 127)
    {
        display->drawPixel(lastIndex - 1, points[lastIndex - 1], SSD1306_WHITE);
        #ifdef DEBUG
            Serial.print(F("Drawing pixel at x: "));
            Serial.print(lastIndex - 1);
            Serial.print(F(", y: "));
            Serial.println(points[lastIndex - 1]);
        #endif 
        displayInfo(display);
    }
    else
    {
        display->clearDisplay();
        for (uint8_t i = 0; i < 128; i++)
        {
            if (points[i] > -1)
            {
                display->drawPixel(i, points[i], SSD1306_WHITE);
            }
        }
        displayInfo(display, true);
    }

    display->display();
}



void TemperatureGraph::displayInfo(Adafruit_SSD1306 *display, bool force)
{
    if (!isGraphTempUpdated || force || lastIndex <= 1)
    {
        #ifdef DEBUG
            Serial.print(F("Updating text, forced: "));
            Serial.print(force);
            Serial.print(F(", text set to: "));
            Serial.print(lastTemperature);
            Serial.println(F("C."));
        #endif 
        display->fillRect(89, 25, 37, 7, SSD1306_BLACK);
        display->setCursor(89, 25);
        display->setTextColor(WHITE);
        display->print(lastTemperature);
        display->println("C");
    }
}
//todo check for min and max temps when adding.
//todo discard erroneus readings like 52836(x100) and replace with previous
float TemperatureGraph::addTemperature(float temp)
{
    uint16_t integerTemp = (uint16_t)(temp * 100);
    uint8_t newTempRatio = integerTemp / lastTemperature;
    if(newTempRatio >= DISCARD_THRESOLD_RATIO) temp = lastTemperature; //Discard wrong value
    #ifdef DEBUG
        Serial.print(F("New temp %: "));
        Serial.print(newTempRatio);
        Serial.println(F(" %."));
    #endif // DEBUG
    if (temp == lastTemperature)
    {
        isGraphTempUpdated = true;
    }
    else
    {
        isGraphTempUpdated = false;
    }
    uint8_t coord = map(integerTemp, lowerBound, upperBound, 32, 0);

    #ifdef DEBUG
        Serial.print(F("Mapped "));
        Serial.print(integerTemp);
        Serial.print(F(" (x100C) to y: "));
        Serial.println(coord);
    #endif 

    if (lastIndex == 127)
    {   
        shiftArray();
        points[127] = coord;
        #ifdef DEBUG
            Serial.print(F("Adding y: "));
            Serial.print(coord);
            Serial.print(F(" in index: "));
            Serial.println(lastIndex);
        #endif 
    }
    else
    {
        points[lastIndex] = coord;
        #ifdef DEBUG
            Serial.print(F("Adding "));
            Serial.print(coord);
            Serial.print(F(" in index: "));
            Serial.println(lastIndex);
        #endif 
        lastIndex++;
    }
    lastTemperature = temp;
    return temp;
}


TemperatureGraph::~TemperatureGraph()
{
}