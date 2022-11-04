# CMake generated Testfile for 
# Source directory: /home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj
# Build directory: /home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(correctExecution "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/tests/test-correct-execution.sh")
set_tests_properties(correctExecution PROPERTIES  WORKING_DIRECTORY "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/tests" _BACKTRACE_TRIPLES "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/CMakeLists.txt;10;add_test;/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/CMakeLists.txt;0;")
add_test(incorrectProgram "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/tests/test-nonexistent-program.sh")
set_tests_properties(incorrectProgram PROPERTIES  WORKING_DIRECTORY "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/tests" _BACKTRACE_TRIPLES "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/CMakeLists.txt;15;add_test;/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/CMakeLists.txt;0;")
add_test(correctExitCode "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/tests/test-correct-exit-code.sh")
set_tests_properties(correctExitCode PROPERTIES  WORKING_DIRECTORY "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/tests" _BACKTRACE_TRIPLES "/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/CMakeLists.txt;20;add_test;/home/ananth/Desktop/CS 3281f22/assignment-8-ananthjj/CMakeLists.txt;0;")
