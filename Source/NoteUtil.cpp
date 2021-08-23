/*
  ==============================================================================

    NoteUtil.cpp
    Created: 21 Jul 2021 12:10:13am
    Author:  walter

  ==============================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

int currentScaleNoteIndex;
int currentScaleTypeIndex;
int currentScaleTypeSize;
const string mainNotes[] = { "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab", "A", "A#/Bb", "B" };

namespace modes {
    // nice identifiers
    enum names
    {
      IONIAN, DORIAN, PHRYGIAN, LYDIAN, MIXOLYDIAN, AEOLIAN, LOCRIAN
    };
    
    // spacings as indices
    // main modes
    int ionian[] = {2,4,5,7,9,11};
    int dorian[] = {2,3,5,7,9,10};
    int phrygian[] = {1,3,5,7,8,9};
    int lydian[] = {2,4,6,7,9,11};
    int mixolydian[] = {2,4,5,7,9,10};
    int aeolian[] = {2,3,5,7,8,10};
    int locrian[] = {1,3,5,6,8,10};
    
    // other cool scales? (don't forget to add to other functions)
}



int modesEnumByName(string scaleTypeName)
{
    if (scaleTypeName == "Ionian") { return modes::IONIAN; }
    if (scaleTypeName == "Dorian") { return modes::DORIAN; }
    if (scaleTypeName == "Phrygian") { return modes::PHRYGIAN; }
    if (scaleTypeName == "Lydian") { return modes::LYDIAN; }
    if (scaleTypeName == "Mixolydian") { return modes::MIXOLYDIAN; }
    if (scaleTypeName == "Aeolian") { return modes::AEOLIAN; }
    if (scaleTypeName == "Locrian") { return modes::LOCRIAN; }
    
    return -1; // bad; no enum val found
}

string getPrettyScaleName(int scaleType)
{
    if (scaleType == modes::IONIAN) {
        return "Ionian";
    }
    else if (scaleType == modes::DORIAN) {
        return "Dorian";
    }
    else if (scaleType == modes::PHRYGIAN) {
        return "Phrygian";
    }
    else if (scaleType == modes::LYDIAN) {
        return "Lydian";
    }
    else if (scaleType == modes::MIXOLYDIAN) {
        return "Mixolydian";
    }
    else if (scaleType == modes::AEOLIAN) {
        return "Aeolian";
    }
    else if (scaleType == modes::LOCRIAN) {
        return "Locrian";
    }
    else {
        return "Chromatic";
    }
}

// return pointer to an array containing the indices
// indicating note spacings from tonic in chosen scale type
int* getNoteSpacings(int scaleType)
{
    if (scaleType == modes::IONIAN) {
        return modes::ionian;
    }
    else if (scaleType == modes::DORIAN) {
        return modes::dorian;
    }
    else if (scaleType == modes::PHRYGIAN) {
        return modes::phrygian;
    }
    else if (scaleType == modes::LYDIAN) {
        return modes::lydian;
    }
    else if (scaleType == modes::MIXOLYDIAN) {
        return modes::mixolydian;
    }
    else if (scaleType == modes::AEOLIAN) {
        return modes::aeolian;
    }
    else if (scaleType == modes::LOCRIAN) {
        return modes::locrian;
    }
    else {
        static int chromatic[] = {1,2,3,4,5,6,7,8,9,10,11};
        return chromatic;
    }
}

int sizeOfScaleType(int index)
{
    return sizeof(getNoteSpacings(index));
}

/* * utility funcs * * * * * * * * * * * * * */

// https://stackoverflow.com/questions/712679/convert-midi-note-numbers-to-name-and-octave#712686
// i.e. input 48 -> get 'C4'
string getNoteNameFromMidi(int midiNote)
{
    if (midiNote == -1)
        return "nullnote";
    int octave = (midiNote / 12) - 1;
    int noteIndex = (midiNote % 12);
    return mainNotes[noteIndex] + to_string(octave);
}

// getNoteNameFromMidi except no octave info
string getNoteFromMidi(int midiNote)
{
    return mainNotes[midiNote % 12];
}

// return true if given midi note is in a scale basic on given tonic and scale type (as enum from modes::)
bool noteIsInScale(int noteInQuestion, int tonicNote, int scaleType, int scaleArraySize)
{
    int tonicIndex = (tonicNote % 12);
    int givenNoteIndex = (noteInQuestion % 12);
    // definitely in scale if it is a tonic
    if (tonicIndex == givenNoteIndex) {
        return true;
    }
    // check if it is a note elsewhere in the scale
    int* toCheck = getNoteSpacings(scaleType);
    for (int pointerInc = 0; pointerInc < scaleArraySize; pointerInc++){
        int validScaleNote = *(toCheck + pointerInc) + tonicIndex;
        if (noteInQuestion % 12 == validScaleNote % 12){
            return true;
        }
    }
    
    return false;
}

// return vector of all valid midi notes in given scale
set<int> genValidNotes(int tonicNote, int scaleType, int scaleArraySize)
{
    currentScaleNoteIndex = (tonicNote % 12);
    currentScaleTypeIndex = scaleType;
    currentScaleTypeSize = sizeOfScaleType(scaleType);
    set<int> end;
    int curNote = 21;
    while (curNote < 108){
        if (noteIsInScale(curNote, tonicNote, scaleType, scaleArraySize)){
            end.insert(curNote);
        }
        curNote++;
    }
    return end;
}
