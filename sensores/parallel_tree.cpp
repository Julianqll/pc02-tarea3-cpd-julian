#include "parallel_tree.h"
#include <omp.h>
#include <algorithm>
#include <cmath>

ParallelTree::ParallelTree(const std::vector<double>& data)
    : SensorTree(data), contadorEstaciones(1) {}

double ParallelTree::calculateMaxAverage() {
  double max_avg = 0.0;

  // Configurar el número máximo de hilos
  #pragma omp parallel num_threads(omp_get_max_threads())
  {
    #pragma omp single nowait
    {
      max_avg = calculateMaxAverageInternal(this);
    }
  }

  return max_avg;
}

double ParallelTree::calculateMaxAverageInternal(SensorTree* node_ptr) {
  if (node_ptr == nullptr) return 0.0;

  // Cálculo del promedio actual
  double sum = 0.0;
  int count = 0;
  for (double value : node_ptr->sensor_data) {
    sum += value;
    count++;
  }

  double current_avg = (count > 0) ? sum / count : 0.0;

  double max_avg_left = 0.0, max_avg_right = 0.0;

  // Dinámicamente ajustar el umbral basado en la profundidad
  const int TASK_THRESHOLD = 1000;
  const bool should_parallelize = contadorEstaciones > TASK_THRESHOLD;

  if (node_ptr->left != nullptr) {
    if (should_parallelize) {
      #pragma omp task shared(max_avg_left)
      {
        max_avg_left = calculateMaxAverageInternal(node_ptr->left);
      }
    } else {
      max_avg_left = calculateMaxAverageInternal(node_ptr->left);
    }
  }

  if (node_ptr->right != nullptr) {
    if (should_parallelize) {
      #pragma omp task shared(max_avg_right)
      {
        max_avg_right = calculateMaxAverageInternal(node_ptr->right);
      }
    } else {
      max_avg_right = calculateMaxAverageInternal(node_ptr->right);
    }
  }

  // Sincronización de tareas
  #pragma omp taskwait

  return std::max({current_avg, max_avg_left, max_avg_right});
}

void ParallelTree::insert(const std::vector<double>& data) {
  insertInternal(this, data);
  contadorEstaciones++;
}

void ParallelTree::insertInternal(SensorTree* node_ptr,
                                  const std::vector<double>& data) {
  if (node_ptr == nullptr) {
    node_ptr = new ParallelTree(data);
    return;
  } else if (node_ptr->left == nullptr) {
    node_ptr->left = new ParallelTree(data);
    return;
  } else if (node_ptr->right == nullptr) {
    node_ptr->right = new ParallelTree(data);
    return;
  }

  if (node_ptr->left != nullptr) insertInternal(node_ptr->left, data);
  if (node_ptr->right != nullptr) insertInternal(node_ptr->right, data);
}
