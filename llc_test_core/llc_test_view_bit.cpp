#include "llc_test_core.h"
#include "llc_view_bit.h"
#include "llc_noise.h"

static_assert(3 == ::llc::bit_offset_field_size<::llc::u0_t>(), "u8 offset width");
static_assert(4 == ::llc::bit_offset_field_size<::llc::u1_t>(), "u16 offset width");
static_assert(5 == ::llc::bit_offset_field_size<::llc::u2_t>(), "u32 offset width");
static_assert(6 == ::llc::bit_offset_field_size<::llc::u3_t>(), "u64 offset width");

GDEFINE_ENUM_TYPE(BIT_VIEW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, OK						,  0, "All bit-view tests passed.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PARTIAL_ITERATOR_POSITION	,  1, "A 10-bit iterator reported the wrong zero-based bit index or logical limit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PARTIAL_END_POSITION		,  2, "A 10-bit view produced the wrong logical end or one-past-storage position.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PARTIAL_DECREMENT			,  3, "Decrementing the end of a 10-bit view did not select bit index 9.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, SIX_BIT_END_POSITION		,  4, "A Base64-style 6-bit view did not stop at bit offset 6 of its first element.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, EMPTY_DEFAULT_POSITION	,  5, "A default empty view did not produce equal iterators with null position pointers.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, EMPTY_DATA_POSITION		,  6, "A zero-length view with backing data did not normalize to equal null iterators.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, ITERATOR_EQUALITY			,  7, "Iterators at different bit positions compared equal because equality was not positional.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PROXY_WRITE				,  8, "Assigning through a mutable bit proxy did not update the selected backing bit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, CONST_ITERATION			,  9, "Const iteration did not visit exactly the logical bit count.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, WIDTH_ITERATOR_POSITION	, 10, "An iterator using a derived offset-field width reported the wrong index or limit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, WIDTH_END_POSITION		, 11, "A partial view using a derived offset-field width produced the wrong end position.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, WIDTH_DECREMENT			, 12, "Decrementing a partial end failed for one of the supported backing integer widths.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, RANDOM_ITERATOR_POSITION	, 13, "A randomized iterator reported the wrong position or logical limit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, RANDOM_ITERATOR_VALUE	, 14, "A randomized iterator read a bit different from its backing integer.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, RANDOM_END_POSITION		, 15, "A randomized view produced the wrong logical or storage end.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, RANDOM_DECREMENT			, 16, "Decrementing a randomized end produced the wrong final bit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, RANDOM_PROXY_WRITE		, 17, "A randomized mutable bit proxy did not update its selected bit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, REVERSE_RESULT			, 18, "reverse_bits() reported an unexpected result.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, REVERSE_VALUE			, 19, "reverse_bits() produced the wrong logical bit order.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, REVERSE_OUTSIDE			, 20, "reverse_bits() modified bits outside the logical view.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, INVALID_CONSTRUCTION		, 21, "view_bit accepted invalid backing storage or an excessive bit count.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, INVALID_SUBSCRIPT		, 22, "view_bit accepted an out-of-range mutable or const subscript.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, INVALID_ITERATOR			, 23, "A bit iterator accepted an invalid boundary operation.");

stxp ::llc::u3_t BIT_RANDOM_SEED	= 0x4249545649455753ULL;
stxp ::llc::u2_t BIT_RANDOM_COUNT	= 128;

// Pointer arithmetic in this suite verifies the element and one-past storage boundaries exposed by bit_iterator<>.

tplt<tpnm T>
sttc ::llc::err_t testPartialWidth(ATestError & errors) {
	T					data[2]		= {};
	::llc::u2_c			bitCount	= szof(T) * 8 + 3;
	::llc::view_bit<T>	bits		{data, bitCount};
	auto				it			= bits.begin();
	auto				end			= bits.end();
	{
		::llc::u2_t count = 0;
		for(; count < bitCount && it != end; ++it, ++count) {
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_ITERATOR_POSITION, it.Index() != count , "%u-bit iterator index:%u, expected:%u.", bcof(T), it.Index(), count);
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_ITERATOR_POSITION, it.Limit() != bitCount , "%u-bit iterator limit:%u, expected:%u.", bcof(T), it.Limit(), bitCount);
		}
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, count != bitCount , "%u-bit visited bit count:%u, expected:%u.", bcof(T), count, bitCount);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, it != end , "%u-bit iterator did not reach the end after %u bits; current index:%u, end index:%u." , bcof(T), bitCount, it.Index(), end.Index());
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, end.Index() != bitCount , "%u-bit end index:%u, expected:%u.", bcof(T), end.Index(), bitCount);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, end.Element != &data[1] , "%u-bit end element:%p, expected:%p.", bcof(T), end.Element, &data[1]);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, end.Offset != 3 , "%u-bit end offset:%u, expected:3.", bcof(T), ::llc::u2_t(end.Offset));
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, end.End != data + 2 , "%u-bit storage end:%p, expected:%p.", bcof(T), end.End, data + 2);
	}
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_WIDTH_DECREMENT, (--end).Index() != bitCount - 1 , "%u-bit end decrement mismatch. index:%u, expected:%u." , bcof(T), end.Index(), bitCount - 1 );
	return 0;
}

