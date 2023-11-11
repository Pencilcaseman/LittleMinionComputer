#include <surge/surge.hpp>

#include <lmc/littleMinionComputer.hpp>

int main() {
	fmt::print("Hello, World\n");

	lmc::LittleMinionComputer computer;
	computer.state().memory() = {
	  901,
	  324,
	  901,
	  325,
	  224,
	  812,
	  524,
	  326,
	  525,
	  324,
	  526,
	  325,
	  524,
	  123,
	  324,
	  225,
	  822,
	  524,
	  902,
	  123,
	  324,
	  615,
	  0,
	  1
	};
	computer.execute();

	for (const auto &val : computer.state().outputBuffer()) { fmt::print("{}\n", val.value()); }

	return 0;
}
