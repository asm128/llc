# LLC migration overview

This document compares the [former monolithic LLC source](../../../bundle_galaxy_hell/gpk/llc) with the current independent LLC source in this directory. It distinguishes incorporated files from missing counterparts, partial replacements, higher-layer responsibilities and stale project metadata. It is the living migration ledger, not authorization to restore files wholesale.

## Comparison scope

- Former monolithic source: [`bundle_galaxy_hell/gpk/llc`](../../../bundle_galaxy_hell/gpk/llc)
- Independent LLC repository: [`llb/llc`](..)
- Independent production source: [`llb/llc/llc`](.)
- Independent shared contract tests: [`llb/llc/llc_test_core`](../llc_test_core)

The former root has no descendant `AGENTS.md`. The independent repository is governed by its [root instructions](../AGENTS.md), which define LLC as the foundational layer and explicitly warn against a premature bulk migration.

## Bounded comparison

The counts below are the 2026-10-08 inventory snapshot; the file ledger is updated as later migration stages land.

- Former tracked source files inspected by inventory: 306 C/C++ headers and implementations.
- Independent production files inspected by inventory: 186 C/C++ headers and implementations.
- After normalizing only the `gpk_` to `llc_` prefix, 165 former files have current counterparts and 141 do not. Those 165 entries map to 164 current files because the former tree contains both `gpk_runtime.h` and `llc_runtime.h`. Twenty-two current production files have no normalized former namesake. Filename survival is lineage evidence, not behavioral equivalence.
- The independent [`llc.vcxproj`](llc.vcxproj) still names 120 files absent from disk: 83 header entries and 37 compilation entries. All 37 absent compilation entries are explicitly `ExcludedFromBuild`; this is migration residue, not an implementation.
- The shared current test runner registers 28 focused suites, including independent Adam7, PNG, geometry, block-container and label suites.

## Major findings

### Already present, not gaps

Current independent LLC retains direct source owners for arrays and views, object and POD containers, pointers, labels, logging, parsing, JSON, XML, paths and files, runtime entry values, TCP/IP, AES, deflate, Base64, encoding, time, synchronization, I2C and SPI. Current additions without former normalized names include CTTI, CCSDS, circuit, bus, Wi-Fi, pack, RX/TX and a dedicated runtime-application layer.

Real current consumers confirm active use: [`llt/dedup`](../../llt/dedup) uses path, file, string and runtime facilities; [`llt/lls`](../../llt/lls) uses JSON and runtime facilities; and [`llt/llc_build`](../../llt/llc_build) uses arguments, runtime and XML.

### Partial replacements

- The former generic `block_container<T, size>` and its null-terminated-string specialization now have current owners and independent focused tests across all LLC integer widths.
- The scalar/vector geometry foundation now has current owners for vectors, lines, triangles, quads, rectangles, ranges, slices, min/max bounds, gauges, spheres, quaternions and matrices. `m3<T>` owns only storage; `m3a2` and `m3a3` separate 2D affine from 3D linear operations, while `m2` and `m4` also distinguish mathematical convention from physical layout. The aliases use LLC width names (`u0` through `u3`, `s0` through `s3`, `f2` and `f3`) and the focused geometry suite instantiates every scalar width.
- PNG loading and Adam7 reconstruction now have current source owners. Grid and image scalar aliases follow the LLC width vocabulary (`gu2_t`, `imgu2_t` and their signed, unsigned and floating-point families), while color aliases retain channel-precision names such as `img8bgra`. PNG loading owns the CRC implementation it requires; PNG writing remains excluded from the build. The focused suites check every Adam7 pass, small-image coverage, reconstructed cells, malformed signatures and all 15 interlaced/non-interlaced PNGSuite basic format pairs.
- The former owning `array_bit` is absent; current `view_bit` covers non-owning bit access and has a focused suite, but it does not replace owning resize and storage behavior.
- Current `SCommandLineArgs`, `argsParse()` and `viewsFromEnvp()` already capture the entry-point environment as raw `NAME=VALUE` views, and current `keyval_split()` can interpret an individual entry. This covers the read/view side of the former environment helpers. What remains is composing those views into an owned, double-null-terminated native environment block when a consumer needs that representation. The `WinMain` entry path currently calls `argsParse()` without an environment, so it also needs an explicit population policy before such composition can be universal. Dynamic-module loading remains separately absent.
- `llc_slice.h` owns only scalar slices and depends only on the scalar foundation. `llc_slice_n2.h` and `llc_slice_n3.h` own the cross-type aliases so neither primitive template absorbs the other merely for a convenient alias. The former gauge-vector wrappers target a removed `gauge<>` type and are deliberately not restored; `gaugemax<>` and `gaugeminmax<>` remain the valid generic owners.

