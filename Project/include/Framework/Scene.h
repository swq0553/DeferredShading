#pragma once

#include "Camera.h"
#include "Node.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>


class Mesh;
class Material;

class Scene
{

public:

	friend class IRenderer;

	//constructors/destructor
	Scene(unsigned const & windowWidth, unsigned const & windowHeight);
	~Scene();

	//getters
	Camera const & GetCamera() const;
	Camera & GetCamera();

	glm::mat4 const & GetProjectionMatrix() const;
	glm::mat4 const & GetViewMatrix() const;

	//setters
	void SetProjection(float const & ry, float const & front, float const & back);
	void SetWindowWidth(unsigned const & width);
	void SetWindowHeight(unsigned const & height);

	//public methods
	Mesh * LoadMesh(std::string const & path);
	void FreeMemory();

	void AddNode(Node * node);

	void Resize(int const & width, int const & height);

private:

	Node * m_rootNode;

	Camera m_camera;

	std::vector<Mesh *> m_meshes;
	std::vector<Material *> m_materials;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	unsigned m_windowWidth;
	unsigned m_windowHeight;

};

