#include <drake/common/eigen_types.h>
#include <drake/common/symbolic/expression.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/primitives/symbolic_vector_system.h>
#include <drake/systems/primitives/vector_log_sink.h>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(int argc, char* argv[]) {
  auto x = drake::symbolic::Variable("x");

  drake::systems::DiagramBuilder<double> diagram_builder;

  drake::systems::SymbolicVectorSystem<double>* system =
      diagram_builder.AddSystem(drake::systems::SymbolicVectorSystemBuilder()
                                    .state(x)
                                    .dynamics(-x + pow(x, 3))
                                    .output(x)
                                    .Build());

  drake::systems::VectorLogSink<double>* logger =
      drake::systems::LogVectorOutput<double>(system->get_output_port(), &diagram_builder);

  std::unique_ptr<drake::systems::Diagram<double>> diagram = diagram_builder.Build();

  // Set the initial conditions, x(0).
  std::unique_ptr<drake::systems::Context<double>> context = diagram->CreateDefaultContext();
  context->SetContinuousState(drake::Vector1d{0.9});

  // Create the simulator, and simulate for 10 seconds.
  drake::systems::Simulator<double> simulator(*diagram, std::move(context));
  simulator.Initialize();
  simulator.AdvanceTo(10);

  // Read log data
  drake::systems::VectorLog<double> log = logger->FindLog(simulator.get_context());

  plt::figure();

  plt::plot(log.sample_times(), log.data(), "tab:red");

  plt::xlabel("Sample time");
  plt::ylabel("Output");
  plt::show();
}
