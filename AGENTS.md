# LLC working memory

This file is operational memory for future work in this repository. Read it before changing `llc` or `llc_test_core`. The source remains authoritative: use the paths below to verify an idiom before extending it.

## Direction

- Treat `llc` as the foundational layer: small types, views, containers, diagnostics, serialization, parsing, compile-time metadata and platform facilities.
- Review and test one facility at a time. Move foundational facilities that still live in `gpk` into `llc` only as their contracts become clear.
- The long-term possible shape is `llc` plus the higher-level engine currently named `gpk_engine`; the old `gpk` may eventually disappear and `gpk_engine` may become `gpk`. Do not perform that migration prematurely or in bulk.
- Tests are primarily design validation and executable contracts. They are not valuable if a failure requires separate forensic work.

## Preserve the local C++ style

- Follow the alignment and compactness of the surrounding file. The author reads vertically aligned code more easily; do not casually reformat it into conventional unaligned C++.
- Prefer concise procedural/functional code, compact single-line functions where they remain readable, and comments only when they add information the code cannot express.
- Use existing LLC aliases and macros (`u0_t`, `s2_t`, `cnst`, `stxp`, `inxp`, `rtrn`, `szof`, and the diagnostic macros) instead of introducing a parallel vocabulary.
- Keep foundational headers light. Avoid adding a dependency tree when one constexpr expression or small local facility will do.
- Use actual APIs and call sites before designing a change. Do not invent missing LLC interfaces from memory.
- Do not add OOP ceremony, registration machinery or external-framework structure unless a demonstrated requirement pays for it.
- Preserve unrelated working-tree edits. This repository is often edited concurrently from Visual Studio.

## Diagnostics are control-flow infrastructure

- Read `llc/llc_log.h` and `llc/llc_log_level.h` before writing error paths. The macros are not merely shorter logging calls: they combine a condition family, log severity, formatted context and a control-flow action.
- Do not mechanically expand an LLC failure into `if(failed(result)) { fprintf(...); return ...; }`. That usually duplicates expression capture, source location, result handling and flow already encoded by the macro family.
- Choose the macro that states the actual contract:
  - distinguish `if_true`, `if_fail`, `if_zero` and `if_null`;
  - distinguish log-only, `break`, `continue`, return-value, fail, and throw behavior;
  - use the formatted variant when operational values make the failure actionable.
- Do not guess a compact macro's semantics from one letter. Inspect its alias in `llc_log_level.h`, especially when it matters whether the exact negative result or a generic failure is returned.
- Preserve cleanup. A macro should compress the diagnostic and branch, not silently skip disconnect/close/reset work. Use a block form when cleanup must precede `continue`, `break` or `return`.
- Use `error_printf`, `warning_printf`, `info_printf` and `always_printf` for diagnostics so callbacks, timestamps and source context keep working. Direct `printf` may still be correct for intentional user-facing CLI output; direct `fprintf(stderr, ...)` is not a substitute for LLC diagnostics.
- `LLC_LOGGING_CASE_STUDY.md` records a concrete failure of this discipline in the initial `lls`, `lls-l` and `lls-t` implementation, with the exact Git commits and files needed to recover the full before/after evidence.

## Views and strings

- Prefer LLC views over raw pointer/count pairs. For string literals use `LLC_CXS("text")`; carry `vcsc_t`/the appropriate string view until a raw pointer is required by an external boundary.
- `view_const_string` and the const string-view path intentionally make empty text safe to consume. Empty text should not crash merely because no characters exist.
- `str()` is a best-effort adapter for consumers that take strings:
  - return a string view directly when the input already has compatible storage;
  - preserve mutability for mutable input and constness for const input;
  - use static-array temporary storage for numeric values that cannot be represented as a view over their original object;
  - avoid copying when a view can be returned safely.
- Important references:
  - `llc/llc_view.h` — `view<>`, string-view aliases, `LLC_CXS()` and `str()`.
  - `llc_test_core/llc_test_str.cpp` — mutable/const arrays, views, static/dynamic POD arrays, empty strings, booleans and numeric storage.
- Empty views may be null. Never form an invalid pointer merely to represent an empty range. One-past-end is valid; `begin - 1` and `end + 1` are not.
- Bounds failures should use the current throwing idiom (`if_null_te()`, `if_true_tef()` and `LLC_FMT_*`) where the API contract throws.
- `view<>` behavior and negative cases live in `llc_test_core/llc_test_view.cpp`. Use those tests before changing constructors, slicing, iteration, lookup, extrema or callback semantics.

