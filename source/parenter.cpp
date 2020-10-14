#include <map>

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTime.h>

#include "parenter.h"
#include "util.h"

MTypeId Parenter::type_ID(0x80004);
MObject Parenter::child_matrix;
MObject Parenter::parent_matrices;
MObject Parenter::chosen_parent;
MObject Parenter::use_translate;
MObject Parenter::use_rotate;
MObject Parenter::time;
MObject Parenter::current_parent;
MObject Parenter::output_translate_x;
MObject Parenter::output_translate_y;
MObject Parenter::output_translate_z;
MObject Parenter::output_translate;
MObject Parenter::output_rotate_x;
MObject Parenter::output_rotate_y;
MObject Parenter::output_rotate_z;
MObject Parenter::output_rotate;


void* Parenter::creator()
{
	return new Parenter();
}


MStatus Parenter::initialize()
{
	MFnMatrixAttribute matrix_fn;
	MFnNumericAttribute numeric_fn;
	MFnUnitAttribute unit_fn;
	MFnCompoundAttribute compound_fn;
	MFnTypedAttribute typed_fn;

	child_matrix = matrix_fn.create("childMatrix", "cdm");
	matrix_fn.setStorable(true);
	addAttribute(child_matrix);

	parent_matrices = matrix_fn.create("parentMatrices", "pms");
	matrix_fn.setStorable(true);
	matrix_fn.setConnectable(true);
	matrix_fn.setArray(true);
	matrix_fn.setUsesArrayDataBuilder(true);
	addAttribute(parent_matrices);

	chosen_parent = numeric_fn.create("chosenParent", "chp", MFnNumericData::kInt);
	numeric_fn.setMin(0);
	numeric_fn.setStorable(true);
	numeric_fn.setKeyable(true);
	numeric_fn.setWritable(true);
	addAttribute(chosen_parent);

	use_translate = numeric_fn.create("useTranslate", "ust", MFnNumericData::kBoolean, 1.0);
	numeric_fn.setStorable(true);
	numeric_fn.setKeyable(true);
	numeric_fn.setWritable(true);
	addAttribute(use_translate);

	use_rotate = numeric_fn.create("useRotate", "usr", MFnNumericData::kBoolean, 1.0);
	numeric_fn.setStorable(true);
	numeric_fn.setKeyable(true);
	numeric_fn.setWritable(true);
	addAttribute(use_rotate);

	time = unit_fn.create("inputTime", "intm", MFnUnitAttribute::kTime);
	unit_fn.setStorable(true);
	unit_fn.setKeyable(true);
	unit_fn.setWritable(true);
	addAttribute(time);

	current_parent = typed_fn.create("currentParent", "curp", MFnData::kString);
	typed_fn.setStorable(true);
	typed_fn.setWritable(true);
	typed_fn.setKeyable(false);
	typed_fn.setChannelBox(true);
	addAttribute(current_parent);


	output_rotate_x = unit_fn.create("outputRotateX", "orx", MFnUnitAttribute::kAngle, 0.0);
	unit_fn.setStorable(false);
	unit_fn.setKeyable(false);
	unit_fn.setWritable(false);
	addAttribute(output_rotate_x);

	output_rotate_y = unit_fn.create("outputRotateY", "ory", MFnUnitAttribute::kAngle, 0.0);
	unit_fn.setStorable(false);
	unit_fn.setKeyable(false);
	unit_fn.setWritable(false);
	addAttribute(output_rotate_y);

	output_rotate_z = unit_fn.create("outputRotateZ", "orz", MFnUnitAttribute::kAngle, 0.0);
	unit_fn.setStorable(false);
	unit_fn.setKeyable(false);
	unit_fn.setWritable(false);
	addAttribute(output_rotate_z);

	output_rotate = compound_fn.create("outputRotate", "orc");
	compound_fn.addChild(output_rotate_x);
	compound_fn.addChild(output_rotate_y);
	compound_fn.addChild(output_rotate_z);
	compound_fn.setStorable(false);
	compound_fn.setKeyable(false);
	compound_fn.setWritable(false);
	addAttribute(output_rotate);

	output_translate_x = numeric_fn.create("outputTranslateX", "otx", MFnNumericData::kDouble, 0);
	numeric_fn.setStorable(false);
	numeric_fn.setKeyable(false);
	numeric_fn.setWritable(false);
	addAttribute(output_translate_x);

	output_translate_y = numeric_fn.create("outputTranslateY", "oty", MFnNumericData::kDouble, 0);
	numeric_fn.setStorable(false);
	numeric_fn.setKeyable(false);
	numeric_fn.setWritable(false);
	addAttribute(output_translate_y);

	output_translate_z = numeric_fn.create("outputTranslateZ", "otz", MFnNumericData::kDouble, 0);
	numeric_fn.setStorable(false);
	numeric_fn.setKeyable(false);
	numeric_fn.setWritable(false);
	addAttribute(output_translate_z);

	output_translate = compound_fn.create("outputTranslate", "ots");
	compound_fn.addChild(output_translate_x);
	compound_fn.addChild(output_translate_y);
	compound_fn.addChild(output_translate_z);
	compound_fn.setStorable(false);
	compound_fn.setKeyable(false);
	compound_fn.setWritable(false);
	addAttribute(output_translate);

	attributeAffects(child_matrix, output_translate);
	attributeAffects(parent_matrices, output_translate);
	attributeAffects(chosen_parent, output_translate);
	attributeAffects(use_translate, output_translate);
	attributeAffects(use_rotate, output_translate);
	attributeAffects(time, output_translate);

	attributeAffects(child_matrix, output_rotate);
	attributeAffects(parent_matrices, output_rotate);
	attributeAffects(chosen_parent, output_rotate);
	attributeAffects(use_translate, output_rotate);
	attributeAffects(use_rotate, output_rotate);
	attributeAffects(time, output_rotate);
	
	return MS::kSuccess;
}

