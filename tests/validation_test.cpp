#include <gtest/gtest.h>
#include <thread>
#include "sequential_tree.h"
#include "parallel_tree.h"

// Test para SequentialTree: Prueba básica
TEST(SequentialTest, pruebaSimple) {
  SequentialTree* arbol_datos;
  arbol_datos = new SequentialTree({18, 0, 17});
  arbol_datos->left = new SequentialTree({25, 20});
  arbol_datos->right = new SequentialTree({17, 19, 0});
  arbol_datos->left->left = new SequentialTree({20, 22});
  arbol_datos->left->right = new SequentialTree({23});

  EXPECT_EQ(23, arbol_datos->calculateMaxAverage());
  delete arbol_datos;
}

// Test para SequentialTree: Prueba de inserción
TEST(SequentialTest, pruebaInsert) {
  SequentialTree* arbol_datos = new SequentialTree({18, 0, 17});
  arbol_datos->insert({25, 20});
  arbol_datos->insert({20, 22});
  arbol_datos->insert({23});
  arbol_datos->insert({17, 19, 0});

  EXPECT_EQ(23, arbol_datos->calculateMaxAverage());
  EXPECT_EQ(5, arbol_datos->contadorEstaciones);
  delete arbol_datos;
}

// Test para SequentialTree: Árbol vacío
TEST(SequentialTest, pruebaVacio) {
  SequentialTree* arbol_vacio = new SequentialTree({});
  EXPECT_EQ(0.0, arbol_vacio->calculateMaxAverage());
  delete arbol_vacio;
}

// Test para SequentialTree: Seguridad de hilos
TEST(SequentialTest, pruebaThreadSafe) {
  const int VALOR_MEDIO = 10;
  const int NUMERO_ELEMENTOS = 5;
  const int NUMERO_VECTORES = 20;

  SequentialTree* arbol_ref = nullptr;

  for (int i = 0; i < NUMERO_VECTORES; i++) {
    std::vector<double> tmp(NUMERO_ELEMENTOS);
    for (int j = 0; j < NUMERO_ELEMENTOS; ++j)
      tmp[j] = j;

    if (arbol_ref == nullptr)
      arbol_ref = new SequentialTree(tmp);
    else
      arbol_ref->insert(tmp);
  }

  SequentialTree* arbol_datos = nullptr;

  std::vector<std::thread> hilos;
  for (int i = 0; i < NUMERO_VECTORES; i++) {
    hilos.push_back(std::thread([&arbol_datos, NUMERO_ELEMENTOS]() {
      std::vector<double> tmp(NUMERO_ELEMENTOS);
      for (int j = 0; j < NUMERO_ELEMENTOS; ++j)
        tmp[j] = j;

      if (arbol_datos == nullptr)
        arbol_datos = new SequentialTree(tmp);
      else
        arbol_datos->insert(tmp);
    }));
  }

  std::for_each(hilos.begin(), hilos.end(), [](std::thread &th) {
    th.join();
  });

  EXPECT_EQ(arbol_datos->contadorEstaciones, NUMERO_VECTORES);
  EXPECT_EQ(arbol_datos->contadorEstaciones, arbol_ref->contadorEstaciones);
  EXPECT_EQ(arbol_datos->calculateMaxAverage(), arbol_ref->calculateMaxAverage());

  delete arbol_datos;
  delete arbol_ref;
}

// Test para ParallelTree: Prueba básica
TEST(ParallelTest, pruebaSimple) {
  ParallelTree* arbol_datos;
  arbol_datos = new ParallelTree({18, 0, 17});
  arbol_datos->left = new ParallelTree({25, 20});
  arbol_datos->right = new ParallelTree({17, 19, 0});
  arbol_datos->left->left = new ParallelTree({20, 22});
  arbol_datos->left->right = new ParallelTree({23});

  EXPECT_EQ(23, arbol_datos->calculateMaxAverage());
  delete arbol_datos;
}

// Test para ParallelTree: Prueba de inserción
TEST(ParallelTest, pruebaInsert) {
  ParallelTree* arbol_datos = new ParallelTree({18, 0, 17});
  arbol_datos->insert({25, 20});
  arbol_datos->insert({20, 22});
  arbol_datos->insert({23});
  arbol_datos->insert({17, 19, 0});

  EXPECT_EQ(23, arbol_datos->calculateMaxAverage());
  delete arbol_datos;
}

// Test para ParallelTree: Árbol vacío
TEST(ParallelTest, pruebaVacio) {
  ParallelTree* arbol_vacio = new ParallelTree({});
  EXPECT_EQ(0.0, arbol_vacio->calculateMaxAverage());
  delete arbol_vacio;
}

// Test para ParallelTree: Seguridad de hilos
TEST(ParallelTest, pruebaThreadSafe) {
  const int VALOR_MEDIO = 10;
  const int NUMERO_ELEMENTOS = 5;
  const int NUMERO_VECTORES = 20;

  ParallelTree* arbol_ref = nullptr;

  for (int i = 0; i < NUMERO_VECTORES; i++) {
    std::vector<double> tmp(NUMERO_ELEMENTOS);
    for (int j = 0; j < NUMERO_ELEMENTOS; ++j)
      tmp[j] = j;

    if (arbol_ref == nullptr)
      arbol_ref = new ParallelTree(tmp);
    else
      arbol_ref->insert(tmp);
  }

  ParallelTree* arbol_datos = nullptr;

  std::vector<std::thread> hilos;
  for (int i = 0; i < NUMERO_VECTORES; i++) {
    hilos.push_back(std::thread([&arbol_datos, NUMERO_ELEMENTOS]() {
      std::vector<double> tmp(NUMERO_ELEMENTOS);
      for (int j = 0; j < NUMERO_ELEMENTOS; ++j)
        tmp[j] = j;

      if (arbol_datos == nullptr)
        arbol_datos = new ParallelTree(tmp);
      else
        arbol_datos->insert(tmp);
    }));
  }

  std::for_each(hilos.begin(), hilos.end(), [](std::thread &th) {
    th.join();
  });

  EXPECT_EQ(arbol_datos->contadorEstaciones, NUMERO_VECTORES);
  EXPECT_EQ(arbol_datos->contadorEstaciones, arbol_ref->contadorEstaciones);
  EXPECT_EQ(arbol_datos->calculateMaxAverage(), arbol_ref->calculateMaxAverage());

  delete arbol_datos;
  delete arbol_ref;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
