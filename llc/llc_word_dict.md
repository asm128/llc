# LLC WordDict

`SWordDict` converts arbitrary text fields into an inverted index:

```text
record fields -> normalized words -> [record identifiers]
```

The dictionary does not need to know whether a record is a product, process, file, log entry, JSON message, `SEvent`, Blitter row or sensor sample. The caller presents the searchable fields as `view<vcst_c>` and chooses the `u2_t` identifier that points back to the authoritative record.

That boundary is the main advantage. Storage owns records, the application chooses which fields are searchable, and WordDict owns only the reusable `word -> record IDs` relation.

## Basic contract

```cpp
::llc::SWordDict             wordIndex;
::llc::aobj<::llc::string>   scratch;
cnst ::llc::vcst_t           fields[] = {"Coffee Grounds", "Bakery and coffee", "food"};

if_fail_fe(::llc::getWordDict(wordIndex, fields, 0, scratch));
```

After that call the index contains entries conceptually equivalent to:

```text
coffee  -> [0]
grounds -> [0]
bakery  -> [0]
food    -> [0]
```

The default stop-word list removes common English and Spanish noise such as `the`, `and`, `de`, `la` and `para`. Repeated appearances of a word in one record do not duplicate that record identifier.

`scratch` is caller-owned working storage. Keep one instance outside an ingestion loop and reuse it; its contents describe only the most recently indexed record.

## Demo 1: index application records without adapting WordDict

```cpp
struct SProduct {
	::llc::vcst_t Name;
	::llc::vcst_t Description;
	::llc::vcst_t Category;
};

::llc::err_t indexProducts
	( ::llc::view<cnst SProduct> products
	, ::llc::SWordDict & wordIndex
	) {
	::llc::aobj<::llc::string> scratch;
	for(::llc::u2_t iProduct = 0; iProduct < products.size(); ++iProduct) {
		cnst SProduct & product = products[iProduct];
		cnst ::llc::vcst_t fields[] = {product.Name, product.Description, product.Category};
		if_fail_fe(::llc::getWordDict(wordIndex, fields, iProduct, scratch));
	}
	rtrn 0;
}
```

Nothing in WordDict depends on `SProduct`. A different caller can pass `{ProcessName, CommandLine, Owner}`, `{Path, Title, Contents}` or `{Method, Route, JsonText}` without creating another indexer.

If two products contain `coffee` and only the second contains `bakery`, the postings are:

```text
coffee -> [0, 1]
bakery -> [1]
```

An `AND` query intersects those postings and immediately produces `[1]`. The query does not scan every product description again.

## Demo 2: choose the noise for a specific domain

```cpp
cnst ::llc::vcst_t logNoise[] = {"request", "status", "info", "pid"};
cnst ::llc::vcst_t fields[]   = {"request build status failed", "pid 26892"};

::llc::SWordDict             logIndex;
::llc::aobj<::llc::string>   scratch;
if_fail_fe(::llc::getWordDict(logIndex, fields, 26892, scratch, logNoise));
```

This keeps useful terms such as `build` and `failed` while omitting protocol noise. A custom list replaces the default list; callers that need both should construct one combined view deliberately.

This is useful for LLS because each surface can choose its own low-value vocabulary while sharing the same index representation:

- processes can omit `exe`, `pid` and common command-line switches;
- build results can omit boilerplate while retaining symbols, filenames and diagnostics;
- services can index instance, route, capability and executable metadata;
- inboxes can index sender, subject and body;
- Blitter can store the records while WordDict stores only their searchable relation.

## Demo 3: index protocol data without coupling the protocol to search

```cpp
::llc::err_t indexProtocolRecord
	( ::llc::SWordDict & wordIndex
	, ::llc::u2_t recordId
	, ::llc::vcst_t eventType
	, ::llc::vcst_t route
	, ::llc::vcst_t jsonMethod
	, ::llc::vcst_t jsonText
	, ::llc::aobj<::llc::string> & scratch
	) {
	cnst ::llc::vcst_t fields[] = {eventType, route, jsonMethod, jsonText};
	rtrn ::llc::getWordDict(wordIndex, fields, recordId, scratch);
}
```

`SEvent` remains the serialized payload and JSON remains one possible representation. Search sees only selected text views. This keeps transport, event routing, storage and indexing independent while allowing all of them to share the same searchable vocabulary.

## Reading postings

The current representation intentionally stays simple and serializable. A minimal read helper is enough for exact lookup:

