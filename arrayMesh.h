

//------------------------------------------------------------------------------------------//
//------------------------------ Class Mesh Methods ----------------------------------------//
int mesh::shoelace( const uint8_t index, boolean projnodes ){
	uint8_t t = 0;
	int surface = 0;
	int (*matrix_ptr)[2];
	if (projnodes){
		matrix_ptr = mesh::proj_nodes;
	}else{
		matrix_ptr = mesh::old_nodes;
	}

	for (; t<3; t++) {
		// (x1y2 - y1x2) + (x2y3 - y2x3) ...
		surface += (  matrix_ptr[ mesh::_faces[index][t] ][0] * matrix_ptr[ mesh::_faces[index][(t<2?t+1:0)] ][1] ) -
		( matrix_ptr[ mesh::_faces[index][(t<2?t+1:0)] ][0] * matrix_ptr[ mesh::_faces[index][t] ][1] );
	}
	return surface * 0.5;
};

bool mesh::is_hidden( const uint8_t index, boolean projnodes ){
	int (*matrix_ptr)[2];
	if (projnodes){
		matrix_ptr = mesh::proj_nodes;
	}else{
		matrix_ptr = mesh::old_nodes;
	}

	return ( ( (matrix_ptr[ mesh::_faces[index][0] ][0] * matrix_ptr[ mesh::_faces[index][1] ][1]) -
	(matrix_ptr[mesh::_faces[index][1] ][0] * matrix_ptr[mesh::_faces[index][0] ][1])   ) +
	( (matrix_ptr[mesh::_faces[index][1] ][0] * matrix_ptr[mesh::_faces[index][2] ][1]) -
	(matrix_ptr[mesh::_faces[index][2] ][0] * matrix_ptr[mesh::_faces[index][1] ][1])   ) +
	( (matrix_ptr[mesh::_faces[index][2] ][0] * matrix_ptr[mesh::_faces[index][0] ][1]) -
	(matrix_ptr[mesh::_faces[index][0] ][0] * matrix_ptr[mesh::_faces[index][2] ][1])   ) ) < 0 ? false : true;
};

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
		if (!is_hidden( i , projnodes)) {
			// draw triangle edges - 0 -> 1 -> 2 -> 0
			//If out of canvas, not print
			
			canvas->_tft.drawLine(matrix_ptr[mesh::_faces[i][0]][0], matrix_ptr[mesh::_faces[i][0]][1], matrix_ptr[mesh::_faces[i][1]][0], matrix_ptr[mesh::_faces[i][1]][1], color);
			canvas->_tft.drawLine(matrix_ptr[mesh::_faces[i][1]][0], matrix_ptr[mesh::_faces[i][1]][1], matrix_ptr[mesh::_faces[i][2]][0], matrix_ptr[mesh::_faces[i][2]][1], color);
			canvas->_tft.drawLine(matrix_ptr[mesh::_faces[i][2]][0], matrix_ptr[mesh::_faces[i][2]][1], matrix_ptr[mesh::_faces[i][0]][0], matrix_ptr[mesh::_faces[i][0]][1], color);
			
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
		if ((surface= shoelace( i, projnodes )) < 0) {
			// this is an ugly hack but it 'somehow' fakes shading
			// depending on the size of the surface of the triangle
			// change the color toward brighter/darker
			color = col * (surface * 0.001);
			//If out of canvas, not print

			canvas->_tft.fillTriangle(matrix_ptr[mesh::_faces[i][0]][0], matrix_ptr[mesh::_faces[i][0]][1],
			matrix_ptr[mesh::_faces[i][1]][0], matrix_ptr[mesh::_faces[i][1]][1],
			matrix_ptr[mesh::_faces[i][2]][0], matrix_ptr[mesh::_faces[i][2]][1],
			color);
		}
	} while(i--);

};

void mesh::update_mesh( Matrix <4, 4, Array<4,4,float> > m ) {
	
	for (int i=0; i< nodesize; i++) {

		float arrayNODES[4][1] = { _nodes[i][0] , _nodes[i][1], _nodes[i][2], 1 };

		Matrix <4, 1, Array<4,1,float> > m_mesh (arrayNODES);
		Matrix <2, 1, Array<2,1,float> > res = mesh2D( m_mesh, m );
		proj_nodes[i][0] = res(0,0);
		proj_nodes[i][1] = res(1,0);

	}
};