tplt<tpnm _dataValues, size_t _dataSize>
sttc ::llc::err_t testPartial(ATestError & errors, _dataValues (&data)[_dataSize]) {
	stxp ::llc::u2_t ELEMENT_BITS			= szof(_dataValues) * 8;
	stxp ::llc::u2_t BIT_COUNT				= 10;
	static_assert(_dataSize * ELEMENT_BITS >= BIT_COUNT, "10-bit view exceeds its backing storage.");

	::llc::u2_c		expectedOffset			= BIT_COUNT % ELEMENT_BITS;

	::llc::view_bit<_dataValues> partial	{data, BIT_COUNT};
	auto it = partial.begin();
	auto end = partial.end();
	{
		::llc::u2_t count = 0;
		for(; count < BIT_COUNT && it != end; ++it, ++count) {
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_ITERATOR_POSITION, it.Index() != count , "%u-bit backing type, 10-bit iterator index:%u, expected:%u.", ELEMENT_BITS, it.Index(), count);
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_ITERATOR_POSITION, it.Limit() != BIT_COUNT , "%u-bit backing type, iterator limit:%u, expected:%u.", ELEMENT_BITS, it.Limit(), BIT_COUNT);
		}
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, count != BIT_COUNT , "%u-bit backing type, 10-bit visited count:%u, expected:%u.", ELEMENT_BITS, count, BIT_COUNT);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, it != end , "%u-bit backing type, iterator did not reach the end after 10 bits; current index:%u, end index:%u." , ELEMENT_BITS, it.Index(), end.Index());
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, end.Index() != BIT_COUNT , "%u-bit backing type, 10-bit end index:%u, expected:%u.", ELEMENT_BITS, end.Index(), BIT_COUNT);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, end.Element != &data[BIT_COUNT / ELEMENT_BITS] , "%u-bit backing type, 10-bit end element:%p, expected:%p." , ELEMENT_BITS, end.Element, &data[BIT_COUNT / ELEMENT_BITS]);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, end.Offset != expectedOffset , "%u-bit backing type, 10-bit end offset:%u, expected:%u." , ELEMENT_BITS, ::llc::u2_t(end.Offset), expectedOffset);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, end.End != data + (BIT_COUNT + ELEMENT_BITS - 1) / ELEMENT_BITS , "%u-bit backing type, 10-bit storage end:%p, expected:%p." , ELEMENT_BITS, end.End, data + (BIT_COUNT + ELEMENT_BITS - 1) / ELEMENT_BITS);
	}
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PARTIAL_DECREMENT, (--end).Index() != BIT_COUNT - 1 , "%u-bit backing type, 10-bit end decrement mismatch. index:%u, expected:%u." , ELEMENT_BITS, end.Index(), BIT_COUNT - 1 );
	return 0;
}

