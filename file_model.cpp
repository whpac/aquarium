#include "file_model.h"

namespace Objects {

	FileModel::FileModel(const char* path) {
		successful = loadObj(path, vertices, texture_coords, normals);
	}
}
