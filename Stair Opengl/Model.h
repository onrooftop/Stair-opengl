#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>

#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
void printMat4(aiMatrix4x4 mat);


const std::string CHEST = "Chest";
const std::string HEAD = "Head";
const std::string ROOT_RIGHT_HAND = "Hand_Right_3";
const std::string ROOT_LEFT_HAND = "Hand_Left_3";
const std::string ROOT_RIGHT_LEG = "Leg_Right_3";
const std::string ROOT_LEFT_LEG = "Leg_Left_3";

class Model
{
public:




	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;

	string directory;
	bool gammaCorrection;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		loadModel(path);
		
		//::cout << root;
	}

	void RotArm(int mode, float dg)
	{
		nodes[mode]->mTransformation = nodes[mode]->mTransformation.RotationX(dg, nodes[mode]->mTransformation);
	}



	// draws the model, and thus all its meshes
	void Draw(Shader shader, glm::mat4 model,int mode, const std::string part, int pressUp, int pressLeft)
	{

		if (CHEST.compare(part) == 0)
		{
			mode = chest_id;
		}
		else if (HEAD.compare(part) == 0)
		{
			mode = head_id;
		}
		else if (ROOT_LEFT_HAND.compare(part) == 0)
		{
			mode = root_left_hand_id + mode;
		}
		else if (ROOT_RIGHT_HAND.compare(part) == 0)
		{
			mode = root_right_hand_id + mode;
		}
		else if (ROOT_LEFT_LEG.compare(part) == 0)
		{
			mode = root_left_leg_id + mode;
		}
		else if (ROOT_RIGHT_LEG.compare(part) == 0)
		{
			mode = root_right_leg_id + mode;
		}
		

		
		std::vector<glm::mat4> model_temp;
		std::vector<glm::mat4> tran_temp;
		std::vector<int> rolls_index_temp;
		std::vector<int> rolls_index;

		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			aiNode *node = nodes[i];

			
			glm::mat4 tran;

			model_temp.clear();
			tran_temp.clear();
			rolls_index.clear();
			rolls_index_temp.clear();

			aiMatrix4x4 m = nodes[0]->mTransformation;

			model_temp.push_back(glm::fmat4x4(m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4));
			rolls_index.push_back(0);

			
			while (nodes[0] != node)
			{
				std::vector<aiNode *>::iterator iter = std::find(nodes.begin(), nodes.end(), node);
				int index = std::distance(nodes.begin(), iter);

				aiMatrix4x4 m = node->mTransformation;

				glm::mat4 mat_temp = glm::fmat4x4(m.a1, m.b1, m.c1, m.d1,
					m.a2, m.b2, m.c2, m.d2,
					m.a3, m.b3, m.c3, m.d3,
					m.a4, m.b4, m.c4, m.d4);

				tran_temp.push_back(mat_temp);

				rolls_index_temp.push_back(index);
				node = node->mParent;// 0 [2 1]
			}

			for (int j = tran_temp.size() - 1; j >= 0; j--)
			{
				model_temp.push_back(tran_temp[j]);
				rolls_index.push_back(rolls_index_temp[j]);

			}

			

			if (mode == i)
			{
				
				rolls[mode] = glm::rotate(rolls[mode], glm::radians(pressUp * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				rolls[mode] = glm::rotate(rolls[mode], glm::radians(pressLeft * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			}


			
			for (int j = 0; j < model_temp.size(); j++)
			{
				tran = tran * (model_temp[j] * rolls[rolls_index[j]]) ;
			}

			tran = model * tran;
	


			shader.setMat4("model", tran);
	

			meshes[i].Draw(shader);

	
		}



			
	}

private:
	const aiScene* scene;
	Assimp::Importer importer;

	std::vector<aiNode *> nodes;
	const aiNode  *rootNode;






	int chest_id;
	int head_id;
	int root_right_hand_id;
	int root_left_hand_id;
	int root_right_leg_id;
	int root_left_leg_id;





	std::vector<glm::mat4> rolls;

	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const &path)
	{
		// read file via ASSIMP
		
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{

			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('\\'));

		rootNode = scene->mRootNode;

	
		// process ASSIMP's root node recursively

		processNode(scene->mRootNode, scene);


	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene)
	{
	
		
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];



			meshes.push_back(processMesh(mesh, scene));
			nodes.push_back(node);
			rolls.push_back(glm::mat4());
			int id;

			if (CHEST.compare(node->mName.C_Str()) == 0)
			{
				//std::cout << node->mName.C_Str() << "\n";
				id = rolls.size() - 1;
				chest_id = id;
			}
			else if (HEAD.compare(node->mName.C_Str()) == 0)
			{
				//std::cout << node->mName.C_Str() << "\n";
				id = rolls.size() - 1;
				head_id = id;
			}
			else if (ROOT_LEFT_HAND.compare(node->mName.C_Str()) == 0)
			{
				//std::cout << node->mName.C_Str() << "\n";
				id = rolls.size() - 1;
				root_left_hand_id = id;
			}
			else if (ROOT_RIGHT_HAND.compare(node->mName.C_Str()) == 0)
			{
				//std::cout << node->mName.C_Str() << "\n";
				id = rolls.size() - 1;
				root_right_hand_id = id;
			}
			else if (ROOT_LEFT_LEG.compare(node->mName.C_Str()) == 0)
			{
				//std::cout << node->mName.C_Str() << "\n";
				id = rolls.size() - 1;
				root_left_leg_id = id;
			}
			else if (ROOT_RIGHT_LEG.compare(node->mName.C_Str()) == 0)
			{
				//std::cout << node->mName.C_Str() << "\n";
				id = rolls.size() - 1;
				root_right_leg_id = id;
			}

			
			//printMat4(node->mTransformation);

		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			//printMat4(node->mTransformation);

			processNode(node->mChildren[i], scene);
			
			//std::cout << "node ";
		}	

	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '\\' + filename;
	//std::cout << filename << endl;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


void printMat4(aiMatrix4x4 mat)
{
	std::printf("%f %f %f %f\n", mat.a1, mat.a2, mat.a3, mat.a4);
	std::printf("%f %f %f %f\n", mat.b1, mat.b2, mat.b3, mat.b4);
	std::printf("%f %f %f %f\n", mat.c1, mat.c2, mat.c3, mat.c4);
	std::printf("%f %f %f %f\n", mat.d1, mat.d2, mat.d3, mat.d4);

	std::cout << std::endl;
}
#endif