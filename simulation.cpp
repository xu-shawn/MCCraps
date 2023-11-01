#include <boost/program_options.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

namespace po = boost::program_options;

double simulate(int value);
bool simulate_once();

int main(int argc, char **argv)
{
    po::options_description description("Usage");
    description.add_options()("help", "prints this message")(
        "size", po::value<int>()->default_value(1000),
        "the size of the simulation");

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
        int value = vm["size"].as<int>();
        std::cout << "Doing " << value << " simulations\n";
        std::cout << simulate(value);
    }
    return 0;
}

double simulate(int size)
{
    int64_t total = 0;
    for (int i = 0; i < size; i++)
    {
        if (simulate_once())
        {
            total++;
        }
        else
        {
            total--;
        }
    }
    return static_cast<double>(total) / size;
}

bool simulate_once()
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, 6);

    int initial_roll = distribution(generator) + distribution(generator);
    int second_roll;

    switch (initial_roll)
    {
    case 7:
    case 11:
        return true;
    case 2:
    case 3:
    case 12:
        return false;
    }

    while (true)
    {
        second_roll = distribution(generator) + distribution(generator);

        if (second_roll == 7)
        {
            return false;
        }

        else if (second_roll == initial_roll)
        {
            return true;
        }
    }
}
