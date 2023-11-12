#pragma once

#include <librapid>

namespace lmc {
	/// An instruction or a piece of data, limited to 3 digits (000-999).
	class Datum {
	public:
		// Use int32 since int16 isn't standard and is sometimes weird
		using ValueType					  = int32_t;
		static constexpr uint64_t numBits = sizeof(ValueType) * 8;

		LIBRAPID_ALWAYS_INLINE Datum()						   = default;
		LIBRAPID_ALWAYS_INLINE Datum(const Datum &)			   = default;
		LIBRAPID_ALWAYS_INLINE Datum(Datum &&)				   = default;
		LIBRAPID_ALWAYS_INLINE Datum &operator=(const Datum &) = default;
		LIBRAPID_ALWAYS_INLINE Datum &operator=(Datum &&)	   = default;

		LIBRAPID_ALWAYS_INLINE Datum(ValueType val); // Defined later

		LIBRAPID_ALWAYS_INLINE Datum &operator=(ValueType val); // Defined later

		// LIBRAPID_NODISCARD ValueType value() const;
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE ValueType value() const {
			return m_value & flagMask;
		}

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE bool underflow() const {
			return m_value & (1 << (numBits - 1));
		}
		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE bool overflow() const {
			return m_value & (1 << (numBits - 2));
		}

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Datum operator+(const Datum &other) const {
			return {value() + other.value()};
		}

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Datum operator+(ValueType other) const {
			return {value() + other};
		}

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Datum operator-(const Datum &other) const {
			return {value() - other.value()};
		}

		LIBRAPID_NODISCARD LIBRAPID_ALWAYS_INLINE Datum operator-(ValueType other) const {
			return {value() - other};
		}

		LIBRAPID_NODISCARD std::string str() const { return fmt::format("{:0>3}", value()); }

		LIBRAPID_NODISCARD std::string repr() const {
			return fmt::format("Datum(uf: {}  of: {} | {:0>3})",
							   underflow() ? 'Y' : 'N',
							   overflow() ? 'Y' : 'N',
							   value());
		}

	protected:
		LIBRAPID_ALWAYS_INLINE void setUnderflow(bool val) {
			if (val)
				m_value |= (1 << (numBits - 1));
			else
				m_value &= ~(1 << (numBits - 1));
		}

		LIBRAPID_ALWAYS_INLINE void setOverflow(bool val) {
			if (val)
				m_value |= (1 << (numBits - 2));
			else
				m_value &= ~(1 << (numBits - 2));
		}

	private:
		// 999 requires 10 bits, so we can use the
		// remaining bits as flags
		// - 1st MSB: underflow
		// - 2nd MSB: overflow
		ValueType m_value					= 0;
		static constexpr ValueType flagMask = 0x3FF;
	};

	namespace detail {
		LIBRAPID_ALWAYS_INLINE Datum::ValueType clamp3(Datum::ValueType val, bool &underflow,
													   bool &overflow);
	} // namespace detail

	LIBRAPID_ALWAYS_INLINE Datum::Datum(ValueType val) {
		bool underflow, overflow;
		m_value = detail::clamp3(val, underflow, overflow);
		setUnderflow(underflow);
		setOverflow(overflow);
	}

	LIBRAPID_ALWAYS_INLINE Datum &Datum::operator=(ValueType val) {
		bool underflow, overflow;
		m_value = detail::clamp3(val, underflow, overflow);
		setUnderflow(underflow);
		setOverflow(overflow);
		return *this;
	}

	namespace detail {
		LIBRAPID_ALWAYS_INLINE Datum::ValueType clamp3(Datum::ValueType val, bool &underflow,
													   bool &overflow) {
			underflow = val < 0;
			overflow  = val > 999;

			// MOD does not work for negative numbers, so
			// we have to do it manually... :(
			// (hopefully the compiler optimizes this)

			while (val < 0) val += 1000;

			while (val > 999) val -= 1000;

			return val;
		}
	} // namespace detail
} // namespace lmc
