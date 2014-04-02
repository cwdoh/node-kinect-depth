var addon = require('./build/Release/addon');
var calculations = process.argv[2] || 100000000;

function runAsync () {
  function done (result) {
    console.log( result );
  }

  console.log( "setDepthRangeFilter" );
  // addon.setDepthRangeFilter( 500, 1000 );

  console.log( "startDepthCapture" );
  addon.captureDepth( done );

  addon.shutdown();
}

runAsync();
