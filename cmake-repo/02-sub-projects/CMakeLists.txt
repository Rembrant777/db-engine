cmake_minimum_required(VERSION 3.25)
project(main_proj)

# we use add_subdirectory cmake inner method
# to add corresponding sub-projects' compile && generated libraries to current main project
# first compile & add sub lib1
add_subdirectory(sublib1)
# then sublib2
add_subdirectory(sublib2)

# then main project
add_subdirectory(main_proj)