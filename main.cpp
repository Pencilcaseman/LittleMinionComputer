#include <lmc/littleMinionComputer.hpp>

int main() {
	fmt::print("Hello, World\n");

	int64_t iters = 1000;
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

	lmc::LittleMinionComputer computer123;
	computer123.state().inputBuffer() = {1, 5};
	computer123.state().memory()	  = {901, 324, 901, 325, 224, 812, 524, 326, 525, 324, 526, 325,
										 524, 123, 324, 225, 822, 524, 902, 123, 324, 615, 0,	1};
	// computer.state().memory() = {505, 106, 307, 902, 000, 1, 2};
	computer123.execute();

	for (const auto &val : computer123.state().outputBuffer()) { fmt::print("{}\n", val.value()); }

	std::fstream exampleFile(fmt::format("{}/examples/3numAvg.lmc", ROOT_DIR), std::ios::in);
	LIBRAPID_ASSERT(exampleFile.good(), "Failed to open file");

	std::string testProgram;
	std::string line;
	while (std::getline(exampleFile, line)) { testProgram += line + "\n"; }

	lmc::LittleMinionComputer computer;
	// computer.initialState().inputBuffer() = {913,632,97};
	// computer.initialState().inputBuffer() = {3, 6, 9};
	computer.initialState().inputBuffer() = {999, 999, 999};
	computer.initialState().memory()	  = lmc::assemble(testProgram, true);

	if (true) {
		fmt::print("Benchmarking...\n");
		librapid::Timer timer;
		timer.setTargetTime(2.5);
		while (timer.isRunning()) {
			computer.reset();
			computer.execute();
		}
		fmt::print("Benchmark Results: {:.3}\n", timer);
	}

	computer.reset();

	while (true) {
		lmc::Datum::ValueType a, b, c;
		fmt::print("Enter values: ");
		if (!(std::cin >> a)) break;
		if (!(std::cin >> b)) break;
		if (!(std::cin >> c)) break;
		computer.state().instructionCounter() = 0;
		computer.cycles()					  = 0;
		computer.state().inputBuffer()		  = {a, b, c};
		computer.execute();

		auto colOut	   = fmt::fg(fmt::color::orange) | fmt::emphasis::bold;
		auto colPipe   = fmt::fg(fmt::color::gray) | fmt::emphasis::bold;
		auto colCycles = fmt::fg(fmt::color::aquamarine) | fmt::emphasis::italic;

		fmt::print(colOut, "{}", computer.state().outputBuffer().back().str());
		fmt::print(colPipe, " | ");
		fmt::print(colCycles, "{} cycles\n", computer.cycles());
	}

	return 0;
}
