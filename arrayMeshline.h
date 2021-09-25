//------------------------------------------------------------------------------------------//
//------------------------------ Class MeshLine Methods ----------------------------------------//

bool doIntersect( int p[2], int q[2], int b1[2], int b2[2] ){
	//CrossProduct
	int v1[2] = { q[1]-p[1], q[2]-p[2] };
	int v2[2] = { b1[1]-q[1], b1[2]-q[2] };
	int v3[2] = { b2[1]-q[1], b2[2]-q[2] };

	int r1= (v1[1]*v2[2]) - (v1[2]*v2[1]);

	int r2 = (v1[1]*v3[2]) - (v1[2]*v3[1]);
	Serial.print("\tr1: ");Serial.print(r1);Serial.print("\tr2: ");Serial.print(r2);
	Serial.print("\ts1: ");Serial.print(signbit(r1));
	if ( signbit(r1) == signbit(r2)){
		return false; // Not crossing
	}else{
		return true;
	}
	/*int o1 = orientation( p1, q1, p2 );
	int o2 = orientation( p1, q1, q2 );
	int o3 = orientation( p2, q2, p1 );
	int o4 = orientation( p2, q2, q1 );
	
	if( o1 != o2 && o3 != o4) return true; 

	//Special Cases collinear
	if (o1 == 0 && onSegment(p1,p2,q1) ) return true;
	if (o2 == 0 && onSegment(p1,q2,q1) ) return true;
	if (o3 == 0 && onSegment(p2,p1,q2) ) return true;
	if (o4 == 0 && onSegment(p2,q1,q2) ) return true;
	*/
	return true;

}

void meshline::update_mesh( Matrix <4, 4, Array<4,4,float> > m ) {
	
	for (int i=0; i< nodesize; i++) {

		float arrayNODES[4][1] = { _nodes[i][0] , _nodes[i][1], _nodes[i][2], 1 };

		Matrix <4, 1, Array<4,1,float> > m_mesh (arrayNODES);
		Matrix <2, 1, Array<2,1,float> > res = mesh2D( m_mesh, m );
		proj_nodes[i][0] = res(0,0);
		proj_nodes[i][1] = res(1,0);

	}
};

void meshline::draw_wireframe(TFT3D *canvas, const uint16_t color, boolean projnodes ){
	int i = facesize-1;
	int (*matrix_ptr)[2];
	if (projnodes){
		matrix_ptr = meshline::proj_nodes;
		do {
			if ( outbound( meshline::_faces[i] ) ){
				canvas->_tft.drawLine(matrix_ptr[meshline::_faces[i][0]][0], matrix_ptr[meshline::_faces[i][0]][1], matrix_ptr[meshline::_faces[i][1]][0], matrix_ptr[meshline::_faces[i][1]][1], color);
			}
		} while(i--);
		Serial.println();
	}else{
		matrix_ptr = meshline::old_nodes;
		do {
			canvas->_tft.drawLine(matrix_ptr[meshline::_faces[i][0]][0], matrix_ptr[meshline::_faces[i][0]][1], matrix_ptr[meshline::_faces[i][1]][0], matrix_ptr[meshline::_faces[i][1]][1], color);
		} while(i--);	
	}
		
};

//Out Bounds depending on both pixel position
//	/ | *  | / 
//	---------  
//	* | 10 | *
//	---------  
//	/ | * | / 
bool meshline::outbound( uint8_t n_face[2]  ){

	uint8_t bound0 = pixelbound( n_face[0] );
	uint8_t bound1 = pixelbound( n_face[1] );

	//Serial.print(n_face[0]);Serial.print("\tBound1: ");Serial.print(bound0);Serial.print("\t -: ");
	//Serial.print("\tBound2: ");Serial.println(bound1);
	uint8_t pbound =  bound0 + bound1 ;

	if ( pbound == 10  ) { 
		return true;
	}else if ( bound0 == bound1 ) {
		return false;
	}else{
		int corner[5][2] = { { 0, 0 }, { TFT_W, 0 } ,{ TFT_W, TFT_H }, { 0, TFT_H } , { 0, 0 }  };
		bool cross = false;
		for (int i = 0; i < 4; ++i){
			//cross = doIntersect( meshline::proj_nodes[n_face[0]], meshline::proj_nodes[n_face[1] ] , corner[i], corner[i+1] );
			//if (cross == true) return true;
		}
		return cross;
		
	}	

	
};

//Pixel Bounds depending on tft position
//	  2 | 9 | 4
//    ---------
//	  7 | 5	| 3
//	  ---------
//	  6 | 1	| 8
//
uint8_t meshline::pixelbound( uint8_t n_pixel  ){
	uint8_t mbound[3][3]={ {2,9,4}, {7,5,3}, {6,1,8} };
	uint8_t row = 1;
	uint8_t col = 1;

	uint8_t bound = 5;
	if ( meshline::proj_nodes[ n_pixel ][0] < 0 ){
		col = 0;
	}
	if ( meshline::proj_nodes[ n_pixel ][0] >  TFT_W ){
		col = 2;
	}
	if ( meshline::proj_nodes[ n_pixel ][1] < 0 ){
		row = 0;
	}
	if ( meshline::proj_nodes[ n_pixel ][1] >  TFT_H ){
		row = 2;
	}
	//Serial.print("Row: ");Serial.print(row);Serial.print("\tCol: ");Serial.print(col);
	//Serial.print("\tP1: ");Serial.print(meshline::proj_nodes[ n_pixel ][0]);
	//Serial.print("\tP2: ");Serial.print(meshline::proj_nodes[ n_pixel ][1]);
	//Serial.println();
	bound = mbound[row][col];
	return bound;
};

