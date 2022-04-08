#include <drake/solvers/mathematical_program.h>
#include <drake/solvers/solve.h>

#include <iostream>

template <typename... Args>
void print(Args&&... value) {
  (std::cout << ... << value) << "\n---\n";
}

int main(int argc, char* argv[]) {
  auto prog = drake::solvers::MathematicalProgram();
  auto x = prog.NewContinuousVariables(1)[0];
  print("x = ", x);
  auto y = prog.NewContinuousVariables(1)[0];
  print("y = ", y);
  auto constraint1 = prog.AddConstraint(x + y >= 1);
  print("constraint1: ", constraint1);
  auto constraint2 = prog.AddConstraint(x + y <= 0);
  print("constraint2: ", constraint2);
  auto cost1 = prog.AddCost(x);
  print("cost1: ", cost1);

  auto result = drake::solvers::Solve(prog);
  print("Success: ", result.is_success());
  print("Solution result: ", result.get_solution_result());
  return 0;
}
