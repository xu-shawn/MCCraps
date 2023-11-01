#include <boost/program_options.hpp>

#include <cstdint>
#include <future>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <thread>

namespace po = boost::program_options;

int64_t simulate(int64_t value);
bool simulate_once(std::mt19937 &generator,
                   std::discrete_distribution<> &twodice);

int main(int argc, char **argv)
{
    po::options_description description("Usage");
    description.add_options()("help", "prints this message")(
        "size", po::value<int64_t>()->default_value(1000),
        "the size of the simulation")("concurrency",
                                      po::value<int32_t>()->default_value(1),
                                      "number of core(s) to use");

    po::positional_options_description pdescription;
    pdescription.add("size", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(description)
                  .positional(pdescription)
                  .run(),
              vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << description << "\n";
        return 0;
    }

    if (vm.count("size"))
    {
        int64_t value = vm["size"].as<int64_t>();
        int32_t thread_num = vm["concurrency"].as<int32_t>();
        std::cout << "Running " << value << " simulations...\n";
        std::vector<std::future<int64_t>> future_results;
        future_results.reserve(thread_num);
        for (int i = 0; i < thread_num - 1; i++)
        {
            future_results.push_back(std::async(simulate, value / thread_num));
        }
        future_results.push_back(
            std::async(simulate, value / thread_num + value % thread_num));

        std::cout << std::fixed << std::setprecision(15)
                  << static_cast<long double>(std::accumulate(
                         future_results.begin(), future_results.end(), 0,
                         [](int64_t partial_sum, std::future<int64_t> &x)
                         { return partial_sum + x.get(); })) /
                         value
                  << std::endl;
    }

    return 0;
}

int64_t simulate(int64_t size)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::discrete_distribution<int> twodice({1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1});

    int64_t total = 0;
    for (int64_t i = 0; i < size; i++)
    {
        if (simulate_once(generator, twodice))
        {
            total++;
        }
        else
        {
            total--;
        }
    }
    return total;
}

bool simulate_once(std::mt19937 &generator,
                   std::discrete_distribution<> &twodice)
{
    int initial_roll = twodice(generator);
    int second_roll;

    switch (initial_roll)
    {
        case 5:
        case 9:
            return true;
        case 0:
        case 1:
        case 10:
            return false;
    }

    while (true)
    {
        second_roll = twodice(generator);

        if (second_roll == 5)
        {
            return false;
        }

        else if (second_roll == initial_roll)
        {
            return true;
        }
    }
}
