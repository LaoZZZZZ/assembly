#ifndef PARAMETER_H
#define PARAMETER_H
#include <memory>
using std::shared_ptr;
#include "assemblyparameter.h"
#include "globalparameter.h"
class Parameter
{
public:
    Parameter(AssemblyParameter* aparams,
              globalParameter* gparams);
    std::shared_ptr<AssemblyParameter>              assemblyParameters()const{return this->_assembleParams;}
    std::shared_ptr<globalParameter>                globalParameters()const{return this->_globalParams;}
private:
    std::shared_ptr<AssemblyParameter>                          _assembleParams;
    std::shared_ptr<globalParameter>                            _globalParams;
};

#endif // PARAMETER_H
