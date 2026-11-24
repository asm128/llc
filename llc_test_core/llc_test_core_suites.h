#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

// The runner creates outputFolder and owns its results.json and console.log.
// Suites may write any other files/subdirectories there; the runner does not inspect them.
// The folder view remains valid for the duration of the suite call.

::llc::err_t test_array_static		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_adam7			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
//::llc::err_t test_app			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ascii_color		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ascii_target		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_array_bit		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_apod_minmax		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_apod_serialize	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_array_obj		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_array_pod		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_args			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_base64			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_bit		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_bitmap_target	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_block			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_block_container	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_camera			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_collision		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_complus		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_packed_int		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_cpow			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ctti		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_deflate		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_enum			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_expression		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_geometry		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_grid(::llc::ATestError & errors, ::llc::vcst_t outputFolder);
::llc::err_t test_grid_serialize(::llc::ATestError & errors, ::llc::vcst_t outputFolder);
::llc::err_t test_grid_image		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_grid_operations	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_gui			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_input			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_index(::llc::ATestError & errors, ::llc::vcst_t outputFolder);
::llc::err_t test_json_expression	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_json		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_keyval			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_label			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_linear_map		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_msbuild_xml		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_xml		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_path			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ptr		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_png			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_process_broker	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
//::llc::err_t test_runtime_module	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_noise			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_stl			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_str			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_system_key		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_type_metadata	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_view			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_view_aliases	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_view_bit		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_view_serialize	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_view_stream		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_vox			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_voxel			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