## Compile-time algorithms and fold expressions

- The project builds as C++20. Loops and state machines can be evaluated at compile time; do not assume constexpr code must be a single expression.
- Prefer an independently testable constexpr algorithm before hiding it behind a macro.
- Return structured compile-time results when diagnostics matter: value/state, error category and exact source offset are more useful than a boolean.
- If returned spans reference parsed text, keep their storage lifetime explicit. The current CTTI parse result owns copies of its structure name and member declaration text.
- Use `static_assert` to prove the important path truly executes at compile time, then add runtime test records for visible diagnostics and negative cases.
- Before recreating compile-time arithmetic or string conversion, inspect `llc/llc_cpow.h`, `llc/llc_view.h` and their suites; some existing operations are already constexpr-capable.
- Fold expressions are useful when one operation genuinely applies to every pack element:
  - ordered comma fold for suite execution: `((operation(suites)), ...);`
  - arithmetic fold for field widths: `(u2_t(0) + ... + u2_t(widths));`
  - logical fold for contracts: `((widths > 0) && ...);`
- Use the comma operator when left-to-right execution is part of the contract. Use `&&`/`||` only when short-circuiting is intended.
- Do not erase callable types merely to make a parameter pack homogeneous. `llc_test_core/llc_test_core.cpp` templates `STestSuite` on the concrete function type and stores a function reference; it intentionally uses neither a raw function pointer nor `std::function`.

## Current CTTI parser milestone

- `llc/llc_ctti.h` is the first independent compile-time reflection component, not finished reflection.
- It currently parses the natural member text intended for a future macro, for example `u0_t x, y; s2_t z; f2_t weight;`.
- The first grammar recognizes canonical scalar spellings from `llc_typeint.h`, plus optional `llc::` and `::llc::` qualification. Native spellings such as `int` are deliberately unsupported in this phase.
- It preserves member order, handles shared declarations and whitespace, accepts empty structures, and reports structured failures for invalid structure names, unsupported types, invalid/missing members and missing semicolons.
- `llc_test_core/llc_test_ctti.cpp` contains compile-time assertions and the negative grammar cases.
- The intended later surface is conceptually a disable-friendly macro such as `LLC_REFLECTED_STRUCT(name, members)`. Keep parsing/metadata independent so the macro can degrade to an ordinary `struct` definition when reflection is disabled.
- Do not jump directly to general C++ parsing. Grow the grammar from LLC's actual type and declaration needs.

## Bit and packed facilities

- `llc/llc_bit.h` contains `bit_field<T, widths...>`:
  - widths, offsets and masks are constexpr;
  - every width must be nonzero;
  - total width must fit the unsigned storage type;
  - fields are addressed by compile-time index;
  - enum storage and enum mask values are an intended use case, including runtime GUI-driven masks.
- `llc_test_core/llc_test_bit.cpp` is the reference for fixed masks, field layout, truncation and all supported storage widths.
- `llc/llc_packed_int.h` is foundational and should remain lightweight. `packed_uint<>` accepts unsigned storage only and derives its bit-field width at compile time with `uint_width_field_size<T>()`.
- Serialization helpers that would add higher-level dependencies belong in the serialization layer rather than `llc_packed_int.h`.
- Save functions append; they do not silently clear caller output. Load functions consume the input view by slicing it past the decoded bytes.
- Validate enough input before dereferencing packed storage. Use the return type and signed-result convention correctly; an unsigned width cannot itself represent failure.
- Reference implementations and contracts:
  - `llc/llc_view_serialize.h`
  - `llc_test_core/llc_test_packed_int.cpp`
  - `llc_test_core/llc_test_view_serialize.cpp`
- `llc/llc_view_bit.h` and `llc_test_core/llc_test_view_bit.cpp` establish these invariants:
  - null empty views are valid;
  - begin/end are real positions with one-past-end semantics;
  - iterator equality means the same logical position;
  - diagnostics report the correct logical bit index;
  - unsigned decrement never depends on underflow;
  - no `Begin - 1` or `End + 1` position is formed;
  - partial final bit counts stop at the logical count, not the end of backing storage;
  - iterator offset width is derived from the backing integer type.

## Test-system contract

