
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
		#include "vectorMesh.h"

		void mesh::draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes ){
			
			int i = facesize-1;
			int (*matrix_ptr)[2];

			if (projnodes){
				matrix_ptr = mesh::proj_nodes;
			}else{
				matrix_ptr = mesh::old_nodes;
			}
			
			do {
				
				if (!mesh::is_hidden( i , projnodes)) {
					
					// draw triangle edges - 0 -> 1 -> 2 -> 0
					//If out of canvas, not print
					
					canvas->_tft.drawLine(matrix_ptr[ (*_faces)[i](0,0)][0], matrix_ptr[ (*_faces)[i](0,0)][1], matrix_ptr[ (*_faces)[i](0,1)][0], matrix_ptr[ (*_faces)[i](0,1)][1], color);
					canvas->_tft.drawLine(matrix_ptr[ (*_faces)[i](0,1)][0], matrix_ptr[ (*_faces)[i](0,1)][1], matrix_ptr[ (*_faces)[i](0,2)][0], matrix_ptr[ (*_faces)[i](0,2)][1], color);
					canvas->_tft.drawLine(matrix_ptr[ (*_faces)[i](0,2)][0], matrix_ptr[ (*_faces)[i](0,2)][1], matrix_ptr[ (*_faces)[i](0,0)][0], matrix_ptr[ (*_faces)[i](0,0)][1], color);
				}
			} while(i--);
		}

		void mesh::draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes ){
			
			int i = facesize-1;
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
					//If out of canvas, not print

					canvas->_tft.fillTriangle(matrix_ptr[ (*_faces)[i](0,0)][0], matrix_ptr[ (*_faces)[i](0,0)][1],
					matrix_ptr[ (*_faces)[i](0,1)][0], matrix_ptr[ (*_faces)[i](0,1)][1],
					matrix_ptr[ (*_faces)[i](0,2)][0], matrix_ptr[ (*_faces)[i](0,2)][1], 
					color);
				}
			} while(i--);
		}

	#else
		#include "arrayMesh.h"
		//------------------------------------------------------------------------------------------//
		//------------------------------ Class Mesh Methods ----------------------------------------//


		void mesh::draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes ){
			int i = facesize-1;
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
					//If out of canvas, not print

					canvas->_tft.drawLine(matrix_ptr[mesh::faces[i][0]][0], matrix_ptr[mesh::faces[i][0]][1], matrix_ptr[mesh::faces[i][1]][0], matrix_ptr[mesh::faces[i][1]][1], color);
					canvas->_tft.drawLine(matrix_ptr[mesh::faces[i][1]][0], matrix_ptr[mesh::faces[i][1]][1], matrix_ptr[mesh::faces[i][2]][0], matrix_ptr[mesh::faces[i][2]][1], color);
					canvas->_tft.drawLine(matrix_ptr[mesh::faces[i][2]][0], matrix_ptr[mesh::faces[i][2]][1], matrix_ptr[mesh::faces[i][0]][0], matrix_ptr[mesh::faces[i][0]][1], color);
				}
			} while(i--);

		};

		void mesh::draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes ){
			int i = facesize-1;
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
					//If out of canvas, not print

					canvas->_tft.fillTriangle(matrix_ptr[mesh::faces[i][0]][0], matrix_ptr[mesh::faces[i][0]][1],
					matrix_ptr[mesh::faces[i][1]][0], matrix_ptr[mesh::faces[i][1]][1],
					matrix_ptr[mesh::faces[i][2]][0], matrix_ptr[mesh::faces[i][2]][1],
					color);
				}
			} while(i--);

		};

	#endif

	Matrix <2, 1 > mesh::mesh2D(Matrix <4, 1 > v, Matrix <4, 4 > m){
		Matrix <4, 1 > res = m*v;
		float arrayNODES[2][1] =  { m(3,3)*((FOV * res(0,0)) / (FOV + res(2,0))) + HALFW   ,  m(3,3)*((FOV * res(1,0)) / (FOV + res(2,0))) + HALFH };
		Matrix <2, 1 > res2D (arrayNODES);
		return res2D;
	}

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

	#ifndef DEBUG_AXIS

		void mesh::draw_axis( TFT3D *canvas, int timer, int d, uint16_t colorX , uint16_t colorY, uint16_t colorZ  ){
			//if ( (millis() - mesh::next_tick) > timer){
				//mesh::next_tick = millis();
				
				Matrix <2, 1 > origin = mesh2D( { 0 ,0, 0, 1}, m_world );

				Matrix <2, 1 > resx = mesh2D( { d ,0, 0, 1}, m_world );
				canvas->_tft.drawLine( origin(0,0) , origin(1,0) , resx(0,0), resx(1,0) , colorX);

				Matrix <2, 1 > resy = mesh2D( { 0 ,d, 0, 1}, m_world );
				canvas->_tft.drawLine( origin(0,0) , origin(1,0), resy(0,0), resy(1,0) , colorY);

				Matrix <2, 1 > resz = mesh2D( { 0 ,0, d, 1}, m_world );
				canvas->_tft.drawLine( origin(0,0) , origin(1,0), resz(0,0), resz(1,0) , colorZ);

			//}
		}
	#endif
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

	//-----------------------------------------------------------------------------------//
	//------------------------------ Update Mesh ----------------------------------------//
	//--- paramater f is a function for scale,rotation, translation management of mesh---//

	void mesh::update( void *f()  ){
		loops = 0;
		while( millis() > next_tick && loops < MAX_FRAMESKIP) {
			f();

			update_mesh( m_world );
			next_tick += SKIP_TICKS;
			loops++;
		}
			
	}

	//Function return transformation matrix manipulating Model
	void mesh::update( Matrix<4, 4 > *f() ){

		loops = 0;
		while( millis() > next_tick && loops < mesh::MAX_FRAMESKIP) {
			f();
			//Matrix <4, 4, float> *m_rot ( f() );

			update_mesh( m_world );

			mesh::next_tick += mesh::SKIP_TICKS;
			loops++;
		}
	}

	void mesh::update( Matrix <4, 4 > matrix ){
		loops = 0;
		
		while( millis() > next_tick && loops < mesh::MAX_FRAMESKIP) {
			m_world = matrix;
			update_mesh( matrix );

			mesh::next_tick += mesh::SKIP_TICKS;
			loops++;
		}
	}

	void mesh::update( float rotx, float roty, float rotz ){
		update( rotx, roty, rotz, 0, 0, 0 );
	}

	void mesh::translate( float tx, float ty, float tz ){
		update( 0, 0, 0, tx, ty, tz );
	}

	void mesh::update( float rotx, float roty, float rotz, float tx, float ty, float tz, float scale = 1 ){
		Matrix <4, 4 > m_rot ( trotx(rotx)*troty(roty)*trotz(rotz) );
		mesh::update(m_rot*transl(  tx,  ty,  tz ));
	}

	//------------------------------------------------------------------------------------------//
	//------------------------------ Mesh Config Methods ----------------------------------------//
	
	void mesh::setdraw_type( uint8_t drawtype ){
		mesh::draw_type = drawtype;
	}

	void mesh::setColors( uint8_t colorpos, uint16_t colour ){
		color[ colorpos ] = colour;
	}

	void mesh::setColors( uint16_t colour[6] ){
		memcpy( color, colour, sizeof(uint16_t)*6);
	}

	void mesh::setVertexColor( uint16_t front, uint16_t back ){
		setColors(0 , front);
		setColors(1 , back);
	}

	void mesh::setWireColor( uint16_t front, uint16_t back ){
		setColors(2 , front);
		setColors(3 , back);
	}

	void mesh::setClearColor( uint16_t clearcolor ){
		setColors(4 , clearcolor);
	}

	void mesh::setFlatColor( uint16_t flatcolor ){
		setColors(5 , flatcolor);
	}

	void mesh::setskip_tick( unsigned char skip_tick ){
		mesh::SKIP_TICKS = skip_tick;
	}

	void mesh::setframe_skip( unsigned char frame_skip ){
		mesh::MAX_FRAMESKIP = frame_skip;
	}

	void mesh::set_tftsize( int posw, int posh ){
		set_tftpos(posw/2, posh/2);
	}

	void mesh::set_tftpos( int posw, int posh ){
		mesh::HALFH = posh;
		mesh::HALFW = posw;
	}

	void mesh::set_world( Matrix<4, 4 > mworld ){
		mesh::HALFW =  mworld(0,0);
		mesh::HALFH =  mworld(1,0);
		long z =  mworld(2,0);
	}


#endif