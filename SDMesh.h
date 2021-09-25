
   void TFT3D::setRoot( String fileDir ){
      TFT3D::_fileRoot = fileDir ;
    }

    bool TFT3D::openSD(){
      if (!SD.begin(TFT_SD_CS)) {
        DUMPS("SD Card Failed");
        return false;
      }else{
        DUMPS("SD Card Success");
        return true;
      }
    }

    File TFT3D::openFile( String fileName ){

      String fileFolder = TFT3D::_fileRoot + fileName;
      //Reset File to the beginning
      if (SD.exists( fileFolder )) {
        DUMPS("File Exists");
        //TFTCanvas::_file.close();
        TFT3D::_file = SD.open( fileFolder , TFT3D::_mode);
        return TFT3D::_file;
      }else{
        DUMPS("File Does not Exist");
      }

    }

    void TFT3D::readFile( String fileName, bool SerialDebug, bool TFTDebug){
      String fileFolder = TFT3D::_fileRoot + fileName;
      
      TFT3D::_file = openFile( fileName );

      if (TFT3D::_file) {
        while (TFT3D::_file.available()) {

          char data = TFT3D::_file.read();
          DUMPV(data);
          DUMPVTFT(data);
        }
        TFT3D::_file.close();
        
      }else {
        // if the file isn't open, pop up an error:
        DUMP("Error opening File -->  ", TFT3D::_fileRoot);
      }
    }

    String TFT3D::readln(File &f){
      
      String data = f.readStringUntil('\n') ;
      DUMPV(data);
      DUMPPRINTLN();
      return data;
      
    };

    uint16_t _read16(File &f) {
      uint16_t result;
      ((uint8_t *)&result)[0] = f.read(); // LSB
      ((uint8_t *)&result)[1] = f.read(); // MSB
      return result;
    }

    uint32_t _read32(File &f) {
      uint32_t result;
      ((uint8_t *)&result)[0] = f.read(); // LSB
      ((uint8_t *)&result)[1] = f.read();
      ((uint8_t *)&result)[2] = f.read();
      ((uint8_t *)&result)[3] = f.read(); // MSB
      return result;
    }

    void TFT3D::loadImage(char *fileName, int x = 0, int y = 0){
      File     bmpFile;
      int      bmpWidth, bmpHeight;   // W+H in pixels
      uint8_t  bmpDepth;              // Bit depth (currently must be 24)
      uint32_t bmpImageoffset;        // Start of image data in file
      uint32_t rowSize;               // Not always = bmpWidth; may have padding
      uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
        //#ifdef _SPFD5408_
          uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
          uint8_t  lcdidx = 0;
          boolean  first = true;
        //#endif
      uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
      boolean  goodBmp = false;       // Set to true on valid header parse
      boolean  flip    = true;        // BMP is stored bottom-to-top
      int      w, h, row, col;
      uint8_t  r, g, b;
      uint32_t pos = 0, startTime = millis();

      if((x >= _tft.width()) || (y >= _tft.height())) return;

      // Open requested file on SD card
      if ((bmpFile = SD.open(fileName)) == NULL) {
        DUMPS("File Not Found" ); DUMPPRINTLN();
        return;
      }

      if(_read16(bmpFile) == 0x4D42) { // BMP signature

        (void)_read32(bmpFile); // Read & ignore creator bytes
        DUMP("File size: ", _read32(bmpFile) ); 
        bmpImageoffset = _read32(bmpFile); // Start of image data
        DUMP(" Header size: ", _read32(bmpFile) );

        // Read DIB header

        bmpWidth  = _read32(bmpFile);
        bmpHeight = _read32(bmpFile);
        if(_read16(bmpFile) == 1) { // # planes -- must be '1'
          bmpDepth = _read16(bmpFile); // bits per pixel

          if((bmpDepth == 24) && (_read32(bmpFile) == 0)) { // 0 = uncompressed

            goodBmp = true; // Supported BMP format -- proceed!

            // BMP rows are padded (if needed) to 4-byte boundary
            rowSize = (bmpWidth * 3 + 3) & ~3;

            // If bmpHeight is negative, image is in top-down order.
            // This is not canon but has been observed in the wild.
            if(bmpHeight < 0) {
              bmpHeight = -bmpHeight;
              flip      = false;
            }

            // Crop area to be loaded
            w = bmpWidth;
            h = bmpHeight;
            if((x+w-1) >= _tft.width())  w = _tft.width()  - x;
            if((y+h-1) >= _tft.height()) h = _tft.height() - y;

            // Set TFT address window to clipped image bounds
            _tft.setAddrWindow(x, y, x+w-1, y+h-1);

            for (row=0; row<h; row++) { // For each scanline...
              // Seek to start of scan line.  It might seem labor-
              // intensive to be doing this on every line, but this
              // method covers a lot of gritty details like cropping
              // and scanline padding.  Also, the seek only takes
              // place if the file position actually needs to change
              // (avoids a lot of cluster math in SD library).
              if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
              else     // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
              if(bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
              }

              for (col=0; col<w; col++) { // For each column...
                // Time to read more pixel data?
                if (buffidx >= sizeof(sdbuffer)) { // Indeed
                  // Push LCD buffer to the display first
                  #ifdef _SPFD5408_
                    if(lcdidx > 0) {
                      _tft.pushColors(lcdbuffer, lcdidx, first);
                      lcdidx = 0;
                      first  = false;
                    }
                  #endif
                  bmpFile.read(sdbuffer, sizeof(sdbuffer));
                  buffidx = 0; // Set index to beginning
                }

                // Convert pixel from BMP to TFT format
                b = sdbuffer[buffidx++];
                g = sdbuffer[buffidx++];
                r = sdbuffer[buffidx++];
                  #ifdef _SPFD5408_
                    lcdbuffer[lcdidx++] = _tft.Color565(r,g,b);
                  #elif ( defined (_ST7735H_) || defined(_ILI9341_))
                    _tft.pushColor(_tft.color565(r,g,b));
                  #endif
                } // end pixel
              } // end scanline
              // Write any remaining data to LCD
              #ifdef _SPFD5408_
                if(lcdidx > 0) {
                  _tft.pushColors(lcdbuffer, lcdidx, first);  
                } 
              #endif
              DUMP("Loaded in: ", millis() - startTime);
            } // end goodBmp
          }
        }

      bmpFile.close();
      if(!goodBmp) DUMPS("BMP format not recognized.");
    }