- Shared test machinery lives in `llc_test_core/llc_test_core.h`; suite timing and dispatch live in `llc_test_core/llc_test_core.cpp`.
- Each suite keeps a private result enum whose name, numeric value and description explain the contract. Generic records aggregate successes and failures without discarding suite-specific meaning.
- `LLC_TEST_CHECK` records a failure and continues whenever continued execution is safe. Multiple failures can illuminate the first defect.
- `LLC_TEST_REQUIRE` records and returns only when continuing would make the test invalid or unsafe.
- A suite's `err_t` is for test-machinery failure. Logical contract failures belong in the shared result collection.
- Production logging macros already provide file, line, function and formatted values. Test checks should add the counterexample: type width, index/count, expected/actual value, seed and iteration where relevant.
- A successful run remains concise but must say what ran: each suite prints elapsed microseconds and executed-check count, followed by the aggregate. `--success-details` prints every successful result group at runtime.
- `LLC_TEST_SUITE(func)` deliberately stringifies the same function token used for dispatch so the diagnostic name cannot drift from the callable.
- The variadic runner uses a fold expression and concrete function references. Do not replace it with a raw function-pointer table.
- Negative tests may intentionally trigger production error logs. Determine success from the named suite result, aggregate summary and process exit code, not from the mere presence of an expected diagnostic line.

## Multiplying coverage economically

- Express the behavioral contract once as a function template, then run every applicable LLC width through it.
- For general integer containers/views, normally cover `u0_t`, `u1_t`, `u2_t`, `u3_t`, `s0_t`, `s1_t`, `s2_t` and `s3_t`.
- For unsigned representations such as packed integers and bit backing storage, normally cover all four unsigned widths.
- Fixed boundary cases and deterministic generated cases complement each other:
  - explicitly test empty, first, middle, last, one-past-end and malformed ranges;
  - explicitly test serialization width transitions;
  - then use `SPRNG` to multiply cases;
  - log the fixed seed and iteration so every generated failure is reproducible.
- Random generation never replaces narrow boundaries that it is unlikely to hit.
- For mutation failures, verify preservation of address, count, contents and terminator—not merely the returned error.
- Aliasing is a first-class contract for `array_pod<>`: reallocation, self-append, scalar self-insertion, overlapping chain insertion and overlapping subview assignment all have regression coverage in `llc_test_core/llc_test_array_pod.cpp`.
- When a test exposes a design defect, fix the implementation and retain the counterexample as a named contract.

## Build and verification workflow

- From this repository root, run:

  `build_test_core.bat Debug x64`

- The batch file finds Visual Studio with `vswhere`, normalizes the inherited `PATH`, builds `zlibvc`, `llc` and `llc_test_core`, then runs the shared executable from the unified output directory.
- The build is C++20 with `/W4 /WX`; treat warnings as failures.
- `LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG` is defined for the test project so expected negative cases do not stop in `crtdebugbreak()`.
- Keep using this shared workflow instead of repairing the Visual Studio environment ad hoc in each session.

## Measured reference point

- Dated reference run: 2026-10-02, Debug x64.
- Result: all **150,822 executed checks** passed across **308 named result groups** in **441,909 microseconds**.
- The system reached this scale in roughly one concentrated day by combining reusable invariants, type templates, deterministic generation and existing diagnostics; it was not produced by writing 150,822 individual test cases.
- Current suites cover `SPRNG`, `bit_field<>`, `cpow()`, the CTTI parser, `str()`, `array_static<>`, `array_pod<>`, the JSON reader, `view<>`, `view_bit<>`, `packed_uint<>` and view serialization.
- Treat these numbers as historical evidence, not a hard-coded expectation. Update this section and `../../profile/when-library-diagnostics-become-a-test-framework.md` when the suite composition or measured totals change materially.

## Review workflow

- Work in reviewable steps. The preferred cycle is:
  1. inspect implementation and real call sites;
  2. identify explicit success and failure contracts;
  3. add or extend the focused suite;
  4. run the complete shared test executable;
  5. evaluate inconsistencies with the author before broad redesign;
  6. fix only the agreed behavior and keep the regression.
- Do not rush to add generalized machinery. Reuse and improve what exists when repeated needs make the abstraction evident.
- Useful context and rationale are recorded in `../../profile/when-library-diagnostics-become-a-test-framework.md`; the executable sources remain the source of truth.
