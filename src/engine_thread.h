#ifndef ENGINE_THREAD_H
#define ENGINE_THREAD_H
#include "messagegenerator.h"
#include <vector>
using std::vector;
/**
  * An example of engine thread
*/
class engine_thread : public Processor
{
public:
    engine_thread(std::shared_ptr<Parameter> p);
private:
    void drive();
private:
};

#endif // ENGINE_THREAD_H
