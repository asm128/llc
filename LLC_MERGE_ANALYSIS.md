# LLC final-merge analysis

Created: `2026-10-26` (`America/Buenos_Aires`).

This record evaluates the 351 canonical filenames in `LLC_FILE_INVENTORY.md` across the four supplied snapshots, in checklist order. It defines what the final merge needs and why; it does not modify product code or tests.

**Revalidation status:** this prior per-file analysis and merge plan is retained as evidence, but Pablo reset every live checklist entry to `0-Unknown` on `2026-10-26`. No conclusion below becomes authoritative again until that file is re-evaluated against the current sources.

## Source labels

- `thecure`: `D:/enterprise/programming_sciences/codebases/llb/llc/llc`, branch evidence `thecure`.
- `GPK`: `D:/enterprise/programming_sciences/codebases/bundle_galaxy_hell/gpk/llc`; `gpk_*` names and source tokens are normalized to `llc_*` only for comparison.
- `repairs-master`: `D:/enterprise/programming_sciences/codebases/repairs/llc/llc`, branch evidence `master`.
- `sanitization`: `D:/enterprise/programming_sciences/codebases/pristinellc/llc/llc`, branch `sanitization`.

The raw directory evidence and timestamps remain in the four `LLC_FILE_INVENTORY_SOURCE*.txt` annexes. Filename presence or normalized text equality is evidence, not by itself proof of behavioral equivalence.

Repository-state parentheticals in per-file source evidence record the sanitization working/index snapshot captured when evaluation began. The index was externally emptied later in the pass, so those parentheticals are historical evidence, not a claim about the final live index.

## Decision rules

1. Historical GPK behavior is the default contract for direct migrations. Namespace/type substitutions required by current LLC may change spelling, not behavior.
2. LLC-native facilities are retained only with their own consumer/contract evidence; they are not forced into a fictitious GPK counterpart.
3. GUI, viewport, input-box and virtual-keyboard migrations are mechanical GPK migrations except for the three explicitly accepted changes: successful size/zoom publication by `updateFramework()`, GPU corner rasterization and the render-target resize event.
4. D3D begins from the as-is GPK ownership/lifecycle contract. Flattened device/scene/text/GUI ownership and extra clear/present cycles are not accepted without demonstrated necessity.
5. Event semantics remain above the carrier. UDP/session separation preserves legacy public overloads, caller-supplied callbacks and rejection of invalid packets before delivery.
6. WordDict preserves the working Obelisco contract: accented bytes map to ASCII, scratch slots remain preallocated and skipped words do not invent compaction semantics.
7. Solar-system rendering preserves both historical rendering routes. Framework metric publication belongs to `updateFramework()` after successful recalculation; `guiUpdateMetrics()` does not own cached size/zoom publication.
8. No source is changed merely to satisfy a newly invented test. Every `Required tests` item below is a test obligation for the separate test pass, not a claim that it already passes.

## Per-file evaluation

### 001 - `llc_adam7.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Sources:** present in all four snapshots. Normalized line counts are thecure 34, GPK 53, repairs-master 34 and sanitization 34. Thecure and sanitization are text-identical after line-ending normalization. Repairs-master has the same generalized algorithm with the older error macro; GPK has a hand-expanded seven-pass calculation.
- **Differences:** the modern implementation validates storage for seven results and derives every pass from `adam7ScaleIndex()`'s base/multiplier pair. GPK duplicates seven formulas and returns zero for valid pixels in small images; for example its pass-zero test rejects a `1x1` image, although Adam7 must cover that pixel. The generalized form also makes the size calculation and interlace coordinate calculation share one table.
- **Final merge requirement:** retain the generalized thecure/sanitization implementation and current error-propagating macro spelling. Do not restore GPK's expanded formulas. Keep the public function names and seven-pass table contract used by PNG.
- **Why:** this is a concrete behavioral correction: pass sizes must cover every source pixel exactly once for every positive image size, and the GPK formula fails that invariant for small dimensions. The modern form is also the common current implementation in two supplied snapshots, rather than an isolated sanitization edit.
- **Required tests:** known Adam7 multiplier/base vectors; rejection of pass indexes `-1` and `7`; rejection of fewer than seven output slots; exact `8x8` pass sizes; exhaustive pixel-count coverage for widths/heights at least `1..17`; interlace placement; and decoding at least one real interlaced PNG including a dimension smaller than eight.

### 002 - `llc_adam7.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Sources:** present in all four snapshots, 36 lines in each. GPK, repairs-master and thecure expose the same declarations and templated interlace traversal after namespace/type normalization. Sanitization differs only at the `adam7ScaleIndex()` error-propagation call, using the current `if_fail_fef` spelling instead of the removed `*_necall` macro.
- **Differences:** the public surface remains `adam7Sizes`, `adam7ScaleIndex` and templated `adam7Interlace`. Coordinate placement, bounds checks and source-to-target traversal are unchanged; there is no new algorithm in the sanitization header.
- **Final merge requirement:** keep the sanitization header's current error macro and otherwise preserve the historical declarations and interlace traversal byte-for-byte modulo current LLC typedefs/formatting. It must remain paired with the generalized `llc_adam7.cpp` size calculation documented above.
- **Why:** this is a required macro migration, not authority to alter the interlace contract. Restoring the removed macro would break the current macro API; changing traversal would create an unrelated PNG behavior change.
- **Required tests:** compile-time instantiation for at least two texel types; propagation of an invalid pass error; clipping when a supplied pass extends beyond the output grid; exact placement of all seven `8x8` passes; and the real interlaced-PNG decode required by `llc_adam7.cpp`.

### 003 - `llc_aes.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 468L/AAF645B931C5; GPK gpk_aes.cpp 468L/9BD96EA4AF71; repairs-master 468L/9BD96EA4AF71; sanitization 469L/CECA16CF6CC8 (clean).
- **Text relationship:** Normalized-text matches: GPK/gpk_aes.cpp = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the AES algorithm and existing ciphertext layout while using current error macros; require a 32-byte key, exact IV size, block-aligned ciphertext and fully validated PKCS#7 padding before resize.
- **Why:** the sanitization delta is mostly macro/error migration, but the API otherwise can read a short IV or accept an invalid padding length; those are concrete bounds/format failures, not license to change the wire format.
- **Required tests:** standard AES vectors at every supported level, appended-IV roundtrip, short IV/key rejection, empty and non-block ciphertext rejection, every legal pad length and corrupt padding.

### 004 - `llc_aes.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 83L/AACEFEDDF4B7; GPK gpk_aes.h 103L/CD3E4B3EC676; repairs-master 83L/AACEFEDDF4B7; sanitization 83L/AACEFEDDF4B7 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the AES algorithm and existing ciphertext layout while using current error macros; require a 32-byte key, exact IV size, block-aligned ciphertext and fully validated PKCS#7 padding before resize.
- **Why:** the sanitization delta is mostly macro/error migration, but the API otherwise can read a short IV or accept an invalid padding length; those are concrete bounds/format failures, not license to change the wire format.
- **Required tests:** standard AES vectors at every supported level, appended-IV roundtrip, short IV/key rejection, empty and non-block ciphertext rejection, every legal pad length and corrupt padding.

### 005 - `llc_align.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 28L/E543AC9207DF; GPK gpk_align.h 31L/79EDA04D8153; repairs-master 28L/E543AC9207DF; sanitization 28L/E543AC9207DF (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 006 - `llc_aobj_pobj.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 20L/D342F9F48954; GPK gpk_aobj_pobj.h 22L/4BFEE17F992E; repairs-master 20L/D342F9F48954; sanitization 20L/D342F9F48954 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 007 - `llc_aobj_ppod.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 15L/0CE26752F0DD; GPK gpk_aobj_ppod.h 14L/701E8045F2AC; repairs-master 15L/0CE26752F0DD; sanitization 15L/0CE26752F0DD (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 008 - `llc_apod_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/A845254FABD3; GPK gpk_apod_color.h 17L/15F79168C247; repairs-master 18L/A845254FABD3; sanitization 18L/A845254FABD3 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 009 - `llc_apod_gauge.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_apod_gauge.h 26L/270FBC2AF5F5; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 010 - `llc_apod_minmax.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 26L/BBB85D029681; GPK gpk_apod_minmax.h 26L/F2CB1F51AF00; repairs-master 26L/BBB85D029681; sanitization 26L/BBB85D029681 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 011 - `llc_apod_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/A5E271D3B4BC; GPK gpk_apod_n2.h 30L/7BF0E0ADC2A3; repairs-master 29L/A5E271D3B4BC; sanitization 29L/A5E271D3B4BC (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 012 - `llc_apod_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 30L/ED9AD699E739; GPK gpk_apod_n3.h 31L/60FCC128A408; repairs-master 30L/ED9AD699E739; sanitization 30L/ED9AD699E739 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 013 - `llc_apod_range.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 25L/ECB48BF653A4; GPK gpk_apod_range.h 26L/9A0EC57029AD; repairs-master 25L/ECB48BF653A4; sanitization 25L/ECB48BF653A4 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 014 - `llc_apod_serialize.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 59L/4CC677216E0C; GPK gpk_apod_serialize.h 63L/11EC37A91861; repairs-master 62L/C2970BB3EA12; sanitization 59L/4CC677216E0C (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 015 - `llc_apod_slice.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 25L/9DB25536FF1B; GPK gpk_apod_slice.h 26L/ADD342FF9868; repairs-master 25L/9DB25536FF1B; sanitization 25L/9DB25536FF1B (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 016 - `llc_apod_tri.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/A69158E3AEC2; GPK gpk_apod_tri.h 30L/295134B55C0D; repairs-master 29L/A69158E3AEC2; sanitization 29L/A69158E3AEC2 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 017 - `llc_apod_tri2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/ED3971C724D7; GPK gpk_apod_tri2.h 30L/790CC7852033; repairs-master 29L/ED3971C724D7; sanitization 29L/ED3971C724D7 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 018 - `llc_apod_tri3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/FB8D02CE6449; GPK gpk_apod_tri3.h 30L/E621D52A821F; repairs-master 29L/FB8D02CE6449; sanitization 29L/FB8D02CE6449 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 019 - `llc_app_impl.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK absent; repairs-master 1L/407A9501E671; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 020 - `llc_app_impl.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 128L/20734EF717AB; GPK gpk_app_impl.h 198L/3070A2F45A42; repairs-master 207L/C8DF95402F88; sanitization 128L/20734EF717AB (untracked).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 021 - `llc_append_css.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 47L/D93B870EFC04; GPK gpk_append_css.cpp 47L/98FB2D2922B9; repairs-master 47L/600BAC82944C; sanitization 47L/577B0AB2D396 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 022 - `llc_append_css.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 20L/739E73748929; GPK gpk_append_css.h 19L/5E7DD6D97549; repairs-master 20L/C55A2513C0BF; sanitization 19L/9BB43C6222F9 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 023 - `llc_append_frontend.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/D2CE68C39A03; GPK gpk_append_frontend.h 15L/E974833D3BF7; repairs-master 14L/D2CE68C39A03; sanitization 14L/C5DDC57F3870 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 024 - `llc_append_html.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 55L/ADB1ECA46B42; GPK gpk_append_html.cpp 57L/E5B2A03C81D4; repairs-master 55L/0C9E03B83166; sanitization 57L/D847EF20C580 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 025 - `llc_append_html.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/2B06B9602BCC; GPK gpk_append_html.h 24L/62E86253602D; repairs-master 24L/2B06B9602BCC; sanitization 24L/71A35F09102E (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 026 - `llc_append_js.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 1L/E9D748E68595; GPK gpk_append_js.cpp 1L/E9D748E68595; repairs-master 1L/E9D748E68595; sanitization 1L/E9D748E68595 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 027 - `llc_append_js.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 11L/B59B0FA15F6F; GPK gpk_append_js.h 11L/B59B0FA15F6F; repairs-master 11L/B59B0FA15F6F; sanitization 11L/B59B0FA15F6F (unstaged modification).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 028 - `llc_append_json.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/9699B3B027E2; GPK gpk_append_json.cpp 14L/D07D438DB783; repairs-master 14L/13C61629D1E9; sanitization 14L/C940B6082F97 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 029 - `llc_append_json.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/F7DF6AD101D0; GPK gpk_append_json.h 18L/8FF06922C7DD; repairs-master 18L/F7DF6AD101D0; sanitization 17L/664D203E5DFA (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 030 - `llc_append_tcpip.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 37L/F9C4D60C86D5; GPK gpk_append_tcpip.cpp 37L/36517708FE43; repairs-master 37L/F9C4D60C86D5; sanitization 37L/05ABC2C47786 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 031 - `llc_append_tcpip.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 17L/0D8C800AEE7A; GPK gpk_append_tcpip.h 16L/E4320F4BAA86; repairs-master 17L/0D8C800AEE7A; sanitization 16L/E4320F4BAA86 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master; GPK/gpk_append_tcpip.h = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 032 - `llc_append_xml.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 57L/31BC4D60E30F; GPK gpk_append_xml.cpp 57L/307429AF12AF; repairs-master 57L/31BC4D60E30F; sanitization 57L/0A88C3DC925F (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 033 - `llc_append_xml.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 21L/A697257A8982; GPK gpk_append_xml.h 20L/8304F8F66AFC; repairs-master 21L/A697257A8982; sanitization 20L/B38C7E08AF77 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 034 - `llc_arduino_stream.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 26L/EE0EC9CE1BC3; GPK absent; repairs-master 26L/EE0EC9CE1BC3; sanitization 26L/EE0EC9CE1BC3 (unstaged modification).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 035 - `llc_arduino_string.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 20L/F637800DA576; GPK absent; repairs-master 20L/F962F1BAD3FF; sanitization 20L/F637800DA576 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 036 - `llc_arduino_string.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/300E007C4331; GPK gpk_arduino_string.h 24L/FE06F35C90BC; repairs-master 29L/300E007C4331; sanitization 29L/300E007C4331 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 037 - `llc_arduino.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/97E5AFCB6EE8; GPK absent; repairs-master 24L/97E5AFCB6EE8; sanitization 24L/97E5AFCB6EE8 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 038 - `llc_args.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 154L/75A84DF8A8D2; GPK gpk_args.cpp 114L/6D02CA643E18; repairs-master 114L/BFA15F1DBE5E; sanitization 160L/79AE3B63F21A (untracked).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the Positionals/Options command-line model without an `Argv` duplicate.
- **Why:** the same datum must not be stored under another name when `Positionals[0]` or `Options[0]` owns it.
- **Required tests:** positional/option parsing, empty values, repeated options and proof no `Argv` member is required.