```cpp
::llc::view<cnst ::llc::u2_t> wordPostings
	( cnst ::llc::SWordDict & wordIndex
	, ::llc::vcst_t word
	) {
	for(cnst ::llc::SWordIndices & entry : wordIndex)
		if(entry.Text == word)
			rtrn {entry.Indices.begin(), entry.Indices.size()};
	rtrn {};
}
```

Single-word search returns one postings view. `OR` merges postings; `AND` intersects them. If callers ingest monotonically increasing record IDs, postings remain ordered and both operations can be linear in the number of matched IDs.

## Why it is convenient

### One indexer for unrelated schemas

The caller selects fields through views instead of teaching WordDict every record type. The indexer therefore belongs in LLC, not in a dashboard, database, protocol or application.

### Source data stays with its owner

The dictionary stores each distinct word plus record identifiers. It does not copy complete records and does not become another database. An identifier can address a Blitter row, array position, stable object ID, request ID or PID according to the caller's contract.

### Search work moves away from interaction time

Indexing performs tokenization once when records arrive or change. Queries operate on compact postings instead of repeatedly scanning every original field. This matters most when a dashboard or agent issues many searches over data that changes less often than it is queried.

### The scratch allocation is reusable

`indexable` is an explicit caller-owned buffer. Reusing it across records avoids creating a new collection for every call and makes temporary memory visible instead of hiding it in global state.

### The index is naturally composable

The same postings support exact lookup, unions, intersections, counts, menus, autocomplete candidates and reverse navigation from a term to its owning records.

## Practical performance rules today

1. Keep one `scratch` array per indexing worker and reuse it.
2. Reserve `wordIndex` when the caller has a useful vocabulary estimate.
3. Pass only fields that users or agents will actually search.
4. Use stable record identifiers; do not encode transient pointers.
5. Ingest monotonically increasing IDs when linear posting intersection is desired.
6. Build separate dictionaries when tenants require independent lifetime, access or vocabulary policy.
7. Treat a custom stop-word list as a replacement, not an extension of the default.

## Improvements with the highest payoff

### 1. Share normalization between ingestion and queries

The next convenience API should expose the same tokenization, lowercase conversion, punctuation handling and aliases used by ingestion. Query code must not invent a second normalization path or `area` and its indexed alias can disagree.

Suggested surface:

```cpp
err_t wordList(aobj<string> & words, view<vcst_c> fields, view<vcst_c> skipWords = WORDDICT_TEXT_SKIPWORDS);
err_t wordPostings(cnst SWordDict & index, vcst_t query, view<cnst u2_t> & output);
```

### 2. Reuse all temporary allocations

The current caller reuses normalized words, but splitting still owns a temporary array inside the implementation. A dedicated scratch value can retain every temporary buffer without changing the dictionary format:

```cpp
struct SWordDictScratch {
	aobj<vcst_t> Split;
	aobj<string> Words;
	string       Word;
	string       Alias;
};
```

This pays off during bulk indexing and keeps the API honest about memory ownership.

### 3. Add a vocabulary accelerator without replacing `SWordDict`

Construction and exact lookup currently scan the distinct-word array. If `W` tokens produce `V` distinct words, construction can approach `O(W * V)`. Keep `SWordDict` as the compact canonical/serializable representation and optionally maintain a secondary `word -> entry index` map while building or serving large dictionaries.

That preserves the simple format while reducing average insertion and lookup toward constant time.

### 4. Upgrade normalization to explicit UTF-8 policy

The inherited alias table handles a specific one-byte character set. Modern files, JSON and remote messages need an explicit UTF-8 folding policy. It should be selectable data, not a hidden platform assumption, and should retain the original term when aliases are added.

### 5. Make token boundaries configurable

Splitting only on spaces and trimming only the inherited punctuation is correct for the restored behavior, but source code, paths, JSON keys and command lines need different boundaries. A tokenizer policy can decide whether `_`, `-`, `/`, `.`, `:` and camel-case transitions separate or preserve a term.

### 6. Add update and removal operations

Append-only ingestion is enough for immutable documents and histories. Processes, services and edited files need removal or replacement by record ID. The efficient version should retain each record's indexed terms so an update touches only affected postings instead of rebuilding the complete dictionary.

### 7. Compact large posting lists

Ordered IDs can be delta-encoded and stored with LLC packed integers. This matters for common terms across millions of records, while small postings should remain cheap and direct. Compression belongs in storage/serialization policy; it should not complicate the basic indexing call.

## Architectural boundary

WordDict should remain deliberately ignorant of:

- file IO and directory traversal;
- JSON parsing;
- `SEvent` serialization;
- Blitter storage;
- network transport;
- GUI controls;
- authorization and tenant selection.

Those owners provide text fields and stable IDs. WordDict returns the reusable relation that all of them can query.
