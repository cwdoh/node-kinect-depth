#ifndef __Kinect_Header_Included__
#define __Kinect_Header_Included__

// configurations
#define DEPTH_WIDTH   640
#define DEPTH_HEIGHT  480

// MACROs
#define SAFE_FREE(a)  if ( a ) { free(a); a = NULL; }

// Type definitions
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;

typedef enum {
  KINECT_STATUS_NONE = 0,
  KINECT_STATUS_RUNNING = 1
} Kinect_Status;

typedef enum {
  Kinect_DepthFilter_Range_Min = 0,
  Kinect_DepthFilter_Range_Max = 10000
} Kinect_DepthFilter_Range;

// APIs
extern BYTE* kinectGetCaptureRGBBufferPointer();
extern int kinectGetCapturedRGBBufferLength();

extern bool kinectInitialze();
extern void kinectDestroy();

extern bool kinectCaptureDepth();
extern bool kinectShutdown();

extern void kinectSetDepthRangeFilter( uint16_t, uint16_t );
extern uint16_t kinectGetMinInDepthRangeFilter();
extern uint16_t kinectGetMaxInDepthRangeFilter();

#endif  // __Kinect_Header_Included__