MStatus Parenter::compute(const MPlug& plug, MDataBlock& data)
{

	if (plug == output_rotate || plug == output_rotate_x || plug == output_rotate_y || plug == output_rotate_z || plug == output_translate || plug == output_translate_x || plug == output_translate_y || plug == output_translate_z)
	{
		
		MTime time_value = data.inputValue(time).asTime();
		MMatrix child_matrix_value = data.inputValue(child_matrix).asMatrix();
		MArrayDataHandle parent_matrices_array = data.inputArrayValue(parent_matrices);
		int parent_choice_value = data.inputValue(chosen_parent).asInt();
		int number_of_parents = parent_matrices_array.elementCount();
		bool use_translate_value = data.inputValue(use_translate).asBool();
		bool use_rotate_value = data.inputValue(use_rotate).asBool();
		should_recompute_offset = false;

		// using int as selection for parent and index for parent matrix array
		// This IF makes sure the parent_choice_value is a valid parent matrix array index
		if (parent_choice_value < number_of_parents)
		{
			parent_matrices_array.jumpToElement(parent_choice_value);
			MMatrix parent_matrix = parent_matrices_array.inputValue().asMatrix();
			if (use_translate_value != last_use_translate)
			{
				should_recompute_offset = true;
				last_use_translate = use_translate_value;
			}

			if (use_rotate_value != last_use_rotate)
			{
				should_recompute_offset = true;
				last_use_rotate = use_rotate_value;
			}

			// chosen parent value selection changed so calculate new offset
			if (parent_choice_value != last_chosen_parent)
			{
				should_recompute_offset = true;
				last_chosen_parent = parent_choice_value;
				data.inputValue(current_parent).set(parents[parent_choice_value]);
				//MPlug current_parent_plug(thisMObject(), current_parent);
				//current_parent_plug.setString(parents[parent_choice_value]);
				// MGlobal::displayInfo(MString("Parent = ") + parents[parent_choice_value]);
			}

			if (should_recompute_offset)
			{
				offset = child_matrix_value * parent_matrix.inverse();
			}

			// offset * parent matrix gives us parent constraint values
			MTransformationMatrix output_matrix(offset * parent_matrix);
			MEulerRotation rotation = output_matrix.eulerRotation();
			MVector translation = output_matrix.getTranslation(MSpace::kWorld);


			if (use_rotate_value || should_recompute_offset)
			{
				data.outputValue(output_rotate).set(rotation.x, rotation.y, rotation.z);
			}
			
			if (use_translate_value || should_recompute_offset)
			{
				data.outputValue(output_translate).set(translation.x, translation.y, translation.z);
			}
			
		}
		else
		{
			MGlobal::displayInfo(MString("Outside parent count. Max Parent = ") + (number_of_parents - 1));
		}

		data.outputValue(output_rotate).setClean();
		data.outputValue(output_translate).setClean();

	}

	return MS::kSuccess;
}

MStatus Parenter::connectionMade(const MPlug & plug, const MPlug & other_plug, bool is_source)
{
	
	MStatus status;
	plug.array(&status);

	if (status == MStatus::kSuccess)
	{
		MStringArray names;
		
		unsigned int index = plug.logicalIndex();
		MString attribute_name = other_plug.partialName();
		MString full_name = other_plug.partialName(true);
		full_name.split('.', names);
		MString parent_name = names[0];

		MGlobal::displayInfo(MString("Connection made, parent = " + parent_name));

		parents[index] = parent_name;
	}

	
	return MPxNode::connectionMade(plug, other_plug, is_source);
}

MStatus Parenter::connectionBroken(const MPlug & plug, const MPlug & other_plug, bool is_source)
{
	
	MStatus status;
	plug.array(&status);

	if (status == MStatus::kSuccess)
	{

		unsigned int index = plug.logicalIndex();
		parents.erase(index);

		MGlobal::displayInfo(MString("Connection broken, index = " + index));

	}
	
	
	return MPxNode::connectionBroken(plug, other_plug, is_source);
}

