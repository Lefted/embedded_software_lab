#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

std::vector<bool> generateChipSeq(std::vector<bool> &genA, std::vector<bool> &genB, const std::vector<unsigned int> &cons)
{
	std::vector<bool> chipSeq;
	for (auto i = 0; i < 1023; i++)
	{
		const auto nextB = genB.at(1) ^ genB.at(2) ^ genB.at(5) ^ genB.at(7) ^ genB.at(8) ^ genB.at(9);
		const auto rotated = genB.at(cons.at(0) - 1) ^ genB.at(cons.at(1) - 1);
		const auto res = genA.at(9) ^ rotated;
		const auto nextA = genA.at(2) ^ genA.at(9);

		// rotate right
		std::rotate(genA.rbegin(), genA.rbegin() + 1, genA.rend());
		std::rotate(genB.rbegin(), genB.rbegin() + 1, genB.rend());
		// overwrite
		genA.at(0) = nextA;
		genB.at(0) = nextB;

		chipSeq.push_back(res);
		// printf("%d", res);
	}
	// printf("\n");
	return chipSeq;
}

int calcCrossCorr(const std::vector<int> &sumSignals, const std::vector<bool> &chipSeq, const unsigned int offset)
{
	int sum = 0;
	for (auto i = 0; i < 1023; i++)
	{
		const auto chip = chipSeq.at(i);
		const auto signal = sumSignals.at(i + offset);
		sum += (chip ? 1 : -1) * signal;
	}
	return sum;
}

