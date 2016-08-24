#include "SOP_Ebb.h"

#include <GU/GU_Detail.h>
#include <GEO/GEO_PrimPoly.h>
#include <GEO/GEO_PrimVDB.h>
#include <GU/GU_PrimVDB.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_TemplateBuilder.h>
#include <CH/CH_LocalVariable.h>
#include <UT/UT_Interrupt.h>
#include <SYS/SYS_Math.h>
#include <ebb.h>

using namespace UT::Literal;

static const char *theDsFile = R"THEDSFILE(
{
    name  ebb 

    parm {
      name   "timestep"
      label  "Timestep"
      type   float
      default { "0.1" }
      range { 0 10 }
    }
    parm {
      name      "ebbinclude"
      label     "Ebb Include Path"
      type      string
      default   { "ebb/include" }
    }
    parm {
      name      "ebbcode"
      label     "Ebb Code"
      type      string
      default   { "import 'ebb'\nlocal L = require 'ebblib'\n" }
      parmtag   { "editor" "1" }
      parmtag   { "editorlines" "8-80" }
      parmtag   { "editorlang" "ebb" }
    }
}
)THEDSFILE";

PRM_Template *
SOP_Ebb::buildTemplates()
{
  static PRM_TemplateBuilder templ("SOP_Ebb.C"_sh, theDsFile);
  return templ.templates();
}

OP_Node *
SOP_Ebb::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
  return new SOP_Ebb(net, name, op);
}

SOP_Ebb::SOP_Ebb(OP_Network *net, const char *name, OP_Operator *op)
    : SOP_Node(net, name, op)
{
  mySopFlags.setManagesDataIDs(true);
}

SOP_Ebb::~SOP_Ebb() {}

OP_ERROR
SOP_Ebb::cookInputGroups(OP_Context &context, int alone)
{
  return error();
}

OP_ERROR
SOP_Ebb::cookMySop(OP_Context &context)
{
  if (lockInputs(context) >= UT_ERROR_ABORT)
    return error();

  gdp->clearAndDestroy();
  duplicateSource(0, context);

  auto t = context.getTime();

  if ( cookInputGroups(context) < UT_ERROR_ABORT )
  {
    // Do stuff here
    run(t);
  }

  unlockInputs();
  return error();
}

void
SOP_Ebb::run(fpreal t)
{
    // Create a Lua interpreter
    lua_State * L = luaL_newstate();
    // and install the standard Lua libraries
    luaL_openlibs(L);

    auto doerror = [](lua_State * L)
    {
      printf("%s\n", luaL_checkstring(L,-1));
    };

    terra_Options terra_options;
    memset(&terra_options, 0, sizeof(terra_Options));
    terra_options.usemcjit = 1;
    if(terra_initwithoptions(L, &terra_options))
        return doerror(L);

    UT_String ebbincludedir;
    getEbbIncludeDir(ebbincludedir, t);

    EbbOptions ebb_options;
    ebb_options.include_dir = ebbincludedir.c_str();
    ebb_options.use_gpu     = 0;
    if(setupebb(L, &ebb_options))
        return doerror(L);

    UT_String ebbcode;
    getEbbCode(ebbcode, t);
    if (terra_dostring(L, ebbcode.c_str()))
        return doerror(L);

}
