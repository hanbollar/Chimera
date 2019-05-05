//
// Copyright (C) 
// 
// File: MayaPluginWizard1Cmd.cpp
//
// MEL Command: MayaPluginWizard1
//
// Author: Maya Plug-in Wizard 2.0
//

// Includes everything needed to register a simple MEL command with Maya.
// 
#include <maya/MSimple.h>
#include <maya/MSyntax.h>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

#include "ZoomorphNode.h"

// Use helper macro to register a command with Maya.  It creates and
// registers a command that does not support undo or redo.  The 
// created class derives off of MPxCommand.
//
//DeclareSimpleCommand( MayaPluginWizard1, "", "2018");

class MayaPluginWizard1 : public MPxCommand {
public:
  virtual MStatus	doIt(const MArgList&);
  static void *creator() { return new MayaPluginWizard1; }
  static MSyntax newSyntax();
};

const char *nameFlag = "-n";
const char *nameLongFlag = "-name";
const char *idFlag = "-id";
const char *idLongFlag = "-iiid";

MSyntax MayaPluginWizard1::newSyntax() {
  MSyntax syntax;

  syntax.addFlag(nameFlag, nameLongFlag, MSyntax::kString);
  syntax.addFlag(idFlag, idLongFlag, MSyntax::kString);

  return syntax;
}

MStatus MayaPluginWizard1::doIt( const MArgList& args ) {
	MStatus stat = MS::kSuccess;

  MString name0;
  MString id0;

  MString baseMeshName;
  MString animalMeshName;

  MArgDatabase argData(syntax(), args);

  if (argData.isFlagSet(nameFlag)) {
    argData.getFlagArgument(nameFlag, 0, name0);
  }

  if (argData.isFlagSet(idFlag)) {
    argData.getFlagArgument(idFlag, 0, id0);
  }

	setResult( "Hello!\n" );

	return stat;
}

MStatus initializePlugin(MObject obj) {
  MFnPlugin plugin(obj, "660", "Any");

  MStatus stat;
  stat = plugin.registerCommand("helloMaya", MayaPluginWizard1::creator, MayaPluginWizard1::newSyntax);
  if (!stat)
    stat.perror("registerCommand failed");

  // Register node
  stat = plugin.registerNode("ZoomorphNode", ZoomorphNode::id, ZoomorphNode::creator, ZoomorphNode::initialize);
  if (!stat)
	  stat.perror("register ZoomorphNode failed");

  MGlobal::executeCommand("source\"" + plugin.loadPath() + "/MyPluginDialog.mel\"");


  return stat;
}

MStatus uninitializePlugin(MObject obj) {
  MFnPlugin plugin(obj);

  MStatus	stat;
  stat = plugin.deregisterCommand("helloMaya");
  if (!stat)
    stat.perror("deregisterCommand failed");

  // Deregister node
  stat = plugin.deregisterNode(ZoomorphNode::id);
  CHECK_MSTATUS_AND_RETURN_IT(stat);

  return stat;
}
