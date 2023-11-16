#include <lmc/littleMinionComputer.hpp>

#include <progressive/prog.hpp>

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

	std::fstream exampleFile(fmt::format("{}/examples/testing3.lmc", ROOT_DIR), std::ios::in);
	LIBRAPID_ASSERT(exampleFile.good(), "Failed to open file");

	std::string testProgram;
	std::string line;
	while (std::getline(exampleFile, line)) { testProgram += line + "\n"; }

	lmc::LittleMinionComputer computer;
	// computer.initialState().inputBuffer() = {913,632,97};
	// computer.initialState().inputBuffer() = {3, 6, 9};
	computer.initialState().inputBuffer() = {999, 999, 999};
	computer.initialState().memory()	  = lmc::assemble(testProgram, true);

	if (false) {
		fmt::print("Benchmarking...\n");
		librapid::Timer timer;
		timer.setTargetTime(2.5);
		while (timer.isRunning()) {
			computer.reset();
			computer.execute();
		}
		fmt::print("Benchmark Results: {:.3}\n", timer);
	}

	if (false) {
		auto numCol	 = fmt::fg(fmt::color::orange) | fmt::emphasis::bold;
		auto errCol	 = fmt::fg(fmt::color::red) | fmt::emphasis::bold;
		auto textCol = fmt::fg(fmt::color::dark_gray) | fmt::emphasis::bold;

		uint64_t maxCycles = 0;

		fmt::print("Testing...\n");
		computer.reset();
		for (prog::Progress a(lmc::Datum::ValueType(0)); a < 1000; ++a) {
			for (lmc::Datum::ValueType b = 0; b < 1000; ++b) {
				for (lmc::Datum::ValueType c = 0; c < 1000; ++c) {
					computer.state().instructionCounter() = 0;
					computer.cycles()					  = 0;
					computer.state().outputBuffer().clear();
					computer.state().inputBuffer() = {a.get(), b, c};
					computer.execute();

					lmc::Datum::ValueType ans = (a.get() + b + c) / 3;
					lmc::Datum ret			  = computer.state().outputBuffer().back();

					if (ret.value() != ans) {
						fmt::print(errCol, "\nError: ");
						fmt::print(textCol, "(");
						fmt::print(numCol, "{}", a.get());
						fmt::print(textCol, " + ");
						fmt::print(numCol, "{}", b);
						fmt::print(textCol, " + ");
						fmt::print(numCol, "{}", c);
						fmt::print(textCol, ") / 3 = ");
						fmt::print(numCol, "{}", ans);
						fmt::print(textCol, " != ");
						fmt::print(numCol, "{}", ret.value());
						fmt::print("\n");
					}

					if (computer.cycles() > maxCycles) {
						maxCycles = computer.cycles();
						a.log(fmt::format("Max cycles: {} ({}, {}, {})", maxCycles, a.get(), b, c));
					}
				}
			}
		}
	}

	computer.reset();

	auto colOut	   = fmt::fg(fmt::color::orange) | fmt::emphasis::bold;
	auto colPipe   = fmt::fg(fmt::color::gray) | fmt::emphasis::bold;
	auto colCycles = fmt::fg(fmt::color::aquamarine) | fmt::emphasis::italic;

	while (true) {
		lmc::Datum::ValueType a, b, c;
		fmt::print("Enter values: ");
		if (!(std::cin >> a)) break;
		if (!(std::cin >> b)) break;
		if (!(std::cin >> c)) break;

		computer.state().instructionCounter() = 0;
		computer.cycles()					  = 0;
		computer.state().outputBuffer().clear();

		computer.reset();

		computer.state().inputBuffer() = {a, b, c};
		computer.execute();

		const auto &buf = computer.state().outputBuffer();
		for (size_t i = 0; i < buf.size(); ++i) {
			fmt::print(colOut, "{}", buf[i].str());
			if (i != buf.size() - 1) fmt::print("\n");
		}
		fmt::print(colPipe, " | ");
		fmt::print(colCycles, "{} cycles\n", computer.cycles());
	}

	return 0;
}
