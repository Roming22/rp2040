# Chord

## Definition
A Chord is a tuple of switches that must all be pressed within a given delay to be activated.
A Chord is defined by 2 parameters:
1. The list of switches that make up the Chord.
2. The activation delay of the Chord.

The Chord is part of the Combo family.

## Behavior
1. The order in which the Chord's switches are pressed do not matter. 
2. The Chord is pending activation as soon as one of the Chord's switches is pressed.
3. The Chord is activated as soon as all the Chord switches have been pressed.
4. A timer with cancel the Chord if it has not been activated in the required timeframe.
5. Pressing a switch that does not belong to the Chord while the Chord is pending activation will cancel the Chord.
6. Releasing any switch that belong to the Chord will trigger the release action of the Chord.

## Unsupported usecases
1. OR operator. To define a chord that activates when (1 and 2) or (1 and 3) are pressed, two chords must be defined. This is a limitation of the existing parser.
2. Mixing Chords and Sequences. It is not possible to write a definition for a usecase that mixes Chords and Sequence. An example would be if a user wanted to have a one behavior if switches 1&2 where pressed as part of a chord, shortly followed by press of switch 3, and another behavior if switches 1&3 where pressed as part of a chord, shortly followed by press of switch 2. This is a limitation of both the parser and the current layout representation.

## Implementation details

### Definition separator
'+' has been chosen as the separator because like the addition it is:
* Commutative
* Associative

'*' could have been chosen, but it is reserved to implement the OR operator, where `1+2*3` defines a Chord that is activated by pressing either switches 1&2 or switches 1&3 simultaneously.

This allows to parse a chord definition as a standard mathematical expression, keeping the known operators precedence.

### Activating the Chord
* A filter is added to the Timeline to end the timeline if any action other than a switch belonging to the Chord is pressed.
* On commit, the filter is cleared, and the timer is stopped.
* On ending the timeline, the timer is stopped.

### Ending other timelines when a Chord is activated
Timelines are order by their Complexity, where the Complexity is the number of event required for the timeline to be valid. E.g. KEYCODE(SPACE) has complexity of 1, CHORD(1,2,3,BACKSPACE) has a complexity of 3.
When a Chord is activated, it will send a signal to it's parent to end any timeline of a lower Complexity.

### Releasing the Chord
* On commit an action event is set to execute the release action for each switch of the Chord.
* The action event also set the action event of the other Chord's switches to a no-op function. This ensure that only the first switch release event triggers the release action, with subsequent switch releases doing nothing.