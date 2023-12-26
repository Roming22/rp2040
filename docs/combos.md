# Combo

## Definition
A Combo is a category of input that requires multiple switches to be pressed within a given delay.

The following actions belong to the Combo catergory:
* [Chord](./chords.md)
* [Sequence](./sequences.md)

## Behavior
1. Once a combo has started, pressing any key that does not belong to the combo will interrupt it.
2. A combo is not interrupted by switch release events.

## Implementation details

### Detecting a Combo activation
* A filter is added to the Timeline to end the timeline if any action other than a switch belonging to the Combo is pressed.
* On commit, the filter is cleared, and the timer is stopped.
* On ending the timeline, the timer is stopped.

### Ending other timelines when a Combo is activated
Timelines are order by their Complexity, where the Complexity is the number of switch events required for the timeline to be valid. E.g. KEYCODE(SPACE) has complexity of 1, CHORD(1,2,3,BACKSPACE) has a complexity of 3.
When a Combo is activated, it will send a signal to it's parent to end any timeline of a lower Complexity.

### Releasing the Combo
* On commit an action event is set to execute the release action for each switch of the Combo.
* The action event also set the action event of the other Combo's switches to a no-op function. This ensure that only the first switch release event triggers the release action, with subsequent switch releases doing nothing.