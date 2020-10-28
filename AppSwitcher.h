#pragma once

#include "Kaleidoscope-Ranges.h"
#include "Kaleidoscope.h"

class AppSwitcher : public kaleidoscope::Plugin {
public:
  AppSwitcher() {}

  void setHoldKey(Key key);
  void setHostHoldKey(Key key);
  void setHostForwardKey(Key key);
  void setHostReverseKey(Key key);

  kaleidoscope::EventHandlerResult beforeReportingState();

  kaleidoscope::EventHandlerResult onKeyswitchEvent(Key &key, KeyAddr key_addr,
                                                    uint8_t key_state);

private:
  bool hold_key_held_;
  bool active_;
  Key hold_key_;
  Key host_hold_key_;
  Key host_forward_key_;
  Key host_reverse_key_;
};

AppSwitcher AppSwitcher;