if get_option('dynamic')
  executable('hex-to-image', ['hex-to-image.cpp', 'stb_image_write.h'])
else
  executable('hex-to-image', ['hex-to-image.cpp', 'stb_image_write.h'], cpp_args : ['-static', '-static-libgcc', '-static-libstdc++'], link_args: ['-static', '-static-libgcc', '-static-libstdc++'])
endif
