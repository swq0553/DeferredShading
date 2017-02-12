#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Node
{

public:

	friend class IRenderer;

	//constructors/destructor
	Node();
	Node(glm::vec3 const & translation, glm::quat const & orientation);
	~Node();

	//public methods
	void AddChild(Node * node);

	//getters
	glm::vec3 const & GetTranslation() const;
	glm::vec3 const & GetScale() const;
	glm::quat const & GetOrientation() const;
	glm::mat4 GetTransformMatrix() const;

	//setters
	void SetTranslation(glm::vec3 const & translation);
	void SetScale(glm::vec3 const & scale);
	void SetOrientation(glm::quat const & orientation);

	//public methods
	virtual bool IsRenderable() const;

private:

	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::quat m_orientation;
	std::vector<Node *> m_children;

};