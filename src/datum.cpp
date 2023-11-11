#include <lmc/datum.hpp>

namespace lmc {
	namespace detail {
		Datum::ValueType clamp3(Datum::ValueType val, bool &underflow, bool &overflow) {
			underflow = val < 0;
			overflow  = val > 999;

			// MOD 1000 does not work for negative numbers, so
			// we have to do it manually... :(
			// (hopefully the compiler optimizes this)

			while (val < 0)
				val += 1000;

			while (val > 999)
				val -= 1000;

			return val;
		}
	} // namespace detail

	Datum::Datum(ValueType val) {
		bool underflow, overflow;
		m_value = detail::clamp3(val, underflow, overflow);
		setUnderflow(underflow);
		setOverflow(overflow);
	}

	Datum &Datum::operator=(ValueType val) {
		bool underflow, overflow;
		m_value = detail::clamp3(val, underflow, overflow);
		setUnderflow(underflow);
		setOverflow(overflow);
		return *this;
	}

	Datum::ValueType Datum::value() const { return m_value & flagMask; }

	bool Datum::underflow() const { return m_value & (1 << (numBits - 1)); }

	bool Datum::overflow() const { return m_value & (1 << (numBits - 2)); }

	std::string Datum::str() const { return fmt::format("{:0>3}", value()); }
	std::string Datum::repr() const {
		return fmt::format("Datum(uf: {}  of: {} | {:0>3})",
						   underflow() ? 'Y' : 'N',
						   overflow() ? 'Y' : 'N',
						   value());
	}

	Datum Datum::operator+(const Datum &other) const { return Datum(value() + other.value()); }
	Datum Datum::operator+(const ValueType other) const { return Datum(value() + other); }

	Datum Datum::operator-(const Datum &other) const { return Datum(value() - other.value()); }
	Datum Datum::operator-(const ValueType other) const { return Datum(value() - other); }

	void Datum::setUnderflow(bool val) {
		if (val)
			m_value |= (1 << (numBits - 1));
		else
			m_value &= ~(1 << (numBits - 1));
	}

	void Datum::setOverflow(bool val) {
		if (val)
			m_value |= (1 << (numBits - 2));
		else
			m_value &= ~(1 << (numBits - 2));
	}
} // namespace lmc
