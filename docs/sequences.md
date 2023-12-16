# Sequence (Not Implemented)

## Definition
A Sequence is a tuple of switches that must all be pressed in the right order, within a given delay to be activated.
A Chord is defined by 2 parameters:
1. The list of switches that make up the Sequence.
2. The list of delays of the Sequence.

The Sequence is part of the Combo family.

## Behavior (Not Implemented)
1. The order in which the Sequence's switches are pressed do matter. 
2. The Sequence is pending activation as soon as the first switch of the Sequence is pressed.
3. The Sequence is activated as soon as all the Sequence switches have been pressed.
4. Pressing a switch resets the delay for the next switch press.
5. The timer with cancel the Sequence if it the next switch press has not occured in the required timeframe.
6. Pressing a switch that does not belong to the Sequence while the Sequence is pending activation will cancel the Sequence.
7. Releasing any switch that belong to the Sequence will trigger the release action of the Sequence.

## Unsupported usecases
1. OR operator. To define a chord that activates when (1 then 2) or (1 then 3) are pressed, two chords must be defined. This is a limitation of the existing parser.
2. Mixing Chords and Sequences. It is not possible to write a definition for a usecase that mixes Chords and Sequence. An example would be if a user wanted to have a one behavior if switches 1&2 where pressed as part of a chord, shortly followed by press of switch 3, and another behavior if switches 1&3 where pressed as part of a chord, shortly followed by press of switch 2. This is a limitation of both the parser and the current layout representation.

## Implementation details

### Definition separator
'-' has been chosen as the separator because like the substraction it is:
* Not commutative
* Not associative

'*' is reserved to implement the OR operator, where `1-2*3` defines a Sequence that is activated by pressing either switches 1&2 or switches 1&3 in sequence.