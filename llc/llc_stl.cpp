#include "llc_stl.h"
#include "llc_file.h"

llc::err_t			llc::stlFileLoad		(vcst_t filename, SSTLFile& file) {
	llc_necall(fileToMemory(filename, file.Raw), "Failed to load file '%.*s'.", LLC_VIEW_ARGS(filename));
	return stlFileLoad(file.Raw, file.Header, file.Triangles);
}

llc::err_t			llc::stlFileLoad		(vu0_t fileInMemory, vcu0_t & out_Header, view<SSTLTriangle> & out_Triangles) {
	vcu0_t						header					= {};
	view<SSTLTriangle>			triangles				= {};
	llc_necall(fileInMemory.slice(header, 0, 80), "Invalid file size: %u bytes.", fileInMemory.size());
	vu0_t						triangleBytes			= {};
	llc_necall(fileInMemory.slice(triangleBytes, 80), "Invalid file size: %u bytes.", fileInMemory.size());
	err_t						bytesRead				= 0;
	llc_necall(bytesRead = viewPascal(triangles, triangleBytes), "Invalid triangle data. File size: %u bytes.", fileInMemory.size());
	out_Header					= header;
	out_Triangles				= triangles;
	return bytesRead;
}

llc::err_t			llc::stlFileLoad		(vcu0_t fileInMemory, vcu0_t & out_Header, view<cnst SSTLTriangle> & out_Triangles) {
	vcu0_t						header					= {};
	view<cnst SSTLTriangle>		triangles				= {};
	llc_necall(fileInMemory.slice(header, 0, 80), "Invalid file size: %u bytes.", fileInMemory.size());
	vcu0_t						triangleBytes			= {};
	llc_necall(fileInMemory.slice(triangleBytes, 80), "Invalid file size: %u bytes.", fileInMemory.size());
	err_t						bytesRead				= 0;
	llc_necall(bytesRead = viewPascal(triangles, triangleBytes), "Invalid triangle data. File size: %u bytes.", fileInMemory.size());
	out_Header					= header;
	out_Triangles				= triangles;
	return bytesRead;
}
