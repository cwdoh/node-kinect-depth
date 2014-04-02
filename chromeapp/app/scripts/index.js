'use strict';
document.addEventListener('DOMContentLoaded', function() {
  setTimeout(function () {
    var image = document.getElementById('image');

    window.socket
    try{
      socket = io.connect('http://localhost:4040');
    }catch(e){

    }
    socket.on('depthframe', function(event) {
        image.src = event.data;
    });
  }, 1000);

  var min = document.getElementById('min');
  var max = document.getElementById('max');

  function setDepthFilter() {
    socket.emit(
      'depthfilter',
      {
        min: parseInt( min.value ),
        max: parseInt( max.value )
      }
    );

    document.getElementById('minval').innerText = '(' + min.value + ')';
    document.getElementById('maxval').innerText = '(' + max.value + ')';
  }

  min.addEventListener('change', function(event) {
    var value = event.target.value;
    max.min = value;
    setDepthFilter();
  });
  max.addEventListener('change', function(event) {
    var value = event.target.value;
    min.max = value;
    setDepthFilter();
  });
}, false);
