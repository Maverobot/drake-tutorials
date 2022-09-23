add_executable(meshcat_sdf meshcat_sdf.cpp)
target_link_libraries(meshcat_sdf PRIVATE drake::drake)

add_executable(multibody_simulation multibody_simulation.cpp)
target_link_libraries(multibody_simulation PRIVATE drake::drake gflags)
