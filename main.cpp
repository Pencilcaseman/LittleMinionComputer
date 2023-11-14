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

	std::string testProgram = R"R0V0G0(
		# Code to compute a divided by b
				IN
				STO	    a
				IN
				STO	    b
		# Keep subtracting a from b until you go negative
		# Keep a count of how many times you do it
		start	LDA	    count
				ADD	    one
				STO	    count
				LDA	    a
				SUB	    b
				STO	    a
				BRP	    start
		done	LDA	    count
		# Subtract one as we went one too far
				SUB	    one
				OUT
				HLT
		a	    DAT	    000
		b	    DAT	    000
		count	DAT	    000
		one	    DAT	    001
	)R0V0G0";

	lmc::LittleMinionComputer computer;
	computer.initialState().memory() = lmc::assemble(testProgram);
	computer.reset();
	computer.execute();
	for (const auto &out : computer.state().outputBuffer()) {
		fmt::print("{}\n", out.str());
	}

	return 0;
}
