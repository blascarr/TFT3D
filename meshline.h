//------------------------------------------------------------------------------------------//
//------------------------------- 2D MeshLine Class ----------------------------------------//


#ifdef _MESH_h
	#define _MESH_h  

		class meshline: public generic_mesh< 2 > {
			public:
				void draw( TFT3D *canvas, uint16_t timer = 10);
				void update_mesh( Matrix <4, 4, Array<4,4,float> > m );
				
				void draw_vertex( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
				void draw_wireframe( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
				
				#ifdef VECTOR_H
				/*
					meshline::meshline(Vector< vector3D > *meshnodes, Vector< uint8_t [2] > *meshfaces ) {
						meshline::next_tick = millis();
						meshline::setnodes( meshnodes );
						meshline::setfaces( meshfaces );
					}

					void set_mesh( Vector< vector3D > *meshnodes, Vector< uint8_t [2] >  *meshfaces){
						meshline::next_tick = millis();
						meshline::setnodes( meshnodes );
						meshline::setfaces( meshfaces );
					}

					void setnodes( Vector< vector3D > *meshnodes){
						_nodes = meshnodes;
						nodesize = meshnodes->Size();
					}

					void setfaces( Vector< uint8_t [2] > *meshfaces ){
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
						

					}*/
					
				#else

					meshline::meshline(float meshnodes[][3], uint8_t meshfaces[][2], int NSIZE , int TSIZE  ) {

						meshline::next_tick = millis();
						nodesize = NSIZE ;
						facesize = TSIZE ;
						meshline::setnodes(meshnodes, nodesize );
						meshline::setfaces(meshfaces, facesize );
					}

					void setnodes( float meshnodes[][3] , int NODE_SIZE = MAX_NODESIZE ){
						memcpy( _nodes, meshnodes, sizeof(float)* NODE_SIZE*3);
						nodesize = NODE_SIZE ;
					}

					void setfaces( uint8_t meshfaces[][2], int TRI_SIZE = MAX_TRISIZE  ){
						memcpy( _faces, meshfaces, sizeof(uint8_t)* TRI_SIZE*2);
						facesize = TRI_SIZE ;
					}

				#endif

	};	

	#ifdef VECTOR_H
		#include "vectorMeshline.h"
	#else
		#include "arrayMeshline.h"
	#endif

	void meshline::draw( TFT3D *canvas, uint16_t timer){
		//( (millis() - mesh::next_tick) > timer)

		if (memcmp(meshline::old_nodes, meshline::proj_nodes, sizeof(meshline::proj_nodes)) ) {
		// render frame
			switch(meshline::draw_type) {
				case 0: 
					meshline::draw_vertex( canvas, color[0],0);
					meshline::draw_vertex( canvas, color[1],1);
				break;
				
				case 1: 
					if (facesize > 32) {
						//mesh::clear_dirty( canvas, mesh::old_nodes);
					}
					else {
						meshline::draw_wireframe( canvas, color[2],0);
						meshline::draw_wireframe( canvas, color[3],1);
					}
				break;

			}
			// copy projected nodes to old_nodes to check if we need to redraw next frame
			memcpy(meshline::old_nodes, meshline::proj_nodes, sizeof(meshline::proj_nodes));
		}
	}

	void meshline::draw_vertex(TFT3D *canvas, const uint16_t color, boolean projnodes ){
		int i = nodesize-1;

		int (*matrix_ptr)[2];
		if (projnodes){
			matrix_ptr = meshline::proj_nodes;
		}else{
			matrix_ptr = meshline::old_nodes;
		}

		do {
			//If out of canvas, not print

			canvas->_tft.drawPixel( matrix_ptr[i][0], matrix_ptr[i][1], color);
		} while(i--);

	};

#endif