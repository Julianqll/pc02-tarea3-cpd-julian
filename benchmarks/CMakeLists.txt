# Buscar todos los archivos .cpp en el directorio
FILE(GLOB_RECURSE ALL_BENCH_CPP *.cpp)

# Incluir la carpeta sensores
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/sensores)

# Buscar OpenMP
FIND_PACKAGE(OpenMP REQUIRED)

# Iterar sobre cada archivo .cpp y crear un ejecutable
foreach(ONE_BENCH_CPP ${ALL_BENCH_CPP})
  # Obtener el nombre base del archivo (sin extensión)
  GET_FILENAME_COMPONENT(ONE_BENCH_EXEC ${ONE_BENCH_CPP} NAME_WE)

  # Definir el nombre del ejecutable
  SET(TARGET_NAME Bench_${ONE_BENCH_EXEC})

  # Crear el ejecutable
  ADD_EXECUTABLE(${TARGET_NAME} ${ONE_BENCH_CPP})

  # Establecer propiedades del ejecutable
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${ONE_BENCH_EXEC})

  # Enlazar las bibliotecas necesarias
  TARGET_LINK_LIBRARIES(${TARGET_NAME}
    benchmark
    sensores  # Cambié de 'sensores' a 'sensores_lib', que es tu biblioteca creada
    OpenMP::OpenMP_CXX
  )
endforeach()
