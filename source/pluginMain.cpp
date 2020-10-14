#include "parenter.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin plugin_fn(obj, "Christian Corsica", "1.0", "Any");

	status = plugin_fn.registerNode("parenter", Parenter::type_ID, Parenter::creator, Parenter::initialize, MPxNode::kDependNode);

	if (status != MS::kSuccess)
		status.perror("Could no register the parenter node");

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin_fn;
	plugin_fn.deregisterNode(Parenter::type_ID);

	return MS::kSuccess;
}