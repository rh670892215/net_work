cmake_minimum_required(VERSION 3.24)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c tcp/hello_server.c tcp/echo_server.c tcp/echo_client.c tcp/hello_client.c tcp/calc_server.c tcp/calc_client.c close/file_client.c close/file_server.c process/fork.c process/wait.c process/wait_pid.c process/signal.c process/sigaction.c process/remove_zombie.c tcp/echo_mpserver.c tcp/echo_mpclient.c process_communication/pipe1.c process_communication/pipe2.c process_communication/pipe3.c tcp/echo_store_server.c select/select.c select/echo_select_server.c oob/oob_send.c oob/oob_receive.c oob/peek_receive.c oob/peek_send.c oob/writev.c oob/readv.c broadcast/news_send.c broadcast/news_receive.c)
