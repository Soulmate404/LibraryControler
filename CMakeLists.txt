cmake_minimum_required(VERSION 3.30)
project(LibraryController C)

set(CMAKE_C_STANDARD 11)


include_directories(C:\\Program\ Files\\MySQL\\MySQL\ Server\ 9.0\\include)
link_directories(C:\\Program\ Files\\MySQL\\MySQL\ Server\ 9.0\\lib)
link_libraries(libmysql)


add_executable(LibraryController Controller/code/main.c
        Mapper/code/mysqlPointer.c
        Mapper/code/AdminMapper.c
        Mapper/code/AdminMapper.h
        Mapper/code/BookBorrowMapper.c
        Mapper/code/BookBorrowMapper.h
        Mapper/code/mysqlPointer.h
        Service/code/adminService.c
        Service/code/adminService.h
        Mapper/code/UserLogMapper.c
        Mapper/code/UserLogMapper.h
        Service/code/bookBorrowService.c
        Service/code/bookBorrowService.h
        Service/code/userLog.c
        Service/code/userLog.h
)
target_link_libraries(LibraryController libmysql)
