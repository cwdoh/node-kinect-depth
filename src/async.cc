#include <node.h>
#include "buffer_compat.h"

#include "Async.h"

#include <libfreenect.h>
#include "kinect.h"

using namespace v8;

typedef struct AsyncData {
  Persistent<Function> callback;  // callback function
} AsyncData;

void AsyncWork(uv_work_t *req) {
  kinectCaptureDepth();
}

void AsyncAfter(uv_work_t *req) {
  HandleScope scope;

  node::Buffer *buffer = node::Buffer::New(kinectGetCapturedRGBBufferLength());
  memcpy(BufferData(buffer), kinectGetCaptureRGBBufferPointer(), kinectGetCapturedRGBBufferLength());

  Handle<Value> argv[] = {
    buffer->handle_
  };

  TryCatch try_catch;

  // fetch our data structure
  AsyncData *asyncData = (AsyncData *)req->data;

  asyncData->callback->Call(Context::GetCurrent()->Global(), 1, argv);

  if (try_catch.HasCaught())
    node::FatalException(try_catch);

  // dispose the Persistent handle so the callback
  // function can be garbage-collected
  asyncData->callback.Dispose();

  // clean up any memory we allocated
  delete asyncData;
  delete req;
}

Handle<Value> captureDepthAsync(const Arguments& args) {
  HandleScope scope;

  if ( args.Length() >= 1 && args[0]->IsFunction() ) {
    // create an async work token
    uv_work_t *req = new uv_work_t;
    // assign our data structure that will be passed around
    AsyncData *asyncData = new AsyncData;
    req->data = asyncData;

    asyncData->callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));

    // pass the work token to libuv to be run when a
    // worker-thread is available to
    uv_queue_work(
      uv_default_loop(),
      req,                          // work token
      AsyncWork,                    // work function
      (uv_after_work_cb)AsyncAfter  // function to run when complete
    );
  }

  return scope.Close(Undefined());
}

Handle<Value> shutdownAsync(const Arguments& args) {
  HandleScope scope;

  if ( kinectShutdown() ) {
    return scope.Close(True());
  }

  return scope.Close(False());
}

Handle<Value> setDepthRangeFilterAsync(const Arguments& args) {
  HandleScope scope;

  if ( args.Length() < 2 ) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
  }
  else if ( !args[0]->IsNumber() || !args[1]->IsNumber() ) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
  }
  else {
    kinectSetDepthRangeFilter( args[0]->NumberValue(), args[1]->NumberValue() );
  }

  return scope.Close(Undefined());
}
