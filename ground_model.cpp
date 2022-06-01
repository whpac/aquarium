#include "ground_model.h"

namespace Objects {

	GroundModel::GroundModel() {
		vertices = {
			-10, 0, -10, 1,
			-10, 0,  10, 1,
			 10, 0,  10, 1,
			-10, 0, -10, 1,
			 10, 0, -10, 1,
			 10, 0,  10, 1
		};

		texture_coords = {
			0, 0,
			0, 1,
			1, 1,
			0, 0,
			1, 0,
			1, 1
		};

		normals = {
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		};
	}
}
