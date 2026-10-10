#include "llc_view.h"
#include "llc_n2.h"

#ifndef LLC_GRID_H_23627
#define LLC_GRID_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplt<tpnm TCell>
	clss grid {
	protected:
		TCell			* Data	= {};
		n2u2_t			Size	= {};
	public:
		tydf TCell		T;

		inxp				grid		() nxpt = default;
		inln				grid		(T * data, u2_t width, u2_t height) : Data{data}, Size{width, height} {
			if_true_tef(0 == data && Size.x && Size.y, "Invalid grid size:{%u, %u}, pointer:%p.", width, height, data);
		}
		inln				grid		(T * data, n2u2_c & metrics) : grid(data, metrics.x, metrics.y) {}
		tplt<size_t width, size_t height>
		inxp				grid		(T (&data)[height][width]) nxpt : Data{&data[0][0]}, Size{width, height} {}

		inxp	oper		grid<cnst T>	() cnst nxpt { rtrn {Data, Size}; }

		view<T>			oper[]		(u2_t row) {
			if_null_te(Data);
			if_true_tef(row >= Size.y, "Invalid row:%u.", row);
			rtrn {&Data[row * Size.x], Size.x};
		}
		view<cnst T>		oper[]		(u2_t row) cnst {
			if_null_te(Data);
			if_true_tef(row >= Size.y, "Invalid row:%u.", row);
			rtrn {&Data[row * Size.x], Size.x};
		}
		T&				oper[]		(n2u2_c & cell) {
			if_null_te(Data);
			if_true_tef(cell.y >= Size.y, "Invalid row:%u.", cell.y);
			if_true_tef(cell.x >= Size.x, "Invalid column:%u.", cell.x);
			rtrn Data[cell.y * Size.x + cell.x];
		}
		cnst T&			oper[]		(n2u2_c & cell) cnst {
			if_null_te(Data);
			if_true_tef(cell.y >= Size.y, "Invalid row:%u.", cell.y);
			if_true_tef(cell.x >= Size.x, "Invalid column:%u.", cell.x);
			rtrn Data[cell.y * Size.x + cell.x];
		}

		inxp	cnst T*		begin		() cnst nxpt { rtrn Data; }
		inxp	cnst T*		end			() cnst nxpt { rtrn Data + size(); }
		inxp	T*			begin		() nxpt { rtrn Data; }
		inxp	T*			end			() nxpt { rtrn Data + size(); }
		inxp	n2u2_c&		metrics		() cnst nxpt { rtrn Size; }
		inxp	n2u1_t		metrics16	() cnst nxpt { rtrn Size.u1(); }
		inxp	u2_t			size		() cnst nxpt { rtrn area(); }
		inxp	u2_t			byte_count	() cnst nxpt { rtrn area() * szof(T); }
		inxp	u2_t			area		() cnst nxpt { rtrn Size.x * Size.y; }

		err_t				fill		(cnst T & value, u2_t offset = 0, u2_t count = (u2_t)-1) {
			if_true_fef(count > size() && count != (u2_t)-1, "Count:%u, grid size:%u.", count, size());
			for(u2_t i = offset, stop = min(size(), count); i < stop; ++i)
				Data[i] = value;
			rtrn 0;
		}
	};
#pragma pack(pop)

	tydf grid<sc_t>	gc, gchar;
	tydf grid<uc_t>	guc, guchar;
	tydf grid<u0_t>	gub, gu8;
	tydf grid<s0_t>	gb, gi8;
	tydf grid<u1_t>	gu16;
	tydf grid<u2_t>	gu32;
	tydf grid<u3_t>	gu64;
	tydf grid<s1_t>	gi16;
	tydf grid<s2_t>	gi32;
	tydf grid<s3_t>	gi64;
	tydf grid<f2_t>	gf32;
	tydf grid<f3_t>	gf64;
} // namespace llc

#endif // LLC_GRID_H_23627
