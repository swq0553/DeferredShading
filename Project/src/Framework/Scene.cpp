#include <Framework/Scene.h>
#include <Framework/Mesh.h>
#include <Framework/Material.h>
#include <Framework/IRenderPass.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma region "Constructors/Destructor"

Scene::Scene(unsigned const & windowWidth, unsigned const & windowHeight) : m_rootNode(new Node("Root")), m_camera(m_viewMatrix), m_meshes(), m_projectionMatrix(), m_viewMatrix(), m_sceneSize(glm::vec3(1, 1, 1)), m_ambientIntensity(glm::vec3(0, 0, 0)), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{

}

Scene::~Scene()
{
	delete m_rootNode;
}

#pragma endregion

#pragma region "Getters"

Camera const & Scene::GetCamera() const
{
	return m_camera;
}

Camera & Scene::GetCamera()
{
	return m_camera;
} 

glm::mat4 const & Scene::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

glm::mat4 const & Scene::GetViewMatrix() const
{
	return m_viewMatrix;
}

glm::vec3 const & Scene::GetSceneSize() const
{
	return m_sceneSize;
}

glm::vec3 const & Scene::GetAmbientIntensity() const
{
	return m_ambientIntensity;
}

#pragma endregion

#pragma region "Setters"

void Scene::SetProjection(float const & ry, float const & front, float const & back)
{
	float rx = ry * (float)m_windowWidth / (float)m_windowHeight;
	m_projectionMatrix = glm::mat4(
		glm::vec4(1.0f / rx, 0.0f, 0.0f, 0.0f), 
		glm::vec4(0.0f, 1.0f / ry, 0.0f, 0.0f), 
		glm::vec4(0.0f, 0.0f, (front - back) / (back - front), -1.0f), 
		glm::vec4(0.0f, 0.0f, (-2.0f * front * back) / (back - front), 0.0f));
}

void Scene::SetWindowWidth(unsigned const & width)
{
	m_windowWidth = width;
}

void Scene::SetWindowHeight(unsigned const & height)
{
	m_windowHeight = height;
}

void Scene::SetSceneSize(glm::vec3 const & sceneSize)
{
	m_sceneSize = sceneSize;
}

void Scene::SetAmbientIntensity(glm::vec3 const & ambientIntensity)
{
	m_ambientIntensity = ambientIntensity;
}

#pragma endregion

#pragma region "Public Methods"

Mesh * Scene::CreateMesh(std::string const & name, std::string const & path)
{
	Assimp::Importer importer;
	aiScene const * scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate);

	if (!scene || !scene->HasMeshes())
		return nullptr;

	//process the object
	aiMesh * assimpMesh = scene->mMeshes[0];
	Mesh * mesh = new Mesh(assimpMesh->mNumFaces, assimpMesh->mFaces, assimpMesh->mVertices, assimpMesh->mNormals, assimpMesh->mTangents, assimpMesh->mTextureCoords);
	m_meshes[name] = mesh;

	return mesh;
}

Material * Scene::CreateMaterial(std::string const & name, glm::vec3 const & kd, glm::vec3 const & ks, float const & alpha)
{
	Material * material = new Material(kd, ks, alpha);
	m_materials[name] = material;
	return material;
}

void Scene::AddNode(Node * node)
{
	m_rootNode->AddChild(node);
}

void Scene::Traverse(IRenderPass const & pass) const
{
	static glm::mat4 identity = glm::mat4();
	for (auto & const node : m_rootNode->m_children)
	{
		TraverseNode(node, pass, identity);
	}
}

void Scene::Resize(int const & width, int const & height)
{
	float ry = 1.0f / m_projectionMatrix[1][1];
	float rx = ry * ((float)width / (float)height);
	m_projectionMatrix[0][0] = 1.0f / rx;
	m_windowWidth = width;
	m_windowHeight = height;
}

void Scene::FreeMemory()
{
	for (auto mesh : m_meshes)
		delete mesh.second;

	for (auto material : m_materials)
		delete material.second;
}

#pragma endregion

#pragma region "Private Methods"

void Scene::TraverseNode(Node * const & node, IRenderPass const & pass, glm::mat4 const & modelMatrix) const
{
	pass.ProcessNode(node, modelMatrix * node->GetTransformMatrixWithScale());
	for (auto & const child : node->m_children)
		TraverseNode(child, pass, modelMatrix * node->GetTransformMatrix());
}

#pragma endregion