tplt<tpnm _dataValues, size_t _dataSize>
sttc ::llc::err_t testSixBits(ATestError & errors, _dataValues (&data)[_dataSize]) {
	stxp ::llc::u2_t ELEMENT_BITS = szof(_dataValues) * 8;
	::llc::view_bit<_dataValues> sixBits{data, 6};
	auto it = sixBits.begin();
	auto end = sixBits.end();
	::llc::u2_t count = 0;
	for(; count < 6 && it != end; ++it, ++count) {}
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, count != 6 , "%u-bit backing type, 6-bit visited count:%u, expected:6.", ELEMENT_BITS, count);
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, it != end , "%u-bit backing type, iterator did not reach the end after 6 bits; current index:%u, end index:%u." , ELEMENT_BITS, it.Index(), end.Index());
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, end.Index() != 6 , "%u-bit backing type, 6-bit end index:%u, expected:6.", ELEMENT_BITS, end.Index());
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, end.Element != data , "%u-bit backing type, 6-bit end element:%p, expected:%p.", ELEMENT_BITS, end.Element, data);
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, end.Offset != 6 , "%u-bit backing type, 6-bit end offset:%u, expected:6.", ELEMENT_BITS, ::llc::u2_t(end.Offset));
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, end.End != data + 1 , "%u-bit backing type, 6-bit storage end:%p, expected:%p.", ELEMENT_BITS, end.End, data + 1);
	return 0;
}

tplt<tpnm _dataValues, size_t _dataSize>
sttc ::llc::err_t testEmpty(ATestError & errors, _dataValues (&data)[_dataSize]) {
	{
		::llc::view_bit<_dataValues> empty;
		cnst auto begin = empty.begin();
		cnst auto end = empty.end();
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_EMPTY_DEFAULT_POSITION, begin != end , "%u-bit backing type, default empty iterators differ. begin element:%p, end element:%p." , bcof(_dataValues), begin.Element, end.Element);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_EMPTY_DEFAULT_POSITION, begin.Begin , "%u-bit backing type, default empty begin base:%p, expected:null." , bcof(_dataValues), begin.Begin);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_EMPTY_DEFAULT_POSITION, end.End , "%u-bit backing type, default empty storage end:%p, expected:null." , bcof(_dataValues), end.End);
	}
	{
		::llc::view_bit<_dataValues> empty{data, 0};
		cnst auto begin = empty.begin();
		cnst auto end = empty.end();
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_EMPTY_DATA_POSITION, begin != end , "%u-bit backing type, data-backed empty iterators differ. begin element:%p, end element:%p." , bcof(_dataValues), begin.Element, end.Element);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_EMPTY_DATA_POSITION, begin.Begin , "%u-bit backing type, data-backed empty begin base:%p, expected:null." , bcof(_dataValues), begin.Begin);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_EMPTY_DATA_POSITION, end.End , "%u-bit backing type, data-backed empty storage end:%p, expected:null." , bcof(_dataValues), end.End);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testIteratorPosition(ATestError & errors) {
	T equalBits = 0;
	::llc::view_bit<T> positions{&equalBits, 2};
	auto first = positions.begin();
	auto second = first;
	++second;
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_ITERATOR_EQUALITY, first == second , "%u-bit backing type, iterators at different offsets compare equal. first index:%u, second index:%u." , bcof(T), first.Index(), second.Index() );
	*first = true;
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_PROXY_WRITE, !positions[0] , "%u-bit backing type, bit proxy write did not update its backing element. element:%" LLC_FMT_U3 ", bit index:0." , bcof(T), ::llc::u3_t(equalBits) );
	return 0;
}