### Genuine consumer-backed gaps

1. **Expression parsing and JSON expression/interpolation.** The former `gpk_expression.*` and `gpk_json_expression.*` are used extensively by [Blitter](../../../blitter) configuration and query code. Current `llc_eval.h` contains arithmetic, minimum and maximum helpers, not an expression evaluator.
2. **Environment-block composition and dynamic-module support.** `SCommandLineArgs::Environment` replaces the former environment-view collection for entry points that receive `envp`; the missing environment behavior is serialization back to the native double-null block and coverage for entry paths that do not populate `Environment`. Former module types and the runtime loader still have no independent source counterpart.
3. **HTTP/HTTPS and CGI.** Former request/response structures, URL decoding, client operations and CGI runtime adapters are absent; current LLC stops at TCP/IP and serialization helpers. Blitter is a concrete consumer.
4. **Block-addressed records, maps and registries.** Generic block-container storage is now incorporated. Former block-addressed record helpers, linear maps and registries remain absent. [`gpk_engine`](../../../bundle_galaxy_hell/gpk/gpk_engine) still consumes `gpk_linear_map_pod.h`, proving that the map family is not merely unused debris.
5. **Selected media and interchange formats.** Font and image serialization, STL and VOX are absent. `gpk_engine` and the focused VOX samples still consume them. Their low-level parsing may be reusable, but image, raster and engine ownership must be separated before integration.
6. **RSA/GPC helpers.** These are absent while AES, Base64 and deflate remain. The former custom crypto surface should be treated as a security-review candidate, not copied merely for parity.

### Higher layer, not independent-LLC gaps

The former directory also contains GUI controls, dialogs, windows, D3D, scene, camera, model, raster, particle, geometry and voxel-rendering systems. Current LLC instructions define the likely long-term split as foundational LLC plus `gpk_engine`; current `gpk_engine` still composes GUI, geometry, PNG, font and raster facilities. These systems are migration work for the higher layer, not evidence that foundational LLC must absorb the former monolith.

## Suggested integration order

1. Reconcile the migration ledger: classify the 132 absent project entries and choose owners; do not restore them wholesale.
2. Continue the small foundational contracts after the completed generic block storage: recover consumer-proven linear-map behavior with focused current-style tests.
3. Add environment-block composition to the current argument/runtime ownership only for a proven consumer, including an explicit `WinMain` population policy; recover dynamic-module primitives separately.
4. Design a typed expression-result contract and preservation tests before adapting the historical parser and interpolator.
5. Layer HTTP and CGI over current TCP/IP, JSON, runtime and argument facilities, preserving transport versus application ownership.
6. Continue codecs and formats only when a real consumer is selected; split byte parsing from engine and image types.
7. Keep GUI, rendering, window and scene systems in the higher layer. Treat former RSA/GPC code as audit material unless a concrete requirement justifies a supported cryptographic contract.

## File ledger

This ledger covers production `.h` and `.cpp` files at the two source roots. `[x]` means the normalized current filename exists; it does not claim every historical symbol or behavior survived. `[ ]` means no normalized current counterpart exists and its disposition remains pending: incorporate, replace with a current generic facility, assign to the higher layer, or reject deliberately. Header/implementation pairs are collapsed only when both files have the same status.

### Incorporated/current counterpart present

