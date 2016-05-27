

#include "ofxThermalPrinterShield.h"

//------------------------------------------
ofxThermalPrinterSheild::ofxThermalPrinterSheild()
{
    
}

ofxThermalPrinterSheild::~ofxThermalPrinterSheild()
{
    
}


void ofxThermalPrinterSheild::setup(string portName){
    mySerial.setup(portName,9600);
}

void ofxThermalPrinterSheild::bmpPrint(string bmpPath, int bmpWidth, int bmpHeight, int mills) {
    std::string path = ofToDataPath(bmpPath);
    
    uint8_t img[bmpWidth * bmpHeight];
    
    {
        FILE *fp = fopen(path.c_str(), "rb");
        fread(img, 1, bmpWidth * bmpHeight, fp);
        fclose(fp);
    }
    
    
    std::vector<uint8_t> data;
    
    for (int i = 0; i < bmpWidth * bmpHeight; i += 8) {
        uint8_t b = 0;
        
        if (img[i + 0] == 1) {
            b = b | 0b10000000;
        }
        
        if (img[i + 1] == 1) {
            b = b | 0b01000000;
        }
        
        if (img[i + 2] == 1) {
            b = b | 0b00100000;
        }
        
        if (img[i + 3] == 1) {
            b = b | 0b00010000;
        }
        
        if (img[i + 4] == 1) {
            b = b | 0b00001000;
        }
        
        if (img[i + 5] == 1) {
            b = b | 0b00000100;
        }
        
        if (img[i + 6] == 1) {
            b = b | 0b00000010;
        }
        
        if (img[i + 7] == 1) {
            b = b | 0b00000001;
        }
        
        data.push_back(b);
    }
    
    const int width = bmpWidth;
    const int height = bmpHeight;
    const int max_line = 20;
    
    for (int j = 0; j < ceil(height / (double)max_line); ++j) {
        
        int start = width / 8 * j * max_line;
        int end= width / 8 * (j + 1) * max_line;
        int cnt = 0;
        
        for (int i = start; i < data.size(); ++i) {
            if (i >= end) {
                break;
            }
            
            ++cnt;
        }
        
        std::string bmp;
        bmp.push_back(1);
        bmp.push_back((cnt + 5) / 100);
        bmp.push_back((cnt + 5) % 100);
        bmp.push_back(0x1C);
        bmp.push_back(0x2A);
        bmp.push_back(0x63);
        bmp.push_back(0x00);
        bmp.push_back(cnt / (width / 8));
        
        
        for (int i = start; i < data.size(); ++i) {
            if (i >= end) {
                break;
            }
     
            bmp.push_back(data[i]);
       }
        
        
        unsigned char *p2 = (unsigned char*)bmp.c_str();
        for (int i = 0; i < bmp.length(); ++i) {
            cout<< ofToString( (int)*p2++ ) << endl;
        }
        mySerial.writeBytes((unsigned char*)bmp.c_str(), bmp.length());
        ofSleepMillis(mills);
        //break;
    }
    

}

void ofxThermalPrinterSheild::QRprint(string html){
    
    std::string qr;
    qr.push_back(2);
    qr.push_back((html.length() + 4 )/ 100);
    qr.push_back((html.length() + 4 ) % 100);
    qr.push_back(29);
    qr.push_back(120);
    qr.push_back(0x4C);
    qr.push_back(html.length());
    qr += html;
    
    unsigned char *p = (unsigned char*)qr.c_str();
    for (int i = 0; i < qr.length(); ++i) {
        cout<< ofToString( (int)*p++ ) << endl;
    }
    
    std::string url;
    url.push_back(3);
    url.push_back(html.length() / 100);
    url.push_back(html.length() % 100);
    url += html;
    
    mySerial.writeBytes((unsigned char*)qr.c_str(), qr.length());
    mySerial.writeBytes((unsigned char*)url.c_str(), url.length());
    

}