tplt<tpnm _dataValues, size_t _dataSize>
sttc ::llc::err_t testConstIteration(ATestError & errors, _dataValues (&data)[_dataSize]) {
	cnst ::llc::view_bit<_dataValues> readOnly{data, 10};
	auto read = readOnly.begin();
	::llc::u2_t count = 0;
	for(; read != readOnly.end(); ++read, ++count)
		(void)(bool)*read;
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_CONST_ITERATION, count != 10 , "%u-bit backing type, const iteration stopped at the wrong position. count:%u, expected:10." , ::llc::u2_t(szof(_dataValues) * 8), count );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testRandom(ATestError & errors) {
	stxp ::llc::u2_t DATA_COUNT	= 8;
	stxp ::llc::u2_t ELEMENT_BITS	= szof(T) * 8;
	stxp ::llc::u2_t MAX_BITS		= DATA_COUNT * ELEMENT_BITS;
	T data[DATA_COUNT] = {};
	::llc::SPRNG random = {BIT_RANDOM_SEED};
	for(::llc::u2_t iRandom = 0; iRandom < BIT_RANDOM_COUNT; ++iRandom) {
		for(::llc::u2_t iData = 0; iData < DATA_COUNT; ++iData)
			data[iData] = T(random.Next());
		cnst ::llc::u2_t bitCount = ::llc::u2_t(random.Next() % (MAX_BITS + 1));
		::llc::view_bit<T> bits{data, bitCount};
		auto iterator = bits.begin();
		auto end = bits.end();
		::llc::u2_t iBit = 0;
		for(; iBit < bitCount && iterator != end; ++iterator, ++iBit) {
			cnst bool expected = 0 != (data[iBit / ELEMENT_BITS] & (T(1) << (iBit % ELEMENT_BITS)));
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_ITERATOR_POSITION, iterator.Index() != iBit , "%u-bit randomized iterator index:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u." , ELEMENT_BITS, iterator.Index(), iBit, BIT_RANDOM_SEED, iRandom, bitCount);
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_ITERATOR_POSITION, iterator.Limit() != bitCount , "%u-bit randomized iterator limit:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u, bit index:%u." , ELEMENT_BITS, iterator.Limit(), bitCount, BIT_RANDOM_SEED, iRandom, iBit);
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_ITERATOR_VALUE, bool(*iterator) != expected , "%u-bit randomized iterator value mismatch. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u, index:%u, value:%u, expected:%u." , ELEMENT_BITS, BIT_RANDOM_SEED, iRandom, bitCount, iBit, ::llc::u2_t(bool(*iterator)), ::llc::u2_t(expected) );
		}
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_END_POSITION, iBit != bitCount , "%u-bit randomized visited count:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u." , ELEMENT_BITS, iBit, bitCount, BIT_RANDOM_SEED, iRandom);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_END_POSITION, iterator != end , "%u-bit randomized iterator did not reach the end after %u bits. seed:%" LLC_FMT_U3 ", iteration:%u, current index:%u, end index:%u." , ELEMENT_BITS, bitCount, BIT_RANDOM_SEED, iRandom, iterator.Index(), end.Index());
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_END_POSITION, end.Index() != bitCount , "%u-bit randomized end index:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u." , ELEMENT_BITS, end.Index(), bitCount, BIT_RANDOM_SEED, iRandom);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_END_POSITION, end.Element != (bitCount ? data + bitCount / ELEMENT_BITS : 0) , "%u-bit randomized end element:%p, expected:%p. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u." , ELEMENT_BITS, end.Element, bitCount ? data + bitCount / ELEMENT_BITS : 0, BIT_RANDOM_SEED, iRandom, bitCount);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_END_POSITION, end.Offset != bitCount % ELEMENT_BITS , "%u-bit randomized end offset:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u." , ELEMENT_BITS, ::llc::u2_t(end.Offset), bitCount % ELEMENT_BITS, BIT_RANDOM_SEED, iRandom, bitCount);
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_END_POSITION, end.End != (bitCount ? data + (bitCount + ELEMENT_BITS - 1) / ELEMENT_BITS : 0) , "%u-bit randomized storage end:%p, expected:%p. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u." , ELEMENT_BITS, end.End, bitCount ? data + (bitCount + ELEMENT_BITS - 1) / ELEMENT_BITS : 0, BIT_RANDOM_SEED, iRandom, bitCount);
		if(bitCount) {
			auto last = end;
			--last;
			cnst ::llc::u2_t lastIndex = bitCount - 1;
			cnst bool expected = 0 != (data[lastIndex / ELEMENT_BITS] & (T(1) << (lastIndex % ELEMENT_BITS)));
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_DECREMENT, last.Index() != lastIndex , "%u-bit randomized decremented index:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u." , ELEMENT_BITS, last.Index(), lastIndex, BIT_RANDOM_SEED, iRandom, bitCount);
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_DECREMENT, bool(*last) != expected , "%u-bit randomized decremented value:%u, expected:%u. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u, bit index:%u." , ELEMENT_BITS, ::llc::u2_t(bool(*last)), ::llc::u2_t(expected), BIT_RANDOM_SEED, iRandom, bitCount, lastIndex);
			cnst ::llc::u2_t writeIndex = ::llc::u2_t(random.Next() % bitCount);
			cnst bool previous = bits[writeIndex];
			bits[writeIndex] = !previous;
			LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_RANDOM_PROXY_WRITE, bool(bits[writeIndex]) == previous , "%u-bit randomized proxy write mismatch. seed:%" LLC_FMT_U3 ", iteration:%u, bit count:%u, index:%u, previous:%u, current:%u." , ELEMENT_BITS, BIT_RANDOM_SEED, iRandom, bitCount, writeIndex, ::llc::u2_t(previous), ::llc::u2_t(bool(bits[writeIndex])) );
		}
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testReverseBits(ATestError & errors) {
	stxp ::llc::u2_t ELEMENT_BITS	= szof(T) * 8;
	T data[2]		= {T(0x5AU), T(0xC3U)};
	T original[2]	= {data[0], data[1]};
	::llc::u2_c bitCount = ELEMENT_BITS + 3;
	::llc::view_bit<T> bits{data, bitCount};
	cnst ::llc::err_t result = ::llc::reverse_bits(bits);
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_REVERSE_RESULT, result , "%u-bit reverse_bits() returned:%i for %u bits." , ELEMENT_BITS, result, bitCount );
	for(::llc::u2_t iBit = 0; iBit < bitCount; ++iBit) {
		::llc::u2_c sourceBit = bitCount - 1 - iBit;
		cnst bool expected = 0 != (original[sourceBit / ELEMENT_BITS] & (T(1) << (sourceBit % ELEMENT_BITS)));
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_REVERSE_VALUE, bool(bits[iBit]) != expected , "%u-bit reverse mismatch. bit count:%u, index:%u, value:%u, expected:%u." , ELEMENT_BITS, bitCount, iBit, ::llc::u2_t(bool(bits[iBit])), ::llc::u2_t(expected) );
	}
	for(::llc::u2_t iBit = bitCount; iBit < ::llc::size(data) * ELEMENT_BITS; ++iBit) {
		cnst bool current = 0 != (data[iBit / ELEMENT_BITS] & (T(1) << (iBit % ELEMENT_BITS)));
		cnst bool expected = 0 != (original[iBit / ELEMENT_BITS] & (T(1) << (iBit % ELEMENT_BITS)));
		LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_REVERSE_OUTSIDE, current != expected , "%u-bit reverse modified an outside bit. bit count:%u, index:%u, value:%u, expected:%u." , ELEMENT_BITS, bitCount, iBit, ::llc::u2_t(current), ::llc::u2_t(expected) );
	}
	T unchanged = T(0xA5U);
	cnst T expectedUnchanged = unchanged;
	::llc::view_bit<T> empty{&unchanged, 0};
	cnst ::llc::err_t emptyResult = ::llc::reverse_bits(empty);
	::llc::view_bit<T> single{&unchanged, 1};
	cnst ::llc::err_t singleResult = ::llc::reverse_bits(single);
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_REVERSE_RESULT, emptyResult , "%u-bit empty reverse result:%i, expected:0.", ELEMENT_BITS, emptyResult);
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_REVERSE_RESULT, singleResult , "%u-bit single-bit reverse result:%i, expected:0.", ELEMENT_BITS, singleResult);
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_REVERSE_OUTSIDE, unchanged != expectedUnchanged , "%u-bit empty or single-bit reverse modified its backing value. value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , ELEMENT_BITS, ::llc::u3_t(unchanged), ::llc::u3_t(expectedUnchanged) );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testInvalid(ATestError & errors) {
#ifdef LLC_WINDOWS
	stxp ::llc::u2_t ELEMENT_BITS = szof(T) * 8;
	T data[2] = {};
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_CONSTRUCTION, !testThrows([&]() { ::llc::view_bit<T> invalid{nullptr, 1}; (void)invalid; }) , "%u-bit view accepted a null pointer with a nonzero count." , ELEMENT_BITS );
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_CONSTRUCTION, !testThrows([&]() { ::llc::view_bit<T> invalid{::llc::u2_t(::llc::size(data) * ELEMENT_BITS + 1), data}; (void)invalid; }) , "%u-bit array view accepted a count beyond its %u-bit capacity." , ELEMENT_BITS, ::llc::u2_t(::llc::size(data) * ELEMENT_BITS) );
	::llc::view_bit<T> bits{data, 1};
	cnst ::llc::view_bit<T> readOnly{data, 1};
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)bits[bits.size()]; }) , "%u-bit mutable view accepted index:%u for size:%u." , ELEMENT_BITS, bits.size(), bits.size() );
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)readOnly[readOnly.size()]; }) , "%u-bit const view accepted index:%u for size:%u." , ELEMENT_BITS, readOnly.size(), readOnly.size() );
	auto mutableEnd = bits.end();
	cnst auto constEnd = readOnly.end();
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { (void)*mutableEnd; }) , "%u-bit mutable iterator dereferenced end index:%u, limit:%u." , ELEMENT_BITS, mutableEnd.Index(), mutableEnd.Limit() );
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { (void)*constEnd; }) , "%u-bit const iterator dereferenced end index:%u, limit:%u." , ELEMENT_BITS, constEnd.Index(), constEnd.Limit() );
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { (void)(bool)mutableEnd; }) , "%u-bit iterator converted end index:%u to bool." , ELEMENT_BITS, mutableEnd.Index() );
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { ++mutableEnd; }) , "%u-bit iterator incremented end index:%u, limit:%u." , ELEMENT_BITS, mutableEnd.Index(), mutableEnd.Limit() );
	auto mutableBegin = bits.begin();
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { --mutableBegin; }) , "%u-bit iterator decremented begin index:%u." , ELEMENT_BITS, mutableBegin.Index() );
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { mutableEnd = true; }) , "%u-bit iterator assigned through end index:%u, limit:%u." , ELEMENT_BITS, mutableEnd.Index(), mutableEnd.Limit() );
	::llc::view_bit<T> empty;
	auto emptyBegin = empty.begin();
	LLC_TEST_CHECKF(errors, BIT_VIEW_TEST_RESULT_INVALID_ITERATOR, !testThrows([&]() { (void)*emptyBegin; }) , "%u-bit default empty iterator was dereferenceable." , ELEMENT_BITS );
