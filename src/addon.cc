#include <node.h>
#include "Async.h"

using namespace v8;

// Expose synchronous and asynchronous access to our
// Estimate() function
void InitAll(Handle<Object> exports) {
  exports->Set(String::NewSymbol("captureDepth"),
      FunctionTemplate::New(captureDepthAsync)->GetFunction());

  exports->Set(String::NewSymbol("shutdown"),
      FunctionTemplate::New(shutdownAsync)->GetFunction());

  exports->Set(String::NewSymbol("setDepthRangeFilter"),
      FunctionTemplate::New(setDepthRangeFilterAsync)->GetFunction());
}

NODE_MODULE(addon, InitAll)
