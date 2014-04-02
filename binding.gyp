{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "./src/addon.cc",
        "./src/async.cc",
        "./src/buffer_compat.cc",
        "./src/kinect.cc",
        "./src/libfreenect_sync.cc"
      ],
      "libraries": [
        "libfreenect.a",
        "libpng.a"
      ],
      "include_dirs": [
        "/usr/local/include/libfreenect",
        "/usr/local/include/libpng15"
      ]
    }
  ]
}
