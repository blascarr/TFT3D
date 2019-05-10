

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
		
		if (!mesh::is_hidden( i , projnodes)) {
			
			// draw triangle edges - 0 -> 1 -> 2 -> 0
			//If out of canvas, not print
			
			//canvas->_tft.drawLine(matrix_ptr[ (*_faces)[i](0,0)][0], matrix_ptr[ (*_faces)[i](0,0)][1], matrix_ptr[ (*_faces)[i](0,1)][0], matrix_ptr[ (*_faces)[i](0,1)][1], color);
			//canvas->_tft.drawLine(matrix_ptr[ (*_faces)[i](0,1)][0], matrix_ptr[ (*_faces)[i](0,1)][1], matrix_ptr[ (*_faces)[i](0,2)][0], matrix_ptr[ (*_faces)[i](0,2)][1], color);
			//canvas->_tft.drawLine(matrix_ptr[ (*_faces)[i](0,2)][0], matrix_ptr[ (*_faces)[i](0,2)][1], matrix_ptr[ (*_faces)[i](0,0)][0], matrix_ptr[ (*_faces)[i](0,0)][1], color);
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

			//canvas->_tft.fillTriangle(matrix_ptr[ (*_faces)[i](0,0)][0], matrix_ptr[ (*_faces)[i](0,0)][1],
			//matrix_ptr[ (*_faces)[i](0,1)][0], matrix_ptr[ (*_faces)[i](0,1)][1],
			//matrix_ptr[ (*_faces)[i](0,2)][0], matrix_ptr[ (*_faces)[i](0,2)][1], 
			//color);
		}
	} while(i--);
}