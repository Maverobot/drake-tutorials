#include <drake/solvers/mathematical_program.h>
#include <drake/solvers/solve.h>

#include <iostream>

template <typename... Args>
void print(Args&&... value) {
  (std::cout << ... << value) << "\n---\n";
}

void update(const Eigen::Ref<const Eigen::VectorXd>& x) {
  print("x = ", x.transpose());
}

int main(int argc, char* argv[]) {
  auto prog = drake::solvers::MathematicalProgram();
  auto x = prog.NewContinuousVariables(2);
  prog.AddConstraint(x[0] * x[1] == 9);
  prog.AddCost(pow(x[0], 2) + pow(x[1], 2));
  prog.AddVisualizationCallback(update, x);

  Eigen::VectorXd x_init(2);
  x_init << 4, 5;
  drake::solvers::Solve(prog, x_init);
  return 0;
}
