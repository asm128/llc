#include "llc_crc.h"
#include "llc_noise.h"

LLC_USING_TYPEINT();

stxp u2_c			LLC_CRC_CRC_SEED			= 18973;
llc::err_t			llc::crcGenerate			(vcu0_t bytes, uint64_t & crc)	{
	crc						= 0;
	for(u2_t i=0; i < bytes.size(); ++i)
		crc						= noise1DBase(crc ^ (u3_t(bytes[i]) | (u3_t(i) << 8)), ::LLC_CRC_CRC_SEED);
	return 0;
}
llc::err_t			llc::crcGenerateAndAppend	(au0_t & bytes)	{
	uint64_t					crcToStore					= 0;
	crcGenerate(bytes, crcToStore);
	if_fail_fe(bytes.append((const u0_t*)&crcToStore, szof(uint64_t)));;
	return 0;
}
llc::err_t			llc::crcVerifyAndRemove		(au0_t & bytes)	{
	if_true_fef(bytes.size() < 8, "Invalid input. No CRC can be found in an array of %u bytes.", bytes.size());
	uint64_t					check						= 0;
	u2_c				startOfCRC					= bytes.size() - 8;
	crcGenerate({bytes.begin(), startOfCRC}, check);
	uint64_t					found						= 0;
	for(u2_t iByte = 0; iByte < szof(found); ++iByte)
		((u0_t*)&found)[iByte]						= bytes[startOfCRC + iByte];
	if_true_fef(check != found, "CRC Check failed: Stored: %llu. Calculated: %llu.", found, check);
	if_fail_fe(bytes.resize(bytes.size() - 8));
	return 0;
}
