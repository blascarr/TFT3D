#ifdef VECTOR_H
	typedef Matrix <1, 2, float> ( vector2D );

	typedef Matrix <1, 3, float> ( vector3D );
	typedef Matrix <1, 3, uint8_t > ( point3D );
#endif

class mesh{
	public:
		int proj_nodes[MAX_NODESIZE][2];
		int old_nodes[MAX_NODESIZE][2];

		//Vector< vector2D > proj_nodes;	
		//Vector< vector2D > old_nodes;

		Vector< vector3D > *_nodes;
		Vector< point3D > *_faces;

		Matrix<4, 4, float> m_world; 

		unsigned char SKIP_TICKS = 20;
		unsigned char MAX_FRAMESKIP = 5;
		int HALFW, HALFH;
		uint16_t color[6]= {YELLOW,BLACK,YELLOW,BLACK,WHITE,GREEN };

		long next_tick;
		unsigned char draw_type = 1;          // 0 - vertex | 1 - wireframe | 2 - flat colors | ...
		uint8_t nodesize = 0;
		uint8_t facesize = 0;

		mesh::mesh(){
			mesh::next_tick = millis();
		}

		mesh::mesh(Vector< vector3D > *meshnodes, Vector< point3D >  *meshfaces):_nodes(meshnodes), _faces(meshfaces) {
			mesh::next_tick = millis();
			mesh::setnodes( meshnodes );
			mesh::setfaces( meshfaces );
		}

		void setmesh( Vector< vector3D > *meshnodes, Vector< point3D >  *meshfaces){
			mesh::next_tick = millis();
			mesh::setnodes( meshnodes );
			mesh::setfaces( meshfaces );
		}


		void setnodes( Vector< vector3D > *meshnodes){
			_nodes = meshnodes;
			nodesize = meshnodes->Size();
		}

		void setfaces( Vector< point3D > *meshfaces){
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
			for(int i = 0; i < facesize ; i++) {
				Serial << "FaceList: " << (*_faces)[i] ;
				Serial.println();
			}

		}

		void setdraw_type( uint8_t drawtype );
		void setColors( uint8_t colorpos, uint16_t colour );
		void setColors( uint16_t colour[6] );
		void setVertexColor( uint16_t front, uint16_t back );
		void setWireColor( uint16_t front, uint16_t back );
		void setClearColor( uint16_t clearcolor );
		void setFlatColor( uint16_t flatcolor );
		void setskip_tick( unsigned char skip_tick );
		void setframe_skip( unsigned char frame_skip );
		void set_tftsize( int posw, int posh );
		void set_tftpos( int posw, int posh );
		void set_world( Matrix<4, 4, float> mworld );
		//---------------------------------------------------------------------------------------------//
		//------------------------------ Update Mesh Functions ----------------------------------------//

		int shoelace( const unsigned char index, boolean projnodes = true );
		bool is_hidden( const unsigned char index, boolean projnodes = true );

		void update( Matrix <4, 4, float> matrix );
		void update( Matrix<4, 4, float> *f() );

		void update( float rotx, float roty, float rotz );
		void translate( float tx, float ty, float tz );

		void update( float rotx, float roty, float rotz, float tx, float ty, float tz, float scale = 1 );

		void update( void *f()  );

		void update_mesh( Matrix <4, 4, float> m );

		void draw( TFT3D *canvas, uint16_t timer = 10);

		void draw_vertex( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
		void draw_wireframe( TFT3D *canvas, const uint16_t color, boolean projnodes = true);
		void draw_flat_color( TFT3D *canvas, uint16_t color, boolean projnodes = true);
		void clear_dirty( TFT3D *canvas , uint16_t color = WHITE , boolean projnodes = true);
};

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
		//Cross product two vectors
		// (x1y2 - y1x2) + (x2y3 - y2x3) ...
		
		surface += ( matrix_ptr[(*_faces)[index](0,t)][0] *  matrix_ptr[(*_faces)[index](0,(t<2?t+1:0))][1] )   -  ( matrix_ptr[(*_faces)[index](0,(t<2?t+1:0))][0] *  matrix_ptr[(*_faces)[index](0,t)][1] ) ;
	
		//surface += (  matrix_ptr[ mesh::faces[index][t] ][0] * matrix_ptr[ mesh::faces[index][(t<2?t+1:0)] ][1] ) - ( matrix_ptr[ mesh::faces[index][(t<2?t+1:0)] ][0] * matrix_ptr[ mesh::faces[index][t] ][1] );
	}
	return surface *0.5;
};

bool mesh::is_hidden( const unsigned char index, boolean projnodes ){
	int (*matrix_ptr)[2];
	
	if (projnodes){
		matrix_ptr = mesh::proj_nodes;
	}else{
		matrix_ptr = mesh::old_nodes;
	}
	//Serial.print( "Is Hidden:") ;
	//Serial.println( matrix_ptr[ (*_faces)[index](0,0) ]( 0, 1 ) ) ;

	bool ishidden = ( matrix_ptr[ (*_faces)[index](0,0) ][0] * matrix_ptr[(*_faces)[index](0,1)][1] -
			matrix_ptr[ (*_faces)[index](0,1) ][0] * matrix_ptr[(*_faces)[index](0,0)][1]  	) 	+ 
		( 	matrix_ptr[ (*_faces)[index](0,1) ][0] * matrix_ptr[(*_faces)[index](0,2)][1] -
			matrix_ptr[ (*_faces)[index](0,2) ][0] * matrix_ptr[(*_faces)[index](0,1)][1]  	)  	+
		(	matrix_ptr[ (*_faces)[index](0,2) ][0] * matrix_ptr[(*_faces)[index](0,0)][1]	-	
			matrix_ptr[ (*_faces)[index](0,0) ][0] * matrix_ptr[(*_faces)[index](0,2)][1]		) < 0 ? false : true;
	
	return ishidden;
};

//-----------------------------------------------------------------------------------//
//------------------------------ Update Mesh ----------------------------------------//
//--- paramater f is a function for scale,rotation, translation management of mesh---//

//Execute an action every update of model

//Dependant Vector.h
void mesh::update_mesh( Matrix <4, 4, float> m ){
	
	for (int i=0; i<nodesize; i++) {
		
		float arrayNODES[4][1] = { (*_nodes)[i](0,0) ,(*_nodes)[i](0,1), (*_nodes)[i](0,2), 1};

		Matrix <4, 1, float> m_mesh (arrayNODES);
		Matrix <4, 1, float> res = m*m_mesh;

		long x =  res(0,0);
		long y =  res(1,0);
		long z =  res(2,0);

		//Scale parameter stored in Transformation Matrix
		long s =  m(3,3);

		// store projected node //
		mesh::proj_nodes[i][0] = s*((FOV * x) / (FOV + z)) + HALFW;
		mesh::proj_nodes[i][1] = s*((FOV * y) / (FOV + z)) + HALFH;
		
	}
	
}
