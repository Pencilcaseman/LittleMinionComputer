#pragma once

#include <librapid>

namespace lmc {
	/// An instruction or a piece of data, limited to 3 digits (000-999).
	class Datum {
	public:
		// Use int32 since int16 isn't standard and is sometimes weird
		using ValueType = int32_t;
		static constexpr uint64_t numBits = sizeof(ValueType) * 8;

		Datum()							= default;
		Datum(const Datum &)			= default;
		Datum(Datum &&)					= default;
		Datum &operator=(const Datum &) = default;
		Datum &operator=(Datum &&)		= default;

		Datum(ValueType val);
		Datum &operator=(ValueType val);

		LIBRAPID_NODISCARD ValueType value() const;
		LIBRAPID_NODISCARD bool underflow() const;
		LIBRAPID_NODISCARD bool overflow() const;

		LIBRAPID_NODISCARD Datum operator+(const Datum &other) const;
		LIBRAPID_NODISCARD Datum operator+(ValueType other) const;

		LIBRAPID_NODISCARD Datum operator-(const Datum &other) const;
		LIBRAPID_NODISCARD Datum operator-(ValueType other) const;

		LIBRAPID_NODISCARD std::string str() const;
		LIBRAPID_NODISCARD std::string repr() const;

	protected:
		void setUnderflow(bool);
		void setOverflow(bool);

	private:
		// 999 requires 10 bits, so we can use the
		// remaining bits as flags
		// - 1st MSB: underflow
		// - 2nd MSB: overflow
		ValueType m_value = 0;
		static constexpr ValueType flagMask = 0x3FF;
	};
} // namespace lmc
