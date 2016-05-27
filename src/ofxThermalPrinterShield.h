#pragma once

#include "ofMain.h"

class ofxThermalPrinterSheild
{
    
public:
    
    ofxThermalPrinterSheild();
    ~ofxThermalPrinterSheild();
    
    void setup(string portName);
    void QRprint(string html);
    
    void bmpPrint(string bmpPath, int bmpWidth, int bmpHeight, int startLine, int mills);
    
private:
    ofSerial mySerial;
    
};

