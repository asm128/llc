#include "llc_view.h"

#ifndef LLC_STREAM_VIEW_H_23627
#define LLC_STREAM_VIEW_H_23627

namespace llc
{
	tplt<tpnm _tElement>
	struct view_stream : public view<_tElement> {
		static_assert(szof(_tElement) == 1);

		u2_t									CursorPosition						= 0;

		using									view<_tElement>::Data;
		using									view<_tElement>::size;
		using									view<_tElement>::view;

		constexpr	u2_t				remaining							() const nxpt	{ rtrn CursorPosition <= size() ? size() - CursorPosition : 0; }

		tplt<tpnm _tPOD>
		err_t								read_pod							(view<_tPOD> output) {
			u2_c								count								= output.size();
			if_true_fef(count > remaining() / szof(_tPOD), "%" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", count, remaining() / szof(_tPOD));
			if_true_fef(count > 0x7FFFFFFFU / szof(_tPOD), "%" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", count, 0x7FFFFFFFU / szof(_tPOD));
			u2_c								byteCount							= count * szof(_tPOD);
			if(byteCount)
				memcpy(output.begin(), &Data[CursorPosition], byteCount);
			CursorPosition						+= byteCount;
			rtrn byteCount;
		}
		tplt<tpnm _tPOD>					inln	err_t	read_pod	(_tPOD & output)			{ rtrn read_pod(view<_tPOD>{&output, 1}); }
		tplt<tpnm _tPOD, size_t _sCount>	inln	err_t	read_pod	(_tPOD (&output)[_sCount])	{ rtrn read_pod(view<_tPOD>{output}); }

		tplt<tpnm _tPOD> requires(false == is_cnst<_tElement>::Value)
		err_t								write_pod							(view<cnst _tPOD> input) {
			u2_c								count								= input.size();
			if_true_fef(count > remaining() / szof(_tPOD), "%" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", count, remaining() / szof(_tPOD));
			if_true_fef(count > 0x7FFFFFFFU / szof(_tPOD), "%" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", count, 0x7FFFFFFFU / szof(_tPOD));
			u2_c								byteCount							= count * szof(_tPOD);
			if(byteCount)
				memcpy(&Data[CursorPosition], input.begin(), byteCount);
			CursorPosition						+= byteCount;
			rtrn byteCount;
		}
		tplt<tpnm _tPOD> requires(false == is_cnst<_tElement>::Value)
		inln	err_t						write_pod							(cnst _tPOD & input)			{ rtrn write_pod(view<cnst _tPOD>{&input, 1}); }
		tplt<tpnm _tPOD, size_t _sCount> requires(false == is_cnst<_tElement>::Value)
		inln	err_t						write_pod							(cnst _tPOD (&input)[_sCount])	{ rtrn write_pod(view<cnst _tPOD>{input}); }
	};
} // namespace

#endif // LLC_STREAM_VIEW_H_23627
