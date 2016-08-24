#pragma once
#include <SOP/SOP_Node.h>

class SOP_Ebb : public SOP_Node
{
public:
    static OP_Node		*myConstructor(OP_Network*, const char *, OP_Operator *);

    static PRM_Template *buildTemplates();

protected:
             SOP_Ebb(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~SOP_Ebb();

    virtual OP_ERROR cookInputGroups(OP_Context &context, int alone = 0);

    virtual OP_ERROR cookMySop(OP_Context &context);

    void             run(fpreal t);

private:
    fpreal getTimestep(fpreal t) { return evalFloat("timestep", 0, t); }
    void getEbbCode(UT_String &str, fpreal t) { evalString(str, "ebbcode", 0, t); }
    void getEbbIncludeDir(UT_String &str, fpreal t) { evalString(str, "ebbinclude", 0, t); }
};
