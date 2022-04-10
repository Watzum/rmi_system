# uncomment and replace upper case WORDS as necessary
project('rmi_system', 'cpp',
        license : ['Boost'],
        version : '0.1',
        # don't change anything of the three following options!
        default_options : ['cpp_std=c++20', 'warning_level=3', 'werror=true'])

thread_dep = dependency('threads')

# begin asio: https://think-async.com/Asio/
add_global_arguments('-I' + get_option('asio_include_dir'), language : 'cpp')
add_global_arguments('-DASIO_STANDALONE', language : 'cpp')
add_global_arguments('-DASIO_NO_DEPRECATED', language : 'cpp')
# end asio

# CLI11: https://github.com/CLIUtils/CLI11
add_global_arguments('-I' + get_option('cli11_include_dir'), language : 'cpp')

# spdlog: https://github.com/gabime/spdlog
add_global_arguments('-I' + get_option('spdlog_include_dir'), language : 'cpp')

# json: https://github.com/nlohmann/json
add_global_arguments('-I' + get_option('json_include_dir'), language : 'cpp')

# add additional include global arguments appropriately!

# # begin protobuf: https://developers.google.com/protocol-buffers/
# # protobuf: must be installed independently and has to be found...
protoc = find_program('protoc', required : true)
protobuf_dep = dependency('protobuf', required : true)

 gen = generator(protoc, \
  output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
  arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/src', '--cpp_out=@BUILD_DIR@', '@PLAINNAME@'])

# # change proto file accordingly
 generated = gen.process('src/functioncall.proto', 'src/returnvalue.proto')
# # end protobuf

# subdir('SUBDIR1')

inc_dir = include_directories('include/rmi_system', 'include/rmi_user')

rmi_user_client = ['src/client/main.cpp']

rmi_user_server = ['src/server/main.cpp', 'src/server/person.cpp', 
  'src/server/abstractMethods.cpp']
rmi_system_server = ['src/rmi/skeleton.cpp']

rmi_server = [rmi_user_server, rmi_system_server, generated]
rmi_client = [rmi_user_client, generated]

#src = [rmi_server, rmi_client, generated]
#src = [rmi_server, rmi_client, generated]

executable('client',
           sources : rmi_client,
           include_directories : inc_dir,
           dependencies : [thread_dep, protobuf_dep]
          )

executable('server',
           sources : rmi_server,
           include_directories : inc_dir,
           dependencies : [thread_dep, protobuf_dep]
          )