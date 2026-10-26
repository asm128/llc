#include "llc_test_core_suites.h"
#include "llc_queue_async.h"

GDEFINE_ENUM_TYPE(TEST_QUEUE_ASYNC_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, POST        , 0, "An asynchronous queue appends posted values under its lock.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, READ_RETAIN , 1, "Reading can copy the queue without clearing it.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, READ_CLEAR  , 2, "Reading can copy and clear the queue in one operation.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, NEXT        , 3, "Next returns FIFO values and one plus the remaining message count.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, CLEAR       , 4, "Clearing removes every queued value.");

sttc ::llc::err_t testQueueAsyncPost(::llc::ATestError & results) {
	::llc::qpod<::llc::u2_t> queue;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_POST, 0 != queue.Post(7));
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_POST, 1 != queue.Post(8));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_POST, queue.Size() != 2);
	rtrn 0;
}

sttc ::llc::err_t testQueueAsyncReadRetain(::llc::ATestError & results) {
	::llc::qpod<::llc::u2_t> queue;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ_RETAIN, 0 != queue.Post(7));
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ_RETAIN, 1 != queue.Post(8));
	::llc::au2_t copied;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ_RETAIN, 0 != queue.Read(copied, false));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_READ_RETAIN, copied.size() != 2 || copied[0] != 7 || copied[1] != 8 || queue.Size() != 2);
	rtrn 0;
}

sttc ::llc::err_t testQueueAsyncReadClear(::llc::ATestError & results) {
	::llc::qpod<::llc::u2_t> queue;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ_CLEAR, 0 != queue.Post(7));
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ_CLEAR, 1 != queue.Post(8));
	::llc::au2_t copied;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ_CLEAR, 0 != queue.Read(copied, true));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_READ_CLEAR, copied.size() != 2 || copied[0] != 7 || copied[1] != 8 || queue.Size() != 0);
	rtrn 0;
}

sttc ::llc::err_t testQueueAsyncNext(::llc::ATestError & results) {
	::llc::qpod<::llc::u2_t> queue;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 0 != queue.Post(7));
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 1 != queue.Post(8));
	::llc::u2_t value = {};
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 2 != queue.Next(value));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_NEXT, value != 7 || queue.Size() != 1);
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 1 != queue.Next(value));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_NEXT, value != 8 || queue.Size() != 0);
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 0 != queue.Next(value));
	rtrn 0;
}

sttc ::llc::err_t testQueueAsyncClear(::llc::ATestError & results) {
	::llc::qpod<::llc::u2_t> queue;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_CLEAR, 0 != queue.Post(9));
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_CLEAR, 0 != queue.Clear());
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_CLEAR, queue.Size() != 0);
	rtrn 0;
}

::llc::err_t testQueueAsync(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testQueueAsyncPost(results));
	result = ::llc::min(result, ::testQueueAsyncReadRetain(results));
	result = ::llc::min(result, ::testQueueAsyncReadClear(results));
	result = ::llc::min(result, ::testQueueAsyncNext(results));
	result = ::llc::min(result, ::testQueueAsyncClear(results));
	rtrn result;
}
