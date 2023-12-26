# Chord

## Definition
A Chord is a tuple of switches that must all be pressed within a given delay to be activated.
A Chord is defined by 2 parameters:
1. The list of switches that make up the Chord.
2. The activation delay of the Chord.

The Chord is part of the [Combo](./combos.md) family.

## Switch Function



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

### SwitchFunction
Internally the chord definition is translated to a SwitchFunction in the form of 
`CH(switch-1, ..., switch-n, keycode)`.

Example: `1+2+3: ENTER` becomes `CH(1,2,3, ENTER)`.