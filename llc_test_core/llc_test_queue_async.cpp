#include "llc_test_core_suites.h"
#include "llc_queue_async.h"

GDEFINE_ENUM_TYPE(TEST_QUEUE_ASYNC_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, POST  , 0, "An asynchronous queue appends posted values under its lock.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, READ  , 1, "Reading can copy the queue with or without clearing it.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, NEXT  , 2, "Next returns FIFO values and one plus the remaining message count.");
GDEFINE_ENUM_VALUED(TEST_QUEUE_ASYNC_RESULT, CLEAR , 3, "Clearing removes every queued value.");

::llc::err_t testQueueAsync(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::qpod<::llc::u2_t> queue;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_POST, 0 != queue.Post(7) || 1 != queue.Post(8));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_POST, queue.Size() != 2);

	::llc::au2_t copied;
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_READ, 0 != queue.Read(copied, false));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_READ, copied.size() != 2 || copied[0] != 7 || copied[1] != 8 || queue.Size() != 2);

	::llc::u2_t value = {};
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 2 != queue.Next(value));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_NEXT, value != 7 || queue.Size() != 1);
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_NEXT, 1 != queue.Next(value));
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_NEXT, value != 8 || queue.Size() != 0 || 0 != queue.Next(value));

	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_CLEAR, 0 != queue.Post(9));
	LLC_TEST_REQUIRE(results, TEST_QUEUE_ASYNC_RESULT_CLEAR, 0 != queue.Clear());
	LLC_TEST_CHECK(results, TEST_QUEUE_ASYNC_RESULT_CLEAR, queue.Size() != 0);
	rtrn 0;
}