### 039 - `llc_args.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 30L/E184CD536281; GPK gpk_args.h 31L/C8E499FA41CA; repairs-master 26L/DB0338091CAE; sanitization 30L/E184CD536281 (untracked).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the Positionals/Options command-line model without an `Argv` duplicate.
- **Why:** the same datum must not be stored under another name when `Positionals[0]` or `Options[0]` owns it.
- **Required tests:** positional/option parsing, empty values, repeated options and proof no `Argv` member is required.

### 040 - `llc_array_base.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 87L/0DCC8CCFB8D9; GPK gpk_array_base.h 87L/08F0EDEC81A5; repairs-master 87L/08F0EDEC81A5; sanitization 87L/BE8D5BCEB6C2 (clean).
- **Text relationship:** Normalized-text matches: GPK/gpk_array_base.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 041 - `llc_array_bit.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 58L/FD310559FE36; GPK gpk_array_bit.h 54L/F1813CF59451; repairs-master 58L/FD310559FE36; sanitization 58L/FD310559FE36 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 042 - `llc_array_circular.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 110L/8CE613126D01; GPK gpk_array_circular.h 110L/B330008622E7; repairs-master 110L/7B5463057624; sanitization 110L/8CE613126D01 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 043 - `llc_array_obj.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 333L/DE39E9BC6B03; GPK gpk_array_obj.h 357L/18A5D40F891C; repairs-master 342L/3DB8A016E73D; sanitization 333L/DE39E9BC6B03 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 044 - `llc_array_pod.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 362L/42E28AF251AD; GPK gpk_array_pod.h 330L/D80E9DD7CC9B; repairs-master 362L/9E2382836FDF; sanitization 362L/42E28AF251AD (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 045 - `llc_array_ptr.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/51577F1B1C86; GPK gpk_array_ptr.h 41L/FABB0FAA4CA6; repairs-master 41L/51577F1B1C86; sanitization 41L/51577F1B1C86 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 046 - `llc_array_static.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 160L/3317BD048DBA; GPK gpk_array_static.h 162L/665D05C690E8; repairs-master 160L/79C16D8C6CCC; sanitization 160L/F2381ABBF563 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 047 - `llc_array.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 19L/06DF1E166ECA; GPK gpk_array.cpp 40L/61CA55261E98; repairs-master 19L/BEDCD6BEC317; sanitization 19L/06DF1E166ECA (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 048 - `llc_array.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 27L/E3FEC12CE135; GPK gpk_array.h 27L/E3FEC12CE135; repairs-master 27L/E3FEC12CE135; sanitization 27L/E3FEC12CE135 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 049 - `llc_ascii_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 75L/070E829486A8; GPK gpk_ascii_color.h 79L/14B56D836ED1; repairs-master 75L/070E829486A8; sanitization 75L/BA6ACE0BB319 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the modern implementation only for demonstrated bounds/error corrections while preserving the historical image/PNG format and raster behavior.
- **Why:** image changes are observable and prior reviews found both real safety bugs and invented unrealistic requirements.
- **Required tests:** small/large dimensions, overflow boundaries, clipped views, interlaced/noninterlaced PNG roundtrip and malformed chunk/filter cases.

### 050 - `llc_ascii_target.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 12L/A9F344762BB1; GPK gpk_ascii_target.cpp 12L/C1BD50F69306; repairs-master 12L/A9F344762BB1; sanitization 12L/A9F344762BB1 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the modern implementation only for demonstrated bounds/error corrections while preserving the historical image/PNG format and raster behavior.
- **Why:** image changes are observable and prior reviews found both real safety bugs and invented unrealistic requirements.
- **Required tests:** small/large dimensions, overflow boundaries, clipped views, interlaced/noninterlaced PNG roundtrip and malformed chunk/filter cases.

### 051 - `llc_ascii_target.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 51L/0037F1E5418B; GPK gpk_ascii_target.h 132L/9C4A0E73218F; repairs-master 51L/0037F1E5418B; sanitization 51L/0037F1E5418B (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the modern implementation only for demonstrated bounds/error corrections while preserving the historical image/PNG format and raster behavior.
- **Why:** image changes are observable and prior reviews found both real safety bugs and invented unrealistic requirements.
- **Required tests:** small/large dimensions, overflow boundaries, clipped views, interlaced/noninterlaced PNG roundtrip and malformed chunk/filter cases.

### 052 - `llc_astatic_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 17L/0548D0EC9F93; GPK gpk_astatic_color.h 17L/0548D0EC9F93; repairs-master 17L/0548D0EC9F93; sanitization 17L/0548D0EC9F93 (untracked).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 053 - `llc_astatic_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 30L/FF391F1C844A; GPK gpk_astatic_n2.h 32L/972C8CF19CF2; repairs-master 30L/FF391F1C844A; sanitization 30L/FF391F1C844A (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 054 - `llc_astatic_serialize.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 22L/2EEF2D3B2D98; GPK gpk_astatic_serialize.h 22L/BDD8368FB020; repairs-master 22L/BDD8368FB020; sanitization 22L/2EEF2D3B2D98 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization; GPK/gpk_astatic_serialize.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 055 - `llc_auto_handler.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 32L/C4BDB6688533; GPK gpk_auto_handler.h 32L/A2E553E676BF; repairs-master 32L/C4BDB6688533; sanitization 32L/2ABC74BF922A (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 056 - `llc_axis (2).h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_axis (2).h 53L/231A59DD990F; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude the duplicate filename from the final tree; use `llc_axis.h` as the sole canonical owner.
- **Why:** the parenthesized file is only a historical duplicate and two owners invite divergent definitions.
- **Required tests:** inventory check proving only the canonical path is registered.

### 057 - `llc_axis.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 54L/67D548648D3D; GPK gpk_axis.h 53L/231A59DD990F; repairs-master 54L/980E168DD68B; sanitization 54L/C8FA8F70DA2F (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 058 - `llc_base64.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 145L/F2FE566CF057; GPK gpk_base64.cpp 106L/04676A853C96; repairs-master 143L/BC75F221A6C6; sanitization 145L/BCA3A0E6ABBD (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the modern implementation only where it preserves the existing public wire/text format and propagates every allocation/parse failure; do not select a variant from hash or line count alone.
- **Why:** codec compatibility is externally observable and silent error suppression was already found in recovered code.
- **Required tests:** published known vectors, roundtrip, malformed/truncated input, zero length, boundary sizes and allocation/error propagation.

### 059 - `llc_base64.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 85L/181989432C65; GPK gpk_base64.h 51L/F5746E5387F3; repairs-master 85L/181989432C65; sanitization 85L/F3F60C5C5D1A (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the modern implementation only where it preserves the existing public wire/text format and propagates every allocation/parse failure; do not select a variant from hash or line count alone.
- **Why:** codec compatibility is externally observable and silent error suppression was already found in recovered code.
- **Required tests:** published known vectors, roundtrip, malformed/truncated input, zero length, boundary sizes and allocation/error propagation.

### 060 - `llc_bit.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 120L/45123A4F4C6C; GPK gpk_bit.h 51L/66CD97D13D44; repairs-master 116L/A4DE72CF5884; sanitization 120L/45123A4F4C6C (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 061 - `llc_bitmap_target.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 279L/28C304C5139C; GPK gpk_bitmap_target.h 360L/CC2D2F166E1B; repairs-master 279L/28C304C5139C; sanitization 279L/28C304C5139C (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the modern implementation only for demonstrated bounds/error corrections while preserving the historical image/PNG format and raster behavior.
- **Why:** image changes are observable and prior reviews found both real safety bugs and invented unrealistic requirements.
- **Required tests:** small/large dimensions, overflow boundaries, clipped views, interlaced/noninterlaced PNG roundtrip and malformed chunk/filter cases.

### 062 - `llc_block_container_nts.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 33L/2EC5BCEBAEEB; GPK gpk_block_container_nts.h 54L/C9DAF30878F3; repairs-master 33L/2EC5BCEBAEEB; sanitization 33L/FDA6D65463D9 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 063 - `llc_block_container.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 87L/BA0CE2015105; GPK gpk_block_container.h 56L/0AC8AA2FF872; repairs-master 87L/BA0CE2015105; sanitization 87L/F46724FF2F10 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 064 - `llc_block.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 21L/9305F159E42C; GPK gpk_block.cpp 51L/555BF38E1DDC; repairs-master 21L/9305F159E42C; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 065 - `llc_block.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/E86BCA60AD9E; GPK gpk_block.h 254L/0668C9DD3C1A; repairs-master 18L/E86BCA60AD9E; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 066 - `llc_broker.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_broker.cpp 1L/335FE57193B7; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 067 - `llc_broker.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_broker.h 16L/38D0A442998F; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 068 - `llc_bus_manager.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/A5A8A8905BD5; GPK absent; repairs-master 18L/A5A8A8905BD5; sanitization 18L/A5A8A8905BD5 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 069 - `llc_bus.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 37L/443DE8F83B48; GPK absent; repairs-master 37L/5466D3A26A21; sanitization 37L/823CB87FAF30 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 070 - `llc_camera.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 28L/338F380A14A9; GPK gpk_camera.h 29L/B289DB65453F; repairs-master 28L/338F380A14A9; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 071 - `llc_ccsds.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/70C277C65D35; GPK absent; repairs-master 41L/96F09AEE275E; sanitization 41L/DC9FB13EAF0B (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 072 - `llc_ccsds.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 117L/384151014107; GPK absent; repairs-master 117L/F62C2C22E4AD; sanitization 118L/1A5B43F15B98 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 073 - `llc_cdn.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cdn.h 45L/B654AF7D11C9; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 074 - `llc_cerrno.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 149L/963E43C9B0FE; GPK gpk_cerrno.h 149L/963E43C9B0FE; repairs-master 149L/963E43C9B0FE; sanitization 149L/963E43C9B0FE (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 075 - `llc_cgi_app_impl_v2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_app_impl_v2.h 34L/537BF6CA853E; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 076 - `llc_cgi_app_impl.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_app_impl.h 38L/15E56ECA4B4B; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 077 - `llc_cgi_main.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_main.cpp 16L/E0B6FBE0A6A3; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 078 - `llc_cgi_module.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_module.cpp 24L/61E76DA4EA27; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 079 - `llc_cgi_module.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_module.h 56L/AA2A264F7357; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 080 - `llc_cgi_runtime.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_runtime.cpp 218L/200F01F95DCB; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 081 - `llc_cgi_runtime.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi_runtime.h 137L/69CFAEC5D4F0; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 082 - `llc_cgi.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi.cpp 69L/B460364D64B8; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 083 - `llc_cgi.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_cgi.h 14L/200AF89929B2; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 084 - `llc_chrono.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 78L/850F52705395; GPK gpk_chrono.cpp 38L/4F36E6F08309; repairs-master 38L/4F36E6F08309; sanitization 78L/3A87E70D2829 (unstaged modification).
- **Text relationship:** Normalized-text matches: GPK/gpk_chrono.cpp = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 085 - `llc_chrono.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 40L/4981F1C4ED06; GPK gpk_chrono.h 35L/0DAF8A07B95C; repairs-master 35L/B74DB6FEA001; sanitization 40L/787AE387C839 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 086 - `llc_circle.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 32L/1DAAA53EA19B; GPK gpk_circle.h 30L/90F2B57B194C; repairs-master 32L/1DAAA53EA19B; sanitization 32L/C14AE917E525 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 087 - `llc_circuit.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 31L/A4A92FE2F8E8; GPK absent; repairs-master 31L/A4A92FE2F8E8; sanitization 14L/78FDE338D41E (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 088 - `llc_circuit.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 15L/DFA92C75B33A; GPK absent; repairs-master 15L/DFA92C75B33A; sanitization 13L/4C9BF3C2FE62 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 089 - `llc_collision.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 163L/552B8CD41FEB; GPK gpk_collision.h 151L/A24C8C9C2156; repairs-master 163L/552B8CD41FEB; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 090 - `llc_color_type.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 23L/71B6E6E294A2; GPK gpk_color_type.h 23L/807F6C6F0FE1; repairs-master 23L/71B6E6E294A2; sanitization 23L/71B6E6E294A2 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 091 - `llc_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 313L/B75F22E3DA9E; GPK gpk_color.h 317L/5683EDB6C3BA; repairs-master 313L/3BF7BD72AEC0; sanitization 313L/B75F22E3DA9E (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 092 - `llc_complus.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 100L/914EBE3F85F2; GPK gpk_complus.h 101L/536A4C4FFEDC; repairs-master 128L/35B837B4DE96; sanitization 101L/9AA3D851EDAF (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the reviewed ownership contract: move transfers and nulls the source, assignment releases the old object exactly once, and AddRef/Release order is exception-safe; no invented inherited assignment imports.
- **Why:** pointer wrappers are ABI/lifetime owners and prior mechanical changes risked leaks, double release or self-assignment corruption.
- **Required tests:** copy/move/self assignment, null/non-null transitions, exact AddRef/Release counts and destruction order.

### 093 - `llc_component_scene_draw.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_component_scene_draw.cpp 129L/AE4AEC122F2B; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 094 - `llc_component_scene.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_component_scene.cpp 313L/7D186B3F7E76; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 095 - `llc_component_scene.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_component_scene.h 310L/3DADFAE7FD1D; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 096 - `llc_connection_ip.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 23L/D377CF7EB2DF; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the split connection design while preserving every legacy public overload: socket/IP/address are outside session state, callbacks are caller parameters rather than per-connection storage, and rejected commands discard the remaining invalid packet before delivery.
- **Why:** this achieves separation without an API break, per-connection callback duplication or exposure of invalid serialized frames.
- **Required tests:** legacy-call compilation, 20k-connection footprint, accepted/rejected command framing, truncation, payload split/reassembly and caller callback delivery.

### 097 - `llc_connection_ip.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 21L/A57E3822A54B; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the split connection design while preserving every legacy public overload: socket/IP/address are outside session state, callbacks are caller parameters rather than per-connection storage, and rejected commands discard the remaining invalid packet before delivery.
- **Why:** this achieves separation without an API break, per-connection callback duplication or exposure of invalid serialized frames.
- **Required tests:** legacy-call compilation, 20k-connection footprint, accepted/rejected command framing, truncation, payload split/reassembly and caller callback delivery.

### 098 - `llc_connection.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 527L/67A7BA989822; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the split connection design while preserving every legacy public overload: socket/IP/address are outside session state, callbacks are caller parameters rather than per-connection storage, and rejected commands discard the remaining invalid packet before delivery.
- **Why:** this achieves separation without an API break, per-connection callback duplication or exposure of invalid serialized frames.
- **Required tests:** legacy-call compilation, 20k-connection footprint, accepted/rejected command framing, truncation, payload split/reassembly and caller callback delivery.

### 099 - `llc_connection.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 146L/30BD9B761114; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the split connection design while preserving every legacy public overload: socket/IP/address are outside session state, callbacks are caller parameters rather than per-connection storage, and rejected commands discard the remaining invalid packet before delivery.
- **Why:** this achieves separation without an API break, per-connection callback duplication or exposure of invalid serialized frames.
- **Required tests:** legacy-call compilation, 20k-connection footprint, accepted/rejected command framing, truncation, payload split/reassembly and caller callback delivery.

### 100 - `llc_coord.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/12D93C25EA8C; GPK gpk_coord.h 19L/EECC7AF6B625; repairs-master 18L/12D93C25EA8C; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 101 - `llc_cpow.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 38L/7F99DFCC7508; GPK gpk_cpow.h 40L/224C704DABE6; repairs-master 40L/224C704DABE6; sanitization 38L/7F99DFCC7508 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization; GPK/gpk_cpow.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 102 - `llc_crc.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 32L/94DAD2BD03E7; GPK absent; repairs-master absent; sanitization 31L/E13B5E6BBC4E (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the modern implementation only where it preserves the existing public wire/text format and propagates every allocation/parse failure; do not select a variant from hash or line count alone.
- **Why:** codec compatibility is externally observable and silent error suppression was already found in recovered code.
- **Required tests:** published known vectors, roundtrip, malformed/truncated input, zero length, boundary sizes and allocation/error propagation.

### 103 - `llc_crc.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/16EBEE61462D; GPK absent; repairs-master absent; sanitization 14L/16EBEE61462D (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the modern implementation only where it preserves the existing public wire/text format and propagates every allocation/parse failure; do not select a variant from hash or line count alone.
- **Why:** codec compatibility is externally observable and silent error suppression was already found in recovered code.
- **Required tests:** published known vectors, roundtrip, malformed/truncated input, zero length, boundary sizes and allocation/error propagation.

### 104 - `llc_cstdio.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 44L/DB5DB48BC059; GPK gpk_cstdio.h 49L/7275C962508C; repairs-master 44L/DABB9AEF8284; sanitization 44L/DB5DB48BC059 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 105 - `llc_cstring.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 130L/32B980946DA9; GPK gpk_cstring.h 130L/59A5AF5D4BD1; repairs-master 130L/23CBC8BB0BD9; sanitization 130L/32B980946DA9 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 106 - `llc_ctti.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 260L/94FAB17B35B0; GPK absent; repairs-master 260L/94FAB17B35B0; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 107 - `llc_d3d.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_d3d.cpp 61L/A9E4AE19643F; repairs-master 61L/A9E4AE19643F; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the as-is GPK D3D device/scene/text/GUI ownership and lifecycle before applying only necessary LLC type substitutions.
- **Why:** flattening the owners and allowing scene and GUI paths to each clear/present changes frame ownership and was not justified.
- **Required tests:** device loss/reset, one clear and one present per frame, scene/text/GUI ordering, resize and real sample rendering.

### 108 - `llc_d3d.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_d3d.h 20L/A5CB700E9401; repairs-master 20L/A5CB700E9401; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the as-is GPK D3D device/scene/text/GUI ownership and lifecycle before applying only necessary LLC type substitutions.
- **Why:** flattening the owners and allowing scene and GUI paths to each clear/present changes frame ownership and was not justified.
- **Required tests:** device loss/reset, one clear and one present per frame, scene/text/GUI ordering, resize and real sample rendering.

### 109 - `llc_datatype.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 375L/A2886F9D2E98; GPK gpk_datatype.cpp 32L/C4C1CCEF39E4; repairs-master 375L/7A5EC989EAA7; sanitization 375L/A2886F9D2E98 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 110 - `llc_datatype.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 551L/822E964CC935; GPK gpk_datatype.h 547L/DC326BABCCCE; repairs-master 551L/4EC8CD92B481; sanitization 551L/822E964CC935 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 111 - `llc_debug.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK absent; repairs-master absent; sanitization 19L/DDD523148A23 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the runtime debug API with breaks disabled by default and no `LLC_PLATFORM_CRT_BREAKPOINT()` reintroduction; a future enable switch must be explicit.
- **Why:** Pablo deliberately disabled automatic breaks and explicitly removed the macro.
- **Required tests:** Debug/Release and Windows/non-Windows compilation, return values and explicit enable/disable behavior if exposed.

### 112 - `llc_debug.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 70L/83C303978222; GPK gpk_debug.h 70L/83C303978222; repairs-master 70L/83C303978222; sanitization 62L/0D81AECF0794 (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_debug.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the runtime debug API with breaks disabled by default and no `LLC_PLATFORM_CRT_BREAKPOINT()` reintroduction; a future enable switch must be explicit.
- **Why:** Pablo deliberately disabled automatic breaks and explicitly removed the macro.
- **Required tests:** Debug/Release and Windows/non-Windows compilation, return values and explicit enable/disable behavior if exposed.

### 113 - `llc_deflate.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 574L/8BBA17389842; GPK gpk_deflate.cpp 340L/FDB5D939ED49; repairs-master 335L/FB09D41EED34; sanitization 574L/4789001578EC (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the repaired deflate/inflate path with propagation from bit I/O, Huffman construction, allocation and final write; preserve the existing compressed format.
- **Why:** the recovered version had concrete unconditional-success paths and a corrupted block-header identifier; those repairs are required, while a format rewrite is not.
- **Required tests:** fixed/dynamic/stored blocks, golden streams, truncation/invalid codes, zero/large input, allocation failure and exact final error propagation.

### 114 - `llc_deflate.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 57L/6A383EB89602; GPK gpk_deflate.h 88L/EC8DC2EE734C; repairs-master 88L/9EF65BF26A37; sanitization 57L/A4D263E14424 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the repaired deflate/inflate path with propagation from bit I/O, Huffman construction, allocation and final write; preserve the existing compressed format.
- **Why:** the recovered version had concrete unconditional-success paths and a corrupted block-header identifier; those repairs are required, while a format rewrite is not.
- **Required tests:** fixed/dynamic/stored blocks, golden streams, truncation/invalid codes, zero/large input, allocation failure and exact final error propagation.

### 115 - `llc_dialog_controls.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_dialog_controls.h 183L/CA1DD4A3A4BE; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** do not restore `SDialog`; keep viewport-based ownership and only migrate a live control primitive if an actual consumer requires it.
- **Why:** Pablo removed unused `SDialog` after the viewport direction was chosen.
- **Required tests:** project-wide no-consumer check plus viewport/control construction tests.

### 116 - `llc_dialog.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_dialog.cpp 354L/83146D2EE6CF; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** do not restore `SDialog`; keep viewport-based ownership and only migrate a live control primitive if an actual consumer requires it.
- **Why:** Pablo removed unused `SDialog` after the viewport direction was chosen.
- **Required tests:** project-wide no-consumer check plus viewport/control construction tests.

### 117 - `llc_dialog.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_dialog.h 151L/CAF03A8DE12C; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** do not restore `SDialog`; keep viewport-based ownership and only migrate a live control primitive if an actual consumer requires it.
- **Why:** Pablo removed unused `SDialog` after the viewport direction was chosen.
- **Required tests:** project-wide no-consumer check plus viewport/control construction tests.

### 118 - `llc_encoding.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 260L/DB3259E4222E; GPK gpk_encoding.cpp 259L/776110945CCB; repairs-master 258L/C0F9903F3DE6; sanitization 260L/994AB96D80DD (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the reviewed encoding/RLE implementation only with every resize, append and hexadecimal parse failure propagated.
- **Why:** bare parenthesized calls previously continued after failed storage or invalid input.
- **Required tests:** hex/base encodings and RLE roundtrip, malformed hex, odd lengths, run boundaries, zero input and injected allocation failure.

### 119 - `llc_encoding.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 69L/8027E126473E; GPK gpk_encoding.h 73L/F74B48FE5AEA; repairs-master 69L/8027E126473E; sanitization 69L/CC3BE7EB7E7F (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the reviewed encoding/RLE implementation only with every resize, append and hexadecimal parse failure propagated.
- **Why:** bare parenthesized calls previously continued after failed storage or invalid input.
- **Required tests:** hex/base encodings and RLE roundtrip, malformed hex, odd lengths, run boundaries, zero input and injected allocation failure.

### 120 - `llc_enum.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 502L/7F07E016969B; GPK gpk_enum.h 502L/4C01FDECD121; repairs-master 502L/05FBBABEC696; sanitization 502L/7F07E016969B (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 121 - `llc_error.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 66L/C23CF37BDB65; GPK absent; repairs-master 66L/C23CF37BDB65; sanitization 66L/C23CF37BDB65 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain current logging/error propagation with debug breaks disabled by default; do not revive removed log-level ownership or per-file opt-out macros.
- **Why:** break behavior was deliberately disabled and logging must not unexpectedly stop applications.
- **Required tests:** callback routing, quiet/console/report modes, failure propagation, explicit break-control API if present, and Debug/Release behavior.

### 122 - `llc_error.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 100L/3EC4CDE1CE82; GPK gpk_error.h 100L/B8F90CD64275; repairs-master 100L/3EC4CDE1CE82; sanitization 100L/A50FB12A8299 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain current logging/error propagation with debug breaks disabled by default; do not revive removed log-level ownership or per-file opt-out macros.
- **Why:** break behavior was deliberately disabled and logging must not unexpectedly stop applications.
- **Required tests:** callback routing, quiet/console/report modes, failure propagation, explicit break-control API if present, and Debug/Release behavior.

### 123 - `llc_eval.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 70L/7DD57E83CADF; GPK gpk_eval.h 70L/ECD7C6ACFBDF; repairs-master 70L/F2E2F9797522; sanitization 70L/07DF8D3B62B9 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 124 - `llc_event_input.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 59L/1B1187AD33BD; GPK gpk_event_input.h 59L/07FF59EE4735; repairs-master 59L/930D5E280529; sanitization 59L/581E031FBB48 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 125 - `llc_event_raster.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 52L/E17D1D022B1E; GPK gpk_event_raster.h 53L/3E2178D78A05; repairs-master 52L/E17D1D022B1E; sanitization 52L/E17D1D022B1E (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 126 - `llc_event_screen.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 48L/DED441D67A6C; GPK gpk_event_screen.h 48L/21654C9D2369; repairs-master 45L/09A4D1F74722; sanitization 48L/A103DF7A240C (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 127 - `llc_event_storage.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/0A7616B38AD7; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 128 - `llc_event.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 132L/CE9F109907F6; GPK gpk_event.h 137L/CA0DD38E92B3; repairs-master 137L/EF593E94F8D5; sanitization 132L/1B6C9B7BE27C (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 129 - `llc_expression.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 418L/DB67EA9B6138; GPK gpk_expression.cpp 387L/D2D46E663953; repairs-master 388L/49450572053F; sanitization 419L/F5EAC059CBBF (untracked).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 130 - `llc_expression.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 71L/BEEBC0398ADD; GPK gpk_expression.h 65L/C061F3945332; repairs-master 65L/42ABBA5892E7; sanitization 71L/BEEBC0398ADD (untracked).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 131 - `llc_file_pack.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 196L/0DC45C8C1D22; GPK absent; repairs-master absent; sanitization 227L/DD867039AD73 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep the restored package count, table/content bounds checks and complete-write/close verification while preserving the historical package layout.
- **Why:** without them a writer reports zero files and a reader dereferences untrusted ranges; changing the layout is unnecessary.
- **Required tests:** golden package compatibility, zero/many files, truncated header/table/content, corrupt sizes, partial writes, close failure and exact content/path roundtrip.

### 132 - `llc_file_pack.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 59L/6D27DB34DD5B; GPK absent; repairs-master absent; sanitization 59L/6D27DB34DD5B (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep the restored package count, table/content bounds checks and complete-write/close verification while preserving the historical package layout.
- **Why:** without them a writer reports zero files and a reader dereferences untrusted ranges; changing the layout is unnecessary.
- **Required tests:** golden package compatibility, zero/many files, truncated header/table/content, corrupt sizes, partial writes, close failure and exact content/path roundtrip.

### 133 - `llc_file.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 304L/4E96483B11FE; GPK gpk_file.cpp 294L/AAE061E490AD; repairs-master 319L/7D89111092E5; sanitization 317L/F9328FD40DD0 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve historical two-digit split names and propagate read, name-construction and write failures in both small and streaming paths.
- **Why:** single-digit part names broke existing sets and ignored I/O produced false success.
- **Required tests:** split/join `.00` through `.100`, historical-set join, small/streaming files, missing part, partial I/O and large offsets.

### 134 - `llc_file.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 35L/F31C75D9F084; GPK gpk_file.h 59L/F5156F415C59; repairs-master 61L/F157545E60A0; sanitization 35L/297458D9622C (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve historical two-digit split names and propagate read, name-construction and write failures in both small and streaming paths.
- **Why:** single-digit part names broke existing sets and ignored I/O produced false success.
- **Required tests:** split/join `.00` through `.100`, historical-set join, small/streaming files, missing part, partial I/O and large offsets.

### 135 - `llc_font_defaults.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 26L/3AC8C81BD6D6; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 136 - `llc_font.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 11L/FF74BF5C9B2C; GPK gpk_font.cpp 27L/CB20AB477304; repairs-master 27L/CECB21D5CDE9; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 137 - `llc_font.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 48L/31C68EF25244; GPK gpk_font.h 45L/867663A808D3; repairs-master 48L/31C68EF25244; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 138 - `llc_frameinfo.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 82L/498567E9AA65; GPK gpk_frameinfo.h 82L/EFBAFD76EE26; repairs-master 82L/CA7F5D9B68AE; sanitization 82L/56F90B876316 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 139 - `llc_framework.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 446L/F8C25FA9D7EF; GPK gpk_framework.cpp 440L/73244013C927; repairs-master 440L/1A724D31CBE9; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** start from GPK behavior and retain only the accepted `updateFramework()` publication: after successful metric recalculation publish `LastSize`/`LastZoom`; never publish them inside `guiUpdateMetrics()`.
- **Why:** this restores the missing caller ownership without legitimizing unrelated GUI rewrites.
- **Required tests:** startup/resize/DPI/font metrics, incremental pre-hit-test reconciliation, publication only on success, and render-target resize event.

### 140 - `llc_framework.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 76L/4EBD778AF368; GPK gpk_framework.h 76L/9E157681454B; repairs-master 76L/9E157681454B; sanitization absent.
- **Text relationship:** Normalized-text matches: GPK/gpk_framework.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** start from GPK behavior and retain only the accepted `updateFramework()` publication: after successful metric recalculation publish `LastSize`/`LastZoom`; never publish them inside `guiUpdateMetrics()`.
- **Why:** this restores the missing caller ownership without legitimizing unrelated GUI rewrites.
- **Required tests:** startup/resize/DPI/font metrics, incremental pre-hit-test reconciliation, publication only on success, and render-target resize event.

### 141 - `llc_functional.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 37L/75768CB9B498; GPK gpk_functional.h 37L/1F12D659CF35; repairs-master 37L/1F12D659CF35; sanitization 37L/86455C4DE2EB (clean).
- **Text relationship:** Normalized-text matches: GPK/gpk_functional.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 142 - `llc_gauge_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gauge_n2.h 27L/D0B24899BB0E; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 143 - `llc_gauge_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gauge_n3.h 27L/9D3B4AB05378; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 144 - `llc_gauge.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 95L/54F0D83EFF8A; GPK gpk_gauge.h 92L/A6C728398E03; repairs-master 95L/54F0D83EFF8A; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 145 - `llc_geometry_buffers.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/DD72B4506BB2; GPK gpk_geometry_buffers.h 18L/CC45EC2791EF; repairs-master 18L/DD72B4506BB2; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 146 - `llc_geometry_draw.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_geometry_draw.cpp 77L/8D7BF2FC4C9F; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 147 - `llc_geometry_draw.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_geometry_draw.h 31L/8203093F4D15; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 148 - `llc_geometry_lh.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 619L/0F4E0BA5A151; GPK gpk_geometry_lh.cpp 619L/35D46494AC35; repairs-master 619L/4CFFB6FE57F4; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 149 - `llc_geometry_lh.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 162L/13CDADF96BED; GPK gpk_geometry_lh.h 162L/D05D4C0C6B54; repairs-master 162L/87741D2980B9; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 150 - `llc_geometry.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 309L/C888A38C62FE; GPK gpk_geometry.cpp 304L/EFB8BEDB1E48; repairs-master 327L/A667E1F38CB8; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 151 - `llc_geometry.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 151L/F39B2FD98414; GPK gpk_geometry.h 151L/A79DDD5AC5DE; repairs-master 162L/3C7ABD007636; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 152 - `llc_geometry2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 30L/C7E487A6C298; GPK gpk_geometry2.h 30L/C7E487A6C298; repairs-master 30L/C7E487A6C298; sanitization 30L/C7E487A6C298 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 153 - `llc_gltf.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gltf.cpp 7L/991D3E6A85AA; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 154 - `llc_gltf.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gltf.h 24L/F6BC37A00044; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 155 - `llc_gpio.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gpio.h 15L/A8428AB6D0BE; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 156 - `llc_grid_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 23L/45AEFD1455AA; GPK gpk_grid_color.h 23L/F7EC38C23D82; repairs-master 23L/45AEFD1455AA; sanitization 23L/45AEFD1455AA (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 157 - `llc_grid_copy.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 288L/B8008D444CB5; GPK gpk_grid_copy.h 287L/61B4FB541EA7; repairs-master 288L/762A023460F4; sanitization 288L/B8008D444CB5 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the existing `n2::s2()` conversion and validate both clipped extents before unsigned conversion; preserve copy sampling/order.
- **Why:** the removed `s2t()` does not compile and checking only X lets negative Y become an enormous row count.
- **Required tests:** all overload instantiations, negative/zero X and Y, partial clipping, overlap and exact copied cells.

### 158 - `llc_grid_scale.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 107L/CDD946A4061F; GPK gpk_grid_scale.h 67L/0281A24C8B03; repairs-master 67L/A91B68D34348; sanitization 67L/F48CA7AF6B3A (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep the compact clipped two-loop implementation only if it preserves the original sampling rectangle; empty rectangles are no-ops and negative extents fail.
- **Why:** the optimization is acceptable only as an operation reduction with identical mapping, not as an unrelated behavior change.
- **Required tests:** unclipped and four-edge clipping, negative/zero extents, empty source, nonzero destination origin and pixel-for-pixel reference comparison.

### 159 - `llc_grid_serialize.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 30L/3245291D4D5C; GPK absent; repairs-master absent; sanitization 30L/3245291D4D5C (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 160 - `llc_grid_static.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 43L/E6EB03F85071; GPK gpk_grid_static.h 51L/C7898BC5D7A0; repairs-master 43L/57C1435E44E6; sanitization 43L/E6EB03F85071 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 161 - `llc_grid.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 92L/7C7EFFE59B4B; GPK gpk_grid.h 174L/FB91742F86F4; repairs-master 85L/DE4B2076AE4D; sanitization 92L/95484C7FA8B8 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain `fill(offset,count)` with stop `min(size, offset + count)` and preserve all other historical grid/view semantics.
- **Why:** using `min(size,count)` treats count as an absolute index and writes nothing for valid nonzero offsets.
- **Required tests:** offset/count boundaries, empty/full fill, clipped count, row views and storage aliasing.

### 162 - `llc_gtl_command.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gtl_command.h 76L/19C09FD00F24; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep `SGTLCommand` only as an internal optional sketch; it must not replace the established UDP/event API or become a new public dependency.
- **Why:** history shows the sketch had no implemented behavior or consumers.
- **Required tests:** no-consumer/API check and, if retained internally, bounded serialization roundtrip.

### 163 - `llc_gui_control_list.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 223L/078831CB1135; GPK gpk_gui_control_list.cpp 223L/815ABCDDE9BE; repairs-master 223L/15080E00B9A2; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore GPK control-list semantics: child hit overrides and clears parent hover/pressed; nonrecursive deletion leaves the surviving child's parent slot unchanged; preserve the historical `Client.Local`/`SizeMinMax` order.
- **Why:** the alternative order/detachment rules were introduced without a reproduced prior failure and can change consumers and hot-path cost.
- **Required tests:** overlapping roots, child/parent hover and press, leave/re-entry, recursive/nonrecursive deletion, slot reuse and geometry order.

### 164 - `llc_gui_control_list.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 87L/0CA94DBC341A; GPK gpk_gui_control_list.h 87L/183B6F16A6EE; repairs-master 87L/CB12521944F6; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore GPK control-list semantics: child hit overrides and clears parent hover/pressed; nonrecursive deletion leaves the surviving child's parent slot unchanged; preserve the historical `Client.Local`/`SizeMinMax` order.
- **Why:** the alternative order/detachment rules were introduced without a reproduced prior failure and can change consumers and hot-path cost.
- **Required tests:** overlapping roots, child/parent hover and press, leave/re-entry, recursive/nonrecursive deletion, slot reuse and geometry order.

### 165 - `llc_gui_control_state.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 155L/EFBB8CB94DC6; GPK gpk_gui_control_state.h 155L/5041D8CACCE4; repairs-master 155L/5041D8CACCE4; sanitization absent.
- **Text relationship:** Normalized-text matches: GPK/gpk_gui_control_state.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 166 - `llc_gui_control.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 243L/C18C4184C5A5; GPK gpk_gui_control.h 242L/E1909F1E1059; repairs-master 243L/3739F854A0F6; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 167 - `llc_gui_desktop.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gui_desktop.cpp 309L/4023A9427D5F; repairs-master 309L/1592F1231DAF; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 168 - `llc_gui_desktop.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_gui_desktop.h 58L/0F925179E175; repairs-master 58L/0F925179E175; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 169 - `llc_gui_draw.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 233L/661097930962; GPK gpk_gui_draw.cpp 211L/3BB143BD75A3; repairs-master 245L/CB777A8B5361; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK draw behavior with client-area clipping performed by the raster; retain only accepted GPU corner rasterization and no control-level early return that changes valid drawing.
- **Why:** client contents must not escape the client, while clipping ownership belongs in the raster rather than an unrelated draw-order rewrite.
- **Required tests:** CPU/GPU pixel comparison, partially clipped image and text, integer corners, font/image views and no drawing outside client.

### 170 - `llc_gui_inputbox.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 213L/F79561232295; GPK gpk_gui_inputbox.cpp 89L/6E60BF86C001; repairs-master 213L/F79561232295; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** perform a mechanical GPK-to-LLC migration only, changing namespace/current typedefs but no behavior, order or ownership.
- **Why:** viewport/input-box behavior is part of the proven GPK GUI and no independent redesign was authorized.
- **Required tests:** GPK parity vectors for construction, focus, editing, scrolling, selection, parent/child geometry and failure cleanup.

### 171 - `llc_gui_inputbox.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 36L/D329212A48F6; GPK gpk_gui_inputbox.h 109L/8059116D014F; repairs-master 38L/5104531FDC1D; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** perform a mechanical GPK-to-LLC migration only, changing namespace/current typedefs but no behavior, order or ownership.
- **Why:** viewport/input-box behavior is part of the proven GPK GUI and no independent redesign was authorized.
- **Required tests:** GPK parity vectors for construction, focus, editing, scrolling, selection, parent/child geometry and failure cleanup.

### 172 - `llc_gui_text.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 82L/35B873BBEEE6; GPK gpk_gui_text.h 82L/3045952A12A0; repairs-master 82L/24A96E9CDAAC; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 173 - `llc_gui_viewport.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 129L/E3821D5BB846; GPK gpk_gui_viewport.cpp 88L/D0CBFBFBF138; repairs-master 88L/AFEFE1DA6479; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** perform a mechanical GPK-to-LLC migration only, changing namespace/current typedefs but no behavior, order or ownership.
- **Why:** viewport/input-box behavior is part of the proven GPK GUI and no independent redesign was authorized.
- **Required tests:** GPK parity vectors for construction, focus, editing, scrolling, selection, parent/child geometry and failure cleanup.

### 174 - `llc_gui_viewport.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 48L/69A1B84915E8; GPK gpk_gui_viewport.h 43L/8C2D28430B28; repairs-master 43L/8C2D28430B28; sanitization absent.
- **Text relationship:** Normalized-text matches: GPK/gpk_gui_viewport.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** perform a mechanical GPK-to-LLC migration only, changing namespace/current typedefs but no behavior, order or ownership.
- **Why:** viewport/input-box behavior is part of the proven GPK GUI and no independent redesign was authorized.
- **Required tests:** GPK parity vectors for construction, focus, editing, scrolling, selection, parent/child geometry and failure cleanup.

### 175 - `llc_gui.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 712L/9E1F6F95971E; GPK gpk_gui.cpp 703L/0D656AA85D9C; repairs-master 712L/0CA35B23D22B; sanitization 714L/55A3830163F1 (untracked).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 176 - `llc_gui.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 77L/EA7A24FFD632; GPK gpk_gui.h 79L/E26558A50573; repairs-master 79L/34EFC1E17823; sanitization 81L/5527C8CAADA2 (untracked).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 177 - `llc_header_checkingg.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK llc_header_checkingg.cpp 172L/5E31D99BB591; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this misspelled checking artifact from the production merge.
- **Why:** it is not a canonical runtime facility and its name records an accidental helper.
- **Required tests:** project/inventory check proving it is not compiled.

### 178 - `llc_http_client.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_http_client.cpp 312L/D0CA04B08205; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 179 - `llc_http_client.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_http_client.h 50L/158F7140ACF0; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 180 - `llc_http.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 169L/E5FF56B79F9B; GPK gpk_http.h 169L/9D74874AEE31; repairs-master absent; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 181 - `llc_https_client.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_https_client.cpp 97L/269953F781DB; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 182 - `llc_i2c.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 510L/1420D9CD7636; GPK gpk_i2c.cpp 42L/67AC307B6474; repairs-master 510L/1420D9CD7636; sanitization 510L/8086C0988862 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 183 - `llc_i2c.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 51L/35BBF0AFA4CA; GPK gpk_i2c.h 32L/520DC7B03766; repairs-master 51L/35BBF0AFA4CA; sanitization 51L/999F37D0E158 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 184 - `llc_image.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 169L/666F514B0A7F; GPK gpk_image.h 225L/0A7D80E56BF4; repairs-master 170L/2DE1BBB8C2CF; sanitization 170L/2DE1BBB8C2CF (clean).
- **Text relationship:** Normalized-text matches: repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve image resize delegation and always rebind the public view after storage resize; keep only realistic overflow/bounds checks.
- **Why:** a stale view is a real defect, while rejecting imaginary 16-GB cases was not the requested contract.
- **Required tests:** resize smaller/larger/zero, view pointer/metrics rebinding, retained texels, allocation failure and practical maximum dimensions.

### 185 - `llc_img_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 17L/CB9EDE56BCF7; GPK gpk_img_color.h 46L/627FBDB20DCF; repairs-master 17L/CB9EDE56BCF7; sanitization 17L/CB9EDE56BCF7 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the modern implementation only for demonstrated bounds/error corrections while preserving the historical image/PNG format and raster behavior.
- **Why:** image changes are observable and prior reviews found both real safety bugs and invented unrealistic requirements.
- **Required tests:** small/large dimensions, overflow boundaries, clipped views, interlaced/noninterlaced PNG roundtrip and malformed chunk/filter cases.

### 186 - `llc_img_serialize.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 38L/902AB843CB60; GPK gpk_img_serialize.h 29L/8CE8AD6DD6DF; repairs-master 38L/902AB843CB60; sanitization 38L/902AB843CB60 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the modern implementation only for demonstrated bounds/error corrections while preserving the historical image/PNG format and raster behavior.
- **Why:** image changes are observable and prior reviews found both real safety bugs and invented unrealistic requirements.
- **Required tests:** small/large dimensions, overflow boundaries, clipped views, interlaced/noninterlaced PNG roundtrip and malformed chunk/filter cases.

### 187 - `llc_inout.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/E68D92462F19; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 188 - `llc_input.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 61L/805E0BDEFE70; GPK gpk_input.h 53L/D6EC1E9A35A5; repairs-master 61L/805E0BDEFE70; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 189 - `llc_io.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_io.h 27L/CE8693DBC43A; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 190 - `llc_json_expression.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 317L/E58FFBDB17DD; GPK gpk_json_expression.cpp 322L/B2398C6341DA; repairs-master 338L/43EF71DD3F31; sanitization 317L/77E41D8B3F6E (untracked).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 191 - `llc_json_expression.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 46L/E78DCD25C40E; GPK gpk_json_expression.h 49L/E37830921F75; repairs-master 46L/FB33E1F9DC3E; sanitization 46L/656CCE494261 (untracked).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 192 - `llc_json.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 786L/5153AFCDECAC; GPK gpk_json.cpp 746L/ABD8BA2C00F0; repairs-master 765L/CE53DA691279; sanitization 782L/03045A6B4F4F (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 193 - `llc_json.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 214L/8497B37689B4; GPK gpk_json.h 183L/BE5885CD98D7; repairs-master 211L/A7C9159C9F33; sanitization 214L/D27B1F82005B (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 194 - `llc_keyed_bit_array.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_keyed_bit_array.h 63L/6EED869D9766; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 195 - `llc_keyval_old.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_keyval_old.cpp 97L/E6EA2DF91745; repairs-master 66L/5DEB782A58F2; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude the legacy owner from the final merge.
- **Why:** an `_old` duplicate would preserve two competing APIs instead of one reviewed contract.
- **Required tests:** inventory/project check proving no consumer includes or compiles it.

### 196 - `llc_keyval_old.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_keyval_old.h 64L/50A02FABDE3E; repairs-master 38L/53E3BFCF225D; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude the legacy owner from the final merge.
- **Why:** an `_old` duplicate would preserve two competing APIs instead of one reviewed contract.
- **Required tests:** inventory/project check proving no consumer includes or compiles it.

### 197 - `llc_keyval_view.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 62L/AD00600E3541; GPK absent; repairs-master absent; sanitization 62L/AD00600E3541 (untracked).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this unrequested split from the final merge and keep key/value view behavior in the requested canonical owners.
- **Why:** Pablo explicitly excluded `keyval_view` when defining the recovery set.
- **Required tests:** compile consumers against `llc_keyval.*` without this header/source.

### 198 - `llc_keyval_view.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 34L/1279A463B352; GPK absent; repairs-master absent; sanitization 34L/1279A463B352 (untracked).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this unrequested split from the final merge and keep key/value view behavior in the requested canonical owners.
- **Why:** Pablo explicitly excluded `keyval_view` when defining the recovery set.
- **Required tests:** compile consumers against `llc_keyval.*` without this header/source.

### 199 - `llc_keyval.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK absent; repairs-master absent; sanitization 1L/E3B0C44298FC (unstaged modification).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge only canonical `llc_keyval.cpp/.h`; preserve ordering, lookup and serialization without importing `keyval_view` or `_old` owners.
- **Why:** Pablo explicitly scoped recovery to the canonical pair.
- **Required tests:** insert/find/update/missing/duplicate keys, stable views, serialization and consumer compilation without excluded headers.

### 200 - `llc_keyval.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 28L/DB79D666DC8A; GPK gpk_keyval.h 28L/DB79D666DC8A; repairs-master 28L/DB79D666DC8A; sanitization 28L/DB79D666DC8A (unstaged modification).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge only canonical `llc_keyval.cpp/.h`; preserve ordering, lookup and serialization without importing `keyval_view` or `_old` owners.
- **Why:** Pablo explicitly scoped recovery to the canonical pair.
- **Required tests:** insert/find/update/missing/duplicate keys, stable views, serialization and consumer compilation without excluded headers.

### 201 - `llc_label_manager.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 118L/C3DE1EA9CBD5; GPK gpk_label_manager.h 98L/97BDBA03F4AD; repairs-master 118L/FBDBB046D8FF; sanitization 118L/C3DE1EA9CBD5 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve label/block-container ownership and the intentional qualified generic clear path; do not add ambiguous inherited overloads.
- **Why:** the NTS container should expose only its safe API while still invoking the generic multi-argument clear.
- **Required tests:** single/multi clear, allocation reuse, stale-ID rejection and manager add/remove/find.

### 202 - `llc_label.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 25L/1B3504847910; GPK gpk_label.cpp 27L/51AA0D6F5FE7; repairs-master 27L/472D09379887; sanitization 25L/1B3504847910 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve label/block-container ownership and the intentional qualified generic clear path; do not add ambiguous inherited overloads.
- **Why:** the NTS container should expose only its safe API while still invoking the generic multi-argument clear.
- **Required tests:** single/multi clear, allocation reuse, stale-ID rejection and manager add/remove/find.

### 203 - `llc_label.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 45L/2931D8AF8C5F; GPK gpk_label.h 42L/17BAF9127749; repairs-master 42L/A1DE5CE87537; sanitization 45L/2931D8AF8C5F (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve label/block-container ownership and the intentional qualified generic clear path; do not add ambiguous inherited overloads.
- **Why:** the NTS container should expose only its safe API while still invoking the generic multi-argument clear.
- **Required tests:** single/multi clear, allocation reuse, stale-ID rejection and manager add/remove/find.

### 204 - `llc_lifecycle.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 28L/B60B8B20D066; GPK absent; repairs-master absent; sanitization 28L/B60B8B20D066 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 205 - `llc_line.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 23L/4C35B33B95DB; GPK gpk_line.h 23L/664EE5283CF6; repairs-master 23L/4C35B33B95DB; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 206 - `llc_line2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 44L/BCC894453643; GPK gpk_line2.h 51L/464F25C944C3; repairs-master 44L/BCC894453643; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 207 - `llc_line3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 42L/8676AD2E3EE8; GPK gpk_line3.h 43L/B3E0FCB40BDA; repairs-master 42L/8676AD2E3EE8; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 208 - `llc_linear_map_pobj.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 100L/4B32DEDEF36B; GPK gpk_linear_map_pobj.h 75L/7067D3DF697E; repairs-master 100L/0254E5749419; sanitization 100L/4B32DEDEF36B (untracked).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 209 - `llc_linear_map_pod.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 79L/4A83B7F9F5A1; GPK gpk_linear_map_pod.h 64L/2836351E213C; repairs-master 79L/81F6C383AEC1; sanitization 79L/4A83B7F9F5A1 (untracked).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 210 - `llc_log_core.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 150L/867365CAB4AB; GPK gpk_log_core.h 153L/827C10074307; repairs-master 149L/509803F0F6B8; sanitization 150L/41891793272B (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current callback/routing behavior with no automatic debug break and no per-consumer `LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG` escape.
- **Why:** logging must report errors without unexpectedly halting callers; the JSON opt-out was explicitly removed.
- **Required tests:** raw/quiet/console/report routing, callback restoration, concurrent writes, failed output and Debug/Release behavior.

### 211 - `llc_log_level.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_log_level.h 1050L/F0E524DE9FA7; repairs-master 1041L/BBE2618DF3AF; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep this deleted; do not reintroduce it.
- **Why:** the separate log-level header was explicitly removed and current logging owns the definitions.
- **Required tests:** build all log consumers without the removed include.

### 212 - `llc_log.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 204L/9940F599AB66; GPK gpk_log.cpp 200L/8C1F5CA034F4; repairs-master 198L/CF6548B05C44; sanitization 204L/9940F599AB66 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current callback/routing behavior with no automatic debug break and no per-consumer `LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG` escape.
- **Why:** logging must report errors without unexpectedly halting callers; the JSON opt-out was explicitly removed.
- **Required tests:** raw/quiet/console/report routing, callback restoration, concurrent writes, failed output and Debug/Release behavior.

### 213 - `llc_log.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 758L/BDF4AAD1447C; GPK gpk_log.h 149L/0B3909D74C3D; repairs-master 149L/DA74D5CFF20E; sanitization 715L/A7BFFB654E7B (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current callback/routing behavior with no automatic debug break and no per-consumer `LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG` escape.
- **Why:** logging must report errors without unexpectedly halting callers; the JSON opt-out was explicitly removed.
- **Required tests:** raw/quiet/console/report routing, callback restoration, concurrent writes, failed output and Debug/Release behavior.

### 214 - `llc_mapblock.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_mapblock.cpp 21L/8E41F2F3D6AC; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 215 - `llc_math.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 71L/5B7BC460A963; GPK gpk_math.h 71L/303F89A63A5B; repairs-master 71L/DB75A1DDF23B; sanitization 71L/DB75A1DDF23B (clean).
- **Text relationship:** Normalized-text matches: repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 216 - `llc_matrix.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 188L/288597CE80FB; GPK gpk_matrix.h 751L/BC212A720E88; repairs-master 188L/288597CE80FB; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 217 - `llc_matrix2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 175L/D3710483B60D; GPK absent; repairs-master 175L/D3710483B60D; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 218 - `llc_matrix3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 385L/C3BA1C29ACEC; GPK absent; repairs-master 385L/C3BA1C29ACEC; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 219 - `llc_member_registry.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 64L/19245561FAE5; GPK gpk_member_registry.h 64L/05D17A667664; repairs-master 64L/ED8BF7647020; sanitization 64L/19245561FAE5 (untracked).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 220 - `llc_memory.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 101L/A08F5B610CA7; GPK gpk_memory.h 131L/97F8621CA3AC; repairs-master 108L/91019789DF90; sanitization 101L/A08F5B610CA7 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 221 - `llc_micro.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 96L/5AC5686CD026; GPK absent; repairs-master 96L/5AC5686CD026; sanitization 96L/E074789170DF (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 222 - `llc_minmax_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 28L/BC722E1A0557; GPK gpk_minmax_n2.h 28L/BC722E1A0557; repairs-master 28L/BC722E1A0557; sanitization 28L/09533C517FE4 (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_minmax_n2.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 223 - `llc_minmax_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/F7C63E4C8BEA; GPK gpk_minmax_n3.h 26L/431EC401BC31; repairs-master 24L/F7C63E4C8BEA; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 224 - `llc_minmax.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 60L/BD2E2F14AC48; GPK gpk_minmax.h 60L/BD2E2F14AC48; repairs-master 60L/BD2E2F14AC48; sanitization 60L/E725C44ACE4D (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_minmax.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 225 - `llc_model.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/3A4DAD13DC03; GPK gpk_model.h 24L/590487ACFFA2; repairs-master 24L/3A4DAD13DC03; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 226 - `llc_module.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 65L/B7DE3F1EDA61; GPK gpk_module.h 65L/CE0FC36A4364; repairs-master 65L/B7DE3F1EDA61; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 227 - `llc_mutex.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 78L/B831F4E389B3; GPK absent; repairs-master 78L/B831F4E389B3; sanitization 78L/B831F4E389B3 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 228 - `llc_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 207L/5D5DCFCC63A4; GPK gpk_n2.h 207L/9076AA7D291C; repairs-master 207L/96FADE902998; sanitization 207L/ADEE866DB403 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 229 - `llc_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 227L/548A0E73F304; GPK gpk_n3.h 225L/C2D99CC7EEEE; repairs-master 227L/548A0E73F304; sanitization 227L/E2ED0E55633C (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 230 - `llc_noise.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 10L/3F2590E5D3B4; GPK gpk_noise.cpp 10L/3F2590E5D3B4; repairs-master 10L/3F2590E5D3B4; sanitization 10L/3F2590E5D3B4 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 231 - `llc_noise.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 93L/87C0FA6AE3CF; GPK gpk_noise.h 93L/87C0FA6AE3CF; repairs-master 93L/87C0FA6AE3CF; sanitization 88L/7BD76E758BAC (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_noise.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 232 - `llc_pack.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 2L/1391DCBCACAB; GPK absent; repairs-master 3L/987D293ED1BD; sanitization 3L/987D293ED1BD (clean).
- **Text relationship:** Normalized-text matches: repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain reviewed modern error propagation and historical on-disk naming/layout; every read, write, allocation and bounds failure must reach the caller.
- **Why:** prior recovery found silent success, unchecked package ranges and split-name compatibility regressions.
- **Required tests:** golden-format roundtrip, historical files/part names, truncation/corruption, partial I/O, large files and allocation failure.

### 233 - `llc_pack.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 46L/0EC6FD87C2EE; GPK absent; repairs-master 46L/BFB5C0AA37D3; sanitization 46L/BFB5C0AA37D3 (clean).
- **Text relationship:** Normalized-text matches: repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain reviewed modern error propagation and historical on-disk naming/layout; every read, write, allocation and bounds failure must reach the caller.
- **Why:** prior recovery found silent success, unchecked package ranges and split-name compatibility regressions.
- **Required tests:** golden-format roundtrip, historical files/part names, truncation/corruption, partial I/O, large files and allocation failure.

### 234 - `llc_packed_int.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 140L/C5527A8E0B8B; GPK gpk_packed_int.h 140L/C5527A8E0B8B; repairs-master 140L/C5527A8E0B8B; sanitization 140L/8DA286FF8FC0 (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_packed_int.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain reviewed modern error propagation and historical on-disk naming/layout; every read, write, allocation and bounds failure must reach the caller.
- **Why:** prior recovery found silent success, unchecked package ranges and split-name compatibility regressions.
- **Required tests:** golden-format roundtrip, historical files/part names, truncation/corruption, partial I/O, large files and allocation failure.

### 235 - `llc_parse.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 162L/582FF6C8AFD8; GPK gpk_parse.cpp 168L/32349B4EB0C6; repairs-master 162L/582FF6C8AFD8; sanitization 162L/582FF6C8AFD8 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 236 - `llc_parse.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 63L/2FE89450B5F6; GPK gpk_parse.h 63L/63F85D01E4E5; repairs-master 63L/7602DB7CDDAD; sanitization 63L/479626593F7F (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 237 - `llc_particle.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 42L/C4CA036A98C8; GPK gpk_particle.cpp 42L/0F4E39CB9A76; repairs-master 42L/0F4E39CB9A76; sanitization absent.
- **Text relationship:** Normalized-text matches: GPK/gpk_particle.cpp = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 238 - `llc_particle.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 179L/8D4869D514E1; GPK gpk_particle.h 179L/356FB46B147C; repairs-master 179L/FF0F31B5AD1A; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 239 - `llc_path.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 465L/29E60CB60CEA; GPK gpk_path.cpp 414L/F6F082158E30; repairs-master 380L/DDF25CCD5961; sanitization 465L/7BFC06AFD7BA (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain reviewed modern error propagation and historical on-disk naming/layout; every read, write, allocation and bounds failure must reach the caller.
- **Why:** prior recovery found silent success, unchecked package ranges and split-name compatibility regressions.
- **Required tests:** golden-format roundtrip, historical files/part names, truncation/corruption, partial I/O, large files and allocation failure.

### 240 - `llc_path.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 71L/8893E1F25680; GPK gpk_path.h 45L/C87F554CF7EF; repairs-master 45L/76815B5A0C86; sanitization 71L/E08FCC063CE9 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain reviewed modern error propagation and historical on-disk naming/layout; every read, write, allocation and bounds failure must reach the caller.
- **Why:** prior recovery found silent success, unchecked package ranges and split-name compatibility regressions.
- **Required tests:** golden-format roundtrip, historical files/part names, truncation/corruption, partial I/O, large files and allocation failure.

### 241 - `llc_platform_error.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_platform_error.cpp 18L/C72F85F738FE; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain current logging/error propagation with debug breaks disabled by default; do not revive removed log-level ownership or per-file opt-out macros.
- **Why:** break behavior was deliberately disabled and logging must not unexpectedly stop applications.
- **Required tests:** callback routing, quiet/console/report modes, failure propagation, explicit break-control API if present, and Debug/Release behavior.

### 242 - `llc_platform_error.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 16L/8073757913A2; GPK gpk_platform_error.h 16L/8073757913A2; repairs-master 16L/8073757913A2; sanitization 16L/A97FDA7EE2A3 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_platform_error.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain current logging/error propagation with debug breaks disabled by default; do not revive removed log-level ownership or per-file opt-out macros.
- **Why:** break behavior was deliberately disabled and logging must not unexpectedly stop applications.
- **Required tests:** callback routing, quiet/console/report modes, failure propagation, explicit break-control API if present, and Debug/Release behavior.

### 243 - `llc_platform_globals.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 298L/F43DB9E829EC; GPK gpk_platform_globals.h 298L/F43DB9E829EC; repairs-master 298L/F43DB9E829EC; sanitization 272L/F9DE637A7BBC (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_platform_globals.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 244 - `llc_png_write.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 85L/277A16AFA62C; GPK gpk_png_write.cpp 112L/9396ED16D1FB; repairs-master 85L/277A16AFA62C; sanitization 85L/8D4BDC5BC315 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep PNG format compatibility plus the reviewed IHDR/tEXt/filter/allocation and signed-bytes-per-pixel checks; every chunk and output range remains length-bounded.
- **Why:** prior code had unbounded keyword search, unchecked struct/range reads, invalid-filter nontermination and unsigned error conversion.
- **Required tests:** golden encode/decode, interlaced PNG, short/corrupt chunks, unterminated tEXt keyword, invalid first filter, unsupported format, truncation and dimension bounds.

### 245 - `llc_png.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 1223L/7EC820E002BC; GPK gpk_png.cpp 1192L/77F4B28A478C; repairs-master 1223L/719C77B9DBA4; sanitization 1246L/E2591D06E64D (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep PNG format compatibility plus the reviewed IHDR/tEXt/filter/allocation and signed-bytes-per-pixel checks; every chunk and output range remains length-bounded.
- **Why:** prior code had unbounded keyword search, unchecked struct/range reads, invalid-filter nontermination and unsigned error conversion.
- **Required tests:** golden encode/decode, interlaced PNG, short/corrupt chunks, unterminated tEXt keyword, invalid first filter, unsupported format, truncation and dimension bounds.

### 246 - `llc_png.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 102L/27AA1677FAD1; GPK gpk_png.h 102L/336917668FA1; repairs-master 102L/27AA1677FAD1; sanitization 100L/07597E6356B2 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep PNG format compatibility plus the reviewed IHDR/tEXt/filter/allocation and signed-bytes-per-pixel checks; every chunk and output range remains length-bounded.
- **Why:** prior code had unbounded keyword search, unchecked struct/range reads, invalid-filter nontermination and unsigned error conversion.
- **Required tests:** golden encode/decode, interlaced PNG, short/corrupt chunks, unterminated tEXt keyword, invalid first filter, unsupported format, truncation and dimension bounds.

### 247 - `llc_pod_definition.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 54L/064FECA9E531; GPK gpk_pod_definition.h 75L/C5B47F92512E; repairs-master 54L/064FECA9E531; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 248 - `llc_process_internal.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 43L/275747B07F72; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 249 - `llc_process_linux.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 345L/0EA58C4AA78C; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 250 - `llc_process_win32.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 201L/5A0CDBD4CB39; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 251 - `llc_process.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 101L/58BF39F9FF26; GPK gpk_process.cpp 47L/0229146A0C82; repairs-master 47L/3A94DC9E8E1D; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 252 - `llc_process.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 248L/094A16DD6C90; GPK gpk_process.h 14L/0135F00852E6; repairs-master 15L/66D640284E79; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 253 - `llc_ptr_nco.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 48L/EDB51879BEA4; GPK gpk_ptr_nco.h 53L/FE6ECD449AF1; repairs-master 48L/55348055A3BD; sanitization 48L/CAA3B63A4D17 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the reviewed ownership contract: move transfers and nulls the source, assignment releases the old object exactly once, and AddRef/Release order is exception-safe; no invented inherited assignment imports.
- **Why:** pointer wrappers are ABI/lifetime owners and prior mechanical changes risked leaks, double release or self-assignment corruption.
- **Required tests:** copy/move/self assignment, null/non-null transitions, exact AddRef/Release counts and destruction order.

### 254 - `llc_ptr_obj.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 26L/2F6487B28A7F; GPK gpk_ptr_obj.h 29L/4F7702634C70; repairs-master 26L/2F6487B28A7F; sanitization 26L/BD101A5D37BE (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the reviewed ownership contract: move transfers and nulls the source, assignment releases the old object exactly once, and AddRef/Release order is exception-safe; no invented inherited assignment imports.
- **Why:** pointer wrappers are ABI/lifetime owners and prior mechanical changes risked leaks, double release or self-assignment corruption.
- **Required tests:** copy/move/self assignment, null/non-null transitions, exact AddRef/Release counts and destruction order.

### 255 - `llc_ptr_pod.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 25L/CEEB411ECABE; GPK gpk_ptr_pod.h 28L/E6B601E5D530; repairs-master 25L/CEEB411ECABE; sanitization 25L/CEEB411ECABE (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the reviewed ownership contract: move transfers and nulls the source, assignment releases the old object exactly once, and AddRef/Release order is exception-safe; no invented inherited assignment imports.
- **Why:** pointer wrappers are ABI/lifetime owners and prior mechanical changes risked leaks, double release or self-assignment corruption.
- **Required tests:** copy/move/self assignment, null/non-null transitions, exact AddRef/Release counts and destruction order.

### 256 - `llc_quad.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 37L/15E2CB258D52; GPK gpk_quad.h 36L/78E1F0625DAD; repairs-master 37L/15E2CB258D52; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 257 - `llc_quad2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 37L/AD74CB57D1E7; GPK gpk_quad2.h 35L/369A1CF018C6; repairs-master 37L/AD74CB57D1E7; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 258 - `llc_quad3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 37L/ED2B43D1001B; GPK gpk_quad3.h 35L/9ED726730C94; repairs-master 37L/ED2B43D1001B; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 259 - `llc_quat.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 175L/ADA5BDD334F9; GPK gpk_quat.h 181L/423A71769A67; repairs-master 175L/ADA5BDD334F9; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 260 - `llc_queue_async.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 59L/749A7028037C; GPK gpk_queue_async.h 59L/A427AE0C2DFE; repairs-master 59L/749A7028037C; sanitization 59L/749A7028037C (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 261 - `llc_queue_event.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 17L/4CC8C851C7A6; GPK gpk_queue_event.h 17L/4CC8C851C7A6; repairs-master 17L/4CC8C851C7A6; sanitization 17L/4CC8C851C7A6 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 262 - `llc_range_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/D4E472A2DE82; GPK gpk_range_n2.h 27L/7DE1E5597B28; repairs-master 24L/D4E472A2DE82; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 263 - `llc_range_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/14A3B093AE78; GPK gpk_range_n3.h 27L/600937E80C8C; repairs-master 24L/14A3B093AE78; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 264 - `llc_range.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 46L/3A0F5E804BB6; GPK gpk_range.h 46L/3A0F5E804BB6; repairs-master 46L/3A0F5E804BB6; sanitization 46L/48774855960A (clean).
- **Text relationship:** Normalized-text matches: thecure = GPK/gpk_range.h = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 265 - `llc_raster_lh.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 646L/9B43EEDE6D8D; GPK gpk_raster_lh.cpp 646L/7FE2F805480B; repairs-master 646L/9A225F7B328D; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 266 - `llc_raster_lh.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 167L/ED3913454F15; GPK gpk_raster_lh.h 166L/063F92DE31F2; repairs-master 167L/7DEF1E4BA4FD; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 267 - `llc_rect_align.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 28L/00121C0667E4; GPK gpk_rect_align.h 49L/DE36DB50538F; repairs-master 28L/00121C0667E4; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 268 - `llc_rect.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 52L/EE380FA3C716; GPK gpk_rect.h 52L/4B647EB5B5EE; repairs-master 52L/EE380FA3C716; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 269 - `llc_rect2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 57L/74A4CE44E998; GPK gpk_rect2.h 52L/EEC7A08F2596; repairs-master 57L/74A4CE44E998; sanitization 55L/7A96BCAD094D (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 270 - `llc_rect3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 49L/7B7A3EB61B8B; GPK gpk_rect3.h 19L/B065B0DC27FD; repairs-master 49L/7B7A3EB61B8B; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 271 - `llc_ref.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 57L/4C84B2CA4E87; GPK gpk_ref.h 77L/10C0CA50ADEE; repairs-master 57L/6B5F93447BC0; sanitization 57L/B2441D23DB8B (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the reviewed ownership contract: move transfers and nulls the source, assignment releases the old object exactly once, and AddRef/Release order is exception-safe; no invented inherited assignment imports.
- **Why:** pointer wrappers are ABI/lifetime owners and prior mechanical changes risked leaks, double release or self-assignment corruption.
- **Required tests:** copy/move/self assignment, null/non-null transitions, exact AddRef/Release counts and destruction order.

### 272 - `llc_rsa.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_rsa.cpp 288L/43A8ED350E25; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** leave this GPK-only RSA implementation out of the final LLC core merge until a live consumer and cryptographic contract are explicitly selected.
- **Why:** none of the supplied LLC snapshots owns it, and importing unaudited cryptography creates unsupported public surface.
- **Required tests:** no-consumer scan; if later admitted, published RSA vectors, key parsing and malformed/cross-size rejection.

### 273 - `llc_rsa.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_rsa.h 38L/2E8D35FB6E00; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** leave this GPK-only RSA implementation out of the final LLC core merge until a live consumer and cryptographic contract are explicitly selected.
- **Why:** none of the supplied LLC snapshots owns it, and importing unaudited cryptography creates unsupported public surface.
- **Required tests:** no-consumer scan; if later admitted, published RSA vectors, key parsing and malformed/cross-size rejection.

### 274 - `llc_runtime_module.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 38L/2CC331ADDB62; GPK gpk_runtime_module.cpp 20L/1BE6DFDE00ED; repairs-master 35L/5FDA6937F614; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 275 - `llc_runtime_module.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 93L/001D062D6841; GPK gpk_runtime_module.h 56L/A2C42862CF9C; repairs-master 57L/EC7E5FB69C42; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 276 - `llc_runtime.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 90L/6A8846E2DEAE; GPK llc_runtime.h 93L/05516194EE90 + gpk_runtime.h 67L/CF998E7751CB; repairs-master 112L/AF5018F6A36D; sanitization 89L/40929E1B3025 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** merge the established application/module/process lifecycle without duplicating entry-point ownership or changing callback order.
- **Why:** runtime ownership is cross-project ABI; source presence alone cannot authorize a second lifecycle path.
- **Required tests:** setup/update/draw/cleanup order, module load/unload, process success/failure/quoting and configured host integration.

### 277 - `llc_rxtx.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 15L/696939033EB3; GPK absent; repairs-master 15L/696939033EB3; sanitization 15L/D966179430FF (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 278 - `llc_safe.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 17L/F9FC93938F43; GPK gpk_safe.h 44L/82B525CA0DCD; repairs-master 17L/6199499A7387; sanitization 17L/6431A1ACB9A2 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the reviewed ownership contract: move transfers and nulls the source, assignment releases the old object exactly once, and AddRef/Release order is exception-safe; no invented inherited assignment imports.
- **Why:** pointer wrappers are ABI/lifetime owners and prior mechanical changes risked leaks, double release or self-assignment corruption.
- **Required tests:** copy/move/self assignment, null/non-null transitions, exact AddRef/Release counts and destruction order.

### 279 - `llc_scene.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_scene.cpp 150L/71658CE0EFCC; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 280 - `llc_scene.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_scene.h 222L/EA47878145F4; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the historical scene ownership and both solar-system rendering routes; integrate with the restored D3D frame owner without adding nested clear/present calls.
- **Why:** the LLC rewrite lost a historical D/C rendering route and static scope checks cannot prove the visible sample.
- **Required tests:** run the actual solar-system sample through both routes, compare rendered frames/camera transforms and assert one frame presentation.

### 281 - `llc_serial.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_serial.h 90L/3B689E91B68C; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 282 - `llc_size.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/9A7801EAB463; GPK gpk_size.h 16L/D00C40B2B691; repairs-master 14L/EAFB87AE3006; sanitization 14L/9A7801EAB463 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 283 - `llc_slice_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/6ACA310EF5B9; GPK gpk_slice_n2.h 27L/F8588F1D2364; repairs-master 24L/6ACA310EF5B9; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 284 - `llc_slice_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/51954C8F3D19; GPK gpk_slice_n3.h 28L/48405CDA0D17; repairs-master 24L/51954C8F3D19; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 285 - `llc_slice.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 47L/2EC6509E77D2; GPK gpk_slice.h 74L/80A8AB3AC0FD; repairs-master 47L/2EC6509E77D2; sanitization 47L/2EC6509E77D2 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 286 - `llc_sphere.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 39L/88726D9451D4; GPK gpk_sphere.h 39L/C77A3BDE260F; repairs-master 39L/88726D9451D4; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 287 - `llc_spi.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 45L/93845484CF8C; GPK gpk_spi.cpp 40L/5BF6B5F2CC08; repairs-master 45L/93845484CF8C; sanitization 45L/93845484CF8C (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 288 - `llc_spi.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 42L/EFB30765000F; GPK gpk_spi.h 31L/ED78710DB5C4; repairs-master 42L/EFB30765000F; sanitization 42L/EFB30765000F (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 289 - `llc_std_cstring.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 26L/25F72BA96E1A; GPK gpk_std_cstring.h 26L/F1E6DA3A26EE; repairs-master 26L/EE77A7EA4347; sanitization 26L/25F72BA96E1A (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 290 - `llc_std_initializer_list.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 113L/805DF2F70423; GPK gpk_std_initializer_list.h 113L/805DF2F70423; repairs-master 113L/805DF2F70423; sanitization 113L/805DF2F70423 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 291 - `llc_std_string.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 22L/BAE36A860582; GPK gpk_std_string.h 22L/A4FD54DCE118; repairs-master 22L/1302537EB398; sanitization 22L/BAE36A860582 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 292 - `llc_stdsocket.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 64L/DAD8B29FABBC; GPK gpk_stdsocket.h 57L/1835A770DA9F; repairs-master 64L/CBF855187EE0; sanitization 64L/4170B6200B47 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 293 - `llc_stdstring.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 7L/9A1556CC02C6; GPK gpk_stdstring.cpp 23L/5BE3843DC532; repairs-master 7L/9A1556CC02C6; sanitization 7L/9A1556CC02C6 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 294 - `llc_stdstring.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 25L/3B6971315854; GPK gpk_stdstring.h 19L/A9C2FEF556E7; repairs-master 25L/2ACEB2BAFAAF; sanitization 25L/3B6971315854 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; avoid compatibility aliases and duplicate overloads.
- **Why:** string is a foundational API and recent ambiguity repairs were intentionally localized.
- **Required tests:** literal/view/string comparisons, empty/embedded-NUL cases, formatting bounds, allocation failure and consumer compilation.

### 295 - `llc_stl.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 34L/D549BB5C67BD; GPK gpk_stl.cpp 17L/A212B9AAD7E8; repairs-master 34L/D549BB5C67BD; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 296 - `llc_stl.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 27L/8F8449BFCDB8; GPK gpk_stl.h 26L/12DD03BE4A0B; repairs-master 27L/8F8449BFCDB8; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 297 - `llc_string_compose.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 17L/1665718DE673; GPK gpk_string_compose.cpp 17L/FF0C89256C20; repairs-master 17L/1665718DE673; sanitization 17L/FD8B32AFC43C (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; keep `llc::string` equality operators in `llc_string.h` only and do not import unrelated base assignments.
- **Why:** the comparison fix was explicitly localized and broad `using asc_t::operator=` changed overload resolution.
- **Required tests:** literal/view/string equality both directions, empty and embedded-NUL cases, copy/move/self assignment, formatting bounds and allocation failure.

### 298 - `llc_string_compose.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 44L/2F1F1511B2AF; GPK gpk_string_compose.h 44L/9F5ED5E094FA; repairs-master 44L/2F1F1511B2AF; sanitization 44L/02F805E772DF (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; keep `llc::string` equality operators in `llc_string.h` only and do not import unrelated base assignments.
- **Why:** the comparison fix was explicitly localized and broad `using asc_t::operator=` changed overload resolution.
- **Required tests:** literal/view/string equality both directions, empty and embedded-NUL cases, copy/move/self assignment, formatting bounds and allocation failure.

### 299 - `llc_string_helper.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_string_helper.cpp 11L/BEEE607F3CA9; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; keep `llc::string` equality operators in `llc_string.h` only and do not import unrelated base assignments.
- **Why:** the comparison fix was explicitly localized and broad `using asc_t::operator=` changed overload resolution.
- **Required tests:** literal/view/string equality both directions, empty and embedded-NUL cases, copy/move/self assignment, formatting bounds and allocation failure.

### 300 - `llc_string_helper.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/2EF952E674CD; GPK gpk_string_helper.h 17L/D0EAA7DFEBB9; repairs-master 18L/2EF952E674CD; sanitization 18L/EF3F14BDFFBF (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; keep `llc::string` equality operators in `llc_string.h` only and do not import unrelated base assignments.
- **Why:** the comparison fix was explicitly localized and broad `using asc_t::operator=` changed overload resolution.
- **Required tests:** literal/view/string equality both directions, empty and embedded-NUL cases, copy/move/self assignment, formatting bounds and allocation failure.

### 301 - `llc_string.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 22L/BF1253A4BE77; GPK absent; repairs-master 22L/0E53AB6AFA17; sanitization 22L/BF1253A4BE77 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; keep `llc::string` equality operators in `llc_string.h` only and do not import unrelated base assignments.
- **Why:** the comparison fix was explicitly localized and broad `using asc_t::operator=` changed overload resolution.
- **Required tests:** literal/view/string equality both directions, empty and embedded-NUL cases, copy/move/self assignment, formatting bounds and allocation failure.

### 302 - `llc_string.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 71L/B0AEF7EE8ABF; GPK gpk_string.h 54L/8A4B87D29AC2; repairs-master 56L/438B587FFDD7; sanitization 71L/42429DFE997F (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve current string/view ownership and formatting semantics; keep `llc::string` equality operators in `llc_string.h` only and do not import unrelated base assignments.
- **Why:** the comparison fix was explicitly localized and broad `using asc_t::operator=` changed overload resolution.
- **Required tests:** literal/view/string equality both directions, empty and embedded-NUL cases, copy/move/self assignment, formatting bounds and allocation failure.

### 303 - `llc_swap.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 25L/68A9A3D0C942; GPK gpk_swap.h 25L/3EE870FAEEF4; repairs-master 25L/68A9A3D0C942; sanitization 17L/19EFDC450A5F (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep the LLC-local three-assignment swap compatible with embedded builds and its established include guard/style.
- **Why:** the header intentionally avoids imposing an unnecessary standard-library dependency across platforms.
- **Required tests:** scalar, aggregate and LLC pointer-wrapper swaps plus embedded compilation.

### 304 - `llc_sync.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 63L/0B186C7000C9; GPK gpk_sync.h 111L/E5AB2328AACB; repairs-master 63L/0B186C7000C9; sanitization 63L/0B186C7000C9 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 305 - `llc_system_event.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 82L/502F8D32EEF6; GPK gpk_system_event.h 80L/D0CB31F4402E; repairs-master 82L/664738D8EAF9; sanitization 80L/85EBDAE814F3 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the typed event hierarchy, numeric identifiers, mask/order rules and payload bytes independently of the transport carrier.
- **Why:** `SEvent` is the semantic language above UDP/TCP; transport work must not redefine it.
- **Required tests:** numeric compatibility, nested event serialization, mask/order filtering, unknown event handling and transport roundtrip as opaque bytes.

### 306 - `llc_system_key.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 206L/797B0AC33C34; GPK gpk_system_key.h 205L/3759EB2C74FD; repairs-master 206L/797B0AC33C34; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the complete GPK keyboard contract while using PC virtual-key numeric values; preserve extended ASCII and remove any artificial `0x7F` ceiling.
- **Why:** only key numbers were a candidate modernization; dropping extended characters removes long-working input.
- **Required tests:** all historical keys, PC VK values, extended-ASCII glyphs, modifiers, key-up/down and text emission.

### 307 - `llc_tcpip.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 326L/293B4DDCFD44; GPK gpk_tcpip.cpp 303L/24825F50E6A9; repairs-master 324L/38E5B5E0E5B2; sanitization 326L/48E4EFE2E322 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 308 - `llc_tcpip.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 166L/E5D9ED7D7DBE; GPK gpk_tcpip.h 166L/18A80FB1139D; repairs-master 166L/CA702A91FDD7; sanitization 167L/DCB8BA432140 (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established network API and error/byte-count semantics while keeping transport below event semantics and compatible with the legacy connection overloads.
- **Why:** carrier refactoring does not authorize changing callers, packet acceptance or socket behavior.
- **Required tests:** loopback send/receive, partial/error paths, IPv4 formatting, invalid/truncated datagrams and legacy overload compilation.

### 309 - `llc_test_core.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 269L/F3C23A4B4D80; GPK absent; repairs-master absent; sanitization 269L/F3C23A4B4D80 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 310 - `llc_test_core.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 229L/44A123311680; GPK absent; repairs-master 80L/8A067DC3892E; sanitization 229L/44A123311680 (clean).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 311 - `llc_timer.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 57L/D014D4A3C196; GPK gpk_timer.cpp 57L/854EBE5C67FC; repairs-master 57L/D014D4A3C196; sanitization 57L/B23ED08D9289 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 312 - `llc_timer.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 35L/9E3BC1C3A408; GPK gpk_timer.h 36L/916C2E9207FC; repairs-master 35L/9E3BC1C3A408; sanitization 35L/9E3BC1C3A408 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 313 - `llc_tri.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 35L/E82807DED64D; GPK gpk_tri.h 47L/8EE4944A07BC; repairs-master 35L/E82807DED64D; sanitization 35L/114E728503C1 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 314 - `llc_tri2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 87L/AB7418573F72; GPK gpk_tri2.h 89L/A0999510204D; repairs-master 87L/AB7418573F72; sanitization 87L/55C605A87E3B (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 315 - `llc_tri3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 110L/5AB745A1706E; GPK gpk_tri3.h 124L/8D5BB6F4F31E; repairs-master 110L/5AB745A1706E; sanitization 124L/A42E32A88E56 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** preserve the established arithmetic/layout contract and take only demonstrated bounds or compile corrections; no coordinate/order rewrite without a failing historical case.
- **Why:** math/raster headers are widely inlined and small semantic changes fan out into GUI and rendering behavior.
- **Required tests:** edge dimensions, signed/unsigned boundaries, clipping, degenerate geometry, layout/size assertions and GPK differential vectors.

### 316 - `llc_type_identifier.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 22L/356978B9FF78; GPK gpk_type_identifier.h 22L/E08FDD620D62; repairs-master 22L/356978B9FF78; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 317 - `llc_type_registry.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 49L/81E6AEBBBE1D; GPK gpk_type_registry.h 49L/7D7B0D0B5F5F; repairs-master absent; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 318 - `llc_typeint.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 63L/4C2396AB232B; GPK gpk_typeint.cpp 66L/EF88751C46D9; repairs-master 63L/4C2396AB232B; sanitization 63L/4C2396AB232B (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 319 - `llc_typeint.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 338L/E267D0F7C62B; GPK gpk_typeint.h 326L/E7B265660CFC; repairs-master 330L/089E131BF967; sanitization 334L/6122FE69FE16 (clean).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** keep only canonical type names and current metadata/enum behavior; do not reintroduce removed `asti*` or other legacy alias blocks.
- **Why:** Pablo explicitly removed the old aliases and duplicate names create migration ambiguity.
- **Required tests:** compile-time identity/size, enum text/value roundtrip, metadata lookup and source scan proving legacy aliases remain absent.

### 320 - `llc_view_bit.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 112L/C7A3DCC9EF37; GPK gpk_view_bit.h 112L/C7A3DCC9EF37; repairs-master 112L/C7A3DCC9EF37; sanitization 112L/C7A3DCC9EF37 (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 321 - `llc_view_color.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/DF3EDCB550DE; GPK gpk_view_color.cpp 28L/8BB8486FEC83; repairs-master 29L/9B690FDDAC8A; sanitization 29L/DF3EDCB550DE (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 322 - `llc_view_color.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 26L/56E238E57E28; GPK gpk_view_color.h 26L/8157F94160E2; repairs-master 26L/56E238E57E28; sanitization 26L/56E238E57E28 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 323 - `llc_view_layered.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_view_layered.h 84L/8671A5AB5625; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 324 - `llc_view_manager.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_view_manager.h 85L/C2A5047967C1; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 325 - `llc_view_n2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/ADC214C68190; GPK gpk_view_n2.h 46L/E4E9E8640FEB; repairs-master 41L/ADC214C68190; sanitization 41L/ADC214C68190 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 326 - `llc_view_n3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/A205A851330B; GPK gpk_view_n3.h 47L/094D2653BF83; repairs-master 41L/A205A851330B; sanitization 41L/A205A851330B (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 327 - `llc_view_pobj.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/9DDD40383467; GPK gpk_view_pobj.h 14L/37499C54CAA1; repairs-master 14L/9DDD40383467; sanitization 14L/9DDD40383467 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 328 - `llc_view_ppod.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 14L/AE02F73B606D; GPK gpk_view_ppod.h 14L/47D443AF49D3; repairs-master 14L/AE02F73B606D; sanitization 14L/AE02F73B606D (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 329 - `llc_view_serialize.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 76L/0872F518F39B; GPK gpk_view_serialize.h 80L/EA9FF770A0AD; repairs-master 76L/0872F518F39B; sanitization 76L/0872F518F39B (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 330 - `llc_view_stream.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 53L/5A7678823797; GPK gpk_view_stream.h 45L/27FEA82786A8; repairs-master 53L/5A7678823797; sanitization 53L/5A7678823797 (untracked).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 331 - `llc_view_tri.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/387815AF0383; GPK gpk_view_tri.h 46L/C172A8F55BDC; repairs-master 41L/387815AF0383; sanitization 41L/387815AF0383 (clean).
- **Text relationship:** Normalized-text matches: thecure = repairs-master = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 332 - `llc_view_tri2.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/B97CF662CEE8; GPK gpk_view_tri2.h 46L/4215F4845C87; repairs-master 41L/B97CF662CEE8; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 333 - `llc_view_tri3.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 41L/B978B4F11094; GPK gpk_view_tri3.h 46L/92C0C60D9BB4; repairs-master 41L/B978B4F11094; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 334 - `llc_view.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 24L/31EB92818754; GPK gpk_view.cpp 24L/D6A905D9ECED; repairs-master 24L/046581779255; sanitization 24L/31EB92818754 (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 335 - `llc_view.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 601L/3B48CE3DD4DC; GPK gpk_view.h 592L/FAC3142D0376; repairs-master 579L/83B05781D41D; sanitization 601L/3B48CE3DD4DC (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = sanitization. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use the common modern LLC container contract as donor when available; preserve element lifetime, allocation failure, view aliasing and exact serialized layout, and do not reintroduce removed aliases.
- **Why:** these are foundation types with broad ABI/performance impact, so textual novelty is not sufficient reason to change behavior.
- **Required tests:** empty/boundary operations, copy/move/self-assignment, allocation failure, iterator/view stability, serialization roundtrip and type traits.

### 336 - `llc_virtual_keyboard.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 18L/5C45C9584AC4; GPK gpk_virtual_keyboard.cpp 21L/0992F2BE3490; repairs-master 18L/5C45C9584AC4; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the complete GPK keyboard contract while using PC virtual-key numeric values; preserve extended ASCII and remove any artificial `0x7F` ceiling.
- **Why:** only key numbers were a candidate modernization; dropping extended characters removes long-working input.
- **Required tests:** all historical keys, PC VK values, extended-ASCII glyphs, modifiers, key-up/down and text emission.

### 337 - `llc_virtual_keyboard.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 29L/F9B7C4359B5F; GPK gpk_virtual_keyboard.h 47L/3F4168E359CA; repairs-master 29L/F9B7C4359B5F; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the complete GPK keyboard contract while using PC virtual-key numeric values; preserve extended ASCII and remove any artificial `0x7F` ceiling.
- **Why:** only key numbers were a candidate modernization; dropping extended characters removes long-working input.
- **Required tests:** all historical keys, PC VK values, extended-ASCII glyphs, modifiers, key-up/down and text emission.

### 338 - `llc_vox.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 437L/6720EBCF7B47; GPK gpk_vox.h 377L/2083796F8A76; repairs-master 437L/28950F14C799; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 339 - `llc_voxel_geometry.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_voxel_geometry.cpp 105L/AFEB02830DF5; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 340 - `llc_voxel_geometry.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure absent; GPK gpk_voxel_geometry.h 24L/5472398ADFE3; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** exclude this GPK-only historical facility from the final LLC core merge unless a live current consumer is separately demonstrated; retain it as lineage evidence.
- **Why:** none of the three LLC snapshots contains it, so automatic import would add unrequested product surface.
- **Required tests:** project/include/consumer scan and, only if later admitted, an isolated contract suite before registration.

### 341 - `llc_voxel.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 242L/C957E91FC658; GPK gpk_voxel.h 235L/A7196733A611; repairs-master 242L/C957E91FC658; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 342 - `llc_wifi.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 1L/53BFB13A06EE; GPK absent; repairs-master 1L/53BFB13A06EE; sanitization 1L/53BFB13A06EE (clean).
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** retain the common thecure/sanitization implementation; review GPK or repairs differences only for lost behavior, not as automatic replacements.
- **Why:** two active LLC snapshots agree exactly and no repository-local rewrite needs promotion.
- **Required tests:** public API smoke coverage, error paths and a GPK differential check for the file's observable operations.

### 343 - `llc_wifi.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 31L/B4C0F13C021B; GPK absent; repairs-master 31L/B4C0F13C021B; sanitization 31L/A7C9C6A1AA7E (unstaged modification).
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** three-way merge from thecure, GPK and repairs into the sanitization owner, choosing each semantic change only with a concrete contract; do not publish the current variant merely because it is present.
- **Why:** the sanitization text differs from the available donors and hash/line count cannot establish correctness.
- **Required tests:** API/ABI compilation, historical differential vectors, boundary/error paths and consumer integration.

### 344 - `llc_window.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 161L/4B3DD63DBF07; GPK gpk_window.cpp 161L/7B15DF455F4B; repairs-master 161L/4AD8E3F022E5; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 345 - `llc_window.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 89L/2F147B407383; GPK gpk_window.h 90L/4F8901DB02EF; repairs-master 90L/DA1CBC54AC18; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 346 - `llc_windows.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 50L/6F32F3884252; GPK gpk_windows.cpp 52L/47B13EB2CC84; repairs-master 51L/8B731B2D8E2C; sanitization 50L/024C3A14D95D (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 347 - `llc_windows.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 15L/CB8E78AA4B88; GPK gpk_windows.h 15L/C1A52141D804; repairs-master 17L/660CFAD3EF40; sanitization 15L/49A2E889DD7F (unstaged modification).
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** use GPK as the behavioral baseline and admit only required LLC namespace/type substitutions plus the three accepted GUI changes at their actual owners.
- **Why:** GUI is performance-sensitive and historical behavior is authoritative; no other validation, cleanup, order, geometry or state rewrite is authorized.
- **Required tests:** file-specific GPK differential tests, control state/geometry tests and pixel/input parity where the file participates.

### 348 - `llc_word_dict.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 76L/1C2D11CF883B; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the thecure WordDict contract: accented bytes map to ASCII, scratch slots stay preallocated, skipped words do not invent compaction.
- **Why:** that is the working Obelisco behavior already recovered; inverse mapping and compact-scratch expectations were identified as invented regressions.
- **Required tests:** accented-to-ASCII vectors, posting lists, skip words, repeated calls and preallocated scratch slot counts.

### 349 - `llc_word_dict.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 23L/CCA04DCED462; GPK absent; repairs-master absent; sanitization absent.
- **Text relationship:** All available snapshots are normalized-text identical. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** restore the thecure WordDict contract: accented bytes map to ASCII, scratch slots stay preallocated, skipped words do not invent compaction.
- **Why:** that is the working Obelisco behavior already recovered; inverse mapping and compact-scratch expectations were identified as invented regressions.
- **Required tests:** accented-to-ASCII vectors, posting lists, skip words, repeated calls and preallocated scratch slot counts.

### 350 - `llc_xml_reader.cpp`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 275L/ED9D54E95738; GPK gpk_xml_reader.cpp 370L/82CED70A2CDE; repairs-master 275L/0A739EAD260A; sanitization absent.
- **Text relationship:** Every available snapshot has a distinct normalized text hash. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

### 351 - `llc_xml_reader.h`

- **State transition:** `0-Unknown -> 1-evaluating -> 2-Merging`.
- **Source evidence:** thecure 68L/2E536A9A63AA; GPK gpk_xml_reader.h 60L/B01FA26CCA38; repairs-master 68L/2E536A9A63AA; sanitization absent.
- **Text relationship:** Normalized-text matches: thecure = repairs-master. The remaining variants differ. Hashes compare line-ending-normalized text with GPK/LLC namespace spelling normalized; a mismatch is not by itself a behavioral defect.
- **Final merge requirement:** recover the thecure implementation as the merge donor, reconciling only demonstrated GPK compatibility and repairs-master error fixes before publication.
- **Why:** the sanitization baseline lacks the file while the current LLC lineage still carries it.
- **Required tests:** public API compile coverage plus focused behavior, failure-path and historical-format tests for this facility.

## Completion validation

- `351` inventory names, `351` checklist records and `351` analysis records; both output orders exactly match `LLC_FILE_INVENTORY.md`.
- Historical result of this pass: `351` records reached `2-Merging`. The later live-checklist reset supersedes those states and places all 351 files at `0-Unknown` pending fresh verification; the analysis remains a plan/evidence source only.
- Every entry contains a source comparison, a final-merge requirement, a reason and required tests. Entries 001-002 spell their Adam7 source comparison as `Sources`/`Differences`; entries 003-351 carry normalized source line counts and short hashes explicitly.
- The historical memory checklist remains byte-identical at SHA-256 `D2C39D58F1E5003387044650C5955B34D3CFEFA8CAE827858015DD84F405D3BB`.
- No product source or test file was edited by this evaluation. No build or test was run. The product index that initially had fingerprint `ab5d966b1ce7c7cd283711c3998f4a0055aa53c3` was externally emptied during the pass; current `HEAD` remains `6e261caca6de66d4b5150212a46cd979ac85a578`, and this evaluation did not stage, reset, commit or restore it.

## Fresh source revalidation pass

This section records the new sequential pass requested after the checklist reset. A file reaches live state `2-Merging` only after its prior plan is checked against all four source roots again and explicitly approved or revised here.

### R001 - `llc_adam7.cpp`

- **Live transition:** `0-Unknown -> 1-Evaluating -> 2-Merging`.
- **Four-source check:** thecure `34L/EE45438D4B41`; GPK `53L/99461468D7E4`; repairs-master `34L/F920BE8F0CED`; sanitization `34L/EE45438D4B41`.
- **Prior plan:** **approved**.
- **Decision:** retain sanitization unchanged. It is text-identical to thecure; repairs differs only by the obsolete condition macro; GPK uses the older hand-expanded pass-size formulas.
- **Reason:** GPK's formulas lose valid pixels for small dimensions, while the generalized base/multiplier calculation covers them and adds the required seven-slot guard. `adam7ScaleIndex()`'s pass table and all unrelated logic remain unchanged.
- **Merge work:** no product delta for this file. Keep its focused Adam7 tests pending the later suite-wide build/runtime pass.

<!-- NEXT-REVALIDATION -->
