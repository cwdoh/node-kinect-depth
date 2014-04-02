#include <node.h>

#include <signal.h>
#include <stdio.h>

#include <png.h>
#include <libfreenect.h>
#include "libfreenect_sync.h"

#include "kinect.h"

// variables
static bool initialzed = false;

static WORD depthFilter_Min = Kinect_DepthFilter_Range_Min;
static WORD depthFilter_Max = Kinect_DepthFilter_Range_Max;

static BYTE* pixelBuffer = NULL;

BYTE* kinectGetCaptureRGBBufferPointer()
{
  return pixelBuffer;
}

int kinectGetCapturedRGBBufferLength() {
  return (DEPTH_WIDTH * DEPTH_HEIGHT * sizeof(BYTE) * 3);
}

bool kinectInitialize()
{
  pixelBuffer = (BYTE*)malloc( DEPTH_WIDTH * DEPTH_HEIGHT * sizeof(BYTE) * 3 );

  initialzed = true;

  freenect_sync_set_led( LED_GREEN, 0 );

  return initialzed;
}

void kinectDestroy()
{
  freenect_sync_stop();

  initialzed = false;

  SAFE_FREE( pixelBuffer );
}

static void captureKinectDepthImage(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
  register DWORD value, range = depthFilter_Max - depthFilter_Min;

  register BYTE *pixelPointer = pixelBuffer;
  register WORD *depthPointer = (WORD*)v_depth;

  int x, y;

  for ( y = DEPTH_HEIGHT ; y > 0 ; y -- ) {
    // create 1 row of bitmap from depth buffer
    for ( x = DEPTH_WIDTH ; x > 0 ; x --, depthPointer ++ ) {
      value = (DWORD)*depthPointer;

      if ( value < depthFilter_Min ) {
        // underflow: set as green
        *pixelPointer ++ = 0x00;  // r
        *pixelPointer ++ = 0xFF;  // CACULATE_DEPTH((BYTE)(value * 0xFF / depthFilter_Min ));  // g
        *pixelPointer ++ = 0x00;  // b
      }
      else if ( depthFilter_Max < value ) {
        // overflow: set as blue
        *pixelPointer ++ = 0x00;  // CACULATE_DEPTH(0xFF);  // r
        *pixelPointer ++ = 0x00;  // g
        *pixelPointer ++ = 0xFF;  // b ~CACULATE_DEPTH((BYTE)((value - depthFilter_Max) * 0xFF / (Kinect_DepthFilter_Range_Max - depthFilter_Max) ));  // b
      }
      else {
        register BYTE color = CACULATE_DEPTH((BYTE)((value - depthFilter_Min) * 0xFF / range ));
        // in range: set as red
        *pixelPointer ++ = color;  // r
        *pixelPointer ++ = 0x00;  // g
        *pixelPointer ++ = 0x00;  // b
      }
    }
  }
}

bool kinectCaptureDepth()
{
  if ( !initialzed ) {
    kinectInitialize();
  }

  uint32_t timestamp;
  void *depth;

  int ret = freenect_sync_get_depth(&depth, &timestamp, 0, FREENECT_DEPTH_MM );

  if ( ret == 0 )
  {
    captureKinectDepthImage( NULL, depth, timestamp );
    return true;
  }

  return false;
}

bool kinectShutdown()
{
  if ( !initialzed ) {
    return false;
  }

  freenect_sync_set_led( LED_OFF, 0 );
  kinectDestroy();

  return true;
}

void kinectSetDepthRangeFilter( uint16_t min, uint16_t max )
{
  if ( min >= max )
    return;

  if ( Kinect_DepthFilter_Range_Min <= min && min <= Kinect_DepthFilter_Range_Max )
    depthFilter_Min = min;
  if ( Kinect_DepthFilter_Range_Min <= max && max <= Kinect_DepthFilter_Range_Max )
    depthFilter_Max = max;
}

uint16_t kinectGetMinInDepthRangeFilter()
{
  return depthFilter_Min;
}

uint16_t kinectGetMaxInDepthRangeFilter()
{
  return depthFilter_Max;
}
