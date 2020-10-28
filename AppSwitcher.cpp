#pragma once

#include "Kaleidoscope-Ranges.h"
#include "Kaleidoscope.h"

#include "AppSwitcher.h"

void AppSwitcher::setHoldKey(Key key) { hold_key_ = key; }
void AppSwitcher::setHostHoldKey(Key key) { host_hold_key_ = key; }
void AppSwitcher::setHostForwardKey(Key key) { host_forward_key_ = key; }
void AppSwitcher::setHostReverseKey(Key key) { host_reverse_key_ = key; }

kaleidoscope::EventHandlerResult AppSwitcher::beforeReportingState() {
  if (active_) {
    kaleidoscope::Runtime.hid().keyboard().pressKey(host_hold_key_, false);
    handleKeyswitchEvent(host_hold_key_, UnknownKeyswitchLocation, IS_PRESSED);
  }
}

kaleidoscope::EventHandlerResult
AppSwitcher::onKeyswitchEvent(Key &key, KeyAddr key_addr, uint8_t key_state) {
  // We want nothing to do with the host_hold_key_. Other keys might
  // deactivate AppSwitcher, but not the host_hold_key_.
  if (key == host_hold_key_) {
    return kaleidoscope::EventHandlerResult::OK;
  }
  // Management of the hold_key_held_ state. There's nothing to do if
  // the user started holding, but we may have work to do if the user
  // released.
  if (key == hold_key_) {
    if (keyIsPressed(key_state)) {
      hold_key_held_ = true;
      return kaleidoscope::EventHandlerResult::OK;
    }
    if (keyToggledOff(key_state)) {
      hold_key_held_ = false;
    }
  }
  if (active_) {
    // AppSwitcher is deactivated when either...
    //  1. The user presses any key outside of the three used for this
    //     plugin (with the exception of host_hold_key_), or
    //  2. The hold key is released.
    bool other_key_pressed =
        keyIsPressed(key_state) &&
        !(key == Key(kaleidoscope::ranges::APPSWITCHER_F) ||
          key == Key(kaleidoscope::ranges::APPSWITCHER_R) || key == hold_key_);
    bool hold_key_released = keyToggledOff(key_state) && key == hold_key_;
    if (other_key_pressed || hold_key_released) {
      active_ = false;
      kaleidoscope::Runtime.serialPort().print(
          "AppSwitcher: Deactivated because of... ");
      kaleidoscope::Runtime.serialPort().println(
          other_key_pressed ? "different key press" : "hold key release");
      kaleidoscope::Runtime.hid().keyboard().releaseKey(host_hold_key_);
      handleKeyswitchEvent(host_hold_key_, UnknownKeyswitchLocation,
                           WAS_PRESSED);
      return kaleidoscope::EventHandlerResult::OK;
    }
  }
  if (hold_key_held_) {
    // When either the APP_SWITCHER_F or APP_SWITCHER_R are pressed
    // while the hold_key_ is held, activate AppSwitcher if it isn't
    // already, and translate to the host_forward_key_ and
    // host_reverse_key_ respectively.
    if (keyIsPressed(key_state) &&
        (key == kaleidoscope::ranges::APPSWITCHER_F ||
         key == kaleidoscope::ranges::APPSWITCHER_R)) {
      if (!active_) {
        kaleidoscope::Runtime.serialPort().println("AppSwitcher: Activated");
        kaleidoscope::Runtime.hid().keyboard().pressKey(host_hold_key_, true);
        handleKeyswitchEvent(host_hold_key_, UnknownKeyswitchLocation,
                             IS_PRESSED);
        active_ = true;
      }
      if (key == kaleidoscope::ranges::APPSWITCHER_F) {
        key = host_forward_key_;
      } else if (key == kaleidoscope::ranges::APPSWITCHER_R) {
        key = host_reverse_key_;
      }
    }
  }
  return kaleidoscope::EventHandlerResult::OK;
}