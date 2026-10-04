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
- Preserve LLC's `err_t` result convention. Operations that can fail or report an operational result return `err_t` and place produced values in output parameters. Direct value returns are for operations that cannot fail, such as arithmetic. A different return type requires Pablo to request and approve that exact exception three separate times.
- After every code edit, re-evaluate the changed signatures and bodies against this file and `../../FIRST_LEVEL_CONSTRAINTS.md`. Check return types before implementation details; then check ignored results, failure propagation, output mutation, ownership and allocation.
- Inspect both the staged and unstaged LLC diff during that post-edit evaluation. A corrected working tree is not sufficient if the rejected version remains staged.
- Do not call an API more convenient because its call expression is shorter when the shorter form removes an error channel, output or behavior. That is functionality erasure, not simplification.
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
- Prefer `always_printf()` for ordinary console output unless a more specific LLC output facility fits better. This keeps output inside the framework even when it is not an error, warning or informational diagnostic.
- Use `error_printf`, `warning_printf` and `info_printf` for their corresponding diagnostic levels so callbacks, timestamps and source context keep working.
- Do not default to `printf()`. If a C-standard output boundary is genuinely required, prefer `fprintf()` with an explicit stream, and recognize that it deliberately bypasses the LLC logging framework.
- `log_print()` and `log_write()` are the direct callback-dispatch boundaries. Do not recreate `base_`, underscored or default-forwarding aliases around them.
- Let `printf_arg()` in `llc_typeint.h` normalize typed pointers for the formatting boundary. Do not scatter `(const void *)` casts through log call sites merely to satisfy `%p`; keep semantic representation casts visible where their meaning is chosen.
- The Arduino flash-string path is deliberately a macro at the literal call site: `log_print_F(text)` must let `F(text)` see the original literal. Keep actual dispatch and policy in functions.
- `LLC_VA_TAIL(...)` isolates optional variadic-comma syntax: C++20 uses `__VA_OPT__`, while older targets retain the compatible fallback. Do not duplicate whole severity macros to solve that preprocessing detail.
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
- The working-tree `split(separator, input)` overload returns `err_t` and mutates the supplied view to its left slice. Do not replace that result channel with a directly returned view. Its intended composition relies on `find()` returning `-1` and `slice()` interpreting `(u2_t)-1` as the complete remaining range; the heavier overload derives the right side from the untouched original at `left.size() + 1`.
- `view<>` behavior and negative cases live in `llc_test_core/llc_test_view.cpp`. Use those tests before changing constructors, slicing, iteration, lookup, extrema or callback semantics.

## Paths, locators and runtime syntax policy

- Path syntax is documented grammar, not behavior that must be discovered by calling the host. Implement Windows, POSIX and URI interpretation in LLC; keep native calls for actual filesystem observation and for runtime context only when a caller does not supply it.
- Parsing policy must be runtime data. A cross-platform program running on Linux must be able to select Windows semantics for supplied text, and a runtime URL must be parseable without choosing a new template type or rebuilding.
- The parser may still be `constexpr`. This means the same state machine can participate in `static_assert` when both input and policy are constant; it does not make compile-time policy selection the primary interface.
- Preserve the complete leading separator/prefix span until the selected policy classifies it. Do not reject or collapse repeated leading separators through a universal duplicate-separator rule.
- Windows policy must distinguish at least ordinary relative paths, current-drive-rooted paths, drive-relative paths such as `C:x`, drive-absolute paths such as `C:/x`, UNC paths and device/file namespace prefixes. `C:x` requires the remembered current directory for drive C when resolution is requested; the parser can identify that requirement without obtaining the value itself.
- Parsing, classification, normalization, composition and resolution against a supplied base do not require `std::filesystem`. Enumeration, existence/type queries, symlink or junction resolution and acquisition of an omitted runtime base are separate native operations.
- URI parsing should expose scheme, authority, path, query and fragment as views. Query key/value parsing and decoding are a separate facility over the returned query view.
- Current working-tree status and remaining debt:
  - an unverified `pathBegin()`/protected-span repair is intended to prevent recursive `pathList()` from stripping `/` to empty or `C:/` to `C:`; focused cases exist, but no build or test run had verified this checkpoint when it was recorded;
  - fixed `LLC_MAX_PATH` buffers accept `snprintf()` truncation lengths and can use the required length as though it were stored length;
  - the recursive overload carries `extension` through recursion without applying it to files.
- Root preservation must be tested end to end through listing, not only through `pathNormalize()`. Include native-root, drive-root and applicable network/device-prefix cases under their selected policies.

## Compile-time algorithms and fold expressions

- The project builds as C++20. Loops and state machines can be evaluated at compile time; do not assume constexpr code must be a single expression.
- Prefer an independently testable constexpr algorithm before hiding it behind a macro.
- Return structured compile-time results when diagnostics matter: value/state, error category and exact source offset are more useful than a boolean.
- If returned spans reference parsed text, keep their storage lifetime explicit. The current CTTI parse result owns copies of its structure name and member declaration text.
- Use `static_assert` to prove the important path truly executes at compile time, then add runtime test records for visible diagnostics and negative cases.
- When replacing a macro with a constexpr function, verify both value and `decltype`. Conditional-expression promotion and `if constexpr` return deduction can produce identical numbers with different types, changing overload resolution.
- The live `szof` macro currently retains its original promoted expression type. Do not enable the commented constexpr replacement until its fixed-versus-size-dependent type contract is chosen deliberately. `bcof(type)` expresses `szof(type) * 8U` without forcing a consumer width.
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
- Keep stable diagnostic context at the caller that owns it. A typed caller should report its type once after the group; do not thread an unchanging type-name pointer through every helper and check.
- Let the private result enum own durable operation identity, numeric value and description. Do not pass a second operation-name string that can become stale.
- Inner helpers report the values that vary per case. Outer callers aggregate successful completion once per type or suite instead of repeating the same file, function and type information for every check.
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

  `build_test_core.bat`

- The batch file defaults to `x64|Debug`, finds Visual Studio with `vswhere`, normalizes the inherited `PATH`, asks the solution to build `llc_test_core` and its dependencies, then runs the shared executable from the unified output directory.
- To build one solution target without running the shared suite, use the same entry point and select its build directory and target, for example:

  `build_test_core.bat llc llc_test_core`

  `build_test_core.bat llt lls`

- Override the defaults by adding platform and configuration after the build directory and target, for example:

  `build_test_core.bat llt lls x64 Release`

- The batch file discovers exactly one `.sln` or `.slnx` in the selected directory instead of depending on a solution filename. It rejects missing or ambiguous solution directories.
- Do not invoke an individual project directly. The discovered solution supplies `SolutionDir`, dependency ordering and the shared `llb/<platform>.<configuration>` output layout; a direct project build can create `obj` and configuration directories inside a source repository.
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
