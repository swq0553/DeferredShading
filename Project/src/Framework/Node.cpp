#include <Framework/Node.h>
#include <glm/gtc/quaternion.hpp>

#pragma region "Constructors/Destructor"
 
Node::Node() : m_translation(0.0f, 0.0f, 0.0f), m_scale(1, 1, 1), m_orientation(), m_children()
{

}

Node::Node(glm::vec3 const & translation, glm::quat const & orientation) : m_translation(translation), m_scale(1, 1, 1), m_orientation(orientation), m_children()
{

}


Node::~Node()
{
	for (auto childNode : m_children)
		delete childNode;
}

#pragma endregion

#pragma region "Public Methods"

void Node::AddChild(Node * node)
{
	m_children.push_back(node);
}

#pragma endregion

#pragma region "Getters"

glm::vec3 const & Node::GetTranslation() const
{
	return m_translation;
}

glm::vec3 const & Node::GetScale() const
{
	return m_scale;
}

glm::quat const & Node::GetOrientation() const
{
	return m_orientation;
}

glm::mat4 Node::GetTransformMatrix() const
{
	return glm::mat4(
		glm::vec4(m_scale.x, 0, 0, 0),
		glm::vec4(0, m_scale.y, 0, 0),
		glm::vec4(0, 0, m_scale.z, 0),
		glm::vec4(m_translation.x, m_translation.y, m_translation.z, 1)
	) * glm::mat4_cast(m_orientation);
}

#pragma endregion

#pragma region "Setters"

void Node::SetTranslation(glm::vec3 const & translation)
{
	m_translation = translation;
}

void Node::SetScale(glm::vec3 const & scale)
{
	m_scale = scale;
}

void Node::SetOrientation(glm::quat const & orientation)
{
	m_orientation = orientation;
}

#pragma endregion

#pragma region "Public Methods"

bool Node::IsRenderable() const
{
	return false;
}

#pragma endregion