- [x] `gpk_aes.{h,cpp}` -> `llc_aes.{h,cpp}`
- [x] `gpk_adam7.{h,cpp}` -> `llc_adam7.{h,cpp}`
- [x] `gpk_aobj_pobj.h` -> `llc_aobj_pobj.h`
- [x] `gpk_aobj_ppod.h` -> `llc_aobj_ppod.h`
- [x] `gpk_apod_color.h` -> `llc_apod_color.h`
- [x] `gpk_apod_n2.h` -> `llc_apod_n2.h`
- [x] `gpk_apod_n3.h` -> `llc_apod_n3.h`
- [x] `gpk_apod_range.h` -> `llc_apod_range.h`
- [x] `gpk_apod_serialize.h` -> `llc_apod_serialize.h`
- [x] `gpk_apod_slice.h` -> `llc_apod_slice.h`
- [x] `gpk_apod_tri.h` -> `llc_apod_tri.h`
- [x] `gpk_apod_tri2.h` -> `llc_apod_tri2.h`
- [x] `gpk_apod_tri3.h` -> `llc_apod_tri3.h`
- [x] `gpk_append_css.{h,cpp}` -> `llc_append_css.{h,cpp}`
- [x] `gpk_append_frontend.h` -> `llc_append_frontend.h`
- [x] `gpk_append_html.{h,cpp}` -> `llc_append_html.{h,cpp}`
- [x] `gpk_append_js.{h,cpp}` -> `llc_append_js.{h,cpp}`
- [x] `gpk_append_json.{h,cpp}` -> `llc_append_json.{h,cpp}`
- [x] `gpk_append_tcpip.{h,cpp}` -> `llc_append_tcpip.{h,cpp}`
- [x] `gpk_append_xml.{h,cpp}` -> `llc_append_xml.{h,cpp}`
- [x] `gpk_arduino_string.h` -> `llc_arduino_string.h`
- [x] `gpk_args.{h,cpp}` -> `llc_args.{h,cpp}`
- [x] `gpk_array.{h,cpp}` -> `llc_array.{h,cpp}`
- [x] `gpk_array_base.h` -> `llc_array_base.h`
- [x] `gpk_array_circular.h` -> `llc_array_circular.h`
- [x] `gpk_array_obj.h` -> `llc_array_obj.h`
- [x] `gpk_array_pod.h` -> `llc_array_pod.h`
- [x] `gpk_array_ptr.h` -> `llc_array_ptr.h`
- [x] `gpk_array_static.h` -> `llc_array_static.h`
- [x] `gpk_astatic_serialize.h` -> `llc_astatic_serialize.h`
- [x] `gpk_auto_handler.h` -> `llc_auto_handler.h`
- [x] `gpk_align.h` -> `llc_align.h`
- [x] `gpk_axis.h` -> `llc_axis.h`
- [x] `gpk_base64.{h,cpp}` -> `llc_base64.{h,cpp}`
- [x] `gpk_bit.h` -> `llc_bit.h`
- [x] `gpk_block_container.h` -> `llc_block_container.h`
- [x] `gpk_block_container_nts.h` -> `llc_block_container_nts.h`
- [x] `gpk_cerrno.h` -> `llc_cerrno.h`
- [x] `gpk_chrono.{h,cpp}` -> `llc_chrono.{h,cpp}`
- [x] `gpk_circle.h` -> `llc_circle.h`
- [x] `gpk_color.h` -> `llc_color.h`
- [x] `gpk_color_type.h` -> `llc_color_type.h`
- [x] `gpk_coord.h` -> `llc_coord.h`
- [x] `gpk_cpow.h` -> `llc_cpow.h`
- [x] `gpk_cstdio.h` -> `llc_cstdio.h`
- [x] `gpk_cstring.h` -> `llc_cstring.h`
- [x] `gpk_datatype.{h,cpp}` -> `llc_datatype.{h,cpp}`
- [x] `gpk_debug.h` -> `llc_debug.h`
- [x] `gpk_deflate.{h,cpp}` -> `llc_deflate.{h,cpp}`
- [x] `gpk_encoding.{h,cpp}` -> `llc_encoding.{h,cpp}`
- [x] `gpk_enum.h` -> `llc_enum.h`
- [x] `gpk_error.h` -> `llc_error.h`
- [x] `gpk_eval.h` -> `llc_eval.h`
- [x] `gpk_event.h` -> `llc_event.h`
- [x] `gpk_event_input.h` -> `llc_event_input.h`
- [x] `gpk_event_raster.h` -> `llc_event_raster.h`
- [x] `gpk_event_screen.h` -> `llc_event_screen.h`
- [x] `gpk_file.{h,cpp}` -> `llc_file.{h,cpp}`
- [x] `gpk_frameinfo.h` -> `llc_frameinfo.h`
- [x] `gpk_functional.h` -> `llc_functional.h`
- [x] `gpk_gauge.h` -> `llc_gauge.h`
- [x] `gpk_geometry2.h` -> `llc_geometry2.h`
- [x] `gpk_grid.h` -> `llc_grid.h` (grid type and scalar aliases; drawing and vector-grid aliases remain separate)
- [x] `gpk_grid_color.h` -> `llc_grid_color.h`
- [x] `gpk_i2c.{h,cpp}` -> `llc_i2c.{h,cpp}`
- [x] `gpk_image.h` -> `llc_image.h` (`img<>` only; `imgmono<>`, render targets and update helpers remain pending)
- [x] `gpk_img_color.h` -> `llc_img_color.h` (image aliases only; render-target and pointer aliases remain pending)
- [x] `gpk_json.{h,cpp}` -> `llc_json.{h,cpp}`
- [x] `gpk_keyval.h` -> `llc_keyval.h`
- [x] `gpk_keyval_old.{h,cpp}` -> `llc_keyval_old.{h,cpp}`
- [x] `gpk_label.{h,cpp}` -> `llc_label.{h,cpp}`
- [x] `gpk_label_manager.h` -> `llc_label_manager.h`
- [x] `gpk_line.h` -> `llc_line.h`
- [x] `gpk_line2.h` -> `llc_line2.h`
- [x] `gpk_line3.h` -> `llc_line3.h`
- [x] `gpk_log.{h,cpp}` -> `llc_log.{h,cpp}`
- [x] `gpk_log_core.h` -> `llc_log_core.h`
- [x] `gpk_log_level.h` -> `llc_log_level.h`
- [x] `gpk_math.h` -> `llc_math.h`
- [x] `gpk_matrix.h` -> `llc_matrix.h`
- [x] `gpk_memory.h` -> `llc_memory.h`
- [x] `gpk_minmax.h` -> `llc_minmax.h`
- [x] `gpk_minmax_n2.h` -> `llc_minmax_n2.h`
- [x] `gpk_minmax_n3.h` -> `llc_minmax_n3.h`
- [x] `gpk_n2.h` -> `llc_n2.h`
- [x] `gpk_n3.h` -> `llc_n3.h`
- [x] `gpk_noise.{h,cpp}` -> `llc_noise.{h,cpp}`
- [x] `gpk_packed_int.h` -> `llc_packed_int.h`
- [x] `gpk_parse.{h,cpp}` -> `llc_parse.{h,cpp}`
- [x] `gpk_path.{h,cpp}` -> `llc_path.{h,cpp}`
- [x] `gpk_platform_error.h` -> `llc_platform_error.h`
- [x] `gpk_platform_globals.h` -> `llc_platform_globals.h`
- [x] `gpk_png.{h,cpp}` -> `llc_png.{h,cpp}`
- [x] `gpk_png_write.cpp` -> `llc_png_write.cpp` (present but excluded from the build pending writer tests)
- [x] `gpk_ptr_nco.h` -> `llc_ptr_nco.h`
- [x] `gpk_ptr_obj.h` -> `llc_ptr_obj.h`
- [x] `gpk_ptr_pod.h` -> `llc_ptr_pod.h`
- [x] `gpk_quad.h` -> `llc_quad.h`
- [x] `gpk_quad2.h` -> `llc_quad2.h`
- [x] `gpk_quad3.h` -> `llc_quad3.h`
- [x] `gpk_quat.h` -> `llc_quat.h`
- [x] `gpk_queue_async.h` -> `llc_queue_async.h`
- [x] `gpk_queue_event.h` -> `llc_queue_event.h`
- [x] `gpk_range.h` -> `llc_range.h`
- [x] `gpk_range_n2.h` -> `llc_range_n2.h`
- [x] `gpk_range_n3.h` -> `llc_range_n3.h`
- [x] `gpk_rect.h` -> `llc_rect.h`
- [x] `gpk_rect2.h` -> `llc_rect2.h`
- [x] `gpk_rect3.h` -> `llc_rect3.h`
- [x] `gpk_ref.h` -> `llc_ref.h`
- [x] `gpk_runtime.h` -> `llc_runtime.h`
- [x] `gpk_safe.h` -> `llc_safe.h`
- [x] `gpk_size.h` -> `llc_size.h`
- [x] `gpk_slice.h` -> `llc_slice.h`
- [x] `gpk_slice_n2.h` -> `llc_slice_n2.h`
- [x] `gpk_slice_n3.h` -> `llc_slice_n3.h`
- [x] `gpk_sphere.h` -> `llc_sphere.h`
- [x] `gpk_spi.{h,cpp}` -> `llc_spi.{h,cpp}`
- [x] `gpk_std_cstring.h` -> `llc_std_cstring.h`
- [x] `gpk_std_initializer_list.h` -> `llc_std_initializer_list.h`
- [x] `gpk_std_string.h` -> `llc_std_string.h`
- [x] `gpk_stdsocket.h` -> `llc_stdsocket.h`
- [x] `gpk_stdstring.{h,cpp}` -> `llc_stdstring.{h,cpp}`
- [x] `gpk_string.h` -> `llc_string.h`
- [x] `gpk_string_compose.{h,cpp}` -> `llc_string_compose.{h,cpp}`
- [x] `gpk_sync.h` -> `llc_sync.h`
- [x] `gpk_system_event.h` -> `llc_system_event.h`
- [x] `gpk_tcpip.{h,cpp}` -> `llc_tcpip.{h,cpp}`
- [x] `gpk_timer.{h,cpp}` -> `llc_timer.{h,cpp}`
- [x] `gpk_tri.h` -> `llc_tri.h`
- [x] `gpk_tri2.h` -> `llc_tri2.h`
- [x] `gpk_tri3.h` -> `llc_tri3.h`
- [x] `gpk_typeint.{h,cpp}` -> `llc_typeint.{h,cpp}`
- [x] `gpk_view.{h,cpp}` -> `llc_view.{h,cpp}`
- [x] `gpk_view_bit.h` -> `llc_view_bit.h`
- [x] `gpk_view_color.{h,cpp}` -> `llc_view_color.{h,cpp}`
- [x] `gpk_view_serialize.h` -> `llc_view_serialize.h`
- [x] `gpk_windows.{h,cpp}` -> `llc_windows.{h,cpp}`
- [x] `gpk_xml_reader.{h,cpp}` -> `llc_xml_reader.{h,cpp}`
- [x] `llc_runtime.h` -> `llc_runtime.h`

