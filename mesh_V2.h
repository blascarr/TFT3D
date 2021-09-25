
#ifndef _MESH_h
	#define _MESH_h  
	
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
	
	static int loops;
	#ifndef MAX_NODESIZE
		#define MAX_NODESIZE 40
	#endif

	#ifndef MAX_TRISIZE
		#define MAX_TRISIZE 40
	#endif
	
	#ifdef VECTOR_H
		typedef Matrix <1, 2, float> ( vector2D );

		typedef Matrix <1, 3, float> ( vector3D );
		typedef Matrix <1, 3, uint8_t > ( point3D );
	#endif

	class mesh3D{
		public:

			#ifndef VECTOR_H
				int NODECOUNT, TRICOUNT;

				int proj_nodes[MAX_NODESIZE][2];
				int old_nodes[MAX_NODESIZE][2];

				float nodes[MAX_NODESIZE][3];
				unsigned char faces[MAX_TRISIZE][3];

				//float* nodes[][3];
				//unsigned char *faces[][3];
			#else
				
				Vector< vector2D > proj_nodes;
				Vector< vector2D > old_nodes;

				//Vector< vector3D > nodes;
				//Vector< point3D > faces;

				Vector< vector3D > *_nodes;
				Vector< point3D > *_faces;
			#endif

			Matrix<4, 4, float> m_world; 

			unsigned char SKIP_TICKS = 20;
			unsigned char MAX_FRAMESKIP = 5;
			int HALFW, HALFH;
			uint16_t color[6]= {YELLOW,BLACK,YELLOW,BLACK,WHITE,GREEN };

			long next_tick;
			unsigned char draw_type = 1;          // 0 - vertex | 1 - wireframe | 2 - flat colors | ...
			uint8_t nodesize = 0;
			uint8_t facesize = 0;

			mesh3D::mesh3D(){
				mesh3D::next_tick = millis();
			}

			#ifndef VECTOR_H
				mesh3D::mesh3D(float meshnodes[][3], unsigned char meshfaces[][3], int NSIZE , int TSIZE ):  NODECOUNT( NSIZE ), TRICOUNT(TSIZE){
					mesh3D::next_tick = millis();
					mesh3D::setnodes(meshnodes, NODECOUNT );
					mesh3D::setfaces(meshfaces, TRICOUNT );
				}

				void setnodes( float meshnodes[][3] , int NODE_SIZE = MAX_NODESIZE ){
					memcpy( mesh3D::nodes, meshnodes, sizeof(float)* NODE_SIZE*3);
				}

				void setfaces( unsigned char meshfaces[][3], int TRI_SIZE = MAX_TRISIZE  ){
					memcpy( mesh3D::faces, meshfaces, sizeof(unsigned char)* TRI_SIZE*3);
				}
			#else

				mesh3D::mesh3D(Vector< vector3D > *meshnodes, Vector< point3D >  *meshfaces):_nodes(meshnodes), _faces(meshfaces) {
					mesh3D::next_tick = millis();
					mesh3D::setnodes( meshnodes );
					mesh3D::setfaces( meshfaces );
				}

				void setnodes( Vector< vector3D > *meshnodes){
					_nodes = meshnodes;
					nodesize = meshnodes->size();
				}

				void setfaces( Vector< point3D > *meshfaces){
					_faces = meshfaces;
					facesize = _faces->size();
				}

				//-----------------NO POINTERS---------------//

				mesh3D::mesh3D(float meshnodes[][1][3], unsigned char meshfaces[][1][3], uint8_t NSIZE, uint8_t TSIZE){
					mesh3D::next_tick = millis();
					nodesize = NSIZE;
					facesize = TSIZE;
					//mesh3D::setnodes(meshnodes, NSIZE );
					//mesh3D::setfaces(meshfaces, TSIZE );
	
				}

				/*void setnodes( vector3D meshnodes[], int NSIZE  , int MAX_NSIZE =MAX_NODESIZE){
					
					nodes.setStorage( meshnodes , MAX_NSIZE ,NSIZE );

					nodesize = nodes.size();
				}*/

				void setnodes( float meshnodes[][1][3], int NSIZE ){
					//memcpy( mesh3D::nodes, meshnodes, sizeof( meshnodes ) );

					/*vector3D nodev [NSIZE];

					for (int i =0; i < NSIZE; i ++){
						//Serial << "NodeList: " << meshnodes[i] ;
						nodev[i] = meshnodes[i];
						//nodes[i] = meshnodes[i];
					}
					nodes.setStorage( nodev , MAX_NODESIZE ,NSIZE );*/
				}

				/*void setfaces( point3D meshfaces[], int TSIZE , int MAX_TSIZE=MAX_TRISIZE ){
					faces.setStorage( meshfaces , MAX_TSIZE ,TSIZE );
					facesize = faces.size();
				}*/

				void printdata(){
					Serial.print("Size Nodes: ");
					Serial.println( nodesize );
					for(int i = 0; i < nodesize; i++) {
						Serial << "NodeList: " << (*_nodes)[i] ;
						Serial.println();
					}
					Serial.println();
					Serial.println();
					Serial.print("Size Faces: ");
					Serial.println( facesize );
					for(int i = 0; i < facesize ; i++) {
						Serial << "FaceList: " << (*_faces)[i] ;
						Serial.println();
					}

				}

			#endif

			void setdraw_type( uint8_t drawtype ){
				mesh3D::draw_type = drawtype;
			}

			void setColors( uint8_t colorpos, uint16_t colour ){
			  color[ colorpos ] = colour;
			}

			void setColors( uint16_t colour[6] ){
				memcpy( color, colour, sizeof(uint16_t)*6);
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
				mesh3D::SKIP_TICKS = skip_tick;
			}

			void setframe_skip( unsigned char frame_skip ){
				mesh3D::MAX_FRAMESKIP = frame_skip;
			}

			void set_tftsize( int posw, int posh ){
				set_tftpos(posw/2, posh/2);
			}

			void set_tftpos( int posw, int posh ){
				mesh3D::HALFH = posh;
				mesh3D::HALFW = posw;
			}

				void set_world( Matrix<4, 4, float> mworld ){
				mesh3D::HALFW =  mworld(0,0);
				mesh3D::HALFH =  mworld(1,0);
				long z =  mworld(2,0);
			}

			//---------------------------------------------------------------------------------------------//
			//------------------------------ Update Mesh Functions ----------------------------------------//

			int shoelace( const unsigned char index, boolean projnodes = true );
			bool is_hidden( const unsigned char index, boolean projnodes = true );

			void update( Matrix <4, 4, float> matrix );
			void update( Matrix<4, 4, float> *f() );
			void update( int rotx, int roty, int rotz );
			void update( void *f()  );
			void update_mesh( Matrix <4, 4, float> m );
			void draw( TFT3D *canvas, uint16_t timer = 10);

			void draw_vertex( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
			void draw_wireframe( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
			void draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes = true);
			void clear_dirty( TFT3D *canvas , uint16_t color = WHITE , boolean projnodes = true);
		};


		//------------------------------------------------------------------------------------------//
		//------------------------------ Class Mesh Methods ----------------------------------------//
		#ifndef VECTOR_H
			int mesh3D::shoelace( const unsigned char index, boolean projnodes ){
				unsigned char t = 0;
				int surface = 0;
				int (*matrix_ptr)[2];
				if (projnodes){
					matrix_ptr = mesh3D::proj_nodes;
				}else{
					matrix_ptr = mesh3D::old_nodes;
				}

				for (; t<3; t++) {
					// (x1y2 - y1x2) + (x2y3 - y2x3) ...
					surface += (  matrix_ptr[ mesh3D::faces[index][t] ][0] * matrix_ptr[ mesh3D::faces[index][(t<2?t+1:0)] ][1] ) -
					( matrix_ptr[ mesh3D::faces[index][(t<2?t+1:0)] ][0] * matrix_ptr[ mesh3D::faces[index][t] ][1] );
				}
				return surface * 0.5;
			};

			bool mesh3D::is_hidden( const unsigned char index, boolean projnodes ){
				int (*matrix_ptr)[2];
				if (projnodes){
					matrix_ptr = mesh3D::proj_nodes;
				}else{
					matrix_ptr = mesh3D::old_nodes;
				}

				return ( ( (matrix_ptr[ mesh3D::faces[index][0] ][0] * matrix_ptr[ mesh3D::faces[index][1] ][1]) -
				(matrix_ptr[mesh3D::faces[index][1] ][0] * matrix_ptr[mesh3D::faces[index][0] ][1])   ) +
				( (matrix_ptr[mesh3D::faces[index][1] ][0] * matrix_ptr[mesh3D::faces[index][2] ][1]) -
				(matrix_ptr[mesh3D::faces[index][2] ][0] * matrix_ptr[mesh3D::faces[index][1] ][1])   ) +
				( (matrix_ptr[mesh3D::faces[index][2] ][0] * matrix_ptr[mesh3D::faces[index][0] ][1]) -
				(matrix_ptr[mesh3D::faces[index][0] ][0] * matrix_ptr[mesh3D::faces[index][2] ][1])   ) ) < 0 ? false : true;
			};

			
		#else
			int mesh3D::shoelace( const unsigned char index, boolean projnodes ){
				unsigned char t = 0;
				int surface = 0;
				Vector< vector2D >(*matrix_ptr);
				
				if (projnodes){
					//matrix_ptr = mesh3D::proj_nodes;
				}else{
					//matrix_ptr = mesh3D::old_nodes;
				}

				for (; t<3; t++) {
					// (x1y2 - y1x2) + (x2y3 - y2x3) ...
					//surface += (  matrix_ptr[ mesh::faces[index][t] ][0] * matrix_ptr[ mesh::faces[index][(t<2?t+1:0)] ][1] ) - ( matrix_ptr[ mesh::faces[index][(t<2?t+1:0)] ][0] * matrix_ptr[ mesh::faces[index][t] ][1] );
				}

				return surface *0.5;
			};

			bool mesh3D::is_hidden( const unsigned char index, boolean projnodes ){
				int (*matrix_ptr)[2];

				return true;
			};
		#endif

		//-----------------------------------------------------------------------------------//
		//------------------------------ Update Mesh ----------------------------------------//
		//--- paramater f is a function for scale,rotation, translation management of mesh---//

		//Execute an action every update of model
		void mesh3D::update( void *f()  ){
			loops = 0;
			while( millis() > next_tick && loops < MAX_FRAMESKIP) {
				f();

				update_mesh( m_world );
				next_tick += SKIP_TICKS;
				loops++;
			}
				
		}

		//Function return transformation matrix manipulating Model
		void mesh3D::update( Matrix<4, 4, float> *f() ){

			loops = 0;
			while( millis() > next_tick && loops < mesh3D::MAX_FRAMESKIP) {
				f();
				//Matrix <4, 4, float> *m_rot ( f() );

				update_mesh( m_world );

				mesh3D::next_tick += mesh3D::SKIP_TICKS;
				loops++;
			}
		}

		void mesh3D::update( Matrix <4, 4, float> matrix ){
			loops = 0;

			while( millis() > next_tick && loops < mesh3D::MAX_FRAMESKIP) {
			  
				update_mesh( matrix );

				mesh3D::next_tick += mesh3D::SKIP_TICKS;
				loops++;
			}
		}

		void mesh3D::update( int rotx, int roty, int rotz ){

			Matrix <4, 4, float> m_rot ( trotx(rotx)*troty(roty)*trotz(rotz) );
			mesh3D::update(m_rot);

		}

		//Dependiente de Vector.h
		void mesh3D::update_mesh( Matrix <4, 4, float> m ){
			#ifndef VECTOR_H
				for (int i=0; i<NODECOUNT; i++) {

					float arrayNODES[4][1] = {mesh3D::nodes[i][0] ,mesh3D::nodes[i][1],mesh3D::nodes[i][2], 0};

					Matrix <4, 1, float> m_mesh (arrayNODES);
					Matrix <4, 1, float> res = m*m_mesh;

					long x =  res(0,0);
					long y =  res(1,0);
					long z =  res(2,0);

					//Scale parameter stored in Transformation Matrix
					long s =  m(3,3);

					// store projected node //
					mesh3D::proj_nodes[i][0] = s*((FOV * x) / (FOV + z)) + HALFW;
					mesh3D::proj_nodes[i][1] = s*((FOV * y) / (FOV + z)) + HALFH;

				}
			#else

			#endif
		}

	#ifdef _TFT3D_h
		//------------------------------------------------------------------------------------------//
		//------------------------------ Class Mesh Methods ----------------------------------------//
		#ifndef VECTOR_H
			void mesh3D::draw_vertex(TFT3D *canvas, const uint16_t color, boolean projnodes ){
				int i = NODECOUNT-1;

				int (*matrix_ptr)[2];
				if (projnodes){
					matrix_ptr = mesh3D::proj_nodes;
				}else{
					matrix_ptr = mesh3D::old_nodes;
				}

				do {
					canvas->_tft.drawPixel( matrix_ptr[i][0], matrix_ptr[i][1], color);
				} while(i--);

			};

			void mesh3D::draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes ){
				int i = TRICOUNT-1;
				int (*matrix_ptr)[2];
				if (projnodes){
					matrix_ptr = mesh3D::proj_nodes;
				}else{
					matrix_ptr = mesh3D::old_nodes;
				}

				do {
					// don't draw triangle with negative surface value
					if (!mesh3D::is_hidden( i , projnodes)) {
						// draw triangle edges - 0 -> 1 -> 2 -> 0
						canvas->_tft.drawLine(matrix_ptr[mesh3D::faces[i][0]][0], matrix_ptr[mesh3D::faces[i][0]][1], matrix_ptr[mesh3D::faces[i][1]][0], matrix_ptr[mesh3D::faces[i][1]][1], color);
						canvas->_tft.drawLine(matrix_ptr[mesh3D::faces[i][1]][0], matrix_ptr[mesh3D::faces[i][1]][1], matrix_ptr[mesh3D::faces[i][2]][0], matrix_ptr[mesh3D::faces[i][2]][1], color);
						canvas->_tft.drawLine(matrix_ptr[mesh3D::faces[i][2]][0], matrix_ptr[mesh3D::faces[i][2]][1], matrix_ptr[mesh3D::faces[i][0]][0], matrix_ptr[mesh3D::faces[i][0]][1], color);
					}
				} while(i--);
			};

			void mesh3D::draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes ){
				int i = TRICOUNT-1;
				int surface;
				uint16_t col = color;

				int (*matrix_ptr)[2];
				if (projnodes){
					matrix_ptr = mesh3D::proj_nodes;
				}else{
					matrix_ptr = mesh3D::old_nodes;
				}

				do {
					// draw only triangles facing us
					if ((surface=mesh3D::shoelace( i, projnodes )) < 0) {
						// this is an ugly hack but it 'somehow' fakes shading
						// depending on the size of the surface of the triangle
						// change the color toward brighter/darker
						color = col * (surface * 0.001);

						canvas->_tft.fillTriangle(matrix_ptr[mesh3D::faces[i][0]][0], matrix_ptr[mesh3D::faces[i][0]][1],
						matrix_ptr[mesh3D::faces[i][1]][0], matrix_ptr[mesh3D::faces[i][1]][1],
						matrix_ptr[mesh3D::faces[i][2]][0], matrix_ptr[mesh3D::faces[i][2]][1],
						color);
					}
				} while(i--);

			};

			void mesh3D::clear_dirty(TFT3D *canvas, uint16_t color = WHITE , boolean projnodes ){
				unsigned char x0=canvas->_tft.width(), y0=canvas->_tft.height(), x1=0, y1=0, c, w, h;

				int (*matrix_ptr)[2];
				if (projnodes){
					matrix_ptr = mesh3D::proj_nodes;
				}else{
					matrix_ptr = mesh3D::old_nodes;
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

			void mesh3D::draw(TFT3D *canvas, uint16_t timer){
				//( (millis() - mesh::next_tick) > timer)
				if (memcmp(mesh3D::old_nodes, mesh3D::proj_nodes, sizeof(mesh3D::proj_nodes)) ) {
				// render frame

				switch(mesh3D::draw_type) {
					case 0: 
						mesh3D::draw_vertex( canvas, color[0],0);
						mesh3D::draw_vertex( canvas, color[1],1);
					break;
					
					case 1: 
						if (TRICOUNT > 32) {
							mesh3D::clear_dirty( canvas, mesh3D::old_nodes);
						}
						else {
							mesh3D::draw_wireframe( canvas, color[2],0);
							mesh3D::draw_wireframe( canvas, color[3],1);
						}
					break;

					case 2: mesh3D::clear_dirty( canvas, color[4], 0);
						mesh3D::draw_flat_color( canvas, color[5],1);
					break;
					
					case 3: mesh3D::draw_flat_color( canvas, color[4],1);
						mesh3D::draw_wireframe( canvas, color[2],1);
					break;
					case 4: mesh3D::draw_flat_color( canvas, color[5],1);
					break;
				}
				// copy projected nodes to old_nodes to check if we need to redraw next frame
				memcpy(mesh3D::old_nodes, mesh3D::proj_nodes, sizeof(mesh3D::proj_nodes));

				}
			}
		#else
			void mesh3D::draw_vertex(TFT3D *canvas, const uint16_t color, boolean projnodes ){

			}

			void mesh3D::draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes ){

			}

			void mesh3D::draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes ){

			}

			void mesh3D::clear_dirty(TFT3D *canvas, uint16_t color = WHITE , boolean projnodes ){

			}

			void mesh3D::draw(TFT3D *canvas, uint16_t timer){

			}

		#endif

	#endif

#endif