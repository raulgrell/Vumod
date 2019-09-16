#pragma once

struct SkeletonData
{
	int jointCount;
	JointData headJoint;

	SkeletonData(int jointCount, JointData headJoint){
		this.jointCount = jointCount;
		this.headJoint = headJoint;
	}
}

struct MeshData
{
    constexpr static int DIMENSIONS = 3;

	float[] vertices;
	float[] textureCoords;
	float[] normals;
	int[] indices;
	int[] jointIds;
	float[] vertexWeights;

	MeshData(float[] vertices, float[] textureCoords, float[] normals, int[] indices,
			int[] jointIds, float[] vertexWeights) {
		this.vertices = vertices;
		this.textureCoords = textureCoords;
		this.normals = normals;
		this.indices = indices;
		this.jointIds = jointIds;
		this.vertexWeights = vertexWeights;
	}

	int getVertexCount() {
		return vertices.length / DIMENSIONS;
	}
}

struct AnimatedModelData {
	MeshData mesh;
	SkeletonData joints;

	AnimatedModelData(MeshData mesh, SkeletonData joints)
	    : mesh(mesh), joints(joints){
	}
};


#pragma once

struct VertexSkinData
{
	std::vector<Integer> jointIds;
	std::vector<Float> weights;

	void addJointEffect(int jointId, float weight){
		for(int i=0;i<weights.size();i++){
			if(weight > weights.get(i)){
				jointIds.add(i, jointId);
				weights.add(i, weight);
				return;
			}
		}
		jointIds.add(jointId);
		weights.add(weight);
	}

	void limitJointNumber(int max){
		if(jointIds.size() > max){
			float[] topWeights = new float[max];
			float total = saveTopWeights(topWeights);
			refillWeightList(topWeights, total);
			removeExcessJointIds(max);
		}else if(jointIds.size() < max){
			fillEmptyWeights(max);
		}
	}

private:
	void fillEmptyWeights(int max){
		while(jointIds.size() < max){
			jointIds.add(0);
			weights.add(0f);
		}
	}

	float saveTopWeights(float[] topWeightsArray){
		float total = 0;
		for(int i=0;i<topWeightsArray.length;i++){
			topWeightsArray[i] = weights.get(i);
			total += topWeightsArray[i];
		}
		return total;
	}

	void refillWeightList(float[] topWeights, float total){
		weights.clear();
		for(int i=0;i<topWeights.length;i++){
			weights.add(Math.min(topWeights[i]/total, 1));
		}
	}

	void removeExcessJointIds(int max){
		while(jointIds.size() > max){
			jointIds.remove(jointIds.size()-1);
		}
	}
}

struct SkinData
{
	final List<String> jointOrder;
	final List<VertexSkinData> verticesSkinData;

	SkinningData(List<String> jointOrder, List<VertexSkinData> verticesSkinData){
		this.jointOrder = jointOrder;
		this.verticesSkinData = verticesSkinData;
	}
}

struct VertexData
{
    constexpr static int NO_INDEX = -1;

	Vec3 position;
	int textureIndex = NO_INDEX;
	int normalIndex = NO_INDEX;
	Vertex duplicateVertex = null;
	int index;
	float length;
	List<Vec3> tangents = new ArrayList<Vec3>();
	Vec3 averagedTangent = new Vec3(0, 0, 0);

	VertexSkinData weightsData;

    Vertex(int index,Vec3 position, VertexSkinData weightsData)
        : index(index), weightsData(weightsData), position(position), length(length) { }

	void addTangent(Vec3 tangent){
		tangents.add(tangent);
	}

	void averageTangents(){
		if(tangents.isEmpty()){
			return;
		}
		for(Vec3 tangent : tangents){
			Vec3.add(averagedTangent, tangent, averagedTangent);
		}
		averagedTangent.normalise();
	}

	boolean isSet(){
		return textureIndex!=NO_INDEX && normalIndex!=NO_INDEX;
	}

	boolean hasSameTextureAndNormal(int textureIndexOther,int normalIndexOther){
		return textureIndexOther==textureIndex && normalIndexOther==normalIndex;
	}
}
