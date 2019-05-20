//------------------------------------------------------------------------------------------//
//------------------------------ Class MeshLine Methods ----------------------------------------//

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
	}else{
		matrix_ptr = meshline::old_nodes;
	}

	do {
		
		//If out of canvas, not print
		canvas->_tft.drawLine(matrix_ptr[meshline::_faces[i][0]][0], matrix_ptr[meshline::_faces[i][0]][1], matrix_ptr[meshline::_faces[i][1]][0], matrix_ptr[meshline::_faces[i][1]][1], color);
	} while(i--);

};