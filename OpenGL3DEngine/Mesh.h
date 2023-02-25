#pragma once

class Mesh {
public:
	std::vector<Vertex> vertexData;
	std::vector<unsigned int> indices;
	bool usesIndex;
	int stride = 8;

	unsigned int VBO;
	// points to VBO data already preconfigured with vertex attribute pointers. 
	// Means that the attribute pointers must be configured only once for each
	// configuration to a different VAO while in the loop, the corresponding
	// VAO can be binded to.
	unsigned int VAO;
	// allows for rendering vertices based on indices by storing the vertex data
	// in GL_ELEMENT_ARRAY_BUFFER instead which holds the vertex of the corresponding
	// index
	unsigned int EBO;
	Mesh(const std::vector<Vertex> _vertexData, std::vector<unsigned int> _indices) {
		vertexData = _vertexData;
		indices = _indices;
		if (_indices.size() == 1) {
			usesIndex = false;
		}
		else {
			usesIndex = true;
		}
		LoadVertexBuffers();
	}

	~Mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		vertexData.clear();
		indices.clear();
	}

	void LoadVertexBuffers() {
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// creates a buffer objects in memory and assigns VBO to a buffer ID
		// that can be used to access that object. The first parameter is for
		// number of buffer items to create
		glGenBuffers(1, &VBO);
		// We can "bind" to several different buffers but only one of each type
		// bind generated buffer to GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// copy the vertexData to the buffer.
		// 4th parameter takes how GPU should manage data
		// OPTIONS:
		// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		// GL_STATIC_DRAW: the data is set only once and used many times.
		// GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

		// for EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		// Tell OpenGL how we've organized our vertices data (Looks at vertices data from the GL_ARRAY_BUFFER VBO)
		// Arg 1: which vertex attribute to configure ("location = 0" from vertexShaderSource)
		// Arg 2: size of vertex. 3 for x, y, z
		// Arg 3: Type of data (x, y, z are floats)
		// Arg 4: if we want data to be "normalized" (different from NDC, just ignore for now)
		// Arg 5: stride (number of bits between vertices = 3*(size of float))
		// Arg 6: "offset of where the position data begins in buffer" learn more later.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

};