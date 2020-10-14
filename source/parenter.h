#pragma once

#include <map>

#include <maya/MMatrix.h>
#include <maya/MTypeId.h>
#include <maya/MPxNode.h>

class Parenter : public MPxNode
{
public:

	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	virtual MStatus connectionMade(const MPlug& plug, const MPlug& other_plug, bool is_source);
	virtual MStatus connectionBroken(const MPlug& plug, const MPlug& other_plug, bool is_source);

public:
	static MTypeId type_ID;
	static MObject child_matrix;
	static MObject parent_matrices;
	static MObject chosen_parent;
	static MObject use_translate;
	static MObject use_rotate;
	static MObject time;
	static MObject current_parent;
	static MObject output_translate_x;
	static MObject output_translate_y;
	static MObject output_translate_z;
	static MObject output_translate;
	static MObject output_rotate_x;
	static MObject output_rotate_y;
	static MObject output_rotate_z;
	static MObject output_rotate;
	static MObject output_chosen_parent;

private:
	std::map<int, MString> parents;
	int last_chosen_parent;
	MMatrix offset;
	bool should_recompute_offset;
	bool last_use_translate;
	bool last_use_rotate;

};