### Rejected deliberately

- [x] `gpk_gauge_n2.h` and `gpk_gauge_n3.h` -> invalid wrappers around the removed historical `gauge<>`; generic gauge owners retained in `llc_gauge.h`

### Pending disposition/no current counterpart

- [ ] `gpk_apod_gauge.h`
- [ ] `gpk_apod_minmax.h`
- [ ] `gpk_app_impl.h`
- [ ] `gpk_array_bit.h`
- [ ] `gpk_ascii_color.h`
- [ ] `gpk_ascii_target.{h,cpp}`
- [ ] `gpk_astatic_color.h`
- [ ] `gpk_astatic_n2.h`
- [ ] `gpk_axis (2).h`
- [ ] `gpk_bitmap_target.h`
- [ ] `gpk_block.{h,cpp}`
- [ ] `gpk_broker.{h,cpp}`
- [ ] `gpk_camera.h`
- [ ] `gpk_cdn.h`
- [ ] `gpk_cgi.{h,cpp}`
- [ ] `gpk_cgi_app_impl.h`
- [ ] `gpk_cgi_app_impl_v2.h`
- [ ] `gpk_cgi_main.cpp`
- [ ] `gpk_cgi_module.{h,cpp}`
- [ ] `gpk_cgi_runtime.{h,cpp}`
- [ ] `gpk_collision.h`
- [ ] `gpk_complus.h`
- [ ] `gpk_component_scene.{h,cpp}`
- [ ] `gpk_component_scene_draw.cpp`
- [ ] `gpk_d3d.{h,cpp}`
- [ ] `gpk_dialog.{h,cpp}`
- [ ] `gpk_dialog_controls.h`
- [ ] `gpk_expression.{h,cpp}`
- [ ] `gpk_font.{h,cpp}`
- [ ] `gpk_framework.{h,cpp}`
- [ ] `gpk_geometry.{h,cpp}`
- [ ] `gpk_geometry_buffers.h`
- [ ] `gpk_geometry_draw.{h,cpp}`
- [ ] `gpk_geometry_lh.{h,cpp}`
- [ ] `gpk_gltf.{h,cpp}`
- [ ] `gpk_gpio.h`
- [ ] `gpk_grid_copy.h`
- [ ] `gpk_grid_scale.h`
- [ ] `gpk_grid_static.h`
- [ ] `gpk_gtl_command.h`
- [ ] `gpk_gui.{h,cpp}`
- [ ] `gpk_gui_control.h`
- [ ] `gpk_gui_control_list.{h,cpp}`
- [ ] `gpk_gui_control_state.h`
- [ ] `gpk_gui_desktop.{h,cpp}`
- [ ] `gpk_gui_draw.cpp`
- [ ] `gpk_gui_inputbox.{h,cpp}`
- [ ] `gpk_gui_text.h`
- [ ] `gpk_gui_viewport.{h,cpp}`
- [ ] `gpk_http.h`
- [ ] `gpk_http_client.{h,cpp}`
- [ ] `gpk_https_client.cpp`
- [ ] `gpk_img_serialize.h`
- [ ] `gpk_input.h`
- [ ] `gpk_io.h`
- [ ] `gpk_json_expression.{h,cpp}`
- [ ] `gpk_keyed_bit_array.h`
- [ ] `gpk_linear_map_pobj.h`
- [ ] `gpk_linear_map_pod.h`
- [ ] `gpk_mapblock.cpp`
- [ ] `gpk_member_registry.h`
- [ ] `gpk_model.h`
- [ ] `gpk_module.h`
- [ ] `gpk_particle.{h,cpp}`
- [ ] `gpk_platform_error.cpp`
- [ ] `gpk_pod_definition.h`
- [ ] `gpk_process.{h,cpp}`
- [ ] `gpk_raster_lh.{h,cpp}`
- [ ] `gpk_rect_align.h`
- [ ] `gpk_rsa.{h,cpp}`
- [ ] `gpk_runtime_module.{h,cpp}`
- [ ] `gpk_scene.{h,cpp}`
- [ ] `gpk_serial.h`
- [ ] `gpk_stl.{h,cpp}`
- [ ] `gpk_string_helper.{h,cpp}`
- [ ] `gpk_swap.h`
- [ ] `gpk_system_key.h`
- [ ] `gpk_type_identifier.h`
- [ ] `gpk_type_registry.h`
- [ ] `gpk_view_layered.h`
- [ ] `gpk_view_manager.h`
- [ ] `gpk_view_n2.h`
- [ ] `gpk_view_n3.h`
- [ ] `gpk_view_pobj.h`
- [ ] `gpk_view_ppod.h`
- [ ] `gpk_view_stream.h`
- [ ] `gpk_view_tri.h`
- [ ] `gpk_view_tri2.h`
- [ ] `gpk_view_tri3.h`
- [ ] `gpk_virtual_keyboard.{h,cpp}`
- [ ] `gpk_vox.h`
- [ ] `gpk_voxel.h`
- [ ] `gpk_voxel_geometry.{h,cpp}`
- [ ] `gpk_window.{h,cpp}`
- [ ] `llc_header_checkingg.cpp`

## Evidence boundary

The filename inventory was refreshed from both working trees on 2026-10-08. It is not a complete API-diff proof and does not authorize implementation of a pending item. The generic block-container and label changes represented here were subsequently built and exercised by Pablo, who reported the complete test run passing. The geometry migration was source-reviewed and registered in the project and test suite on 2026-10-09; it has not been built or executed. No build, test or Git mutation was performed as part of this ledger refresh, and unrelated working-tree changes were preserved.

