/*
  Design and created by Blascarr

  TFT3D
  Name    : Blascarr
  Description: TFT3D.h
  version : 1.0

	TFT3D is a generic library that supports differents TFT Screens based on Adafruit_GFX.

	This library is part of a educational course to learn C++ in practice in https://github.com/blascarr/TFTCourse or http://www.blascarr.com/ webpage.

	This Library gives some helpful classes to manipulate 3D Models and graphics visualization for TFT Screen. With TFT3D we extend some dependency issues for use a variety of TFTScreens with Adafruit TFT library . Nowadays is supported

    	ST7735 https://github.com/adafruit/Adafruit-ST7735-Library
    	SPFD5408 https://github.com/adafruit/TFTLCD-Library

	    ILI9341 https://github.com/adafruit/Adafruit_ILI9341

	    BasicLinearAlgebra is needed to integrate Matrix concepts and graphics terms.

	https://github.com/tomstewart89/BasicLinearAlgebra
  	
  	Blascarr invests time and resources providing this open source code like some other libraries, please
  	respect the job and support open-source software.
    
    Written by Adrian for Blascarr
*/

#ifndef _TFT3D_h
  #define _TFT3D_h  

    #if defined(ARDUINO) && ARDUINO >= 100
      #include "Arduino.h"
    #else
      #include "WProgram.h"
    #endif

    #include "BasicLinearAlgebra.h"
    #include "TFT_config.h"

    #define SERIALDEBUG Serial

    #ifdef TFTDEBUG
      #define  DUMP(s, v)  { SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v); }
      #define  DUMPV(v)  { SERIALDEBUG.print(v); }
      #define  DUMPS(s)    { SERIALDEBUG.print(F(s));}
      #define  DUMPPRINTLN() { SERIALDEBUG.println();}
      #define  DUMPTFT(s, v)  { SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v); }
      #define  DUMPVTFT(v)  { SERIALDEBUG.print(v); }
      #define  DUMPSTFT(s)    { SERIALDEBUG.print(F(s));}
    #else
      #define  DUMP(s, v)
      #define  DUMPV(v)
      #define  DUMPS(s)
      #define  DUMPPRINTLN() 
      #define  DUMPTFT(s, v)
      #define  DUMPVTFT(v)
      #define  DUMPSTFT(s)
    #endif

    #if  defined(_ADAFRUIT_GFX_H ) || defined( _PDQ_GFX_H )
      
    #ifndef FOV
      #define FOV 64
    #endif  

    static int loops;
    
    //--------------Event for changing Draw Type-----------------------//
    //Solamente son utiles estas definiciones si se crea una estructura nodes y una estructura faces

    //#define NODE(a, b, nodes) (long)(pgm_read_dword(&nodes[a][b]))
    //#define EDGE(a, b) pgm_read_byte(&faces[a][b])

    //#define NODEFLOAT(a, b, nodes) (float)(pgm_read_float(&nodes[a][b]))
   // #define EDGEFLOAT(a, b, faces) pgm_read_byte(&faces[a][b])

    //static int proj_nodes[NODECOUNT][2];         // projected nodes (x,y)
    //static int old_nodes[NODECOUNT][2];          // projected nodes of previous frame to check if we need to redraw   

      class TFT3D{
        public:
          
          enum class TFType
            {
              UNKNOWN,
              TFT_ST7735,
              TFT_ILI9341,
              TFT_SPFD548,
              TFT_PDQST7735,
              TFT_PDQILI9341
          };

          TFType _tftype = TFType::UNKNOWN ;

          #ifdef _ADAFRUIT_ST7735H_

            TFT3D( Adafruit_ST7735& tft ): _tft(tft), _tftype(TFType::TFT_ST7735) { 
            }
          #endif

          #ifdef _PDQ_ST7735H_
            TFT3D( PDQ_ST7735& tft ): _tft(tft), _tftype(TFType::TFT_PDQST7735) {
                
            }
          #endif

          #ifdef _ADAFRUIT_ILI9341H_
            TFT3D( Adafruit_ILI9341& tft ): _tft(tft), _tftype(TFType::TFT_ILI9341) {
                
            }
          #endif

          #ifdef _ADAFRUIT_TFTLCD_H_
            TFT3D( Adafruit_TFTLCD& tft ): _tft(tft), _tftype(TFType::TFT_SPFD548) {
                
            }
          #endif

          #ifdef _PDQ_ILI9341H_
            TFT3D( PDQ_ILI9341& tft ): _tft(tft), _tftype(TFType::TFT_PDQILI9341) {
                
            }
          #endif

          //------------------- TFT Reference Definition ------------------------
          #ifdef _ADAFRUIT_ST7735H_ 
            Adafruit_ST7735& _tft;
          #endif

          #ifdef _PDQ_ST7735H_
            PDQ_ST7735& _tft;
          #endif

          #ifdef _ADAFRUIT_ILI9341H_
            Adafruit_ILI9341& _tft;
          #endif

          #ifdef _PDQ_ILI9341H_
            PDQ_ILI9341& _tft;
          #endif

          #ifdef _ADAFRUIT_TFTLCD_H_
            Adafruit_TFTLCD& _tft;
          #endif

          TFType getTFType(void){
            return _tftype;
          }

          void init();
          void update(void){};
          

          //No habilitado el desarrollo de formas desde acceso a flash
          //void draw_vertex( const int (*n)[2], const uint16_t color);

          //void draw_wireframe(  const int (*n)[2], const uint16_t color);

          //void draw_flat_color(  const int (*n)[2], uint16_t color);

          //void clear_dirty( uint16_t color = WHITE , boolean projnodes = true);
          //void draw ( const int (*n)[2], uint16_t timer = 0);
      };

      class mesh{
        public:
          int proj_nodes[NODECOUNT][2];
          int old_nodes[NODECOUNT][2];
          float nodes[NODECOUNT][3];
          unsigned char faces[TRICOUNT][3];
          Matrix<4, 4, float> m_world; 

          unsigned char SKIP_TICKS = 20;
          unsigned char MAX_FRAMESKIP = 5;
          int HALFW, HALFH;
          uint16_t color[6]= {YELLOW,BLACK,YELLOW,BLACK,WHITE,GREEN };

          long next_tick;
          unsigned char draw_type = 1;          // 0 - vertex | 1 - wireframe | 2 - flat colors | ...

          mesh::mesh(){
            mesh::next_tick = millis();
          }

          mesh::mesh(float meshnodes[][3], unsigned char meshfaces[][3]){
            mesh::next_tick = millis();
            memcpy( mesh::nodes, meshnodes, sizeof(float)* NODECOUNT*3);
            memcpy( mesh::faces, meshfaces, sizeof(unsigned char)* TRICOUNT*3);
          }

          void setnodes( float meshnodes[][3] ){
            memcpy( mesh::nodes, meshnodes, sizeof(float)* NODECOUNT*3);
          }

          void setfaces( unsigned char meshfaces[][3] ){
            memcpy( mesh::faces, meshfaces, sizeof(unsigned char)* TRICOUNT*3);
          }

          void setdraw_type( uint8_t drawtype ){
            mesh::draw_type = drawtype;
          }

          void setColors( uint8_t colorpos, uint16_t colour ){
              color[ colorpos ] = colour;
          }

          void setVertexColor( uint16_t front, uint16_t back ){
              setColors(0 , front);
              setColors(1 , back);
          }

          void setWireColor( uint16_t front, uint16_t back ){
              setColors(2 , front);
              setColors(3 , back);
          }

          void setClearColor( uint16_t clearcolor ){
              setColors(4 , clearcolor);
          }

          void setFlatColor( uint16_t flatcolor ){
              setColors(5 , flatcolor);
          }

          void setskip_tick( unsigned char skip_tick ){
            mesh::SKIP_TICKS = skip_tick;
          }

          void setframe_skip( unsigned char frame_skip ){
            mesh::MAX_FRAMESKIP = frame_skip;
          }

          void set_tftsize( int posw, int posh ){
            set_tftpos(posw/2, posh/2);
          }

          void set_tftpos( int posw, int posh ){
            mesh::HALFH = posh;
            mesh::HALFW = posw;
          }

          void set_world( Matrix<4, 4, float> mworld ){
            mesh::HALFW =  mworld(0,0);
            mesh::HALFH =  mworld(1,0);
            long z =  mworld(2,0);
          }

          //---------------------------------------------------------------------------------------------//
          //------------------------------ Update Mesh Functions ----------------------------------------//

          int shoelace( const unsigned char index, boolean projnodes = true );
          bool is_hidden( const unsigned char index, boolean projnodes = true );

          void update( Matrix <4, 4, float> matrix );
          void update( Matrix<4, 4, float> *f() );
          void update( int rotx, int roty, int rotz );
          void update_mesh( void *f()  );
          void draw(TFT3D *canvas, uint16_t timer = 10);

          void draw_vertex(TFT3D *canvas, const uint16_t color, boolean projnodes = true);
          void draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes = true);
          void draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes = true);
          void clear_dirty(TFT3D *canvas , uint16_t color = WHITE , boolean projnodes = true);

      };


      //---------------------------------------------------------------------------------------------//
      //----------------------------- TFT3D Initialization Methods ----------------------------------//
      
      void TFT3D::init() {
            #ifdef _ADAFRUIT_ILI9341H_
              if( _tftype == TFType::TFT_ILI9341 ){
                _tft.begin();
              }
            #endif

            #ifdef _ADAFRUIT_ST7735H_
              if( _tftype == TFType::TFT_ST7735 ){
                _tft.initR( TAB );
              }
            #endif
            #ifdef _ADAFRUIT_TFTLCD_H_
              if( _tftype == TFType::TFT_SPFD548 ){
                _tft.begin( _tft.readID();
              }
            #endif
            #ifdef _PDQ_ST7735H_
              if( _tftype == TFType::TFT_PDQST7735 ){
                _tft.begin();
              }
            #endif

            #ifdef _PDQ_ILI9341H_
              if( _tftype == TFType::TFT_PDQILI9341 ){
                _tft.begin();
              }
            #endif
      }

      //------------------------------------------------------------------------------------------//
        //------------------------------ Class Mesh Methods ----------------------------------------//
        int mesh::shoelace( const unsigned char index, boolean projnodes ){
          unsigned char t = 0;
          int surface = 0;
          int (*matrix_ptr)[2];
          if (projnodes){
            matrix_ptr = mesh::proj_nodes;
          }else{
            matrix_ptr = mesh::old_nodes;
          }

          for (; t<3; t++) {
          // (x1y2 - y1x2) + (x2y3 - y2x3) ...
          surface += (  matrix_ptr[ mesh::faces[index][t] ][0] * matrix_ptr[ mesh::faces[index][(t<2?t+1:0)] ][1] ) -
            ( matrix_ptr[ mesh::faces[index][(t<2?t+1:0)] ][0] * matrix_ptr[ mesh::faces[index][t] ][1] );
          }
          return surface * 0.5;
        };

        bool mesh::is_hidden( const unsigned char index, boolean projnodes ){
          int (*matrix_ptr)[2];
          if (projnodes){
            matrix_ptr = mesh::proj_nodes;
          }else{
            matrix_ptr = mesh::old_nodes;
          }

          return ( ( (matrix_ptr[ mesh::faces[index][0] ][0] * matrix_ptr[ mesh::faces[index][1] ][1]) -
            (matrix_ptr[mesh::faces[index][1] ][0] * matrix_ptr[mesh::faces[index][0] ][1])   ) +
            ( (matrix_ptr[mesh::faces[index][1] ][0] * matrix_ptr[mesh::faces[index][2] ][1]) -
            (matrix_ptr[mesh::faces[index][2] ][0] * matrix_ptr[mesh::faces[index][1] ][1])   ) +
            ( (matrix_ptr[mesh::faces[index][2] ][0] * matrix_ptr[mesh::faces[index][0] ][1]) -
            (matrix_ptr[mesh::faces[index][0] ][0] * matrix_ptr[mesh::faces[index][2] ][1])   ) ) < 0 ? false : true;
        };

      //-----------------------------------------------------------------------------------//
      //------------------------------ Update Mesh ----------------------------------------//
      //--- paramater f is a function for scale,rotation, translation management of mesh---//

      
      void mesh::update( Matrix<4, 4, float> *f() ){

        loops = 0;
        while( millis() > next_tick && loops < mesh::MAX_FRAMESKIP) {
          f();
          //Matrix <4, 4, float> *m_rot ( f() );

          for (int i=0; i<NODECOUNT; i++) {
      
            float arrayNODES[4][1] = {mesh::nodes[i][0] ,mesh::nodes[i][1],mesh::nodes[i][2], 0};
            Matrix <4, 1, float> m_mesh (arrayNODES);

            Matrix <4, 1, float> res = m_world*m_mesh;

            long x =  res(0,0);
            long y =  res(1,0);
            long z =  res(2,0);
            
            // store projected node // Siempre me da el mismo valor ¿?¿?¿?
            mesh::proj_nodes[i][0] = (FOV * x) / (FOV + z) + HALFW;
            mesh::proj_nodes[i][1] = (FOV * y) / (FOV + z) + HALFH;
          }
          mesh::next_tick += mesh::SKIP_TICKS;
          loops++;
        }
      }

      void mesh::update( int rotx, int roty, int rotz ){

        Matrix <4, 4, float> m_rot ( trotx(rotx)*troty(roty)*trotz(rotz) );
        mesh::update(m_rot);

      }

      void mesh::update( Matrix <4, 4, float> matrix ){
        loops = 0;
        
        while( millis() > next_tick && loops < mesh::MAX_FRAMESKIP) {
          
          for (int i=0; i<NODECOUNT; i++) {
      
            float arrayNODES[4][1] = {mesh::nodes[i][0] ,mesh::nodes[i][1],mesh::nodes[i][2], 0};
            
            Matrix <4, 1, float> m_mesh (arrayNODES);
            Matrix <4, 1, float> res = matrix*m_mesh;

            long x =  res(0,0);
            long y =  res(1,0);
            long z =  res(2,0);
            
            // store projected node // Siempre me da el mismo valor ¿?¿?¿?
            mesh::proj_nodes[i][0] = (FOV * x) / (FOV + z) + HALFW;
            mesh::proj_nodes[i][1] = (FOV * y) / (FOV + z) + HALFH;
            
          }
          mesh::next_tick += mesh::SKIP_TICKS;
          loops++;
        }
      }

      void mesh::update_mesh( void *f()  ){
          loops = 0;
          while( millis() > next_tick && loops < MAX_FRAMESKIP) {
            f();

            for (int i=0; i<NODECOUNT; i++) {
              //DUMP (" X ", NODEFLOAT(i,0) );
              //DUMP (" Y ", NODEFLOAT(i,1) );
              //DUMP (" Z ", NODEFLOAT(i,2) );
              //DUMPPRINTLN();
              float arrayNODES[4][1] = {mesh::nodes[i][0] ,mesh::nodes[i][1],mesh::nodes[i][2], 0};
              Matrix <4, 1, float> m_mesh (arrayNODES);

              Matrix <4, 1, float> res = m_world*m_mesh;

              long x =  res(0,0);
              long y =  res(1,0);
              long z =  res(2,0);
              
              // store projected node // Siempre me da el mismo valor ¿?¿?¿?
              proj_nodes[i][0] = (FOV * x) / (FOV + z) + HALFW;
              proj_nodes[i][1] = (FOV * y) / (FOV + z) + HALFH;

              //DUMP("Node: ", i);
              //DUMP (" dX ", x);DUMP (" dY ", y); DUMP (" dZ ", z);
              //DUMPPRINTLN();
              //DUMP (" X ", ((FOV * x) / (FOV + z) + HALFW));
              //DUMP (" Y ", ((FOV * y) / (FOV + z) + HALFH));
              //DUMPPRINTLN();
            }
            next_tick += SKIP_TICKS;
            loops++;
          }
        }

      //-----------------------------------------------------------------------------------//
      //--------------------- Mesh Auxiliar Functions -------------------------------------//
      //--- -------------------------------------------------------------------------------//


        /*int shoelace( const int (*n)[2], const unsigned char index){
          unsigned char t = 0;
          int surface = 0;

          for (; t<3; t++) {
          // (x1y2 - y1x2) + (x2y3 - y2x3) ...
          surface += (n[EDGE(index,t)][0]           * n[EDGE(index,(t<2?t+1:0))][1]) -
            (n[EDGE(index,(t<2?t+1:0))][0] * n[EDGE(index,t)][1]);
          }
          return surface * 0.5;
        };

        bool is_hidden(const int (*n)[2], const unsigned char index){
          return ( ( (n[EDGE(index,0)][0] * n[EDGE(index,1)][1]) -
            (n[EDGE(index,1)][0] * n[EDGE(index,0)][1])   ) +
            ( (n[EDGE(index,1)][0] * n[EDGE(index,2)][1]) -
            (n[EDGE(index,2)][0] * n[EDGE(index,1)][1])   ) +
            ( (n[EDGE(index,2)][0] * n[EDGE(index,0)][1]) -
            (n[EDGE(index,0)][0] * n[EDGE(index,2)][1])   ) ) < 0 ? false : true;
        };*/


      //-----------------------------------------------------------------------------------//
      //------------------------- TFT3D Class Methods -------------------------------------//
      //--- -------------------------------------------------------------------------------//

      /*void TFT3D::draw_vertex( const int (*n)[2], const uint16_t color){
        int i = NODECOUNT-1;
        do {
          _tft.drawPixel(n[i][0],n[i][1], color);
        } while(i--);
      };

        void TFT3D::draw_wireframe( const int (*n)[2], const uint16_t color){
          int i = TRICOUNT-1;
          do {
            // don't draw triangle with negative surface value
            if (!is_hidden(n, i)) {
              // draw triangle edges - 0 -> 1 -> 2 -> 0
              _tft.drawLine(n[EDGE(i,0)][0], n[EDGE(i,0)][1], n[EDGE(i,1)][0], n[EDGE(i,1)][1], color);
              _tft.drawLine(n[EDGE(i,1)][0], n[EDGE(i,1)][1], n[EDGE(i,2)][0], n[EDGE(i,2)][1], color);
              _tft.drawLine(n[EDGE(i,2)][0], n[EDGE(i,2)][1], n[EDGE(i,0)][0], n[EDGE(i,0)][1], color);
            }
          } while(i--);
        };

        void TFT3D::draw_flat_color(  const int (*n)[2], uint16_t color){
          int i = TRICOUNT-1;
          int surface;
          uint16_t col = color;
          do {
            // draw only triangles facing us
            if ((surface=shoelace(n, i)) < 0) {
              // this is an ugly hack but it 'somehow' fakes shading
              // depending on the size of the surface of the triangle
              // change the color toward brighter/darker
              color = col * (surface * 0.001);

              _tft.fillTriangle(n[EDGE(i,0)][0], n[EDGE(i,0)][1],
                n[EDGE(i,1)][0], n[EDGE(i,1)][1],
                n[EDGE(i,2)][0], n[EDGE(i,2)][1],
                color);
            }
          } while(i--);
        };*/


        //------------------------------------------------------------------------------------------//
        //------------------------------ Class Mesh Methods ----------------------------------------//

        void mesh::draw_vertex(TFT3D *canvas, const uint16_t color, boolean projnodes ){
          int i = NODECOUNT-1;

          int (*matrix_ptr)[2];
          if (projnodes){
            matrix_ptr = mesh::proj_nodes;
          }else{
            matrix_ptr = mesh::old_nodes;
          }

          do {
            canvas->_tft.drawPixel( matrix_ptr[i][0], matrix_ptr[i][1], color);
          } while(i--);
          
        };

        void mesh::draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes ){
          int i = TRICOUNT-1;
          int (*matrix_ptr)[2];
          if (projnodes){
            matrix_ptr = mesh::proj_nodes;
          }else{
            matrix_ptr = mesh::old_nodes;
          }

          do {
            // don't draw triangle with negative surface value
            if (!mesh::is_hidden( i , projnodes)) {
              // draw triangle edges - 0 -> 1 -> 2 -> 0
              canvas->_tft.drawLine(matrix_ptr[mesh::faces[i][0]][0], matrix_ptr[mesh::faces[i][0]][1], matrix_ptr[mesh::faces[i][1]][0], matrix_ptr[mesh::faces[i][1]][1], color);
              canvas->_tft.drawLine(matrix_ptr[mesh::faces[i][1]][0], matrix_ptr[mesh::faces[i][1]][1], matrix_ptr[mesh::faces[i][2]][0], matrix_ptr[mesh::faces[i][2]][1], color);
              canvas->_tft.drawLine(matrix_ptr[mesh::faces[i][2]][0], matrix_ptr[mesh::faces[i][2]][1], matrix_ptr[mesh::faces[i][0]][0], matrix_ptr[mesh::faces[i][0]][1], color);
            }
          } while(i--);

        };

        void mesh::draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes ){
          int i = TRICOUNT-1;
          int surface;
          uint16_t col = color;

          int (*matrix_ptr)[2];
          if (projnodes){
            matrix_ptr = mesh::proj_nodes;
          }else{
            matrix_ptr = mesh::old_nodes;
          }

          do {
            // draw only triangles facing us
            if ((surface=mesh::shoelace( i, projnodes )) < 0) {
              // this is an ugly hack but it 'somehow' fakes shading
              // depending on the size of the surface of the triangle
              // change the color toward brighter/darker
              color = col * (surface * 0.001);

              canvas->_tft.fillTriangle(matrix_ptr[mesh::faces[i][0]][0], matrix_ptr[mesh::faces[i][0]][1],
                matrix_ptr[mesh::faces[i][1]][0], matrix_ptr[mesh::faces[i][1]][1],
                matrix_ptr[mesh::faces[i][2]][0], matrix_ptr[mesh::faces[i][2]][1],
                color);
            }
          } while(i--);

        };

        void mesh::clear_dirty(TFT3D *canvas, uint16_t color = WHITE , boolean projnodes ){
          unsigned char x0=canvas->_tft.width(), y0=canvas->_tft.height(), x1=0, y1=0, c, w, h;
          
          int (*matrix_ptr)[2];
          if (projnodes){
            matrix_ptr = mesh::proj_nodes;
          }else{
            matrix_ptr = mesh::old_nodes;
          }
          // get bounding box of mesh

          for (c=0; c<NODECOUNT; c++) {
            if (matrix_ptr[c][0] < x0) x0 = matrix_ptr[c][0];
            if (matrix_ptr[c][0] > x1) x1 = matrix_ptr[c][0];
            if (matrix_ptr[c][1] < y0) y0 = matrix_ptr[c][1];
            if (matrix_ptr[c][1] > y1) y1 = matrix_ptr[c][1];
          }

          // Clear area for PDF_GFX
          #ifdef _PDQ_GFX_H
              canvas->_tft.spi_begin();
              canvas->_tft.setAddrWindow_(x0, y0, x1, y1);
              h = (y1-y0);
              w = (x1-x0)+1;
              do {
                canvas->_tft.spiWrite16(color, w);
              } while (h--);
              canvas->_tft.spi_end();
          #endif

          // Clear area for Adafruit_GFX
          #ifdef _ADAFRUIT_GFX_H
              canvas->_tft.startWrite();
              canvas->_tft.setAddrWindow(x0, y0, x1, y1);
              h = (y1-y0);
              w = (x1-x0)+1;
              do{
                canvas->_tft.writePixels(color, w);
              } while (h--);
              canvas->_tft.endWrite();
          #endif

        };

        void mesh::draw(TFT3D *canvas, uint16_t timer){
          //( (millis() - mesh::next_tick) > timer)
          if (memcmp(mesh::old_nodes, mesh::proj_nodes, sizeof(mesh::proj_nodes)) ) {
          // render frame

            switch(mesh::draw_type) {
              case 0: 
              mesh::draw_vertex( canvas, color[0],0);
              mesh::draw_vertex( canvas, color[1],1);
            break;
              case 1: if (TRICOUNT > 32) {
              mesh::clear_dirty( canvas, mesh::old_nodes);
            }
            else {
              mesh::draw_wireframe( canvas, color[2],0);
              mesh::draw_wireframe( canvas, color[3],1);
            }
            break;
            case 2: mesh::clear_dirty( canvas, color[4], 0);
              mesh::draw_flat_color( canvas, color[5],1);
            break;
            case 3: mesh::draw_flat_color( canvas, color[4],1);
              mesh::draw_wireframe( canvas, color[2],1);
            break;
            case 4: mesh::draw_flat_color( canvas, color[5],1);
            break;
            }
            // copy projected nodes to old_nodes to check if we need to redraw next frame

            memcpy(mesh::old_nodes, mesh::proj_nodes, sizeof(mesh::proj_nodes));
            
          }
        }


  #endif
#endif