#include <UT/UT_DSOVersion.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include "SOP/SOP_Ebb.h"

// Register sop operator
void
newSopOperator(OP_OperatorTable *table)
{
    table->addOperator(new OP_Operator(
        "hdk_ebb",                   // Internal name
        "Ebb",                       // UI name
        SOP_Ebb::myConstructor,      // How to build the SOP
        SOP_Ebb::buildTemplates(),   // My parameters
        1,                           // Min # of sources
        1,                           // Max # of sources
        0,//SOP_Ebb::myVariables,    // Local variables
        OP_FLAG_GENERATOR));         // Flag it as generator
}
