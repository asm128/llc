#include "llc_label_manager.h"
#include "llc_n2.h"
#include "llc_file.h"

#ifndef LLC_ENGINE_FILE_H_23627
#define LLC_ENGINE_FILE_H_23627

namespace llc 
{
	struct SRAMFolder {
		::llc::aobj<::llc::vcst_t>		Names;
		::llc::apod<::llc::rangeu2_t>	Ranges;
		::llc::au0_t					Contents;
		::llc::au0_t					LoadCache;

		::llc::error_t					AddFile			(::llc::vcst_t fileName, ::llc::vu0_t fileContents) {
			const uint32_t						ssizes[]		= {Ranges.size(), Contents.size(), Names.size()};
			uint32_t							itemIndex;
			::llc::resize(::llc::min<const uint32_t>(ssizes), Ranges, Contents, Names);
			if_fail_fef(Ranges.push_back({Contents.size(), fileContents.size()})	, "Failed to store file: %s", toString(fileName).begin());
			if_fail_fef(Contents.append(fileContents)							, "Failed to store file: %s", toString(fileName).begin());
			if(failed(itemIndex = Names.push_back(fileName))) {
				error_printf("Failed to store file: %s", toString(fileName).begin());
				if_fail_fe(Contents.resize(Ranges[Ranges.size() - 1].Offset));
				if_fail_fe(Ranges.resize(Ranges.size() - 1));
			}
			return itemIndex;
		}
		::llc::error_t					AddFile			(::llc::vcst_t filePath) {
			LoadCache.clear();
			if_fail_fe(llc::fileToMemory(filePath, LoadCache));
			return AddFile(filePath, LoadCache);
		}
		::llc::error_t					GetFile			(uint32_t index, ::llc::vcu0_t & view)		const	{ return (view = {&Contents[Ranges[index].Offset], Ranges[index].Count}).size(); }
		::llc::error_t					GetFile			(::llc::vcst_t filePath, ::llc::vcu0_t & view)	const	{
			::llc::error_t						index			= -1;
			if_fail_fef(index = ::llc::find(filePath, ::llc::vcvcc{Names}), "File not found: '%s'", toString(filePath).begin());
			return GetFile(index, view);
		}
	};


} // namespace

#endif // LLC_ENGINE_FILE_H_23627