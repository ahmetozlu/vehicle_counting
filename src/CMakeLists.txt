add_executable(vehicle_counting_demo Blob.cpp
Blob.h
Main.cpp
)

target_link_libraries( vehicle_counting_demo ${OpenCV_LIBRARIES} )
     
install (TARGETS vehicle_counting_demo DESTINATION bin)
