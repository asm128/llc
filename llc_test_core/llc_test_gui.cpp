#include "llc_test_core.h"

::llc::err_t testGUIControl(ATestError & errors);
::llc::err_t testGUIModel(ATestError & errors);
::llc::err_t testGUILayout(ATestError & errors);
::llc::err_t testGUIRender(ATestError & errors);

::llc::err_t testGUI(ATestError & errors) {
	if_fail_fe(testGUIControl(errors));
	if_fail_fe(testGUIModel(errors));
	if_fail_fe(testGUILayout(errors));
	rtrn testGUIRender(errors);
}
