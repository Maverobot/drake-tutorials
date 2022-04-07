#include <drake/solvers/mathematical_program.h>

#include <utils/utils.h>

int main() {
  drake::solvers::MathematicalProgram prog;
  auto x = prog.NewContinuousVariables(2);

  print(x);
  print(1 + 2 * x[0] + 3 * x[1] + 4 * x[1]);

  auto y = prog.NewContinuousVariables(2, "dog");
  print(y);
  print(y[0] + y[0] + y[1] * y[1] * y[1]);

  auto var_matrix = prog.NewContinuousVariables(3, 2, "A");
  print(var_matrix);

  return 0;
}
