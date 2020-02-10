#include <TemperatureGraph.h>


TemperatureGraph::TemperatureGraph(/* args */)
{
    for(uint8_t x = 0; x < 128; x ++){
        points[x] = -1;
    }  
}

void TemperatureGraph::shiftArray(){
    for (uint8_t i = 0; i < 127; i++)
    {
        if(i < 127){
            
            points[i + 1] = points[i];
            //TODO ADD DEBUG PRINTS
            
        }   
    }
    
}

//TODO fix overflow, add dynamic bounds, display bounds (maybe on button press)

void TemperatureGraph::drawGraph(Adafruit_SSD1306* display){
    display->clearDisplay();
    for (uint8_t i = 0; i < 128; i++)
    {
        if(points[i] > -1){
            display->drawPixel(i, points[i], SSD1306_WHITE);
        }
    }
    if (!isGraphTempUpdated)
    {
        display->setCursor(90,25);
        display->setTextColor(WHITE);
        display->print(lastTemperature);
        display->print("C");    
    }
    
    display->display();
}
//todo check for min and max temps when adding.

float TemperatureGraph::addTemperature(float temp){
    if(temp == lastTemperature){
        isGraphTempUpdated = true;
    }else{
        isGraphTempUpdated = false;
    }
    uint8_t coord = map((temp * 100), 1850, 2175, 32, 0);
    if(lastIndex == 127){
        shiftArray();
        points[lastIndex] = coord;
    }else
    {
        points[lastIndex] = coord;
        lastIndex++;
    }
    lastTemperature = temp;
    return temp;
}


TemperatureGraph::~TemperatureGraph()
{
}