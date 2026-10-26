# LLC test suite checklist - Axiom

This tracker owns only Axiom's clean-room `.cpp` suite progress for inventory files 351 through 202 in descending order.

It must not replace, rewrite or annotate Wirewright's `LLC_TEST_SUITE_CHECKLIST.md` or Conduit's sanitization/merge state.

## State contract

- `0-Unprocessed`: no focused suite has started for this file.
- `1-Implementing`: its focused suite is being written or refined.
- `2-Testing`: its focused suite source is ready for later registration and verification.
- `3-Tested`: its required build/runtime evidence has been recorded.

A checkbox is marked only at `3-Tested`.

## Files

- [ ] `llc_xml_reader.cpp` - `0-Unprocessed`
- [ ] `llc_word_dict.cpp` - `0-Unprocessed`

`llc_word_dict.cpp` is provisional: Pablo rejected the damaged thecure snapshot as an authority. Recover and compare the earlier original before registration or runtime verification.

