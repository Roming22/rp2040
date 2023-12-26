# TapHold

## Definition
A TapHold is switch press that behaves differently whether the switch is being release before or after a  given delay.

A TapHold is defined by 3 parameters:
1. The action to be executed if the switch is tapped.
2. The action to be executed if the switch is held.
3. The delay where the switch goes from `tap` to `hold`.

## Switch Function
A TapHold is declared as `TH(switchfunc_tap|switchfunc_hold)`.

## Behavior
1. Pressing a switch with a TapHold SwitchFunc generates 2 timelines.
2. The Tap timeline is in control of the lifetime of both timelines.
3. Control of the timelines is handled through a Timer and a release action.
4. If the Timer expires, an event is sent to the Tap timeline. Since the event is not registed, this will end the timeline.
5. An action to prune Hold timeline is added to the release Event on the Tap timeline. Therefore releasing the switch before the timer expires will end the Hold timeline.
6. Pressing or releasing any other switch does not impact the SwitchFunc.