message(STATUS "FetchContent: cpptrace")

FetchContent_Declare(
  cpptrace
  GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
  GIT_TAG        v0.8.2 # <HASH or TAG>
)
FetchContent_MakeAvailable(cpptrace)