#include <TemperatureGraph.h>

TemperatureGraph::TemperatureGraph(uint16_t lower_bound, uint16_t upper_bound)
{
    for (uint8_t x = 0; x < 128; x++)
    {
        points[x] = -1;
        pointVals[x] = 0;
    }
}

void TemperatureGraph::shiftArray()
{
    uint8_t temp[128];
    uint16_t tempVals[128];

    //clone array

    for (uint8_t i = 0; i < 128; i++)
    {
        temp[i] = points[i];
        tempVals[i] = pointVals[i];
    }

    for (uint8_t i = 0; i < 128; i++)
    {
        if (i < 127)
        {
            points[i] = temp[i + 1];
            pointVals[i] = tempVals[i + 1];
        }
    }

    points[127] = -1;
    pointVals[127] = 0;
}

//TODO fix overflow, add dynamic bounds, display bounds (maybe on button press)

void TemperatureGraph::drawGraph(Adafruit_SSD1306 *display)
{
    if (lastIndex < 127)
    {
        if(remapped){
            display->clearDisplay();
            displayInfo(display, true);

                for (uint8_t i = 0; i < lastIndex; i++)
                    {
                        if (points[i] > -1)
                        {
                            display->drawPixel(i, points[i], SSD1306_WHITE);
                        }
                    }
            remapped = false;
        }else{
            display->drawPixel(lastIndex - 1, points[lastIndex - 1], SSD1306_WHITE);
            #ifdef DEBUG
                Serial.print(F("Drawing pixel at x: "));
                Serial.print(lastIndex - 1);
                Serial.print(F(", y: "));
                Serial.println(points[lastIndex - 1]);
            #endif 
            displayInfo(display);
        }
    }
    else
    {
        display->clearDisplay();
        //Draw text before graph so points can overlay black rects
        displayInfo(display, true);

        for (uint8_t i = 0; i < 128; i++)
        {
            if (points[i] > -1)
            {
                display->drawPixel(i, points[i], SSD1306_WHITE);
            }
        }
    }

    display->display();
}



void TemperatureGraph::displayInfo(Adafruit_SSD1306 *display, bool force)
{
    if (!isGraphTempUpdated || force || lastIndex <= 1)
    {
        uint8_t textPos = 25;
        if(lastIndex > 89){
            long sum = 0;
            uint16_t ct = lastIndex - 89;
            for (uint8_t i = 89; i < 128; i++)
            {
                sum += points [i];
            }
            sum = sum / ct;
            if(sum >= 25) textPos = 0;
            else textPos = 25;
        }
        #ifdef DEBUG
            Serial.print(F("Updating text, forced: "));
            Serial.print(force);
            Serial.print(F(", text set to: "));
            Serial.print(lastTemperature);
            Serial.println(F("C."));
        #endif 
        display->fillRect(89, 0, 37, 7, SSD1306_BLACK);
        display->fillRect(89, 25, 37, 7, SSD1306_BLACK);
        display->setCursor(89, textPos);
        display->setTextColor(WHITE);
        display->print(lastTemperature);
        display->println("C");
    }
}
//todo check for min and max temps when adding.
//todo discard erroneus readings like 52836(x100) and replace with previous
float TemperatureGraph::addTemperature(float temp)
{
    float diff = lastTemperature - temp;
    
    if(abs(diff) > 15 && lastIndex > 0)  temp = lastTemperature; //Discard wrong value

    if(lastIndex == 0) diff = 0;    

    uint16_t n_temp = (uint16_t)(temp*100);
    uint8_t coord = map(n_temp, lowerBound, upperBound, 31, 4);

    isGraphTempUpdated = (temp == lastTemperature);
    
    #ifdef DEBUG
        Serial.print(F("Mapped "));
        Serial.print(n_temp);
        Serial.print(F(" (x100C) to y: "));
        Serial.println(coord);
    #endif

    if(lastIndex >= 127){
        lastIndex = 127;
        shiftArray();
    }

    points[lastIndex] = coord;
    pointVals[lastIndex] = n_temp;

    diff = temp - lastTemperature;
    lastTemperature = temp;

     #ifdef DEBUG
        Serial.print(F("New temp diff: "));
        Serial.print(diff);
        Serial.print(F(", last temp integer: "));
        Serial.print(pointVals[lastIndex - 1]);
        Serial.print(F(", new temp integer: "));
        Serial.print(pointVals[lastIndex]);
        Serial.println(F(" ."));
    #endif // DEBUG
    updateBounds(diff);
    if(lastIndex < 127) lastIndex++;
    return temp;
}

void TemperatureGraph::updateBounds(float diff){
    #ifdef DEBUG
        Serial.print(F("Diff: "));
        Serial.println(diff);
        Serial.print(F("UPPER BOUND: "));
        Serial.println(upperBound);
        Serial.print(F("LOWER BOUND: "));
        Serial.println(lowerBound);
    #endif // DEBUG
    if(lastIndex >  0){
    #ifdef DEBUG
        Serial.print(F("Index: "));
        Serial.println(lastIndex);
        Serial.print(F("Temp in array: "));
        Serial.println(pointVals[lastIndex]);
    #endif // DEBUG
        int16_t distToLow = pointVals[lastIndex] - lowerBound;
        int16_t distToUp = upperBound - pointVals[lastIndex];
        uint16_t amplitude = distToLow + distToUp;
    #ifdef DEBUG
        Serial.print(F("UPPER DIST: "));
        Serial.println(distToUp);
        Serial.print(F("LOWER DIST: "));
        Serial.println(distToLow);
        Serial.print(F("Amplitude: "));
        Serial.println(amplitude);
    #endif // DEBUG
        int16_t cent = (2 * amplitude) / 3;
        if(distToLow < 0){
            lowerBound += distToLow;
        }else if (distToLow == 0)
        {
            lowerBound = lowerBound - MAX_AMPLITUDE/GRAPH_HEIGHT;
        }else{
            if(distToLow > cent){
                diff = -1;
                lowerBound += 5;
            }
        }  
        if(distToUp < 0){
            upperBound = upperBound - distToUp;
        }else if (distToUp == 0)
        {
            upperBound += MAX_AMPLITUDE/GRAPH_HEIGHT;
        }else
        {
            if(distToUp > (cent - 8)){
                diff = 1;
                upperBound += 5;
            }
        }
        
        amplitude = upperBound - lowerBound;


        if (amplitude > MAX_AMPLITUDE){
            if(distToUp < distToLow){
                lowerBound += 10;
            }else if(distToUp > distToLow){
                upperBound -= 10;
            }
        }
        remap();
        return;
    }else{
        upperBound = pointVals[lastIndex] + ((MAX_AMPLITUDE/3) + 10);
        lowerBound = pointVals[lastIndex] - (MAX_AMPLITUDE/3);
    }
}

void TemperatureGraph::remap(){
    for (uint8_t i = 0; i < lastIndex; i++)
    {
       points[i] = map(pointVals[i], lowerBound, upperBound, 31, 4);
       remapped = true;
    }
    
}


TemperatureGraph::~TemperatureGraph()
{
}