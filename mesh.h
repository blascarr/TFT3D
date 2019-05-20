
#ifndef _MESH_h
	#define _MESH_h  
	
	static int loops;
	#ifndef MAX_NODESIZE
		#define MAX_NODESIZE 40
	#endif

	#ifndef MAX_TRISIZE
		#define MAX_TRISIZE 40
	#endif

	#if (defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)) && ( defined( __SD_H__ ) )
		#pragma message("Warning!  You need more memory to execute 3DMesh Models. Use Arduino MEGA instead.")		
	#endif

	#ifdef VECTOR_H
		typedef Matrix <1, 3, Array<1,3,float> > ( vector3D );
	#endif

	template<int n_rel = 3>
	class generic_mesh{
		public:
			int proj_nodes[MAX_NODESIZE][2];
			int old_nodes[MAX_NODESIZE][2];

			Matrix<4, 4, Array<4,4,float> > m_world = { 1, 0, 0,0,    0, 1, 0, 0,   0, 0, 1, 0,    0, 0, 0, 1}; ; 
			unsigned char SKIP_TICKS = 20;
			unsigned char MAX_FRAMESKIP = 5;
			int HALFW, HALFH;
			uint16_t color[6]= {YELLOW,BLACK,YELLOW,BLACK,WHITE,GREEN };

			long next_tick;
			unsigned char draw_type = 1;          // 0 - vertex | 1 - wireframe | 2 - flat colors | ...
			uint8_t nodesize = 0;
			uint8_t facesize = 0;

			#ifndef VECTOR_H
				float _nodes [MAX_NODESIZE][3];
				uint8_t _faces [MAX_TRISIZE][n_rel];
			#else
				Vector< vector3D > *_nodes;
				Vector< uint8_t [n_rel] > *_faces;
			#endif

			#ifdef DEBUG_AXIS
				Matrix<4, 4, Array<4,4,float> > old_world;
			#endif

			generic_mesh(){
				generic_mesh::next_tick = millis();
			}

			//------------------------------------------------------------------------------------------//
			//------------------------------ Mesh Config Methods ----------------------------------------//
	
			void setdraw_type( uint8_t drawtype ){ draw_type = drawtype; };
			void setColors( uint8_t colorpos, uint16_t colour ) {color[ colorpos ] = colour; };
			void setColors( uint16_t colour[6] ){ memcpy( color, colour, sizeof(uint16_t)*6); };
			void setVertexColor( uint16_t front, uint16_t back ){
				setColors(0 , front);
				setColors(1 , back);
			};
			void setWireColor( uint16_t front, uint16_t back ){
				setColors(2 , front);
				setColors(3 , back);
			};
			void setClearColor( uint16_t clearcolor ){ setColors(4 , clearcolor); }
			void setFlatColor( uint16_t flatcolor ) { setColors(5 , flatcolor); }
			void setskip_tick( unsigned char skip_tick ){ SKIP_TICKS = skip_tick; }
			void setframe_skip( unsigned char frame_skip ){ MAX_FRAMESKIP = frame_skip; }
			void set_tftsize( int posw, int posh ){ set_tftpos(posw/2, posh/2); }

			void set_tftpos( int posw, int posh ){
				HALFH = posh;
				HALFW = posw;
			};
			void set_world( Matrix<4, 4, Array<4,4,float> > mworld ){ m_world = mworld; }
			
			//-----------------------------------------------------------------------------------//
			//------------------------------ Update Mesh ----------------------------------------//
			//--- paramater f is a function for scale,rotation, translation management of mesh---//
			
			void update( Matrix <4, 4, Array<4,4,float> > matrix ){
				loops = 0;

				while( millis() > next_tick && loops < MAX_FRAMESKIP) {
					
					set_world(matrix);
					update_mesh( matrix );
					
					next_tick += SKIP_TICKS;
					loops++;
				}
					
			};

			//Function return transformation matrix manipulating Model

			void update( Matrix<4, 4, Array<4,4,float> > *f() ){

				loops = 0;
				while( millis() > next_tick && loops < MAX_FRAMESKIP) {

					// f returns a Matrix that change m_world
					set_world( f() );
					update_mesh( m_world );

					next_tick += SKIP_TICKS;
					loops++;
				}
			};

			void update( void *f()  ){
				loops = 0;
				while( millis() > next_tick && loops < MAX_FRAMESKIP) {
					f(); // Update m_world inside function f.

					update_mesh( m_world );
					next_tick += SKIP_TICKS;
					loops++;
				}
					
			};

			void rotate( float gx, float gy, float gz ){ 
				Matrix <3, 3, Array<3,3,float> > m_rot = ( m_world * trotx(gx)*troty(gy)*trotz(gz) ).Submatrix( Slice<0,3>(), Slice<0,3>() );
				
				RPose( m_rot );
			}

			void rotateTo( float rotx, float roty, float rotz ){ 
				Matrix <3, 3, Array<3,3,float> > m_rot = ( trotx(rotx)*troty(roty)*trotz(rotz) ).Submatrix( Slice<0,3>(), Slice<0,3>() );
				RPose( m_rot ); 
			}
			
			void move( float dx, float dy, float dz ){ 
				float arr[3][1] = { {m_world(0,3)+dx}, {m_world(1,3)+dy}, {m_world(2,3)+dz} };
				TPose( arr );
			}

			void moveTo( float tx, float ty, float tz ){ 
				float arr[3][1] = { {tx}, {ty}, {tz} };
				TPose( arr );
			}

			void update( float rotx, float roty, float rotz, float tx, float ty, float tz, float scale = 1 ){
				Matrix <4, 4, Array<4,4,float> > m_rot ( trotx(rotx)*troty(roty)*trotz(rotz) );
				pose( m_rot*transl(  tx,  ty,  tz ) );
			};

			virtual void update_mesh( Matrix <4, 4, Array<4,4,float> > m ){
				for (int i=0; i< nodesize; i++) {

					//float arrayNODES[4][1] = { _nodes[i][0] , _nodes[i][1], _nodes[i][2], 1 };

					//Matrix <4, 1 > m_mesh (arrayNODES);

					//Matrix <2, 1 > res = mesh2D( m_mesh, m );
					//proj_nodes[i][0] = res(0,0);
					//proj_nodes[i][1] = res(1,0);

				}
			};

			#ifdef DEBUG_AXIS
				void draw_axis( TFT3D *canvas, int d, uint16_t colorX = RED , uint16_t colorY = BLUE, uint16_t colorZ = GREEN ){
					
					if ( memcmp( &m_world, &old_world, sizeof(m_world) ) ){
						//Define origin of model
						Matrix <2, 1, Array<2,1,float> > old_origin = mesh2D( { 0 ,0, 0, 1}, old_world );
						Matrix <2, 1, Array<2,1,float> > origin = mesh2D( { 0 ,0, 0, 1}, m_world );
						Matrix <2, 1, Array<2,1,float> > resx;
						Matrix <2, 1, Array<2,1,float> > resy;
						Matrix <2, 1, Array<2,1,float> > resz;
						//Clear old axis
						resx = mesh2D( { d ,0, 0, 1}, old_world );
						canvas->_tft.drawLine( old_origin(0,0) , old_origin(1,0) , resx(0,0), resx(1,0) , color[1]);
						
						resy = mesh2D( { 0 ,d, 0, 1}, old_world );
						canvas->_tft.drawLine( old_origin(0,0) , old_origin(1,0), resy(0,0), resy(1,0) , color[1]);

						resz = mesh2D( { 0 ,0, d, 1}, old_world );
						canvas->_tft.drawLine( old_origin(0,0) , old_origin(1,0), resz(0,0), resz(1,0) , color[1]);

						//Paint new axis
						resx = mesh2D( { d ,0, 0, 1}, m_world );
						canvas->_tft.drawLine( origin(0,0) , origin(1,0) , resx(0,0), resx(1,0) , colorX);

						resy = mesh2D( { 0 ,d, 0, 1}, m_world );
						canvas->_tft.drawLine( origin(0,0) , origin(1,0), resy(0,0), resy(1,0) , colorY);

						resz = mesh2D( { 0 ,0, d, 1}, m_world );
						canvas->_tft.drawLine( origin(0,0) , origin(1,0), resz(0,0), resz(1,0) , colorZ);

						old_world = m_world;

					}
				};
			#endif

		protected:
			Matrix <2, 1, Array<2,1,float> > mesh2D(Matrix <4, 1, Array<4,1,float> > v, Matrix <4, 4, Array<4,4,float> > m){

				Matrix <4, 1, Array<4,1,float> > res = m*v;
				float arrayNODES[2][1] =  { m(3,3)*((FOV * res(0,0)) / (FOV + res(2,0))) + HALFW   ,  m(3,3)*((FOV * res(1,0)) / (FOV + res(2,0))) + HALFH };
				Matrix <2, 1, Array<2,1,float> > res2D (arrayNODES);

				return res2D;
			};

			void pose( Matrix <4, 4, Array<4,4,float> > matrix ){
				update( m_world ); 
			}

			void RPose( Matrix <3, 3, Array<3,3,float> > m_rot ){
				m_world.Submatrix(  Slice<0,3>(), Slice<0,3>() )  = m_rot;
				update( m_world ); 
			}

			void TPose( float m_trans[3][1] ){
				m_world.Submatrix( Slice<0,3>(), Slice<3,4>() )  = m_trans;
				update( m_world ); 
			}
	};

	class mesh: public generic_mesh< 3 > {
		public:
			void draw( TFT3D *canvas, uint16_t timer = 10);
			void update_mesh( Matrix <4, 4, Array<4,4,float> > m );
			
			void draw_vertex( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
			void draw_wireframe( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
			void draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes = true);

			void clear_dirty( TFT3D *canvas , uint16_t color = WHITE , boolean projnodes = true);

			int shoelace( const unsigned char index, boolean projnodes = true );
			bool is_hidden( const unsigned char index, boolean projnodes = true );
			
			#ifdef VECTOR_H

				mesh::mesh(Vector< vector3D > *meshnodes, Vector< uint8_t [3] > *meshfaces ) {
					mesh::next_tick = millis();
					mesh::setnodes( meshnodes );
					mesh::setfaces( meshfaces );
				}

				void set_mesh( Vector< vector3D > *meshnodes, Vector< uint8_t [3] >  *meshfaces){
					mesh::next_tick = millis();
					mesh::setnodes( meshnodes );
					mesh::setfaces( meshfaces );
				}

				void setnodes( Vector< vector3D > *meshnodes){
					_nodes = meshnodes;
					nodesize = meshnodes->Size();
				}

				void setfaces( Vector< uint8_t [3] > *meshfaces ){
					_faces = meshfaces;
					facesize = _faces->Size();
				}

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
					/*for(int i = 0; i < facesize ; i++) {
						Serial << "FaceList: " << (*_faces)[i] ;
						Serial.println();
					}*/

				}
				
			#else

				mesh::mesh(float meshnodes[][3], uint8_t meshfaces[][3], int NSIZE , int TSIZE  ) {

					mesh::next_tick = millis();
					nodesize = NSIZE ;
					facesize = TSIZE ;
					mesh::setnodes(meshnodes, nodesize );
					mesh::setfaces(meshfaces, facesize );
				}

				void setnodes( float meshnodes[][3] , int NODE_SIZE = MAX_NODESIZE ){
					memcpy( _nodes, meshnodes, sizeof(float)* NODE_SIZE*3);
					nodesize = NODE_SIZE ;
				}

				void setfaces( uint8_t meshfaces[][3], int TRI_SIZE = MAX_TRISIZE  ){
					memcpy( _faces, meshfaces, sizeof(uint8_t)* TRI_SIZE*3);
					facesize = TRI_SIZE ;
				}

				
			#endif

	};	

	void mesh::draw( TFT3D *canvas, uint16_t timer){
		//( (millis() - mesh::next_tick) > timer)

		if (memcmp(mesh::old_nodes, mesh::proj_nodes, sizeof(mesh::proj_nodes)) ) {
		// render frame
			switch(mesh::draw_type) {
				case 0: 
					mesh::draw_vertex( canvas, color[0],0);
					mesh::draw_vertex( canvas, color[1],1);
				break;
				
				case 1: 
					if (facesize > 32) {
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

	void mesh::draw_vertex(TFT3D *canvas, const uint16_t color, boolean projnodes ){
		int i = nodesize-1;

		int (*matrix_ptr)[2];
		if (projnodes){
			matrix_ptr = mesh::proj_nodes;
		}else{
			matrix_ptr = mesh::old_nodes;
		}

		do {
			//If out of canvas, not print

			canvas->_tft.drawPixel( matrix_ptr[i][0], matrix_ptr[i][1], color);
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

		for (c=0; c<nodesize; c++) {
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
				//canvas->_tft.writePixels(color, w);
			} while (h--);
			canvas->_tft.endWrite();
		#endif
	};

	#ifdef VECTOR_H
		#include "vectorMesh.h"
	#else
		#include "arrayMesh.h"
	#endif


#endif