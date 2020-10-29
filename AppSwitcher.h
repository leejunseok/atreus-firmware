#pragma once

#include "Kaleidoscope.h"

class AppSwitcher_ : public kaleidoscope::Plugin {
public:
  AppSwitcher_(void) {}

  void setHoldKey(Key key);
  void setHostHoldKey(Key key);
  void setHostForwardKey(Key key);
  void setHostReverseKey(Key key);

  kaleidoscope::EventHandlerResult beforeReportingState();

  kaleidoscope::EventHandlerResult onKeyswitchEvent(Key &key, KeyAddr key_addr,
                                                    uint8_t key_state);

private:
  static bool hold_key_held_;
  static bool active_;
  static Key hold_key_;
  static Key host_hold_key_;
  static Key host_forward_key_;
  static Key host_reverse_key_;
};

extern AppSwitcher_ AppSwitcher;