int test_latency()
{
	PROFILE_START("total");
	std::vector<int> sumSignals = {-2, 2, 2, 0, -2, -4, 2, -2, 0, 2, 2, 0, 2, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0, 4, -2, -4, -2, 0, 2, 4, 2, 2, 0, -4, 0, -2, 2, 0, -2, 2, -4, -2, -2, 0, -4, 2, 0, 4, 0, 0, -2, 0, 2, 2, 0, -2, -2, -2, 4, -2, 0, -2, 0, 0, 0, 0, 2, 0, 0, -2, -2, 2, -2, 0, -2, 2, 0, 2, 0, -2, 0, 2, 4, -2, 0, -2, 0, 4, -2, 0, 2, 0, 4, 2, 2, 2, 0, -4, 2, -2, 2, -4, -2, 0, -2, -2, 2, 0, 4, 0, 2, 0, 0, -2, 0, -2, -2, 0, -2, 0, 2, -2, 0, -2, 2, 0, 0, -2, 2, 0, 0, 0, 2, 2, -2, 2, 2, 0, 2, 2, 0, -2, 2, -2, 0, -2, 2, 0, 0, 2, -2, 4, 0, 2, 2, -2, 0, 0, 2, 0, -2, 0, -2, 0, 4, 2, 0, -2, -2, -2, -2, 0, 0, 4, -4, -2, 0, -2, 2, 2, -2, -4, -2, 0, -4, 2, -2, 4, -4, -2, 0, 0, 0, 0, 0, 2, 4, 0, 4, 0, 4, 0, 0, 0, 4, 2, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, -4, -4, -2, 0, 0, -2, 0, -4, -2, 2, 2, -4, 0, 2, 0, -2, 0, 0, 0, 2, -2, 2, 0, -2, 4, 0, 0, 2, 2, 2, -2, 2, 0, 0, 2, 0, 0, -2, 0, 0, 2, -2, 2, 2, 0, -4, 0, -2, -4, 2, -4, -4, 2, -2, 0, -2, 0, -2, 0, 0, 4, -4, 2, 4, 0, -2, 0, 0, -2, 4, 0, 2, 0, -2, -2, 2, -2, 0, 2, -2, -2, -2, 0, 0, 2, -2, -2, -2, -2, 0, -2, 2, -2, 0, 0, -4, 0, -2, 2, 0, -2, -2, -2, 0, -2, 2, 2, 2, 0, 0, -2, 0, -2, 2, 2, 2, 2, 0, 2, 2, -2, -2, 2, -2, 2, 4, 2, 0, 2, 2, 0, -2, 0, 0, 2, 0, 0, -2, 4, 0, 2, -2, 0, -2, 2, 0, 4, -2, 2, 2, 2, 0, 2, 2, 4, -2, -2, 2, -2, -2, -2, 0, 0, 0, -2, -4, 0, 2, 0, 0, -2, 0, 0, -2, -2, 0, -2, 2, 2, 0, 2, -2, 0, 0, -2, 0, 2, 0, 0, -2, 0, 0, -2, 2, 0, 0, 0, 0, 2, 0, 4, 4, 0, 2, -2, -2, 2, 0, 0, 4, 0, 0, -2, 0, 0, 2, -4, 0, 2, 2, 4, -2, -2, -2, 2, -2, 2, 2, -2, 4, -2, 0, -2, 0, 2, 2, 0, -4, -2, 2, 0, 0, -2, 2, 4, -2, 0, 2, 0, 2, 0, 4, -2, 0, 0, 2, 0, -2, 0, -2, 0, 0, -2, 2, 2, -2, -2, -2, -2, -2, 2, -2, 0, 0, 0, -4, 0, 0, 0, -4, 2, 0, 0, 0, 0, -2, 0, 2, 0, 0, 2, 0, -4, 2, 0, 2, 0, -2, 0, 0, 0, -2, 2, 0, -2, -2, 2, 2, -2, -2, 0, 0, 0, 0, 2, 0, 0, 0, -4, 2, 4, 0, 4, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2, -2, 2, -4, -2, 0, 0, -2, 0, -2, 0, 0, 2, -2, 0, 2, -2, -4, 2, -2, 0, -2, 0, -4, 0, 4, 4, 0, 0, 0, 0, -2, 0, -2, -2, 0, 0, 2, 2, 2, 0, -2, 2, 0, -2, 0, 2, 2, 0, 0, 0, 2, -2, 2, -4, -2, -2, 2, 0, 0, 2, 0, 0, -4, 0, 0, -2, 2, 0, -2, 0, 2, 4, 4, 0, 0, 0, -2, 0, 2, 4, 2, -2, 0, -2, 0, 0, 2, 4, -2, 2, 0, 0, 0, 2, -2, 0, 2, 0, 4, -2, 2, -2, 0, 4, -2, 0, 0, 0, -2, 0, 2, 0, 0, -2, -4, -2, 2, -2, 2, -2, -2, 2, -2, 0, 0, 0, -2, 0, -2, 0, 0, 4, -2, 2, 2, 2, 0, -2, 2, 0, 0, 2, 0, 0, 2, 2, 0, 0, -2, 4, 0, -2, -2, -2, 4, -4, -2, 0, -2, 2, 0, 2, 4, -2, -4, 0, 2, 0, 0, 0, 2, -2, -2, -4, 2, 2, 0, 0, -2, -2, -4, 0, 4, 0, 0, 0, 0, 0, 0, -2, 2, -4, -2, 0, -2, -2, -4, 2, 2, 2, 0, 0, 0, -2, -2, 0, 0, 0, 2, 0, 2, -2, -4, 2, 0, 0, 0, 2, 0, 2, 2, 0, -2, 0, 2, 4, 2, 0, 2, 0, -2, -4, -2, 0, 0, 0, -2, -4, 2, -2, -2, 0, 4, -2, -2, -2, -2, 4, 0, -2, 0, 2, 2, 4, 0, 2, 2, 0, 0, 0, -2, -2, -2, 0, -2, 0, 0, 0, 2, -4, -4, 0, 4, 2, 0, 0, -2, -2, 0, -4, 0, 0, 2, 2, 2, 0, 0, 2, 2, -2, -4, 2, 0, 2, 2, -2, 2, -2, 4, 2, 0, -2, 0, 2, 2, 2, 2, -2, -2, 0, 2, -2, -2, -2, -4, 0, 2, -2, -2, -4, 0, -4, 2, 0, -2, 0, 0, -2, 0, 0, 0, -2, 2, 0, -2, 0, 2, -2, -4, -2, -2, -2, 0, 0, -2, 4, -2, 4, 0, -2, 0, -2, -2, 2, 2, 2, -2, -2, -4, 4, 0, -2, -2, 0, -2, 0, 0, 0, 2, 2, 2, 2, -2, 2, 2, -4, -2, -4, 0, 4, -2, 2, 2, 2, -2, 0, 4, -2, 4, 2, -2, 0, 0, 2, -4, 2, 0, 2, -2, 0, 2, 2, -2, 0, 2, 0, -4, 2, 0, 2, 0, 0, -4, 0, 2, 0, -4, 0, 0, -2, -2, 2, -4, 2, 0, 2, 0, -4, 2, 0, 0, 0, 4, -2, 0, 0, -2, 0, -4, 2, 0, 4, 2, 0, 4, 2, -2, -2, 0, 0, -2, 0, 0, -2, 0, 2, 0, 2, 2, 4, 2, -4, 0, 4, 2, -2, 0, 4, 2, -4, -2, 2, 2, 0, -2, -4, 2, -2, 0, 2, 2, 0, 2, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0, 4, -2, -4, -2, 0, 2, 4, 2, 2, 0, -4, 0, -2, 2, 0, -2, 2, -4, -2, -2, 0, -4, 2, 0, 4, 0, 0, -2, 0, 2, 2, 0, -2, -2, -2, 4, -2, 0, -2, 0, 0, 0, 0, 2, 0, 0, -2, -2, 2, -2, 0, -2, 2, 0, 2, 0, -2, 0, 2, 4, -2, 0, -2, 0, 4, -2, 0, 2, 0, 4, 2, 2, 2, 0, -4, 2, -2, 2, -4, -2, 0, -2, -2, 2, 0, 4, 0, 2, 0, 0, -2, 0, -2, -2, 0, -2, 0, 2, -2, 0, -2, 2, 0, 0, -2, 2, 0, 0, 0, 2, 2, -2, 2, 2, 0, 2, 2, 0, -2, 2, -2, 0, -2, 2, 0, 0, 2, -2, 4, 0, 2, 2, -2, 0, 0, 2, 0, -2, 0, -2, 0, 4, 2, 0, -2, -2, -2, -2, 0, 0, 4, -4, -2, 0, -2, 2, 2, -2, -4, -2, 0, -4, 2, -2, 4, -4, -2, 0, 0, 0, 0, 0, 2, 4, 0, 4, 0, 4, 0, 0, 0, 4, 2, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, -4, -4, -2, 0, 0, -2, 0, -4, -2, 2, 2, -4, 0, 2, 0, -2, 0, 0, 0, 2, -2, 2, 0, -2, 4, 0, 0, 2, 2, 2, -2, 2, 0, 0, 2, 0, 0, -2, 0, 0, 2, -2, 2, 2, 0, -4, 0, -2, -4, 2, -4, -4, 2, -2, 0, -2, 0, -2, 0, 0, 4, -4, 2, 4, 0, -2, 0, 0, -2, 4, 0, 2, 0, -2, -2, 2, -2, 0, 2, -2, -2, -2, 0, 0, 2, -2, -2, -2, -2, 0, -2, 2, -2, 0, 0, -4, 0, -2, 2, 0, -2, -2, -2, 0, -2, 2, 2, 2, 0, 0, -2, 0, -2, 2, 2, 2, 2, 0, 2, 2, -2, -2, 2, -2, 2, 4, 2, 0, 2, 2, 0, -2, 0, 0, 2, 0, 0, -2, 4, 0, 2, -2, 0, -2, 2, 0, 4, -2, 2, 2, 2, 0, 2, 2, 4, -2, -2, 2, -2, -2, -2, 0, 0, 0, -2, -4, 0, 2, 0, 0, -2, 0, 0, -2, -2, 0, -2, 2, 2, 0, 2, -2, 0, 0, -2, 0, 2, 0, 0, -2, 0, 0, -2, 2, 0, 0, 0, 0, 2, 0, 4, 4, 0, 2, -2, -2, 2, 0, 0, 4, 0, 0, -2, 0, 0, 2, -4, 0, 2, 2, 4, -2, -2, -2, 2, -2, 2, 2, -2, 4, -2, 0, -2, 0, 2, 2, 0, -4, -2, 2, 0, 0, -2, 2, 4, -2, 0, 2, 0, 2, 0, 4, -2, 0, 0, 2, 0, -2, 0, -2, 0, 0, -2, 2, 2, -2, -2, -2, -2, -2, 2, -2, 0, 0, 0, -4, 0, 0, 0, -4, 2, 0, 0, 0, 0, -2, 0, 2, 0, 0, 2, 0, -4, 2, 0, 2, 0, -2, 0, 0, 0, -2, 2, 0, -2, -2, 2, 2, -2, -2, 0, 0, 0, 0, 2, 0, 0, 0, -4, 2, 4, 0, 4, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2, -2, 2, -4, -2, 0, 0, -2, 0, -2, 0, 0, 2, -2, 0, 2, -2, -4, 2, -2, 0, -2, 0, -4, 0, 4, 4, 0, 0, 0, 0, -2, 0, -2, -2, 0, 0, 2, 2, 2, 0, -2, 2, 0, -2, 0, 2, 2, 0, 0, 0, 2, -2, 2, -4, -2, -2, 2, 0, 0, 2, 0, 0, -4, 0, 0, -2, 2, 0, -2, 0, 2, 4, 4, 0, 0, 0, -2, 0, 2, 4, 2, -2, 0, -2, 0, 0, 2, 4, -2, 2, 0, 0, 0, 2, -2, 0, 2, 0, 4, -2, 2, -2, 0, 4, -2, 0, 0, 0, -2, 0, 2, 0, 0, -2, -4, -2, 2, -2, 2, -2, -2, 2, -2, 0, 0, 0, -2, 0, -2, 0, 0, 4, -2, 2, 2, 2, 0, -2, 2, 0, 0, 2, 0, 0, 2, 2, 0, 0, -2, 4, 0, -2, -2, -2, 4, -4, -2, 0, -2, 2, 0, 2, 4, -2, -4, 0, 2, 0, 0, 0, 2, -2, -2, -4, 2, 2, 0, 0, -2, -2, -4, 0, 4, 0, 0, 0, 0, 0, 0, -2, 2, -4, -2, 0, -2, -2, -4, 2, 2, 2, 0, 0, 0, -2, -2, 0, 0, 0, 2, 0, 2, -2, -4, 2, 0, 0, 0, 2, 0, 2, 2, 0, -2, 0, 2, 4, 2, 0, 2, 0, -2, -4, -2, 0, 0, 0, -2, -4, 2, -2, -2, 0, 4, -2, -2, -2, -2, 4, 0, -2, 0, 2, 2, 4, 0, 2, 2, 0, 0, 0, -2, -2, -2, 0, -2, 0, 0, 0, 2, -4, -4, 0, 4, 2, 0, 0, -2, -2, 0, -4, 0, 0, 2, 2, 2, 0, 0, 2, 2, -2, -4, 2, 0, 2, 2, -2, 2, -2, 4, 2, 0, -2, 0, 2, 2, 2, 2, -2, -2, 0, 2, -2, -2, -2, -4, 0, 2, -2, -2, -4, 0, -4, 2, 0, -2, 0, 0, -2, 0, 0, 0, -2, 2, 0, -2, 0, 2, -2, -4, -2, -2, -2, 0, 0, -2, 4, -2, 4, 0, -2, 0, -2, -2, 2, 2, 2, -2, -2, -4, 4, 0, -2, -2, 0, -2, 0, 0, 0, 2, 2, 2, 2, -2, 2, 2, -4, -2, -4, 0, 4, -2, 2, 2, 2, -2, 0, 4, -2, 4, 2, -2, 0, 0, 2, -4, 2, 0, 2, -2, 0, 2, 2, -2, 0, 2, 0, -4, 2, 0, 2, 0, 0, -4, 0, 2, 0, -4, 0, 0, -2, -2, 2, -4, 2, 0, 2, 0, -4, 2, 0, 0, 0, 4, -2, 0, 0, -2, 0, -4, 2, 0, 4, 2, 0, 4, 2, -2, -2, 0, 0, -2, 0, 0, -2, 0, 2, 0, 2, 2, 4, 2, -4, 0, 4, 2, -2, 0, 4, 2, -4};
	for (int i = 0; i < 1023; i++)
	{
		if (sumSignals.at(i) != sumSignals.at(i + 1023))
		{
			std::cerr << "Error: Signal is not repeated" << std::endl;
			return 1;
		}
	}

	// Gold Sequence
	PROFILE_START("gold");
	std::vector<std::vector<bool>> goldCodes;
	std::vector<std::vector<unsigned int>> cons = {{2, 6}, {3, 7}, {4, 8}, {5, 9}, {1, 9}, {2, 10}, {1, 8}, {2, 9}, {3, 10}, {2, 3}, {3, 4}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9}, {1, 3}, {4, 6}};
	for (auto j = 0; j < 24; j++)
	{
		std::vector<bool> genA(10, true);
		std::vector<bool> genB(10, true);

		std::vector<bool> chipSeq = generateChipSeq(genA, genB, cons[j]);
		goldCodes.push_back(chipSeq);
	}
	PROFILE_STOP("gold");

	PROFILE_START("sats");
	for (auto satellite = 0; satellite < 24; satellite++)
	{
		std::vector<bool> chipSeq = goldCodes[satellite];
		for (auto offset = 0; offset < 1023; offset++)
		{
			const auto crossCorr = calcCrossCorr(sumSignals, chipSeq, offset);
			if (crossCorr > 828 || crossCorr < -828)
			{
				// printf("%d\n", crossCorr);
				printf("Satellite %d has sent bit %d (delta = %d)\n", satellite + 1, crossCorr > 828 ? 1 : 0, offset + 1);
			}
		}
	}
	PROFILE_STOP("sats");
	PROFILE_STOP("total");
	return 0;
}

int main()
{
	const size_t warmups = 1000;
	const size_t tests = 100;

	PROFILE_RUN_ALL(warmups, tests,
					test_latency(););

	return 0;
}
