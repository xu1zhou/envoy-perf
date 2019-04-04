#pragma once

#include "common/common/logger.h"

#include "envoy/network/address.h"
#include "envoy/stats/store.h"

#include "nighthawk/client/client_worker.h"
#include "nighthawk/client/factories.h"
#include "nighthawk/client/options.h"
#include "nighthawk/common/statistic.h"

namespace Nighthawk {
namespace Client {

class ProcessContext;

class Main : public Envoy::Logger::Loggable<Envoy::Logger::Id::main> {
public:
  Main(int argc, const char* const* argv);
  Main(Client::OptionsPtr&& options);
  ~Main();
  bool run();

private:
  uint32_t determineConcurrency() const;
  void configureComponentLogLevels(spdlog::level::level_enum level);
  bool runWorkers(ProcessContext& context, std::vector<StatisticPtr>& merged_statistics,
                  std::map<std::string, uint64_t>& merged_counters) const;
  std::vector<StatisticPtr>
  mergeWorkerStatistics(const StatisticFactory& statistic_factory,
                        const std::vector<ClientWorkerPtr>& workers) const;

  std::map<std::string, uint64_t>
  mergeWorkerCounters(const std::vector<ClientWorkerPtr>& workers) const;

  void writeOutput(ProcessContext& context, const std::vector<StatisticPtr>& merged_statistics,
                   const std::map<std::string, uint64_t>& merged_counters) const;

  OptionsPtr options_;
  std::unique_ptr<Envoy::Logger::Context> logging_context_;
};

} // namespace Client
} // namespace Nighthawk
