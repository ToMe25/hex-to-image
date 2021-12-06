if get_option('dynamic')
  exe = executable('hex-to-image', ['hex-to-image.cpp', 'stb_image_write.h'])
else
  exe = executable('hex-to-image', ['hex-to-image.cpp', 'stb_image_write.h'], cpp_args : ['-static', '-static-libgcc', '-static-libstdc++'], link_args: ['-static', '-static-libgcc', '-static-libstdc++'])
endif

hello_world_in = join_paths(join_paths(meson.source_root(), 'test'), 'hello-world.txt')
test('hello world test(3 digits)', exe, args: ['-d', '3', '-i', hello_world_in, '-o', 'hello-world.png', '--override'])

test_in = join_paths(join_paths(meson.source_root(), 'test'), 'test.txt')
test('test test(3 digits)', exe, args: ['-i', test_in, '-o', 'test.png', '--override'])
