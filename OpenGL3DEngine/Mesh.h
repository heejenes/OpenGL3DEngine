#pragma once


class Mesh {
protected:
	unsigned int VBO = 0;
	// points to VBO data already preconfigured with vertex attribute pointers. 
	// Means that the attribute pointers must be configured only once for each
	// configuration to a different VAO while in the loop, the corresponding
	// VAO can be binded to.
	unsigned int VAO = 0;
	// allows for rendering vertices based on indices by storing the vertex data
	// in GL_ELEMENT_ARRAY_BUFFER instead which holds the vertex of the corresponding
	// index
	unsigned int EBO = 0;
	bool assigned = false;
public:
	std::vector<Vertex> vertexData;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	bool isInstanced = false;
	std::vector<Vertex> vertexDataInstance;
	std::vector<unsigned int> indicesInstance;

	bool usesIndex;
	Emitter emitter;
	int stride = 12;

	Mesh(
		std::vector<Vertex> _vertexData, 
		std::vector<unsigned int> _indices, 
		Texture* _texture = nullptr,
		Emitter _emitter = Emitter()
	) {
		std::copy(_vertexData.begin(), _vertexData.end(), back_inserter(vertexData));
		std::copy(_indices.begin(), _indices.end(), back_inserter(indices));
		textures.push_back(_texture);
		emitter = _emitter;
		if (_indices.size() == 1) {
			usesIndex = false;
		}
		else {
			usesIndex = true;
		}
		LoadVertexBuffers();
	}
	Mesh(
		std::vector<Vertex> _vertexData,
		unsigned int* _indices,
		int indicesSize,
		Texture* _texture,
		Emitter _emitter = Emitter(),
		bool _isInstanced = false
	) {
		isInstanced = _isInstanced;
		textures.push_back(_texture);
		emitter = _emitter;
		if (isInstanced) {
			std::copy(_vertexData.begin(), _vertexData.end(), back_inserter(vertexDataInstance));
			indicesInstance.clear();
			indicesInstance.resize(indicesSize);
			std::copy(_indices, _indices + indicesSize, indicesInstance.begin());
			if (indicesInstance.size() == 1) {
				usesIndex = false;
			}
			else {
				usesIndex = true;
			}
		}
		else {
			std::copy(_vertexData.begin(), _vertexData.end(), back_inserter(vertexData));
			indices.clear();
			indices.resize(indicesSize);
			std::copy(_indices, _indices + indicesSize, indices.begin());
			if (indices.size() == 1) {
				usesIndex = false;
			}
			else {
				usesIndex = true;
			}
		}
		LoadVertexBuffers();
	}
	Mesh(
		std::vector<Vertex> _vertexData = std::vector<Vertex>{},
		std::vector<unsigned int> _indices = std::vector<unsigned int>{},
		std::vector<Texture*> _textures = std::vector<Texture*>{},
		Emitter _emitter = Emitter()
	) {
		std::copy(_vertexData.begin(), _vertexData.end(), back_inserter(vertexData));
		std::copy(_indices.begin(), _indices.end(), back_inserter(indices));
		std::copy(_textures.begin(), _textures.end(), back_inserter(textures));
		emitter = _emitter;
		if (_indices.size() == 1) {
			usesIndex = false;
		}
		else {
			usesIndex = true;
		}
		LoadVertexBuffers();
	}
	unsigned int getEBO() {
		return EBO;
	}
	unsigned int getVBO() {
		return VBO;
	}
	unsigned int getVAO() {
		return VAO;
	}
	unsigned int* getEBOAd() {
		return &EBO;
	}
	unsigned int* getVBOAd() {
		return &VBO;
	}
	unsigned int* getVAOAd() {
		return &VAO;
	}
	~Mesh() {
		if (assigned) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			if (usesIndex) {
				glDeleteBuffers(1, &EBO);
			}
		}
		vertexData.clear();
		indices.clear();
		textures.clear();
	}
	void operator= (Mesh _mesh) {
		vertexData.clear();
		indices.clear();
		textures.clear();
		std::copy(_mesh.vertexData.begin(), _mesh.vertexData.end(), back_inserter(vertexData));
		std::copy(_mesh.indices.begin(), _mesh.indices.end(), back_inserter(indices));
		std::copy(_mesh.textures.begin(), _mesh.textures.end(), back_inserter(textures));
		emitter = _mesh.emitter;
		isInstanced = _mesh.isInstanced;
		if (indices.size() == 1) {
			usesIndex = false;
		}
		else {
			usesIndex = true;
		}
		if (assigned) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			if (usesIndex) {
				glDeleteBuffers(1, &EBO);
			}
		}
		LoadVertexBuffers();
	}
	Mesh(const Mesh& _mesh) {
		vertexData.clear();
		indices.clear();
		textures.clear();
		std::copy(_mesh.vertexData.begin(), _mesh.vertexData.end(), back_inserter(vertexData));
		std::copy(_mesh.indices.begin(), _mesh.indices.end(), back_inserter(indices));
		std::copy(_mesh.textures.begin(), _mesh.textures.end(), back_inserter(textures));
		emitter = _mesh.emitter;
		isInstanced = _mesh.isInstanced;
		if (indices.size() == 1) {
			usesIndex = false;
		}
		else {
			usesIndex = true;
		}
		if (assigned) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			if (usesIndex) {
				glDeleteBuffers(1, &EBO);
			}
		}
		LoadVertexBuffers();
	}

	void LoadVertexBuffers() {
		if (isInstanced) {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			// for EBO
			if (usesIndex) {
				glGenBuffers(1, &EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesInstance.size() * sizeof(unsigned int), indicesInstance.data(), GL_STATIC_DRAW);
			}

			// copy the vertexData to the buffer.
			// 4th parameter takes how GPU should manage data
			// OPTIONS:
			// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
			// GL_STATIC_DRAW: the data is set only once and used many times.
			// GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			glBufferData(GL_ARRAY_BUFFER, vertexDataInstance.size() * sizeof(Vertex), vertexDataInstance.data(), GL_STATIC_DRAW);

			// Tell OpenGL how we've organized our vertices data (Looks at vertices data from the GL_ARRAY_BUFFER VBO)
			// Arg 1: which vertex attribute to configure ("location = 0" from vertexShaderSource)
			// Arg 2: size of vertex. 3 for x, y, z
			// Arg 3: Type of data (x, y, z are floats)
			// Arg 4: if we want data to be "normalized" (different from NDC, just ignore for now)
			// Arg 5: stride (number of bits between vertices = 3*(size of float))
			// Arg 6: "offset of where the position data begins in buffer" learn more later.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offsetof(Vertex, TexCoords));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offsetof(Vertex, rgba));
			glEnableVertexAttribArray(3);

			glBindVertexArray(0);
		}
		else {
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);

			// for EBO
			if (usesIndex) {
				glGenBuffers(1, &EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
			}

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// copy the vertexData to the buffer.
			// 4th parameter takes how GPU should manage data
			// OPTIONS:
			// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
			// GL_STATIC_DRAW: the data is set only once and used many times.
			// GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

			// Tell OpenGL how we've organized our vertices data (Looks at vertices data from the GL_ARRAY_BUFFER VBO)
			// Arg 1: which vertex attribute to configure ("location = 0" from vertexShaderSource)
			// Arg 2: size of vertex. 3 for x, y, z
			// Arg 3: Type of data (x, y, z are floats)
			// Arg 4: if we want data to be "normalized" (different from NDC, just ignore for now)
			// Arg 5: stride (number of bits between vertices = 3*(size of float))
			// Arg 6: "offset of where the position data begins in buffer" learn more later.
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offsetof(Vertex, TexCoords));
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offsetof(Vertex, rgba));
			glEnableVertexAttribArray(3);

			glBindVertexArray(0);
		}
		assigned = true;
	}
};