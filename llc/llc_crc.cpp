#include "llc_crc.h"
#include "llc_noise.h"

LLC_USING_TYPEINT();

stxp u2_c			LLC_CRC_CRC_SEED			= 18973;
llc::err_t			llc::crcGenerate			(vcu0_t bytes, uint64_t & crc)	{
	crc						= 0;
	u2_c				lastPos						= bytes.size() - 1;
	for(u2_t i=0; i < bytes.size(); ++i) {
		crc						+= noise1DBase(bytes[i], ::LLC_CRC_CRC_SEED);
		crc						+= noise1DBase(bytes[lastPos - i], ::LLC_CRC_CRC_SEED);
	}
	return 0;
}
llc::err_t			llc::crcGenerateAndAppend	(au0_t & bytes)	{
	uint64_t					crcToStore					= 0;
	crcGenerate(bytes, crcToStore);
	llc_necs(bytes.append((const u0_t*)&crcToStore, szof(uint64_t)));;
	return 0;
}
llc::err_t			llc::crcVerifyAndRemove		(au0_t & bytes)	{
	if_true_fef(bytes.size() < 8, "Invalid input. No CRC can be found in an array of %u bytes.", bytes.size());
	uint64_t					check						= 0;
	u2_c				startOfCRC					= bytes.size() - 8;
	crcGenerate({bytes.begin(), startOfCRC}, check);
	const uint64_t				found						= *(uint64_t*)&bytes[startOfCRC];
	if_true_fef(check != found, "CRC Check failed: Stored: %llu. Calculated: %llu.", found, check);
	llc_necs(bytes.resize(bytes.size() - 8));
	return 0;
}
