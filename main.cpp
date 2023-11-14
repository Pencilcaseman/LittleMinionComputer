#include <surge/surge.hpp>
 
#include <lmc/littleMinionComputer.hpp>

int main() {
	fmt::print("Hello, World\n");

	int64_t iters = 100000;
	double start  = librapid::now();
	for (int64_t i = 0; i < iters; ++i) {
		lmc::LittleMinionComputer computer;
		computer.state().inputBuffer() = {0, 999};
		computer.state().memory() = {901, 324, 901, 325, 224, 812, 524, 326, 525, 324, 526, 325,
									 524, 123, 324, 225, 822, 524, 902, 123, 324, 615, 0,	1};
		computer.execute();
	}
	double end = librapid::now();
	fmt::print("Time: {:.3f}\n", librapid::formatTime((end - start) / (double)iters));

	lmc::LittleMinionComputer computer;
	computer.state().inputBuffer() = {1, 5};
	computer.state().memory()	   = {901, 324, 901, 325, 224, 812, 524, 326, 525, 324, 526, 325,
									  524, 123, 324, 225, 822, 524, 902, 123, 324, 615, 0,	 1};
	// computer.state().memory() = {505, 106, 307, 902, 000, 1, 2};
	computer.execute();

	for (const auto &val : computer.state().outputBuffer()) { fmt::print("{}\n", val.value()); }

	return 0;
}
