#include "ground_model.h"

namespace Objects {

	GroundModel::GroundModel(float size) {
		vertices = {
			-size, 0, -size, 1,
			-size, 0,  size, 1,
			 size, 0,  size, 1,
			-size, 0, -size, 1,
			 size, 0, -size, 1,
			 size, 0,  size, 1
		};

		float t = size / 2;

		texture_coords = {
			0, 0,
			0, t,
			t, t,
			0, 0,
			t, 0,
			t, t
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
