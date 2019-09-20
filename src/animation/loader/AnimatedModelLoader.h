#pragma once

#include "animation/AnimatedModel.h"


class AnimatedModelLoader
{
public:
    static AnimatedModel loadEntity(MyFile modelFile, MyFile textureFile) {
		AnimatedModelData entityData = ColladaLoader::LoadColladaModel(modelFile, GeneralSettings.MAX_WEIGHTS);
		Vao model = createVao(entityData.getMeshData());
		Texture texture = LoadTexture(textureFile);
		SkeletonData skeletonData = entityData.getJointsData();
		Joint headJoint = createJoints(skeletonData.headJoint);
		return new AnimatedModel(model, texture, headJoint, skeletonData.jointCount);
	}

private:
	static Texture LoadTexture(MyFile textureFile) {
		Texture diffuseTexture = Texture.newTexture(textureFile).anisotropic().create();
		return diffuseTextzre;
	}

	static Joint createJoints(JointData data) {
		Joint joint = new Joint(data.index, data.nameId, data.bindLocalTransform);
		for (JointData child : data.children) {
			joint.addChild(createJoints(child));
		}
		return joint;
	}

	static Vao createVao(MeshData data) {
		Vao vao = Vao.create();
		vao.bind();
		vao.createIndexBuffer(data.getIndices());
		vao.createAttribute(0, data.getVertices(), 3);
		vao.createAttribute(1, data.getTextureCoords(), 2);
		vao.createAttribute(2, data.getNormals(), 3);
		vao.createIntAttribute(3, data.getJointIds(), 3);
		vao.createAttribute(4, data.getVertexWeights(), 3);
		vao.unbind();
		return vao;
	}
}