#else
	(void)errors;
#endif
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testType(ATestError & errors) {
	T data[2] = {(T)0xA5U, (T)0x02U};
	if_fail_fe(testPartial			(errors, data));
	if_fail_fe(testSixBits			(errors, data));
	if_fail_fe(testEmpty			(errors, data));
	if_fail_fe(testIteratorPosition<T>	(errors));
	if_fail_fe(testConstIteration		(errors, data));
	if_fail_fe(testPartialWidth<T>		(errors));
	if_fail_fe(testReverseBits<T>		(errors));
	if_fail_fe(testInvalid<T>			(errors));
	return testRandom<T>(errors);
}

tplt<tpnm T>
sttc ::llc::err_t testTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%2u-bit suite completed: %u/%u checks passed, %u failed.", bcof(T), typeChecks - typeFailures, typeChecks, typeFailures);
	return 0;
}

::llc::err_t testViewBit(ATestError & errors) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u3_t>(errors));
	if(failureCount == testErrorCount(errors))
		always_printf("Element widths tested successfully:\n%u, %u, %u and %u bits."
			, ::llc::u2_t(szof(::llc::u0_t) * 8), ::llc::u2_t(szof(::llc::u1_t) * 8), ::llc::u2_t(szof(::llc::u2_t) * 8), ::llc::u2_t(szof(::llc::u3_t) * 8)
			);
	return 0;
}
